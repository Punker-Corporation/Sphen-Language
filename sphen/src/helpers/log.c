#include "log.h"
#include "str.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


static char* getfg(const char* key, char len){
    switch(len){
        case 3: 
            if(memcmp(key, "red", 3)==0) return "31";
        case 4: 
            if(memcmp(key, "cyan", 4)==0) return "36";
            if(memcmp(key, "blue", 4)==0) return "34";
        case 5: 
            if(memcmp(key, "clear", 3)==0) return "39";
            if(memcmp(key, "black", 5)==0) return "30";
            if(memcmp(key, "white", 5)==0) return "37";
            if(memcmp(key, "green", 5)==0) return "32";
        case 6:
            if(memcmp(key, "yellow", 6)==0) return "33";
        case 7:
            if(memcmp(key, "magenta", 7)==0) return "35";
        default: return "39";
    }
}

static char* getbg(const char* key, char len){
    switch(len){
        case 3: 
            if(memcmp(key, "red", 3)==0) return ";41";
        case 4: 
            if(memcmp(key, "cyan", 4)==0) return ";46";
            if(memcmp(key, "blue", 4)==0) return ";44";
        case 5: 
            if(memcmp(key, "clear", 3)==0) return ";49";
            if(memcmp(key, "black", 5)==0) return ";40";
            if(memcmp(key, "white", 5)==0) return ";47";
            if(memcmp(key, "green", 5)==0) return ";42";
        case 6:
            if(memcmp(key, "yellow", 6)==0) return ";43";
        case 7:
            if(memcmp(key, "magenta", 7)==0) return ";45";
        default: return ";49";
    }
}

static char* getattr(const char* key, char len){
    switch(len){
        case 4:
            if(memcmp(key, "slim", 4)==0) return ";2";
            if(memcmp(key, "bold", 4)==0) return ";1";
        case 5:
            if(memcmp(key, "blink", 5)==0) return ";5";
        case 6:
            if(memcmp(key, "italic", 6)==0) return ";3";
            if(memcmp(key, "strike", 6)==0) return ";9";
        case 7:
            if(memcmp(key, "reverse", 7)==0) return ";7";
        case 9:
            if(memcmp(key, "underline", 9)==0) return ";4";
        default: return ";1";
    }
}
void _text_format(char* s, unsigned long int size, const char* fmt, ...){
	va_list args;
	if(!s){
		puts("TEXT_FORMAT:STRING:NULL");
		exit(EXIT_FAILURE);
	};
    va_start(args, fmt);
   	vsnprintf(s, size, fmt, args);
	va_end(args);
}
void _print_format(const char* fmt, ...){
	char msg[LOG_SIZE];
	va_list args;
	
    va_start(args, fmt);
   	vsnprintf(msg, LOG_SIZE, fmt, args);
	va_end(args);
	_print_log(msg);
}


void _print_log(const char* text){
    
    str_t str = str_new(NULL);
    
    int i = 0;
    while(text[i]){
        if(text[i] == '$' && text[i+1] == '<'){
            i += 2;
            str_t esc = str_new(NULL);
            int index = 0;
            str_app(&str, "\e[");
            while(text[i] && text[i] != ':'){
                if(!isalpha(text[i]) && text[i]!=':'){ i++; continue; }
                while(isalpha(text[i])) 
                	str_push(&esc, text[i++]);

                switch(index) {
                    case 0: 	str_app(&str, getfg(esc.data, esc.len)); break;
                    case 1: 	str_app(&str, getbg(esc.data, esc.len)); break;
                    default: 	str_app(&str, getattr(esc.data, esc.len)); break;
                }
                str_free(&esc);
                index++;
            }
            i++; // skip the comma or closing '>'
            //str_free(esc.data);
            str_push(&str, 'm');
            while(text[i] && !(text[i] == '>' && text[i+1] == '$'))
                str_push(&str, text[i++]);
            if(text[i] == '>' && text[i+1] == '$'){
                i += 2;
            	str_app(&str, "\e[0m");
            }
            continue;
        }
        str_push(&str, text[i++]);
    }
    str_app(&str, "\e[0m");
    puts(str.data);
    str_free(&str);
}

