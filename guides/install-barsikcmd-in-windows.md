# 📘 **Гайд: Как установить BarsikCMD на Windows (через Linux‑среду)**

BarsikCMD — это **нативная Linux‑утилита**.  
На Windows она **не работает напрямую** (CMD/PowerShell не поддерживаются).  

Но её можно установить и использовать на Windows через Linux‑среду:

- **лучший вариант** — виртуальная машина Linux Mint  
- **рабочий, но средний вариант** — WSL (Windows Subsystem for Linux)

---

# 🟢 Вариант 1 — Рекомендуемый  
# **Установка через виртуальную машину Linux Mint**

Это самый стабильный и предсказуемый способ.

---

## 📥 1. Установка Linux Mint в виртуалке

Скачать Linux Mint:  
https://www.linuxmint.com/download.php

1. Установите VirtualBox или VMware  
2. Создайте виртуальную машину:
   - 2–4 GB RAM  
   - 20 GB диска  
3. Установите Linux Mint внутри виртуалки

---

## 📦 2. (Необязательно, но рекомендуется) Установка зависимости для update

BarsikCMD может работать и без неё, но команда `update` будет стабильнее.

```bash
sudo apt install libcurl4-openssl-dev
```

---

## 📥 3. Установка BarsikCMD через .deb (рекомендуется)

Вместо сборки из исходников проще установить готовый пакет.

### Скачивание `.deb` через команду:

```bash
wget https://github.com/barsik0396/BarsikCMD/releases/download/2026.3.3/barsik-cmd_2026.3.3_amd64.deb
```

### Установка:

```bash
sudo dpkg -i barsik-cmd_2026.3.3_amd64.deb
```

Если появятся ошибки зависимостей:

```bash
sudo apt --fix-broken install
```

---

## ▶️ 4. Запуск

```bash
barsikcmd
```

BarsikCMD установлен и готов к работе.

---

# 🟠 Вариант 2 — Рабочий, но не идеальный  
# **Установка через WSL (Windows Subsystem for Linux)**

WSL работает, но стабильность средняя: **⭐⭐⭐☆☆**  
Даже если при установке появляются ошибки зависимостей — BarsikCMD всё равно запускается, и `update` тоже работает.

---

## 📥 1. Установка WSL

В PowerShell от имени администратора:

```powershell
wsl --install
```

После перезагрузки установится Ubuntu.

---

## 📦 2. (Необязательно) Установка зависимости

```bash
sudo apt install libcurl4-openssl-dev
```

---

## 📥 3. Установка BarsikCMD через .deb

```bash
wget https://github.com/barsik0396/BarsikCMD/releases/download/2026.3.3/barsik-cmd_2026.3.3_amd64.deb
sudo dpkg -i barsik-cmd_2026.3.3_amd64.deb
sudo apt --fix-broken install
```

⚠️ Даже если `dpkg` покажет ошибки зависимостей — BarsikCMD всё равно будет работать.

---

## ▶️ 4. Запуск

```bash
barsikcmd
```

Особенности WSL:

- `update` работает, но может вести себя нестабильно  
- графический парольный диалог не появится  
- но сама программа запускается нормально  

---

# 🔴 Вариант 3 — Невозможный  
# **BarsikCMD напрямую в Windows**

BarsikCMD **не работает** в:

- CMD  
- PowerShell  
- Windows Terminal без Linux  

Это не баг — это архитектура проекта.

---

# 📊 Итог

| Способ | Работает | Стабильность | Рекомендация |
|-------|----------|--------------|--------------|
| Виртуальная машина Linux Mint | ✔️ | ⭐⭐⭐⭐⭐ | Лучший вариант |
| WSL | ✔️ | ⭐⭐⭐☆☆ | Работает, но возможны проблемы с update |
| Windows CMD/PowerShell | ❌ | — | Невозможно |