
![2025121622072900-57B4628D2267231D57E0FC1078C0596D](https://github.com/user-attachments/assets/c2f8ff28-c0e8-465b-982e-d06f799ed9b8)




![2025121622130700-68C370F3B4A0DB855DFC57E1427942CF](https://github.com/user-attachments/assets/e5dc685b-d5c4-4687-8559-2b5e1faffbf4)








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


