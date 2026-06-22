#ifndef SPHEN_LOG_H
#define SPHEN_LOG_H

#define LOG_SIZE 256
extern void _print_log(const char* text);
extern void _print_format(const char* format, ...);
extern void _text_format(char* s, unsigned long int size, const char* format, ...);

#endif
