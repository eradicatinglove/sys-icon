
![2025121622130700-68C370F3B4A0DB855DFC57E1427942CF (1)](https://github.com/user-attachments/assets/954bedf0-7081-4b3a-9c81-287893aa2682)





![2025121622130700-68C370F3B4A0DB855DFC57E1427942CF](https://github.com/user-attachments/assets/99c10818-1d18-4fef-bd06-cfc0e616c647)







# sys-icon

`sys-icon` is an Atmosphère **sysmodule** for Nintendo Switch that intercepts  
`ns:GetApplicationControlData` to provide **modified application metadata**
(such as icons and names) **at runtime**.

It is intended for **homebrew tools and launchers** that read live control data.

---

## ⚠️ Important limitation

> **sys-icon cannot change game icons on the Nintendo HOME menu on firmware 21.0.0+.**

Starting with firmware 21.0.0, the HOME menu:
- extracts icons at install time
- stores them in an internal cache
- never reloads them dynamically

Because of this, **no sysmodule or theme can change HOME menu icons at runtime**.

To change HOME icons, you must **rebuild and reinstall the NSP**.

---

## ✅ What sys-icon CAN do

- Override application **icons and names at runtime**
- Works on **firmware 21.0.0+**
- Works with **latest Atmosphère**
- Does **not** modify installed game files
- Does **not** require reinstalling games

Visible in tools that use live control data:
- Sphaira
- Other homebrew applications

---

## ❌ What sys-icon CANNOT do

- Change icons on the Nintendo HOME menu
- Work with NXThemes
- Modify installed NSP / NCA files

---

## 📍 Where changes are visible

| Location | Icon override |
|--------|----------------|
| HOME menu | ❌ No |
| Sphaira | ✅ Yes |
| Homebrew tools | ✅ Yes |

---

## 🛠 Requirements

- Nintendo Switch with custom firmware
- Atmosphère (latest recommended)
- Firmware 21.0.0+
- devkitPro with devkitA64 installed

---

## 🔧 How to build

Make sure `devkitA64` is installed.

---

### 2. Clone the repository

```bash
git clone https://github.com/eradicatinglove/sys-icon.git
cd sys-icon
make clean
make FEAT_ALL="Y" TOGL_LOGGING="Y"


