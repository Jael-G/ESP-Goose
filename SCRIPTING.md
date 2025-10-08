# ESPGoose Scripting Language Documentation

ESPGoose uses a custom scripting language inspired by Ducky Script, designed for keystroke and mouse automation on ESP32-S2/S3 boards. This guide explains the available commands, syntax, and usage examples.

---

## 📜 Script Basics
- Each line is a command followed by arguments (if needed).
- Lines are executed sequentially.
- Empty lines are ignored.
- Arguments are separated by spaces.

---

## 🗝️ Supported Commands

### TYPE \<text\>
Types the given text using the keyboard.
```
TYPE Hello, World!
```

### PRESS \<key\>
Presses a key or key code. Accepts key names (see below) or single characters.
```
PRESS KEY_LEFT_CTRL
PRESS a
```

### RELEASE \<key\>
Releases a key or key code.
```
RELEASE KEY_LEFT_CTRL
RELEASE a
```

### RELEASEALL
Releases all pressed keys.
```
RELEASEALL
```

### DELAY \<milliseconds\>
Waits for the specified time (in ms).
```
DELAY 1000
```

### MOUSEDX \<distance\>
Moves the mouse horizontally by the given amount (-127 to 127 per step).
```
MOUSEDX 50
```

### MOUSEDY \<distance\>
Moves the mouse vertically by the given amount (-127 to 127 per step).
```
MOUSEDY -30
```

### LEFTC
Performs a left mouse click.
```
LEFTC
```

### RIGHTC
Performs a right mouse click.
```
RIGHTC
```

---

## 🔑 Key Names
You can use any of the following key names for PRESS/RELEASE commands:
- KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_LEFT_GUI
- KEY_RIGHT_CTRL, KEY_RIGHT_SHIFT, KEY_RIGHT_ALT, KEY_RIGHT_GUI
- KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW
- KEY_MENU, KEY_SPACE, KEY_BACKSPACE, KEY_TAB, KEY_RETURN, KEY_ESC
- KEY_INSERT, KEY_DELETE, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_HOME, KEY_END
- KEY_NUM_LOCK, KEY_CAPS_LOCK
- KEY_F1 to KEY_F24
- KEY_PRINT_SCREEN, KEY_SCROLL_LOCK, KEY_PAUSE
- Numeric keypad: KEY_KP_0 to KEY_KP_9, KEY_KP_SLASH, KEY_KP_ASTERISK, KEY_KP_MINUS, KEY_KP_PLUS, KEY_KP_ENTER, KEY_KP_DOT

Or use single characters (e.g., `a`, `1`, `!`).

---

## 📝 Example Script
```
DELAY 500
TYPE ESPGoose Activated!
PRESS KEY_LEFT_GUI
PRESS r
RELEASEALL
DELAY 200
TYPE cmd
PRESS KEY_RETURN
RELEASE KEY_RETURN
DELAY 500
TYPE echo Hello from ESPGoose!
PRESS KEY_RETURN
RELEASE KEY_RETURN
```

---

## ⚠️ Notes
- Commands are case-sensitive.
- Unknown commands are ignored and logged.
- Mouse movement is limited to ±127 per step (multiple steps allowed).

---

## 📚 More Info
- See the source code in `lib/Interpreter/interpreter.cpp` for advanced usage.
- For questions or contributions, open an issue or pull request!
