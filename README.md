# sys-icon

`sys-icon` is an Atmosphère **sysmodule** for Nintendo Switch that intercepts `ns:am2` and `ns:ro` to provide **custom application icons and metadata at runtime** — including on the Nintendo HOME menu (qlaunch).

---

## ✅ Compatibility

| Home Menu / Launcher | Icon Override |
|---|---|
| qlaunch (Nintendo HOME menu) | ✅ Yes |
| uLaunch | ✅ Yes |
| Sphaira | ✅ Yes |
| Other homebrew launchers | ✅ Yes (if they use ns:am2 or ns:ro) |

> **Tested on Firmware 21.2.0 / Atmosphère 1.10.2**

---

## 📁 Icon File Setup

Place your icon files in the game's content folder on your SD card:

```
SD:/atmosphere/contents/<TitleID>/
    icon.jpg        ← 256x256 JPEG  (required)
    icon174.jpg     ← 174x174 JPEG  (optional — for "All Software" view)
    config.ini      ← optional metadata overrides
```

### Icon Requirements

| File | Size | Notes |
|---|---|---|
| `icon.jpg` | 256x256 | Required. Shows on HOME menu and most views |
| `icon174.jpg` | 174x174 | Optional. Shows in "All Software" and Options sidebar |

> If `icon174.jpg` is not present, the original icon will show in those views.

### config.ini Format

```ini
[override_nacp]
name=My Custom Name
author=My Author
display_version=1.0
```

---

## 🛠 Requirements

- Nintendo Switch with custom firmware
- Atmosphère 1.10.2 or later
- Firmware 21.0.0 or later
- devkitPro with devkitA64

---

## 🔧 Building

### 1. Clone the repository

```bash
git clone https://github.com/eradicatinglove/sys-icon.git
cd sys-icon
```

### 2. Pull the Atmosphere libraries

```bash
cd lib/ams
git init
git remote add origin https://github.com/Atmosphere-NX/Atmosphere.git
git fetch --depth 1 origin 61ac03e22d20460f2032a2b733d35a86bbc870e0
git checkout FETCH_HEAD
cd ../..
```

### 3. Build libstratosphere

```bash
cd lib/ams/libraries/libstratosphere
make
cd ../../../..
```

### 4. Build the module

```bash
make clean && make
```

The output will be placed in:
```
out/atmosphere/contents/00FF69636F6EFF00/exefs.nsp
out/atmosphere/contents/00FF69636F6EFF00/flags/boot2.flag
```

---

## 📦 Installation

Copy the output folder to your SD card:

```
SD:/atmosphere/contents/00FF69636F6EFF00/exefs.nsp
SD:/atmosphere/contents/00FF69636F6EFF00/flags/boot2.flag
```

Reboot your Switch. The module loads automatically at boot.

---

## 🔄 Updating Atmosphere Libraries

When a new Atmosphere release comes out:

1. Get the new commit hash from https://github.com/Atmosphere-NX/Atmosphere/releases
2. Update the lib:
```bash
cd lib/ams
git fetch --depth 1 origin <new-commit-hash>
git checkout FETCH_HEAD
cd libraries/libstratosphere
make clean && make
cd ../../../..
make clean && make
```

---

## 📝 Logging

A log file is written to `SD:/sys-icon.txt` on every boot. Check this file if icons are not showing — it will tell you exactly which title IDs were intercepted and whether icon files were found.

---

## License

GPL-2.0 — see [LICENSE](LICENSE)
