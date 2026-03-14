# BarsikCMD — Командная строка Барсика 😼
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Linux-blue)
![Version](https://img.shields.io/badge/version-2026.3.3-orange)
![Status](https://img.shields.io/badge/status-active-success)
![Open Source](https://img.shields.io/badge/open--source-yes-brightgreen)


**BarsikCMD** — это лёгкая, быстрая и полностью **open‑source** командная строка, созданная как альтернатива стандартным терминалам.  
Проект открыт на 100%: **вы можете изменять код, добавлять фичи и делать форки — лицензия это разрешает.**

---

# ❓ Почему именно BarsikCMD?

## 1. 🚀 Быстрота
BarsikCMD запускается мгновенно, работает без задержек и обновляется очень быстро.

## 2. ✅ Простота
Никаких сложных настроек:
- не нужно указывать сервер обновлений,
- установка занимает пару секунд,
- документация понятная,
- команды интуитивные.

## 3. 👀 Приватность
BarsikCMD **не собирает данные**:
- нет телеметрии,
- нет сохранения истории команд,
- подключение к серверу происходит только для загрузки обновлений и DEB‑пакетов.

## 4. 📂 Открытость
Исходники доступны полностью.  
Код лежит в папке `code`, и каждый может посмотреть, как всё работает внутри.

---

# 🆕 Последние релизы

| Название       | Версия   | Что нового                                   | Поддерживается | GitHub Releases | Скачать DEB |
|----------------|----------|-----------------------------------------------|----------------|-----------------|-------------|
| Update fix     | 2026.3.3 | Исправлено поведение команды `update`         | ✅             | [тут](https://github.com/barsik0396/BarsikCMD/releases/tag/2026.3.3) | [тут](https://github.com/barsik0396/BarsikCMD/releases/download/2026.3.3/barsik-cmd_2026.3.3_amd64.deb) |
| First update   | 2026.3.2 | Добавлена команда `update` и цветной вывод    | ✅             | [тут](https://github.com/barsik0396/BarsikCMD/releases/tag/2026.3.2) | [тут](https://github.com/barsik0396/BarsikCMD/releases/download/2026.3.2/barsik-cmd_2026.3.2_amd64.deb) |
| First release  | 2026.3.1 | Первый релиз                                  | ⚠️             | [тут](https://github.com/barsik0396/BarsikCMD/releases/tag/2026.3.1) | [тут](https://github.com/barsik0396/BarsikCMD/releases/download/2026.3.1/barsik-cmd_2026.3.1_amd64.deb) |

---

# 🚀 Быстрый старт

1. Скачайте последний релиз:  
   **DEB 2026.3.3** → https://github.com/barsik0396/BarsikCMD/releases/tag/2026.3.3  
2. Установите пакет.
3. Запустите команду:  
   ```
   barsikcmd
   ```
4. Готово — BarsikCMD работает!

---

# ◼️ Основные команды

> Формат: `команда [необязательный] <обязательный>`

| Команда | Использование       | Описание |
|---------|----------------------|----------|
| `clear` | `clear`              | Очистить экран |
| `echo`  | `echo [текст]`       | Вывести текст |
| `exit`  | `exit`               | Выйти из BarsikCMD |
| `help`  | `help`               | Список команд |
| `update`| `update`             | Обновить BarsikCMD |
| `ver`   | `ver`                | Показать версию |

---

# ❌ Типичные ошибки и решения

## Ошибка: «Более поздняя версия уже установлена»
Это значит, что у вас стоит более новая версия.

**Решение:**
```bash
sudo apt remove barsikcmd
```

## Ошибка: «curl вернул ошибку» при `update`
**Решения:**
1. Не нажимайте CTRL+C во время обновления.  
2. Проверьте интернет‑соединение.

---

# ❔ FAQ

<details>
<summary>Почему BarsikCMD только для Linux?</summary>

BarsikCMD разрабатывается под Linux, потому что автор проекта перешёл на Linux и не планирует создавать Windows‑версию.
</details>

<details>
<summary>Есть ли документация?</summary>

Да! Документация находится здесь:  
https://github.com/barsik0396/BarsikCMD/wiki
</details>

<details>
<summary>Когда появится язык автоматизации BarsikCMD?</summary>

Планируется в версиях **2026.3.5 — 2026.4.0**.  
Расширение для VS Code — примерно **2026.5.4 — 2026.6.1**.
</details>

<details>
<summary>Почему BarsikCMD не популярен?</summary>

Основные причины:
1. Небольшое количество команд.  
2. Трудоёмкая реализация функции «Try BarsikCMD» на сайте (занимает от 30 минут до 3 дней).
</details>

---

# 🐾 Дополнительно
История создания проекта: **Creating.md**  
Исходники: `/code`  
Лицензия: свободная, разрешает модификации и форки.
