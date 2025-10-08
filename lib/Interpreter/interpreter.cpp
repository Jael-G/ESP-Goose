#include <Arduino.h>
#include "interpreter.h"


std::map<String, std::function<void(const String &)>> commandMap = {
    {"TYPE", actionType},
    {"PRESS", actionPress},
    {"RELEASE", actionRelease},
    {"RELEASEALL", actionReleaseAll},
    {"DELAY", actionDelay},
    {"MOUSEDX", actionMoveMouseDX},
    {"MOUSEDY", actionMoveMouseDY},
    {"LEFTC", actionLeftClick},
    {"RIGHTC", actionRightClick}};

std::map<String, uint8_t> keyMap = {
    {"KEY_LEFT_CTRL", KEY_LEFT_CTRL},
    {"KEY_LEFT_SHIFT", KEY_LEFT_SHIFT},
    {"KEY_LEFT_ALT", KEY_LEFT_ALT},
    {"KEY_LEFT_GUI", KEY_LEFT_GUI},
    {"KEY_RIGHT_CTRL", KEY_RIGHT_CTRL},
    {"KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT},
    {"KEY_RIGHT_ALT", KEY_RIGHT_ALT},
    {"KEY_RIGHT_GUI", KEY_RIGHT_GUI},

    {"KEY_UP_ARROW", KEY_UP_ARROW},
    {"KEY_DOWN_ARROW", KEY_DOWN_ARROW},
    {"KEY_LEFT_ARROW", KEY_LEFT_ARROW},
    {"KEY_RIGHT_ARROW", KEY_RIGHT_ARROW},
    {"KEY_MENU", KEY_MENU},
    {"KEY_SPACE", KEY_SPACE},
    {"KEY_BACKSPACE", KEY_BACKSPACE},
    {"KEY_TAB", KEY_TAB},
    {"KEY_RETURN", KEY_RETURN},
    {"KEY_ESC", KEY_ESC},
    {"KEY_INSERT", KEY_INSERT},
    {"KEY_DELETE", KEY_DELETE},
    {"KEY_PAGE_UP", KEY_PAGE_UP},
    {"KEY_PAGE_DOWN", KEY_PAGE_DOWN},
    {"KEY_HOME", KEY_HOME},
    {"KEY_END", KEY_END},
    {"KEY_NUM_LOCK", KEY_NUM_LOCK},
    {"KEY_CAPS_LOCK", KEY_CAPS_LOCK},
    {"KEY_F1", KEY_F1},
    {"KEY_F2", KEY_F2},
    {"KEY_F3", KEY_F3},
    {"KEY_F4", KEY_F4},
    {"KEY_F5", KEY_F5},
    {"KEY_F6", KEY_F6},
    {"KEY_F7", KEY_F7},
    {"KEY_F8", KEY_F8},
    {"KEY_F9", KEY_F9},
    {"KEY_F10", KEY_F10},
    {"KEY_F11", KEY_F11},
    {"KEY_F12", KEY_F12},
    {"KEY_F13", KEY_F13},
    {"KEY_F14", KEY_F14},
    {"KEY_F15", KEY_F15},
    {"KEY_F16", KEY_F16},
    {"KEY_F17", KEY_F17},
    {"KEY_F18", KEY_F18},
    {"KEY_F19", KEY_F19},
    {"KEY_F20", KEY_F20},
    {"KEY_F21", KEY_F21},
    {"KEY_F22", KEY_F22},
    {"KEY_F23", KEY_F23},
    {"KEY_F24", KEY_F24},
    {"KEY_PRINT_SCREEN", KEY_PRINT_SCREEN},
    {"KEY_SCROLL_LOCK", KEY_SCROLL_LOCK},
    {"KEY_PAUSE", KEY_PAUSE},

    // Numerickeypad
    {"KEY_KP_SLASH", KEY_KP_SLASH},
    {"KEY_KP_ASTERISK", KEY_KP_ASTERISK},
    {"KEY_KP_MINUS", KEY_KP_MINUS},
    {"KEY_KP_PLUS", KEY_KP_PLUS},
    {"KEY_KP_ENTER", KEY_KP_ENTER},
    {"KEY_KP_1", KEY_KP_1},
    {"KEY_KP_2", KEY_KP_2},
    {"KEY_KP_3", KEY_KP_3},
    {"KEY_KP_4", KEY_KP_4},
    {"KEY_KP_5", KEY_KP_5},
    {"KEY_KP_6", KEY_KP_6},
    {"KEY_KP_7", KEY_KP_7},
    {"KEY_KP_8", KEY_KP_8},
    {"KEY_KP_9", KEY_KP_9},
    {"KEY_KP_0", KEY_KP_0},
    {"KEY_KP_DOT", KEY_KP_DOT},
};

/**
 * @brief Returns the key code for a given key string, or the ASCII value of the first character if not found.
 * @param key The key string to look up.
 * @return The key code or ASCII value.
 */
uint8_t getCodeOrChar(const String &key)
{
    auto it = keyMap.find(key);
    if (it != keyMap.end())
    {
        INTERPRETER_LOGD("getCodeOrChar - key found: " + key + " -> code: " + String(it->second));
        return it->second;
    }

    INTERPRETER_LOGD("getCodeOrChar - key not found: " + key + " -> using ASCII: " + String(key[0]));
    return key[0];
}

/**
 * @brief Splits a script string into lines and stores them in an output array.
 * @param input The input script as a single string.
 * @param output The array to store each line.
 * @param maxLines The maximum number of lines to store.
 * @return The number of lines split and stored in output.
 */
int splitScript(const String &input, String output[], int maxLines)
{
    String tempStr = "";
    int outputIndex = 0;

    for (int i = 0; i < input.length(); i++)
    {
        char character = input[i];

        if (character == '\n')
        {
            if (tempStr.length() > 0)
            {
                if (outputIndex < maxLines)
                {
                    output[outputIndex] = tempStr;
                    outputIndex++;
                }
            }
            tempStr = "";
        }
        else
        {
            tempStr += character;
        }
    }

    if (tempStr.length() > 0 && outputIndex < maxLines)
    {
        output[outputIndex] = tempStr;
        outputIndex++;
    }

    return outputIndex;
}

/**
 * @brief Runs a script by executing each line using runLine.
 * @param script Array of script lines.
 * @param length Number of lines in the script.
 */
void runScript(const String script[], int length)
{
    for (int i = 0; i < length; i++)
    {

        String line = script[i];

        String temp = line;
        temp.trim();

        if (temp.length() == 0)
            continue; // empty line removed

        runLine(line);
    }
}

/**
 * @brief Parses and executes a single line of script by mapping the command to its action.
 * @param line The script line to execute.
 */
void runLine(const String &line)
{
    int spaceIndex = line.indexOf(' ');

    String cmd = (spaceIndex == -1) ? line : line.substring(0, spaceIndex);
    cmd.trim();

    String arg = (spaceIndex == -1) ? "" : line.substring(spaceIndex + 1);

    auto it = commandMap.find(cmd);
    if (it != commandMap.end())
    {
        it->second(arg);
        return;
    }

    INTERPRETER_LOGD("Unknown command: '" + String(cmd) + "'");
}

/**
 * @brief Types the given text using the Keyboard library.
 * @param text The text to type.
 */
void actionType(const String &text)
{
    INTERPRETER_LOGD("actionType: " + text);
    Keyboard.print(text);
}

/**
 * @brief Presses a key or key code using the Keyboard library.
 * @param key The key string or character to press.
 */
void actionPress(const String &key)
{
    int code = getCodeOrChar(key);
    INTERPRETER_LOGD("actionPress: " + key + " -> code: " + String(code));
    Keyboard.press(code);
}
/**
 * @brief Releases a key or key code using the Keyboard library.
 * @param key The key string or character to release.
 */
void actionRelease(const String &key)
{
    int code = getCodeOrChar(key);
    INTERPRETER_LOGD("actionRelease: " + key + " -> code: " + String(code));
    Keyboard.release(code);
}

/**
 * @brief Releases all pressed keys using the Keyboard library.
 * @param dummyArg Unused argument (for compatibility).
 */
void actionReleaseAll(const String & = "")
{
    INTERPRETER_LOGD("actionReleaseAll");
    Keyboard.releaseAll();
}

/**
 * @brief Delays execution for a specified number of milliseconds.
 * @param ms The delay time in milliseconds as a string.
 */
void actionDelay(const String &ms)
{
    int delayTime = ms.toInt();
    INTERPRETER_LOGD("actionDelay: " + String(delayTime) + " ms");
    delay(delayTime);
}

/**
 * @brief Moves the mouse horizontally by the specified amount.
 * @param dx The distance to move in the X direction as a string.
 */
void actionMoveMouseDX(const String &dx)
{
    int intDx = dx.toInt();
    INTERPRETER_LOGD("actionMoveMouseDX: " + String(intDx));

    while (intDx != 0)
    {
        int8_t step = std::clamp(intDx, -127, 127);
        Mouse.move(step, 0);
        intDx -= step;
    }
}

/**
 * @brief Moves the mouse vertically by the specified amount.
 * @param dy The distance to move in the Y direction as a string.
 */
void actionMoveMouseDY(const String &dy)
{
    int intDy = dy.toInt();
    INTERPRETER_LOGD("actionMoveMouseDY: " + String(intDy));
    Serial.println(intDy);

    while (intDy != 0)
    {
        int8_t step = std::clamp(intDy, -127, 127);
        Mouse.move(0, step);
        intDy -= step;
    }
}

/**
 * @brief Performs a left mouse click.
 * @param dummyArg Unused argument (for compatibility).
 */
void actionLeftClick(const String &dummyArg)
{
    INTERPRETER_LOGD("actionLeftClick");
    Mouse.click(MOUSE_LEFT);
}

/**
 * @brief Performs a right mouse click.
 * @param dummyArg Unused argument (for compatibility).
 */
void actionRightClick(const String &dummyArg)
{
    INTERPRETER_LOGD("actionRightClick");
    Mouse.click(MOUSE_RIGHT);
}
