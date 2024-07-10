#ifndef LOG_H
#define LOG_H

#include "SEGGER_RTT.h"

#define LOGD(format, ...) SEGGER_RTT_printf(0, "%s%s"format"\r\n%s", RTT_CTRL_TEXT_WHITE , "D: ", ##__VA_ARGS__, RTT_CTRL_RESET)
#define LOGI(format, ...) SEGGER_RTT_printf(0, "%s%s"format"\r\n%s", RTT_CTRL_TEXT_GREEN , "I: ", ##__VA_ARGS__, RTT_CTRL_RESET)
#define LOGW(format, ...) SEGGER_RTT_printf(0, "%s%s"format"\r\n%s", RTT_CTRL_TEXT_YELLOW, "W: ", ##__VA_ARGS__, RTT_CTRL_RESET)
#define LOGE(format, ...) SEGGER_RTT_printf(0, "%s%s"format"\r\n%s", RTT_CTRL_TEXT_RED   , "E: ", ##__VA_ARGS__, RTT_CTRL_RESET)
#define LOGF(format, ...) SEGGER_RTT_printf(0, "%s%s"format"\r\n%s", RTT_CTRL_TEXT_BLACK , "F: ", ##__VA_ARGS__, RTT_CTRL_RESET)

#endif /* LOG_H */
