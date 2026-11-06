# PUBG Zygisk Suite v2.0.0

**Full Zygisk mod suite for PUBG Mobile with ESP, Aimbot, Skin Changer, and Anti-Cheat Bypass**

---

## 📥 Download

### Option 1: Download ZIP
[**Download Module ZIP**](https://github.com/kukuwka123bi-tech/lolvan/archive/refs/heads/pubg-module.zip)

Then extract and use the `pubg_full_suite` folder.

### Option 2: Clone Repository
```bash
git clone https://github.com/kukuwka123bi-tech/lolvan.git
cd lolvan
git checkout pubg-module
```

---

## 📦 Module Structure

```
pubg_full_suite/
├── module.prop              # Module metadata
├── customize.sh            # Install script
├── post-fs-data.sh         # Post-fs-data script
├── service.sh              # Service script
├── uninstall.sh            # Uninstall script
├── config.json             # Configuration
├── BUILD.md                # Build instructions
├── README.md               # This file
├── META-INF/
│   └── com/google/android/
│       ├── update-binary  # Magisk update script
│       └── updater-script
├── zygisk/
│   ├── arm64-v8a.so        # Compiled library (PLACEHOLDER - build it!)
│   └── zygisk.hpp          # Zygisk API header
└── src/
    ├── main.cpp            # Main module code
    └── CMakeLists.txt      # CMake build config
```

---

## ⚡ Quick Install

### 1. Build the Library

```bash
# Navigate to src directory
cd pubg_full_suite/src

# Create build directory
mkdir build && cd build

# Build with CMake
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-26 \
  -DANDROID_STL=c++_static

# Compile
make -j$(nproc)

# Copy library to module
cp libpubg_suite.so ../../zygisk/arm64-v8a.so
```

### 2. Create Module ZIP

```bash
# From pubg_full_suite directory
cd ..
zip -r pubg_full_suite.zip pubg_full_suite -x "pubg_full_suite/src/build/*"
```

### 3. Install via Magisk

1. Open Magisk app
2. Go to **Modules**
3. Tap **Install from storage**
4. Select `pubg_full_suite.zip`
5. **Reboot your device**
6. Enable **Zygisk** in Magisk settings

---

## 🎯 Features

### ESP
- ✅ Box
- ✅ Skeleton
- ✅ Health Bar
- ✅ Distance
- ✅ Name

### Aimbot
- ✅ FOV: 1-180°
- ✅ Smoothing: 0.01-1.0
- ✅ Bone selection (Head, Chest, Pelvis, Nearest)
- ✅ Visibility check
- ✅ Team check

### Profiles
- **LEGIT**: Stealth mode (3° FOV, 0.08 smooth, chest aim)
- **RAGE**: Full power (180° FOV, instant, head aim)
- **OFF**: All features disabled

### Weapon
- ✅ No Recoil
- ✅ No Spread

### Visuals
- ✅ iPad View (4:3 aspect ratio)
- ✅ Wallhack
- ✅ Chams
- ✅ Glow

### Other
- ✅ Radar
- ✅ Skin Changer
- ✅ Speed Hack
- ✅ Auto Fire

---

## ⚠️ Important Notes

### 1. Update Offsets
After each game update, you **MUST** update memory offsets in `src/main.cpp`:

```cpp
namespace Offsets {
    uintptr_t GWorld               = 0x12345678;  // UPDATE!
    uintptr_t GNames               = 0x87654321;  // UPDATE!
    // ... all other offsets
}
```

Use **Ghidra** or **IDA Pro** to find new offsets.

### 2. Enable Zygisk
1. Open Magisk app
2. Settings → Enable **Zygisk**
3. Reboot

### 3. Game Packages
Module automatically detects:
- `com.tencent.tmgp.pubgmhd` (Global)
- `pubg.krmobile` (Korea)
- `com.vng.pubgmobile` (Vietnam)
- `com.tencent.ig` (China)
- `com.pubg.imobile` (BGMI)

---

## 🛡️ Anti-Cheat Bypass

The module includes:
- Module hiding from `/proc/self/maps`
- Text section CRC spoofing
- Syscall hook bypass

**Recommendations:**
- Use LEGIT profile for safety
- Don't use RAGE mode continuously
- Keep offsets updated
- Use on clean device

---

## 📚 Full Documentation

See [BUILD.md](BUILD.md) for detailed build and configuration instructions.

---

## 🤝 Support

For issues or questions, check the documentation or create an issue in the repository.

---

**⚠️ Legal Disclaimer:**

THIS MODULE IS FOR EDUCATIONAL PURPOSES ONLY.
- Use at your own risk
- Developer is not responsible for any bans
- Do not use in competitive matches
- Respect the game's terms of service
