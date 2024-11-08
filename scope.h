#ifndef _SCOPE_H
#define _SCOPE_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "id_attrs.h"
#include "ast.h"
#include "symtab.h"
#include "scope_check.h"
#include "utilities.h"
#include "id_use.h"

// Maximum number of declarations that can be stored in a scope
#define MAX_SCOPE_SIZE 4096

// Association between an identifier and its attributes
typedef struct {
    const char *id;
    id_attrs *attrs;
} scope_assoc_t;

// Definition of a scope structure
// - size: current number of identifiers in this scope
// - loc_count: number of variables and constants declared
typedef struct scope_s {
    unsigned int size;
    unsigned int loc_count;
    scope_assoc_t *entries[MAX_SCOPE_SIZE];
} scope_t;

// Function prototypes for managing scopes
scope_t *scope_create();  // Create a new scope
void scope_destroy(scope_t *scope);  // Destroy a scope and free memory

unsigned int scope_loc_count(scope_t *s);  // Get the count of declarations
unsigned int scope_size(scope_t *s);  // Get the current scope size
bool scope_full(scope_t *s);  // Check if the scope is full

bool scope_declared(scope_t *s, const char *name);  // Check if name is declared in the current scope
void scope_insert(scope_t *s, const char *name, id_attrs *attrs);  // Insert a new identifier with attributes
id_attrs *scope_lookup(scope_t *s, const char *name);  // Lookup an identifier's attributes in the scope

#endif