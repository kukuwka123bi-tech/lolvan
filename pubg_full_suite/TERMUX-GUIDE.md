# PUBG Zygisk Suite - Termux Compilation Guide

## 📱 Compile on Android with Termux

This guide will help you compile the PUBG Zygisk Suite module directly on your Android device using Termux.

---

## ⚙️ Requirements

- **Android device with root**
- **Termux app** (from F-Droid: https://f-droid.org/en/packages/com.termux/)
- **~2GB free storage** (for NDK and compilation)
- **Internet connection**

---

## 🚀 Quick Start (5 minutes)

### Step 1: Install Termux

1. Download and install **Termux from F-Droid** (NOT from Google Play!)
   - [Download Termux](https://f-droid.org/en/packages/com.termux/)

2. Open Termux and run:
```bash
termux-change-repo mirror
```
Select a fast mirror (e.g., `mirror.moe` or `grimler`)

---

### Step 2: Download the Module

In Termux, run:
```bash
cd ~
git clone https://github.com/kukuwka123bi-tech/lolvan.git
cd lolvan/pubg_full_suite
```

---

### Step 3: Run Setup Script

```bash
chmod +x setup-termux.sh
./setup-termux.sh
```

This will:
- Update Termux packages
- Install required tools (git, cmake, clang, etc.)
- Download and install Android NDK (~500MB)
- Set up environment variables

**Note:** This may take 5-10 minutes depending on your internet speed.

---

### Step 4: Compile the Module

```bash
chmod +x build-module.sh
./build-module.sh
```

This will:
- Compile the C++ code
- Create the library file
- Package everything into a ZIP

**Note:** Compilation may take 2-5 minutes.

---

### Step 5: Install via Magisk

1. The compiled module will be at: `pubg_full_suite.zip`

2. Copy it to your device storage:
```bash
cp pubg_full_suite.zip /sdcard/Download/
```

3. Open **Magisk** app
4. Go to **Modules**
5. Tap **Install from storage**
6. Select `pubg_full_suite.zip`
7. **Reboot your device**

---

### Step 6: Enable Zygisk

1. Open **Magisk** app
2. Tap **Settings** (gear icon)
3. Enable **Zygisk**
4. **Reboot your device**

---

### Step 7: Test the Module

1. Open **PUBG Mobile**
2. Check logs with:
```bash
adb logcat | grep PUBG_SUITE
```
Or in Termux:
```bash
logcat | grep PUBG_SUITE
```

You should see:
```
PUBG Zygisk Suite - Initializing...
libUE4.so loaded at: 0x...
libtersafe.so loaded at: 0x...
PUBG Zygisk Suite - READY!
```

---

## 📝 Important Notes

### 1. Update Offsets!

After each game update, you **MUST** update the memory offsets in `src/main.cpp`:

```cpp
namespace Offsets {
    uintptr_t GWorld                    = 0x0;        // UPDATE THIS!
    uintptr_t GNames                    = 0x0;        // UPDATE THIS!
    // ... all other offsets
}
```

**How to find offsets:**
1. Dump `libUE4.so` from `/data/app/com.tencent.tmgp.pubgmhd-.../lib/arm64/`
2. Open in **Ghidra** (on your PC)
3. Search for strings: `"GWorld"`, `"PersistentLevel"`, etc.
4. Find cross-references to get the actual offsets

### 2. Game Compatibility

The module works with:
- PUBG Mobile Global: `com.tencent.tmgp.pubgmhd`
- PUBG Mobile KR: `pubg.krmobile`
- PUBG Mobile VN: `com.vng.pubgmobile`
- PUBG Mobile CN: `com.tencent.ig`
- BGMI: `com.pubg.imobile`

### 3. Troubleshooting

**Problem: Module not loading**
- Check Zygisk is enabled in Magisk
- Verify module is installed: `ls /data/adb/modules/`
- Check logs: `logcat | grep PUBG_SUITE`

**Problem: Game crashes**
- Offsets may be wrong
- Update offsets for your game version

**Problem: Features not working**
- Update offsets
- Check if the game has been updated

---

## 🎯 Features

### ESP
- Box around enemies
- Health bar
- Distance indicator

### Aimbot
- Automatic targeting
- Configurable FOV
- Smoothing
- Bone selection

### Profiles
- **LEGIT**: Stealth mode (low FOV, smooth)
- **RAGE**: Full power (high FOV, instant)
- **OFF**: All features disabled

### Other
- No Recoil
- No Spread
- iPad View (4:3 aspect ratio)
- Skin Changer
- Wallhack
- Radar

---

## 🔧 Customization

Edit `src/main.cpp` to:
- Change offsets
- Modify profiles
- Add new features
- Adjust settings

---

## ⚠️ Legal Disclaimer

**FOR EDUCATIONAL PURPOSES ONLY**

- Use at your own risk
- Developer is not responsible for any bans
- Do not use in competitive matches
- Respect the game's terms of service

---

## 📞 Support

If you have issues:
1. Check this guide again
2. Check the logs: `logcat | grep PUBG_SUITE`
3. Update offsets for your game version
4. Ask for help with specific errors

---

**Good luck and happy gaming!** 🎮
