#pragma once
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

// Master switch
#define DEBUG_MODE 1

#if DEBUG_MODE
    #define LOG(tag, msg, level) do { \
        const char* color = (strcmp(level, "E") == 0) ? COLOR_RED : \
                            (strcmp(level, "W") == 0) ? COLOR_YELLOW : \
                            (strcmp(level, "I") == 0) ? COLOR_GREEN : \
                            (strcmp(level, "D") == 0) ? COLOR_WHITE : \
                            COLOR_CYAN; \
        Serial.printf("[%lu] %s (%s:%d) %s(): %s[%s]: %s%S\n", \
            millis(), level, __FILE__, __LINE__, __func__, color, tag, String(msg).c_str(), COLOR_RESET); \
    } while(0)

    // Interpreter logs
    #define INTERPRETER_LOGE(msg) LOG("Interpreter", msg, "E")
    #define INTERPRETER_LOGW(msg) LOG("Interpreter", msg, "W")
    #define INTERPRETER_LOGI(msg) LOG("Interpreter", msg, "I")
    #define INTERPRETER_LOGD(msg) LOG("Interpreter", msg, "D")
    #define INTERPRETER_LOGV(msg) LOG("Interpreter", msg, "V")

    // Server logs
    #define SERVER_LOGE(msg) LOG("Server", msg, "E")
    #define SERVER_LOGW(msg) LOG("Server", msg, "W")
    #define SERVER_LOGI(msg) LOG("Server", msg, "I")
    #define SERVER_LOGD(msg) LOG("Server", msg, "D")
    #define SERVER_LOGV(msg) LOG("Server", msg, "V")

#else

  #define LOG(tag, msg, level)
  #define INTERPRETER_LOGE(msg)
  #define INTERPRETER_LOGW(msg)
  #define INTERPRETER_LOGI(msg)
  #define INTERPRETER_LOGD(msg)
  #define INTERPRETER_LOGV(msg)
  #define SERVER_LOGE(msg)
  #define SERVER_LOGW(msg)
  #define SERVER_LOGI(msg)
  #define SERVER_LOGD(msg)
  #define SERVER_LOGV(msg)

#endif
