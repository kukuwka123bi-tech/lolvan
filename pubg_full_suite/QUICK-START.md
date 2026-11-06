# 🚀 БЫСТРЫЙ СТАРТ - PUBG Zygisk Suite

**Для Android с root правами**

---

## 📱 ВСЁ ЗА 5 МИНУТ

### Шаг 1: Установи Termux
1. Скачай **Termux из F-Droid** (НЕ из Google Play!)
   - [Скачать Termux](https://f-droid.org/en/packages/com.termux/)
2. Установи и открой Termux

---

### Шаг 2: Запусти установку
В Termux выполни **все команды по порядку:**

```bash
cd ~
```

```bash
git clone https://github.com/kukuwka123bi-tech/lolvan.git
```

```bash
cd lolvan/pubg_full_suite
```

```bash
chmod +x INSTALL.sh
```

```bash
./INSTALL.sh
```

---

### Шаг 3: Выбери опцию

В меню выбери:
- **1** → Full Setup + Compile + Install (РЕКОМЕНДУЮ)

Дождись окончания установки (~10 минут)

---

### Шаг 4: Установи модуль

После компиляции:

1. **Скопруй файл** в память устройства:
```bash
cp pubg_full_suite.zip /sdcard/Download/
```

2. **Открой Magisk**
3. **Modules → Install from storage**
4. **Выбери pubg_full_suite.zip**
5. **Перезагрузи устройство**

---

### Шаг 5: Включи Zygisk

1. Открой **Magisk**
2. **Settings** (шестерёнка)
3. **Включи Zygisk**
4. **Перезагрузи устройство**

---

## ✅ ПРОВЕРКА РАБОТЫ

Запусти PUBG и проверь логи:
```bash
logcat | grep PUBG_SUITE
```

Ты должен увидеть:
```
PUBG Zygisk Suite - Initializing...
libUE4.so loaded at: 0x...
libtersafe.so loaded at: 0x...
PUBG Zygisk Suite - READY!
```

---

## ⚠️ ВАЖНО! ОФФСЕТЫ!

**ПЕРЕД ИСПОЛЬЗОВАНИЕМ ОБЯЗАТЕЛЬНО ОБНОВИ ОФФСЕТЫ!**

Открой файл:
```bash
nano src/main.cpp
```

Найди секцию:
```cpp
namespace Offsets {
    uintptr_t GWorld                    = 0x0;        // ❌ ИЗМЕНИ!
    uintptr_t GNames                    = 0x0;        // ❌ ИЗМЕНИ!
    // ...
}
```

**Как найти оффсеты:**
1. Скачай **Ghidra** на ПК: [ghidra-sre.org](https://ghidra-sre.org/)
2. Сделай дамп `libUE4.so` из игры:
   ```bash
   adb pull /data/app/com.tencent.tmgp.pubgmhd-.../lib/arm64/libUE4.so
   ```
3. Открой в Ghidra и найди оффсеты
4. Обнови их в `main.cpp`
5. Пересобери модуль:
   ```bash
   ./build-module.sh
   ```

---

## 🎯 УПРАВЛЕНИЕ

### Сменить профиль:
Открой `src/main.cpp` и измени:
```cpp
ApplyProfile(Profile::LEGIT);  // LEGIT, RAGE или OFF
```

Затем пересобери модуль.

---

## 🐞 РЕШЕНИЕ ПРОБЛЕМ

### Модуль не устанавливается
- ✅ Проверь, что Magisk установлен
- ✅ Проверь, что Zygisk включен
- ✅ Проверь, что файл `pubg_full_suite.zip` в правильной папке

### Игра вылетает
- ❌ Оффсеты неверные
- ✅ Обнови оффсеты для своей версии игры

### Функции не работают
- ❌ Оффсеты неверные
- ✅ Обнови оффсеты
- ✅ Проверь, что модуль загружен

---

## 📌 ПОЛЕЗНЫЕ КОМАНДЫ

| Команда | Описание |
|---------|----------|
| `./INSTALL.sh` | Главное меню |
| `./setup-termux.sh` | Установка Termux |
| `./build-module.sh` | Сборка модуля |
| `logcat \| grep PUBG_SUITE` | Просмотр логов |
| `nano src/main.cpp` | Редактировать код |

---

## 📥 СКАЧАТЬ ГОТОВЫЙ АРХИВ

Если не хочешь использовать Termux:

**[СКАЧАТЬ PUBG Zygisk Suite](https://github.com/kukuwka123bi-tech/lolvan/archive/refs/heads/pubg-module.zip)**

---

**🎉 ВСЁ ГОТОВО! Удачной игры!** 🎮
