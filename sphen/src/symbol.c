#include <stdlib.h>
#include <string.h>
#include "symbol.h"

static inline u32_t hash(const char* str) {
    u32_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
            
    return hash % HASH_SIZE;
}

SymbolTable create_symbol_table() {
    SymbolTable st;
    st.current_scope = NULL;
    return st;
}

void enter_scope(SymbolTable* st) {
    Scope* new_scope = (Scope*)calloc(1, sizeof(Scope));
    new_scope->parent = st->current_scope;
    st->current_scope = new_scope;
}

void exit_scope(SymbolTable* st) {
    if (!st->current_scope) return;
    
    Scope* old_scope = st->current_scope;
    st->current_scope = old_scope->parent;

	for(u32_t i = 0; i < HASH_SIZE; i++){
		Symbol* cur = old_scope->table[i];
		while(cur){
			Symbol* next = cur->next;
			free(cur->name);
			free(cur->type_name);
			free(cur);
			cur = next;
		}
	}
    
    free(old_scope); 
}


// Insere um símbolo no escopo ATUAL
bool insert_symbol(SymbolTable* st, char* name, SymbolKind kind, char* type) {
    if (!st->current_scope) return false;
    if (!name || !type) return false;

    u32_t index = hash(name);
    
    Symbol* current = st->current_scope->table[index];
    while (current) {
        if (memcmp(current->name, name, strlen(name)) == 0) return false;
        current = current->next;
    }

    Symbol* new_sym = (Symbol*)malloc(sizeof(Symbol));
    if (!new_sym) return false;
    memset(new_sym, 0, sizeof(Symbol));
    new_sym->name = (char*)malloc(strlen(name) + 1);
    new_sym->type_name = (char*)malloc(strlen(type) + 1);
	if(!new_sym->name || !new_sym->type_name){
		free(new_sym->name);
		free(new_sym->type_name);
		free(new_sym);
		return false;
	}

    new_sym->kind = kind;
    memcpy(new_sym->type_name, type, strlen(type)+1);
    
    new_sym->next = st->current_scope->table[index];
    st->current_scope->table[index] = new_sym;
    
    return true;
}

Symbol* lookup_symbol(SymbolTable* st, const char* name) {
    Scope* scope = st->current_scope;
    u32_t index = hash(name);

    while (scope) {
        Symbol* current = scope->table[index];
        while (current) {
            if (strcmp(current->name, name) == 0)
                return current;
            
            current = current->next;
        }
        scope = scope->parent;
    }
    return NULL;
}
