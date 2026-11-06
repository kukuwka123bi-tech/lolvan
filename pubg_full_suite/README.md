# PUBG Zygisk Suite v2.0.0

**🎮 Полноценный Zygisk модуль для PUBG Mobile с ESP, Aimbot, Skin Changer и Anti-Cheat Bypass**

---

## 📥 СКАЧАТЬ ГОТОВЫЙ МОДУЛЬ

### 🔗 **Прямая ссылка:**
**[СКАЧАТЬ PUBG Zygisk Suite](https://github.com/kukuwka123bi-tech/lolvan/archive/refs/heads/pubg-module.zip)**

---

## 🚀 БЫСТРЫЙ СТАРТ (для Android с root)

**Если у тебя Android с root, выполни эти команды в Termux:**

```bash
cd ~
git clone https://github.com/kukuwka123bi-tech/lolvan.git
cd lolvan/pubg_full_suite
chmod +x INSTALL.sh
./INSTALL.sh
```

**Затем выбери опцию 1 и следуй инструкциям!**

---

## 📁 СТРУКТУРА МОДУЛЯ

```
pubg_full_suite/
├── INSTALL.sh              # 🎯 ОСНОВНОЙ СКРИПТ (запускай этот!)
├── setup-termux.sh         # Установка Termux
├── build-module.sh         # Сборка модуля
├── TERMUX-GUIDE.md         # Руководство по Termux
├── module.prop             # Методанные модуля
├── customize.sh            # Скрипт установки
├── post-fs-data.sh         # Скрипт после монтирования
├── service.sh              # Сервисный скрипт
├── uninstall.sh            # Скрипт удаления
├── config.json             # Конфигурация
├── BUILD.md                # Руководство по сборке
├── META-INF/               # Magisk файлы
│   └── com/google/android/
│       ├── update-binary
│       └── updater-script
├── zygisk/
│   ├── arm64-v8a.so        # ⚠️ Заглушка (нужно скомпилировать!)
│   └── zygisk.hpp          # Zygisk API
└── src/
    ├── main.cpp            # ✅ Основной код
    └── CMakeLists.txt      # Конфигурация сборки
```

---

## 🎯 ЧТО УМЕЕТ МОДУЛЬ

### ✅ ESP (Экранный радар)
- Box - Рамка вокруг врагов
- Health Bar - Полоска здоровья
- Distance - Дистанция до врага
- Skeleton - Скелет врагов

### ✅ Aimbot (Автонаведение)
- FOV: 1-180°
- Сглаживание: 0.01-1.0
- Выбор кости: Голова/Грудь/Таз/Ближайшая
- Проверка видимости
- Игнорирование союзников

### ✅ Профили
| Профиль | FOV | Сглаживание | Кость | No Recoil | Wallhack |
|---------|-----|-------------|-------|-----------|----------|
| **LEGIT** | 3° | 0.08 | Грудь | ❌ | ❌ |
| **RAGE** | 180° | 1.0 | Голова | ✅ | ✅ |
| **OFF** | - | - | - | ❌ | ❌ |

### ✅ Оружейные хаки
- No Recoil - Нет отдачи
- No Spread - Нет разброса

### ✅ Визуальные хаки
- iPad View - Соотношение сторон 4:3
- Wallhack - Видение через стены
- Chams - Окраска моделей
- Glow - Свечение врагов

### ✅ Прочее
- Radar - Минимап
- Skin Changer - Смена скинов
- Speed Hack - Ускорение

---

## 📱 УСТАНОВКА

### 🎯 Способ 1: Через Termux (РЕКОМЕНДУЮ)

1. Установи **Termux** из F-Droid: [Скачать](https://f-droid.org/en/packages/com.termux/)
2. Открой Termux и выполни:
```bash
cd ~
git clone https://github.com/kukuwka123bi-tech/lolvan.git
cd lolvan/pubg_full_suite
chmod +x INSTALL.sh
./INSTALL.sh
```
3. Выбери **опцию 1** (Full Setup + Compile + Install)
4. Дождись окончания установки (~10 минут)
5. Скопруй `pubg_full_suite.zip` в `/sdcard/Download/`
6. Установи через Magisk

### 📦 Способ 2: На ПК

1. Скачай архив: [СКАЧАТЬ](https://github.com/kukuwka123bi-tech/lolvan/archive/refs/heads/pubg-module.zip)
2. Разархивируй
3. Скомпилируй библиотеку (см. BUILD.md)
4. Создай ZIP для Magisk
5. Установи через Magisk

---

## ⚠️ ВАЖНО! ОФФСЕТЫ!

**⚠️ ОБЯЗАТЕЛЬНО ОБНОВИ ОФФСЕТЫ ПЕРЕД ИСПОЛЬЗОВАНИЕМ!**

В файле `src/main.cpp`:
```cpp
namespace Offsets {
    uintptr_t GWorld                    = 0x0;        // ❌ НУЖНО ОБНОВИТЬ!
    uintptr_t GNames                    = 0x0;        // ❌ НУЖНО ОБНОВИТЬ!
    // ... другие оффсеты
}
```

**Как найти оффсеты:**
1. Сделай дамп `libUE4.so` из игры
2. Открой в **Ghidra**
3. Найди оффсеты для всех переменных
4. Обнови их в `main.cpp`
5. Пересобери модуль

---

## 🎮 СОВМЕСТИМОСТЬ

✅ PUBG Mobile Global - `com.tencent.tmgp.pubgmhd`
✅ PUBG Mobile KR - `pubg.krmobile`
✅ PUBG Mobile VN - `com.vng.pubgmobile`
✅ PUBG Mobile CN - `com.tencent.ig`
✅ BGMI - `com.pubg.imobile`

---

## 📚 ДОКУМЕНТАЦИЯ

- **[TERMUX-GUIDE.md](TERMUX-GUIDE.md)** - Подробное руководство
- **[BUILD.md](BUILD.md)** - Сборка и конфигурация

---

## ⚠️ ПРАВОВОЕ ПРЕДУПРЕЖДЕНИЕ

**ТОЛЬКО ДЛЯ ОЗНАКОМИТЕЛЬНЫХ ЦЕЛЕЙ!**
- Используй на свой страх и риск
- Разработчик не несет ответственности за бан

---

**🎉 ВСЁ ГОТОВО!** 🎮
