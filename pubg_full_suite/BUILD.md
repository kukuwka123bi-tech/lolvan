# PUBG Zygisk Suite - Build & Installation Guide

## Module Information

- **Module ID:** `pubg_full_suite`
- **Name:** PUBG Zygisk Suite (ESP/Aim/Skin/Bypass)
- **Version:** v2.0.0
- **Author:** dj
- **Description:** Full Zygisk mod suite for PUBG Mobile

---

## Prerequisites

- **Android NDK** r25+ (Recommended: r26b)
- **CMake** 3.18+
- **Magisk** 24.0+ with Zygisk enabled
- **Zygisk API Headers** (`zygisk.hpp`)

---

## Building the Module

### Step 1: Set up NDK
```bash
export NDK_HOME=$HOME/Android/Sdk/ndk/26.2.11394342
```

### Step 2: Build
```bash
cd pubg_full_suite/src
mkdir -p build && cd build
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$NDK_HOME/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-26 \
  -DANDROID_STL=c++_static
make -j$(nproc)
```

### Step 3: Copy library
```bash
cp libpubg_suite.so ../../zygisk/arm64-v8a.so
```

---

## Installation

### Method 1: Magisk App
1. Open Magisk app
2. Modules → Install from storage
3. Select `pubg_full_suite.zip`
4. Reboot

### Method 2: ADB
```bash
adb push pubg_full_suite.zip /sdcard/Download/
adb shell su -c "magisk --install-module /sdcard/Download/pubg_full_suite.zip"
adb reboot
```

---

## Game Compatibility

- PUBG Mobile Global: `com.tencent.tmgp.pubgmhd`
- PUBG Mobile KR: `pubg.krmobile`
- PUBG Mobile VN: `com.vng.pubgmobile`
- PUBG Mobile CN: `com.tencent.ig`
- BGMI: `com.pubg.imobile`

---

## Features

### ESP
- Box, Skeleton, Health Bar, Distance, Name
- Configurable colors and visibility

### Aimbot
- FOV: 1-180 degrees
- Smoothing: 0.01-1.0
- Bone selection: Head, Chest, Pelvis, Nearest
- Visibility check
- Team check

### Profiles
- **LEGIT**: Low FOV (3°), smooth (0.08), chest aim
- **RAGE**: 180° FOV, instant (1.0), head aim
- **OFF**: All features disabled

### Weapon Hacks
- No Recoil
- No Spread

### Visuals
- iPad View (aspect ratio 4:3)
- Wallhack
- Chams
- Glow

### Other
- Radar
- Skin Changer
- Speed Hack
- Auto Fire

---

## Important Notes

1. **Update Offsets**: After each game update, you MUST update memory offsets in `main.cpp`
2. **Enable Zygisk**: In Magisk settings, enable Zygisk and reboot
3. **Test First**: Always test with LEGIT profile before using RAGE
4. **Use Responsibly**: High risk of detection with RAGE mode

---

## Troubleshooting

### Module Not Loading
- Check Zygisk is enabled
- Verify module is installed: `ls /data/adb/modules/`
- Check logs: `cat /cache/magisk.log`

### Game Crashes
- Offsets may be wrong
- Check logcat: `adb logcat | grep -i pubg`

### Features Not Working
- Update offsets for your game version
- Test with different profiles

---

## Legal Disclaimer

**FOR EDUCATIONAL PURPOSES ONLY**
- Use at your own risk
- Developer is not responsible for bans
- Do not use in competitive matches
- Respect game's terms of service
