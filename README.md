
![2025121622130700-68C370F3B4A0DB855DFC57E1427942CF (1)](https://github.com/user-attachments/assets/954bedf0-7081-4b3a-9c81-287893aa2682)





![2025121622130700-68C370F3B4A0DB855DFC57E1427942CF](https://github.com/user-attachments/assets/99c10818-1d18-4fef-bd06-cfc0e616c647)


![2025122002192600-DB1426D1DFD034027CECDE9C2DD914B8](https://github.com/user-attachments/assets/06cc5b60-9d1d-4f8b-8e8e-b66ae3b22c4d)



# sys-icon
### *A lightweight Atmosphère sysmodule for per-title icon & name overrides*  
by **eradicatinglove**

---

## ⚠️ Important Notice

To see your **custom icons** and **edited titles**, you **MUST** use **my modded version of uLaunch** found here (https://github.com/eradicatinglove/uLaunch/releases).

The official Nintendo Switch HOME Menu **does not refresh icons** on modern firmwares.  
Only my **modded uLaunch** reads `sys-icon`’s live override data.

> ❗ **Without my modded uLaunch, your new icons will NOT show.**

---

## 📌 What Does sys-icon Do?

`sys-icon` intercepts `ns:GetApplicationControlData` to provide **modified application metadata at runtime**  
(such as custom icons, title names, author strings, and display versions).

### ✔️ sys-icon CAN:
- Override icons and names **dynamically**
- Work on **firmware 21.0.0+**
- Work with **latest Atmosphère**
- Avoid NSP rebuilding or reinstalling games
- Provide overrides to homebrew launchers that read live control data

### ❌ sys-icon CANNOT:
> sys-icon **cannot update Nintendo HOME Menu icons** on FW 21.0.0+  
because Nintendo now caches icons permanently when a title is installed.

To change HOME menu icons, you must **rebuild and reinstall the NSP**.

---

## 📍 Where Changes Are Visible

| Location | Icon Override |
|----------|----------------|
| HOME Menu | ❌ No |
| Sphaira | ✅ Yes |
| uLaunch (modded version) | ✅ Yes |
| Other Homebrew Readers | ✅ Yes |

---

# 🛠 Requirements

From your original GitHub README (integrated here):

- Nintendo Switch with custom firmware  
- **Atmosphère (latest recommended)**  
- **Firmware 21.0.0+**  
- Tools that support live overrides (Sphaira, modded uLaunch)  
- **devkitA64** (if building from source)  

---

# 🔧 How to Build sys-icon  

Make sure `devkitA64` is installed.

### 1. Clone the repository

```bash
git clone https://github.com/eradicatinglove/sys-icon.git
cd sys-icon
```

### 2. Build the sysmodule

```bash
make clean
make dist FEAT_ALL="Y" TOGL_LOGGING="Y"
```

The compiled module will appear in the `dist/` directory.

---

# 📁 Installation

### **1. Download & Extract sys-icon**
Open `sys-icon.zip` using **7-Zip** (recommended).

Inside you will find an `atmosphere/` folder.  
Drag this folder to the **root of your SD card**:

```
SD:/atmosphere/
```

### **2. Confirm sysmodule location**

```
sd:/atmosphere/contents/0x00FF69636F6EFF00/
```

You do **not** need to modify anything inside this folder.

---

# 🖼 Setting Up Custom Icons & Titles

### Step 1 — Go to:

```
sd:/atmosphere/contents/
```

### Step 2 — Find your game’s Title ID

Use my app Title-Id-Lister, Tinfoil.io, DBI, NX-Shell, or any homebrew title manager.

Example:

```
Street Fighter Collection → 0100024008310000
```

### Step 3 — Create a folder for that Title ID:

```
sd:/atmosphere/contents/0100024008310000/
```

### Step 4 — Add the config file

Place your provided `config.ini` inside the folder.
note* if you dont want to change the title name dont add config.ini

Default example:

```ini
[override_nacp]
name=
author=eradicatinglove
display_version=1.0.0
```

### Step 5 — Edit it

```ini
[override_nacp]
name=Street Fighter Collection
author=eradicatinglove
display_version=1.0.0
```

---

# 🖼 Adding Your Custom Icon

- Must be named: **icon.jpg**
- Must be **256×256**
- Format: JPG
- Suggested quality: **~80%**

Place it next to your config:

```
sd:/atmosphere/contents/[TITLEID]/config.ini
sd:/atmosphere/contents/[TITLEID]/icon.jpg
```

---

# 🔄 Reboot the Switch

A reboot is required for sys-icon to load your overrides.

---

# 🚀 Launch With My Modded uLaunch  
⭐ **REQUIRED** ⭐

My modded uLaunch can be found here (https://github.com/eradicatinglove/uLaunch/releases) reads the updated control data and displays the:

- custom icons  
- custom titles  
- custom author strings  

The HOME Menu cannot display these changes.

---

# 🔧 Troubleshooting

**Icon not showing?**
- Ensure file is named `icon.jpg`
- Confirm resolution is exactly **256×256**
- Check JPG quality (~80%)
- Make sure you used **my modded uLaunch**
- Reboot the device

**Title not updating?**
- Confirm Title ID folder name is correct
- Ensure config uses this format:

```ini
[override_nacp]
name=Your Name
author=Your Author
display_version=1.0.0
```

- Reboot

---

# 📂 Folder Structure Example

```
SD:/atmosphere/
└── contents/
    ├── sys-icon/                <-- sysmodule
    └── 0100024008310000/        <-- your game
        ├── config.ini
        └── icon.jpg
```

---

# Credits

- Created by **eradicatinglove**
- Inspired by sys-tweak workflow but rebuilt cleaner






