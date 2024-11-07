#include "scope.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "id_attrs.h"
#include "ast.h"
#include "symtab.h"
#include "scope_check.h"
#include "utilities.h"
#include "id_use.h"


// Create a new scope, allocating memory and initializing attributes
scope_t *scope_create() {
    scope_t *new_scope = (scope_t *)malloc(sizeof(scope_t));
    if (!new_scope) {
        bail_with_error("No space for new scope!");
    }
    new_scope->size = 0;
    new_scope->loc_count = 0;
    for (int i = 0; i < MAX_SCOPE_SIZE; i++) {
        new_scope->entries[i] = NULL;
    }
    return new_scope;
}

// Return the number of variable and constant declarations in this scope
unsigned int scope_loc_count(scope_t *s) {
    return s->loc_count;
}

// Return the current scope size (number of declared identifiers)
unsigned int scope_size(scope_t *s) {
    return s->size;
}

// Check if the current scope is full
bool scope_full(scope_t *s) {
    return s->size >= MAX_SCOPE_SIZE;
}

// Insert a new identifier with attributes into the scope
void scope_insert(scope_t *s, const char *name, id_attrs *attrs) {
    if (scope_declared(s, name)) {
        bail_with_error("Duplicate declaration in current scope for \"%s\"", name);
    }
    if (scope_full(s)) {
        bail_with_error("Scope is full!");
    }
    scope_assoc_t *new_assoc = (scope_assoc_t *)malloc(sizeof(scope_assoc_t));
    if (!new_assoc) {
        bail_with_error("Unable to allocate space for scope association!");
    }
    new_assoc->id = strdup(name);
    new_assoc->attrs = attrs;
    attrs->loc_offset = s->loc_count++; // Set location and increment location count
    s->entries[s->size++] = new_assoc;
}

// Check if an identifier is declared in the current scope
bool scope_declared(scope_t *s, const char *name) {
    return scope_lookup(s, name) != NULL;
}

// Look up an identifier in the current scope, returning its attributes if found
id_attrs *scope_lookup(scope_t *s, const char *name) {
    for (int i = 0; i < s->size; i++) {
        if (strcmp(s->entries[i]->id, name) == 0) {
            return s->entries[i]->attrs;
        }
    }
    return NULL;  // Not found
}

// Destroy a scope, freeing memory allocated for its entries
void scope_destroy(scope_t *s) {
    if (s) {
        for (int i = 0; i < s->size; i++) {
            free(s->entries[i]->id); // Free the identifier name
            free(s->entries[i]);
        }
        free(s);
    }
}