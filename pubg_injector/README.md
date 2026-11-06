# PUBG Injector APK Project

**🎮 Android APK с инъекцией в PUBG через Zygisk**

---

## 📥 СКАЧАТЬ ПРОЕКТ

**[СКАЧАТЬ PUBG Injector Project](https://github.com/kukuwka123bi-tech/lolvan/archive/refs/heads/pubg-module.zip)**

После скачивания открой архив и найди папку `pubg_injector/`

---

## 🚀 КАК СОБРАТЬ APK

### Шаг 1: Установи Android Studio
- Скачай: [developer.android.com/studio](https://developer.android.com/studio)
- Установи все компоненты (SDK, NDK, CMake)

### Шаг 2: Открой проект
- File → Open → выбери папку `pubg_injector/`
- Дождись синхронизации Gradle

### Шаг 3: Собери APK
- Build → Build APK → выбери `release`
- Дождись окончания (~2-5 минут)

### Шаг 4: Найди APK
- APK будет в: `pubg_injector/app/build/outputs/apk/release/app-release.apk`

---

## 📱 КАК ИСПОЛЬЗОВАТЬ

1. Установи APK на устройство
2. Включи Zygisk в Magisk
3. Запусти PUBG Injector
4. Нажми START
5. Запусти PUBG

---

## ⚠️ ВАЖНО!

**ОБЯЗАТЕЛЬНО ОБНОВИ ОФФСЕТЫ ПЕРЕД ИСПОЛЬЗОВАНИЕМ!**

Открой файлы:
- `pubg_injector/app/src/main/jni/native-lib.cpp`
- `pubg_injector/app/src/main/zygisk/zygisk-module.cpp`

Найди и обнови оффсеты для своей версии PUBG.

---

## 🎯 ФУНКЦИИ

✅ ESP (Box, Health, Distance, Skeleton)  
✅ Aimbot (FOV, Smoothing, Bone Selection)  
✅ No Recoil / No Spread  
✅ iPad View (4:3 Aspect Ratio)  
✅ Wallhack / Chams / Glow  
✅ Radar  
✅ Skin Changer  
✅ Anti-Cheat Bypass  

---

## 📁 СТРУКТУРА

```
pubg_injector/
├── README.md
├── settings.gradle
├── build.gradle
└── app/
    ├── build.gradle
    └── src/main/
        ├── java/com/pubg/injector/
        │   ├── MainActivity.java
        │   ├── MainApp.java
        │   └── NativeLoader.java
        ├── jni/
        │   ├── native-lib.cpp
        │   ├── zygisk.hpp
        │   └── CMakeLists.txt
        ├── zygisk/
        │   ├── zygisk-module.cpp
        │   └── CMakeLists.txt
        └── res/
            └── layout/activity_main.xml
```

---

## 🎮 СОВМЕСТИМОСТЬ

✅ PUBG Mobile Global  
✅ PUBG Mobile KR  
✅ PUBG Mobile VN  
✅ PUBG Mobile CN  
✅ BGMI  

---

**🎉 ВСЁ ГОТОВО! Собирай и используй!** 🎮
