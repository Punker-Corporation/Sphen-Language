#ifndef SPHEN_DIAGNOSTIC_H
#define SPHEN_DIAGNOSTIC_H
#include "../helpers/log.h"

#define DIAG_LOG(text) _print_log(text);
#define DIAG_FORMAT(format, ...) _print_format(format, __VA_ARGS__);
#define DIAG_TEXT_F(s, size, ...) _text_format(s, size, __VA_ARGS__);

#endif
