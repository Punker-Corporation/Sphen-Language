#ifndef SPHEN_SYMBOL_H
#define SPHEN_SYMBOL_H

/*
	under development
*/

#include "helpers/arena.h"
#define HASH_SIZE 256

typedef enum {
    SYM_VAR,
    SYM_TYPE,
    SYM_FUNC,
    SYM_CLASS,
    SYM_STRUCT,
    SYM_ENUM
} SymbolKind;

typedef struct Symbol {
    char* name;
    SymbolKind kind;
    char* type_name;
    
    union {
        struct { bool is_const; } var_data;
        struct { char** param_types; int param_count; } func_data;
        struct { struct Scope* members; } class_struct_data; 
    } details;

    struct Symbol* next;
} Symbol;

typedef struct Scope {
    Symbol* table[HASH_SIZE];
    struct Scope* parent;
} Scope;

typedef struct {
    Scope* current_scope;
} SymbolTable;


extern SymbolTable create_symbol_table();
extern void enter_scope(SymbolTable* st);
extern void exit_scope(SymbolTable* st);
extern bool insert_symbol(SymbolTable* st, char* name, SymbolKind kind, char* type);
extern Symbol* lookup_symbol(SymbolTable* st, const char* name);

#endif
