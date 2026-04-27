![2025121622130700-68C370F3B4A0DB855DFC57E1427942CF (1)](https://github.com/user-attachments/assets/954bedf0-7081-4b3a-9c81-287893aa2682)

![2025121622130700-68C370F3B4A0DB855DFC57E1427942CF](https://github.com/user-attachments/assets/99c10818-1d18-4fef-bd06-cfc0e616c647)

# sys-icon

`sys-icon` is an Atmosphère **sysmodule** for Nintendo Switch that intercepts  
`ns:GetApplicationControlData` to provide **modified application metadata**
(such as icons and names) **at runtime**.

It is intended for **homebrew tools, launchers, and the Nintendo HOME menu**.

---

## ✅ What sys-icon CAN do

- Override application **icons and names at runtime**
- Works on **firmware 22.1.0 and under**
- Works with **latest Atmosphère**
- Changes are visible on the **Nintendo HOME menu**
- Works with **homebrew launchers and tools**
- Does **not** modify installed game files
- Does **not** require reinstalling games

---

## ❌ What sys-icon CANNOT do

- Work on firmware above **22.1.0**
- Work with NXThemes
- Modify installed NSP / NCA files

---

## 📍 Where changes are visible

| Location | Icon override |
|----------|---------------|
| HOME menu | ✅ Yes |
| Sphaira | ✅ Yes |
| Homebrew tools | ✅ Yes |

---

## 🛠 Requirements

- Nintendo Switch with custom firmware
- Atmosphère (latest recommended)
- Firmware **22.1.0 or below**

---

## 🔧 How to build

Make sure `devkitA64` is installed.

### Clone the repository

```bash
git clone https://github.com/eradicatinglove/sys-icon.git
cd sys-icon
make clean
make FEAT_ALL="Y" TOGL_LOGGING="Y"
```
