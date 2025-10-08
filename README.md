# ESPGoose

A powerful, open-source keystroke and mouse automation tool for ESP32-S2 and ESP32-S3 boards, inspired by the legendary Rubber Ducky. ESPGoose uses its own scripting language (see `SCRIPTING.md`) and is controlled via a built-in WiFi web interface.

---

## 🚀 Features
- **ESPGoose Script**: Write and run payloads in a custom scripting language inspired by Ducky Script.
- **WiFi-Controlled**: Configure and launch payloads from any device via a web interface.
- **Plug-and-Play**: Just flash, plug in, and unleash your payloads.
- **Cross-Platform**: Works on Windows, macOS, and Linux targets.
- **Modular Design**: Clean codebase for easy customization and extension.
- **Open Source**: MIT licensed, hackable, and community-driven.
---

## 🌐 WiFi Control & Web Interface
ESPGoose is controlled entirely over WiFi:

1. Power up your ESP32-S2/S3 board.
2. Connect to its WiFi network:
   - **SSID:** `ESP32-S2`
   - **Password:** `password`
   - (Defaults can be changed in `src/main.cpp`)
3. Open your browser and go to [http://10.0.0.2](http://10.0.0.2)
4. Use the web interface to upload and run scripts, control mouse, and more.

![Web Interface Preview](https://via.placeholder.com/600x300?text=ESPGoose+Web+UI+Preview)

*Replace this image with a real screenshot in the future!*

---

## 📦 Project Structure
```
include/         # Project includes
lib/             # Libraries (common utilities, interpreter)
src/             # Main source files
platformio.ini   # PlatformIO configuration
```

---

## ⚡ Supported Hardware
**Only compatible with ESP32-S2 and ESP32-S3 boards!**
- Tested on Lolin S2 Mini and ESP32-S3-GEEK
- Other S2/S3 boards may work, but are not officially supported

---

## 🛠️ Getting Started
1. **Clone the repo**
   ```sh
   git clone https://github.com/Jael-G/ESP-Goose.git
   ```
2. **Open with PlatformIO (VS Code recommended)**
3. **Connect your ESP32-S2/S3 board**
4. **Upload the firmware**

---

## 📝 Usage
- Write your payloads using the ESPGoose Scripting Language (see `SCRIPTING.md`).
- Connect to the ESPGoose WiFi and use the web interface to upload and run your script.
- Flash and run!

---

## 📚 Documentation
- [ESPGoose Scripting Language](SCRIPTING.md)
- See `lib/Interpreter/` for interpreter details
- More docs coming soon!

---

## 🤝 Contributing
Pull requests, issues, and feature suggestions are welcome! Please follow the standard GitHub workflow.

---

## 📄 License
MIT License. See `LICENSE` for details.

---

## 💡 Inspiration
Inspired by [Hak5 Rubber Ducky](https://shop.hak5.org/products/usb-rubber-ducky) and the open-source community.

---

## ⚠️ Disclaimer
This tool is for educational and authorized testing purposes only. Do not use on devices you do not own or have explicit permission to test.

