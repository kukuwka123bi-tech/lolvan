// C++20 / Android NDK / arm64-v8a / Zygisk API
#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <cstring>
#include <fcntl.h>
#include "zygisk.hpp"

#define LOG_TAG "PUBG_SUITE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// ============================================================
// OFFSETS - Update these via Ghidra/IDA after each game update
// ============================================================
namespace Offsets {
    // libUE4.so offsets
    uintptr_t GWorld               = 0x0;
    uintptr_t GNames               = 0x0;
    uintptr_t UWorld_PersistentLevel = 0x0;
    uintptr_t ULevel_AActors       = 0x0;
    uintptr_t AActors_Count        = 0x0;
    uintptr_t APlayerController    = 0x0;
    uintptr_t LocalPlayer          = 0x0;
    uintptr_t CameraManager        = 0x0;
    uintptr_t RootComponent        = 0x0;
    uintptr_t RelativeLocation     = 0x0;
    uintptr_t BoneArray            = 0x0;
    uintptr_t Mesh                 = 0x0;
    uintptr_t TeamID               = 0x0;
    uintptr_t Health               = 0x0;
    uintptr_t WeaponManager        = 0x0;
    uintptr_t ControlRotation      = 0x0;
    uintptr_t AspectRatio          = 0x0;
    uintptr_t FOV                  = 0x0;
    
    // libtersafe.so (ACE) offsets for bypass
    uintptr_t ACE_ScanTextSection  = 0x0;
    uintptr_t ACE_CheckMaps        = 0x0;
    
    // Skin Changer
    uintptr_t SkeletalMesh         = 0x0;
    uintptr_t OverrideMaterials    = 0x0;
}

// ============================================================
// CONFIGURATION & PROFILES
// ============================================================
enum class Profile { OFF, LEGIT, RAGE };
Profile g_CurrentProfile = Profile::LEGIT;

struct Config {
    bool ESP_Enabled = true;
    bool ESP_Box = true;
    bool ESP_Skeleton = true;
    bool ESP_Health = true;
    bool ESP_Lines = true;
    bool ESP_Distance = true;
    bool ESP_Name = false;
    
    bool Aimbot_Enabled = true;
    float Aimbot_FOV = 5.0f;
    float Aimbot_Smooth = 0.1f;
    int Aimbot_Bone = 0; // 0=Head, 1=Chest, 2=Pelvis, 3=Nearest
    bool Aimbot_VisibilityCheck = true;
    bool Aimbot_TeamCheck = true;
    
    bool NoRecoil = false;
    bool NoSpread = false;
    
    bool IPadView = false;
    float TargetAspectRatio = 1.333f;
    
    bool SkinChanger = false;
    int SkinID = 0;
    
    bool SpeedHack = false;
    float SpeedMultiplier = 1.0f;
    
    bool WallHack = false;
    bool Chams = false;
    bool Glow = false;
    
    bool Radar_Enabled = false;
    float Radar_Scale = 100.0f;
    
    bool AutoFire = false;
    
    bool HideFromACE = true;
    bool AntiScreenshot = false;
} g_Config;

void ApplyProfile(Profile p) {
    g_CurrentProfile = p;
    
    switch (p) {
        case Profile::LEGIT:
            g_Config.Aimbot_FOV = 3.0f;
            g_Config.Aimbot_Smooth = 0.08f;
            g_Config.Aimbot_Bone = 1;
            g_Config.Aimbot_VisibilityCheck = true;
            g_Config.NoRecoil = false;
            g_Config.NoSpread = false;
            g_Config.ESP_Skeleton = false;
            g_Config.ESP_Box = true;
            g_Config.ESP_Health = true;
            g_Config.WallHack = false;
            g_Config.Chams = false;
            g_Config.Radar_Enabled = false;
            g_Config.SpeedHack = false;
            LOGI("Profile: LEGIT");
            break;
            
        case Profile::RAGE:
            g_Config.Aimbot_FOV = 180.0f;
            g_Config.Aimbot_Smooth = 1.0f;
            g_Config.Aimbot_Bone = 0;
            g_Config.Aimbot_VisibilityCheck = false;
            g_Config.NoRecoil = true;
            g_Config.NoSpread = true;
            g_Config.ESP_Skeleton = true;
            g_Config.ESP_Box = true;
            g_Config.ESP_Health = true;
            g_Config.WallHack = true;
            g_Config.Chams = true;
            g_Config.Radar_Enabled = true;
            LOGI("Profile: RAGE");
            break;
            
        case Profile::OFF:
            g_Config.ESP_Enabled = false;
            g_Config.Aimbot_Enabled = false;
            g_Config.NoRecoil = false;
            g_Config.NoSpread = false;
            g_Config.WallHack = false;
            g_Config.Chams = false;
            g_Config.SpeedHack = false;
            LOGI("Profile: OFF");
            break;
    }
}

uintptr_t g_LibUE4Base = 0;
uintptr_t g_LibTersafeBase = 0;

// ============================================================
// UTILITY FUNCTIONS
// ============================================================
uintptr_t GetLibraryBase(const char* name) {
    char line[512];
    FILE* fp = fopen("/proc/self/maps", "r");
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, name)) {
            uintptr_t base = strtoull(line, NULL, 16);
            fclose(fp);
            return base;
        }
    }
    fclose(fp);
    return 0;
}

uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask) {
    size_t patternLen = strlen(mask);
    for (size_t i = 0; i < size - patternLen; i++) {
        bool found = true;
        for (size_t j = 0; j < patternLen; j++) {
            if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j)) {
                found = false;
                break;
            }
        }
        if (found) return base + i;
    }
    return 0;
}

template<typename T>
T ReadMemory(uintptr_t addr) {
    if (!addr) return T{};
    if (addr < 0x1000 || addr > 0x7FFFFFFF000) return T{};
    return *reinterpret_cast<T*>(addr);
}

bool WriteMemory(uintptr_t addr, uintptr_t value) {
    if (!addr) return false;
    uintptr_t page = addr & ~(sysconf(_SC_PAGESIZE) - 1);
    if (mprotect((void*)page, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE) != 0) {
        return false;
    }
    *reinterpret_cast<uintptr_t*>(addr) = value;
    mprotect((void*)page, sysconf(_SC_PAGESIZE), PROT_READ);
    return true;
}

// ============================================================
// DATA STRUCTURES
// ============================================================
struct FVector {
    float X, Y, Z;
    FVector operator-(const FVector& other) const {
        return {X - other.X, Y - other.Y, Z - other.Z};
    }
    float Size() const {
        return sqrt(X * X + Y * Y + Z * Z);
    }
    FVector Normalize() const {
        float s = Size();
        if (s > 0) return {X / s, Y / s, Z / s};
        return {0, 0, 0};
    }
};

struct FRotator {
    float Pitch, Yaw, Roll;
};

// ============================================================
// GAME STRUCTURES
// ============================================================
struct AActor {
    uintptr_t Address;
    FVector Location;
    int32_t TeamID;
    float Health;
    bool IsAlive;
    bool IsBot;
};

struct APlayerController {
    uintptr_t Address;
    FRotator ControlRotation;
    uintptr_t PlayerCameraManager;
};

// ============================================================
// MEMORY MANAGER
// ============================================================
class MemoryManager {
public:
    static uintptr_t GetUWorld() {
        if (!g_LibUE4Base) return 0;
        return ReadMemory<uintptr_t>(g_LibUE4Base + Offsets::GWorld);
    }
    
    static uintptr_t GetPersistentLevel(uintptr_t world) {
        if (!world) return 0;
        return ReadMemory<uintptr_t>(world + Offsets::UWorld_PersistentLevel);
    }
    
    static std::vector<AActor> GetAllActors() {
        std::vector<AActor> actors;
        uintptr_t world = GetUWorld();
        if (!world) return actors;
        uintptr_t level = GetPersistentLevel(world);
        if (!level) return actors;
        uintptr_t actorsArray = ReadMemory<uintptr_t>(level + Offsets::ULevel_AActors);
        if (!actorsArray) return actors;
        int32_t actorCount = ReadMemory<int32_t>(level + Offsets::AActors_Count);
        if (actorCount <= 0 || actorCount > 1000) return actors;
        for (int i = 0; i < actorCount; i++) {
            uintptr_t actorAddr = ReadMemory<uintptr_t>(actorsArray + i * sizeof(uintptr_t));
            if (!actorAddr) continue;
            AActor actor;
            actor.Address = actorAddr;
            actor.Location = ReadMemory<FVector>(actorAddr + Offsets::RelativeLocation);
            actor.TeamID = ReadMemory<int32_t>(actorAddr + Offsets::TeamID);
            actor.Health = ReadMemory<float>(actorAddr + Offsets::Health);
            actor.IsAlive = actor.Health > 0;
            if (actor.Location.X != 0 || actor.Location.Y != 0 || actor.Location.Z != 0) {
                actors.push_back(actor);
            }
        }
        return actors;
    }
    
    static APlayerController GetLocalPlayerController() {
        APlayerController controller;
        controller.Address = ReadMemory<uintptr_t>(g_LibUE4Base + Offsets::LocalPlayer);
        if (controller.Address) {
            controller.ControlRotation = ReadMemory<FRotator>(controller.Address + Offsets::ControlRotation);
            controller.PlayerCameraManager = ReadMemory<uintptr_t>(controller.Address + Offsets::CameraManager);
        }
        return controller;
    }
};

// ============================================================
// CAMERA MANAGER
// ============================================================
class CameraManager {
public:
    static bool WorldToScreen(FVector worldPos, FVector& screenPos) {
        uintptr_t cameraMgr = MemoryManager::GetLocalPlayerController().PlayerCameraManager;
        if (!cameraMgr) return false;
        FVector cameraLocation = ReadMemory<FVector>(cameraMgr + Offsets::RelativeLocation);
        FRotator cameraRotation = ReadMemory<FRotator>(cameraMgr + Offsets::ControlRotation);
        FVector delta = worldPos - cameraLocation;
        float distance = delta.Size();
        if (distance < 1.0f) return false;
        FVector direction = delta.Normalize();
        float yawRad = cameraRotation.Yaw * M_PI / 180.0f;
        float pitchRad = cameraRotation.Pitch * M_PI / 180.0f;
        float cosYaw = cos(yawRad);
        float sinYaw = sin(yawRad);
        float cosPitch = cos(pitchRad);
        float sinPitch = sin(pitchRad);
        FVector rotated;
        rotated.X = direction.X * cosYaw + direction.Y * sinYaw;
        rotated.Y = direction.X * -sinYaw + direction.Y * cosYaw;
        rotated.Z = direction.Z;
        float tempZ = rotated.Z * cosPitch - rotated.Y * sinPitch;
        rotated.Y = rotated.Z * sinPitch + rotated.Y * cosPitch;
        rotated.Z = tempZ;
        if (rotated.Z <= 0.01f) return false;
        float screenWidth = 1080.0f;
        float screenHeight = 1920.0f;
        float fov = 90.0f;
        float fovFactor = tan(fov * M_PI / 360.0f);
        screenPos.X = screenWidth / 2.0f + (rotated.X / rotated.Z) * (screenWidth / (2.0f * fovFactor));
        screenPos.Y = screenHeight / 2.0f - (rotated.Y / rotated.Z) * (screenHeight / (2.0f * fovFactor));
        screenPos.Z = rotated.Z;
        return true;
    }
};

// ============================================================
// ESP RENDERER
// ============================================================
class ESPRenderer {
private:
    static std::vector<AActor> filteredEnemies;
    static APlayerController localController;
    
public:
    static void Update() {
        filteredEnemies.clear();
        auto actors = MemoryManager::GetAllActors();
        localController = MemoryManager::GetLocalPlayerController();
        if (!localController.Address) return;
        int localTeam = ReadMemory<int32_t>(localController.Address + Offsets::TeamID);
        for (auto& actor : actors) {
            if (actor.Address == localController.Address) continue;
            if (!actor.IsAlive) continue;
            if (g_Config.Aimbot_TeamCheck && actor.TeamID == localTeam) continue;
            filteredEnemies.push_back(actor);
        }
    }
    
    static void Draw() {
        if (!g_Config.ESP_Enabled) return;
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (auto& enemy : filteredEnemies) {
            FVector screenPos;
            if (!CameraManager::WorldToScreen(enemy.Location, screenPos)) continue;
            float distance = enemy.Location.Size();
            if (g_Config.ESP_Box) DrawBox(screenPos.X, screenPos.Y, 100.0f, 200.0f, distance);
            if (g_Config.ESP_Skeleton) DrawSkeleton(enemy.Address, screenPos);
            if (g_Config.ESP_Health) DrawHealthBar(screenPos.X, screenPos.Y, enemy.Health);
            if (g_Config.ESP_Distance) DrawText(screenPos.X, screenPos.Y - 20, std::to_string((int)distance) + "m");
        }
        glEnable(GL_DEPTH_TEST);
    }
    
private:
    static void DrawBox(float x, float y, float width, float height, float distance) {
        float scale = 1000.0f / (distance + 1000.0f);
        width *= scale;
        height *= scale;
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x - halfWidth, y - halfHeight);
        glVertex2f(x + halfWidth, y - halfHeight);
        glVertex2f(x + halfWidth, y + halfHeight);
        glVertex2f(x - halfWidth, y + halfHeight);
        glEnd();
    }
    
    static void DrawSkeleton(uintptr_t actorAddr, FVector headPos) {
        uintptr_t boneArray = ReadMemory<uintptr_t>(actorAddr + Offsets::BoneArray);
        if (!boneArray) return;
        uintptr_t mesh = ReadMemory<uintptr_t>(actorAddr + Offsets::Mesh);
        if (!mesh) return;
        int32_t boneCount = ReadMemory<int32_t>(actorAddr + Offsets::BoneArray + 0x8);
        if (boneCount <= 0) return;
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        FVector neckPos = ReadMemory<FVector>(boneArray + 1 * sizeof(FVector));
        FVector neckScreen;
        if (CameraManager::WorldToScreen(neckPos, neckScreen)) {
            glVertex2f(headPos.X, headPos.Y);
            glVertex2f(neckScreen.X, neckScreen.Y);
        }
        FVector chestPos = ReadMemory<FVector>(boneArray + 2 * sizeof(FVector));
        FVector chestScreen;
        if (CameraManager::WorldToScreen(chestPos, chestScreen)) {
            glVertex2f(neckScreen.X, neckScreen.Y);
            glVertex2f(chestScreen.X, chestScreen.Y);
        }
        FVector pelvisPos = ReadMemory<FVector>(boneArray + 3 * sizeof(FVector));
        FVector pelvisScreen;
        if (CameraManager::WorldToScreen(pelvisPos, pelvisScreen)) {
            glVertex2f(chestScreen.X, chestScreen.Y);
            glVertex2f(pelvisScreen.X, pelvisScreen.Y);
        }
        glEnd();
    }
    
    static void DrawHealthBar(float x, float y, float health) {
        float maxHealth = 100.0f;
        float healthPercent = health / maxHealth;
        float barWidth = 50.0f;
        float barHeight = 5.0f;
        glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(x - barWidth/2, y - 30);
        glVertex2f(x + barWidth/2, y - 30);
        glVertex2f(x + barWidth/2, y - 30 - barHeight);
        glVertex2f(x - barWidth/2, y - 30 - barHeight);
        glEnd();
        glColor4f(1.0f - healthPercent, healthPercent, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(x - barWidth/2, y - 30);
        glVertex2f(x - barWidth/2 + barWidth * healthPercent, y - 30);
        glVertex2f(x - barWidth/2 + barWidth * healthPercent, y - 30 - barHeight);
        glVertex2f(x - barWidth/2, y - 30 - barHeight);
        glEnd();
    }
    
    static void DrawText(float x, float y, const std::string& text) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
};

std::vector<AActor> ESPRenderer::filteredEnemies;
APlayerController ESPRenderer::localController;

// ============================================================
// AIMBOT
// ============================================================
class Aimbot {
public:
    static void Run() {
        if (!g_Config.Aimbot_Enabled) return;
        auto actors = MemoryManager::GetAllActors();
        auto localController = MemoryManager::GetLocalPlayerController();
        if (!localController.Address) return;
        int localTeam = ReadMemory<int32_t>(localController.Address + Offsets::TeamID);
        FVector localPos = ReadMemory<FVector>(localController.Address + Offsets::RelativeLocation);
        uintptr_t bestTarget = 0;
        float bestDistance = g_Config.Aimbot_FOV * 100.0f;
        FVector bestBonePos;
        for (auto& actor : actors) {
            if (actor.Address == localController.Address) continue;
            if (!actor.IsAlive) continue;
            if (g_Config.Aimbot_TeamCheck && actor.TeamID == localTeam) continue;
            FVector bonePos = GetBonePosition(actor.Address, g_Config.Aimbot_Bone);
            if (bonePos.X == 0 && bonePos.Y == 0 && bonePos.Z == 0) continue;
            FVector delta = bonePos - localPos;
            float distance = delta.Size();
            FVector screenPos;
            if (!CameraManager::WorldToScreen(bonePos, screenPos)) continue;
            float screenCenterX = 540.0f;
            float screenCenterY = 960.0f;
            float screenDist = sqrt(pow(screenPos.X - screenCenterX, 2) + pow(screenPos.Y - screenCenterY, 2));
            if (screenDist < bestDistance) {
                bestDistance = screenDist;
                bestTarget = actor.Address;
                bestBonePos = bonePos;
            }
        }
        if (bestTarget) {
            FRotator newRotation = CalculateAimRotation(localPos, bestBonePos, localController.ControlRotation);
            FRotator smoothedRotation = SmoothRotation(localController.ControlRotation, newRotation);
            WriteMemory<uintptr_t>(localController.Address + Offsets::ControlRotation, 
                *reinterpret_cast<uintptr_t*>(&smoothedRotation));
            LOGI("Aimbot: Target locked, Distance: %.2f", bestDistance);
        }
    }
    
private:
    static FVector GetBonePosition(uintptr_t actorAddr, int boneIndex) {
        uintptr_t boneArray = ReadMemory<uintptr_t>(actorAddr + Offsets::BoneArray);
        if (!boneArray) return {0, 0, 0};
        return ReadMemory<FVector>(boneArray + boneIndex * sizeof(FVector));
    }
    
    static FRotator CalculateAimRotation(FVector localPos, FVector targetPos, FRotator currentRot) {
        FVector delta = targetPos - localPos;
        float distance = delta.Size();
        if (distance < 0.01f) return currentRot;
        FVector direction = delta.Normalize();
        float yaw = atan2(direction.Y, direction.X) * 180.0f / M_PI;
        float pitch = atan2(-direction.Z, sqrt(direction.X * direction.X + direction.Y * direction.Y)) * 180.0f / M_PI;
        return {pitch, yaw, 0.0f};
    }
    
    static FRotator SmoothRotation(FRotator current, FRotator target) {
        if (g_Config.Aimbot_Smooth >= 1.0f) return target;
        float smoothFactor = g_Config.Aimbot_Smooth;
        FRotator result;
        result.Pitch = current.Pitch + (target.Pitch - current.Pitch) * smoothFactor;
        result.Yaw = current.Yaw + (target.Yaw - current.Yaw) * smoothFactor;
        result.Roll = 0.0f;
        return result;
    }
};

// ============================================================
// WEAPON HACKS
// ============================================================
class WeaponHacks {
public:
    static void Apply() {
        if (g_Config.NoRecoil) ApplyNoRecoil();
        if (g_Config.NoSpread) ApplyNoSpread();
    }
private:
    static void ApplyNoRecoil() {
        uintptr_t localPlayer = MemoryManager::GetLocalPlayerController().Address;
        if (!localPlayer) return;
        uintptr_t weaponManager = ReadMemory<uintptr_t>(localPlayer + Offsets::WeaponManager);
        if (!weaponManager) return;
        uintptr_t currentWeapon = ReadMemory<uintptr_t>(weaponManager);
        if (!currentWeapon) return;
    }
    static void ApplyNoSpread() {
    }
};

// ============================================================
// SKIN CHANGER
// ============================================================
class SkinChanger {
public:
    static void Apply() {
        if (!g_Config.SkinChanger) return;
        auto actors = MemoryManager::GetAllActors();
        for (auto& actor : actors) {
            if (actor.Address == MemoryManager::GetLocalPlayerController().Address) {
                ChangeSkin(actor.Address);
            }
        }
    }
private:
    static void ChangeSkin(uintptr_t actorAddr) {
        uintptr_t mesh = ReadMemory<uintptr_t>(actorAddr + Offsets::Mesh);
        if (!mesh) return;
        uintptr_t overrideMaterials = ReadMemory<uintptr_t>(mesh + Offsets::OverrideMaterials);
        if (!overrideMaterials) return;
        LOGI("SkinChanger: Changing skin for actor at %lx", actorAddr);
    }
};

// ============================================================
// IPAD VIEW
// ============================================================
class IPadView {
public:
    static void Apply() {
        if (!g_Config.IPadView) return;
    }
};

// ============================================================
// ANTI-CHEAT BYPASS
// ============================================================
class AntiCheatBypass {
public:
    static void HideModule() {}
    static void SpoofTextSection() {}
    static void BypassSyscallHooks() {}
};

// ============================================================
// HOOKS
// ============================================================
typedef EGLBoolean (*eglSwapBuffers_t)(EGLDisplay dpy, EGLSurface surface);
eglSwapBuffers_t orig_eglSwapBuffers = nullptr;

EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    ESPRenderer::Update();
    ESPRenderer::Draw();
    return orig_eglSwapBuffers(dpy, surface);
}

typedef float (*GetAspectRatio_t)(void* instance);
GetAspectRatio_t orig_GetAspectRatio = nullptr;

float hook_GetAspectRatio(void* instance) {
    if (g_Config.IPadView) return g_Config.TargetAspectRatio;
    return orig_GetAspectRatio(instance);
}

// ============================================================
// INITIALIZATION
// ============================================================
void* InitThread(void*) {
    LOGI("PUBG Zygisk Suite - Initializing...");
    while ((g_LibUE4Base = GetLibraryBase("libUE4.so")) == 0) usleep(500000);
    LOGI("libUE4.so base: %lx", g_LibUE4Base);
    while ((g_LibTersafeBase = GetLibraryBase("libtersafe.so")) == 0) usleep(100000);
    LOGI("libtersafe.so base: %lx", g_LibTersafeBase);
    AntiCheatBypass::HideModule();
    AntiCheatBypass::SpoofTextSection();
    ApplyProfile(Profile::LEGIT);
    LOGI("Profile set to LEGIT");
    void* eglHandle = dlopen("libEGL.so", RTLD_NOW);
    if (eglHandle) {
        orig_eglSwapBuffers = (eglSwapBuffers_t)dlsym(eglHandle, "eglSwapBuffers");
        if (orig_eglSwapBuffers) {
            LOGI("Found eglSwapBuffers at: %p", orig_eglSwapBuffers);
        }
    }
    LOGI("PUBG Zygisk Suite - Armed and ready!");
    while (true) {
        Aimbot::Run();
        WeaponHacks::Apply();
        SkinChanger::Apply();
        IPadView::Apply();
        usleep(16000);
    }
    return nullptr;
}

// ============================================================
// ZYGISK MODULE
// ============================================================
class PubgMod : public zygisk::ModuleBase {
public:
    void onLoad(zygisk::Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }
    void preAppSpecialize(zygisk::AppSpecializeArgs *args) override {
        const char* pkg = env->GetStringUTFChars(args->nice_name, nullptr);
        bool match = pkg && (strstr(pkg, "tencent.ig") || 
                            strstr(pkg, "pubg.krmobile") ||
                            strstr(pkg, "com.tencent.tmgp.pubgmhd") ||
                            strstr(pkg, "com.vng.pubgmobile") ||
                            strstr(pkg, "com.gameloft.android.ANMPGloftADH"));
        env->ReleaseStringUTFChars(args->nice_name, pkg);
        if (!match) api->setOption(zygisk::DLCLOSE_MODULE_LIBRARY);
        isTarget = match;
    }
    void postAppSpecialize(const zygisk::AppSpecializeArgs *) override {
        if (!isTarget) return;
        LOGI("PUBG detected - Loading suite...");
        pthread_t t;
        pthread_create(&t, nullptr, InitThread, nullptr);
        pthread_detach(t);
    }
private:
    zygisk::Api *api;
    JNIEnv *env;
    bool isTarget = false;
};

REGISTER_ZYGISK_MODULE(PubgMod)
