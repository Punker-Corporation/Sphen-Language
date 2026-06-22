#ifndef SPHEN_DEBUG_H
#define SPHEN_DEBUG_H

#include "log.h"

#ifdef SPHEN_DEBUG
	#define DEBUG_LOG(text) _print_log(text);
	#define DEBUG_FORMAT(format, ...) _print_format(format, __VA_ARGS__);
	#define DEBUG_TEXT_F(s, size, ...) _text_format(s, size, __VA_ARGS__);
#else
	#define DEBUG_LOG(text)
	#define DEBUG_FORMAT(format, ...)
	#define DEBUG_TEXT_F(s, size, ...)
#endif

#endif
