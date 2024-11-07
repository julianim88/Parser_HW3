/* $Id: symtab.h,v 1.5 2023/11/03 12:29:45 leavens Exp $ */
#ifndef _SYMTAB_H
#define _SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"//not sure if this is needed here, but leaving it in case
#include "id_attrs.h"
#include "ast.h"
#include "symtab.h"
#include "scope_check.h"
#include "utilities.h"
#include "id_use.h"

// Maximum nesting level of scopes
#define MAX_NESTING 100

// Function prototypes for managing the symbol table
void symtab_initialize();  // Initialize the symbol table
unsigned int symtab_size();  // Get the number of scopes in the symbol table
bool symtab_empty();  // Check if the symbol table is empty
bool symtab_full();  // Check if the symbol table is full

void symtab_enter_scope();  // Start a new scope
void symtab_leave_scope();  // Exit the current scope

unsigned int symtab_scope_loc_count();  // Get count of declarations in current scope
unsigned int symtab_scope_size();  // Get current scope size
bool symtab_scope_full();  // Check if the current scope is full
unsigned int symtab_current_nesting_level();  // Get current nesting level

bool symtab_declared(const char *name);  // Check if name is declared in any scope
bool symtab_declared_in_current_scope(const char *name);  // Check if name is declared in the current scope
void symtab_insert(const char *name, id_attrs *attrs);  // Insert identifier with attributes into current scope
id_use *symtab_lookup(const char *name);  // Lookup an identifier across all scopes

#endif