#include "symtab.h"
#include "scope.h"
#include "utilities.h"

//symbol table is a stack of scope

// index of the top of the stack of scopes
static int symtab_top = -1;

// the symbol table itself
static scope_t *symtab[MAX_NESTING];

// initialize the symbol table
void symtab_initialize()
{
    // initialize the internal state
    symtab_top = -1;
    for (int i = 0; i < MAX_NESTING; i++) {
	symtab[i] = NULL;
    }
}

//returns number of scopes currently in symtab
unsigned int symtab_size(){
    return symtab_top + 1;
}

//returns true if symtab is empty
bool symtab_empty(){
    return (symtab_size() == 0);
}

// Return the current scope's next location count (of variables).
unsigned int symtab_scope_loc_count(){
    return scope_loc_count(symtab[symtab_top]);
}

// Return the current scope's size (the number of declared ids).
unsigned int symtab_scope_size(){
    return scope_size(symtab[symtab_top]);
}

//checks if the current scope is full
bool symtab_scope_full(){
    return scope_full(symtab[symtab_top]);
}

// Return the current nesting level of the symbol table.
// The first enterscope will have this return 0 as its nesting level.
unsigned int symtab_current_nesting_level(){
    // assert(symtab_top_idx >= 0);
    return symtab_top;
}

//checks if the symboltable is full
bool symtab_full(){
    return symtab_current_nesting_level() == MAX_NESTING - 1;
}

//checks if a name was declared in any scope in the symbol table
bool symtab_declared(const char *name){
    return symtab_lookup(name) != NULL;
}

//checks if a name was declared in the current scope
bool symtab_declared_in_current_scope(const char *name){
    id_attrs *attrs = scope_lookup(symtab[symtab_top], name);
    return attrs != NULL;
}

// Put the given name, which is to be declared with kind k,
// and has its declaration at the given file location (floc),
// into the current scope's symbol table at the offset scope_next_offset().
static void add_ident(scope_t *s, const char *name, id_attrs *attrs)
{
    id_attrs *old_attrs = scope_lookup(s, name);
    if (old_attrs != NULL) {
        bail_with_prog_error(attrs->file_loc,
		      "symtab_insert called with an already declared variable\"%s\"!",
		      name);
    } else {
	scope_insert(s, name, attrs);
    }
}

// Requires: !symtab_declared_in_current_scope(name) && attrs != NULL.
// If !symtab_declared_in_current_scope(name), then modify the current scope
// to add an association from the given name to attrs.
void symtab_insert(const char *name, id_attrs *attrs){
    add_ident(symtab[symtab_top], name, attrs);
}

// Requires: !symtab_full()
// Start a new scope (for a procedure)
void symtab_enter_scope()
{
    symtab_top++;
    symtab[symtab_top] = scope_create();
}

// Requires: !symtab_empty()
void symtab_leave_scope()
{
    if (symtab_top < 0) {
	bail_with_error("Cannot leave scope, no scope on symtab's stack!");
    }
    symtab_top--;
}

// Return (a pointer to) the attributes of the given name 
// or NULL if there is no association for name in the symbol table.
// (this looks back through all scopes).
id_use *symtab_lookup(const char *name)
{
    unsigned int levelsOut = 0;
    for (int level = symtab_top; 0 <= level; level--) {
        id_attrs *attrs = scope_lookup(symtab[level], name);
        if (attrs != NULL) {
            return id_use_create(attrs, levelsOut);
        }
        levelsOut++;
    }
    return NULL;
}

// We'll use lexical addresses in HW4...
// Requires: symtab_declared(name)
// Return (a pointer to) the lexical address of the given name
// or NULL if there is no association for name.
/*
lexical_address *symtab_lexical_address(const char *name)
{
    // maintaining: -1 <= level <= symtab_top_idx;
    // maintaining: (for all int j:
    //                level < j <= symtab_top_idx
    //                   ==> !scope_declared(symtab[j], name))
    for (int level = symtab_top_idx; 0 <= level; level--) {
	id_attrs *attrs = scope_lookup(symtab[level], name);
	if (attrs != NULL) {
	    return lexical_address_create(symtab_top_idx - level,
					  attrs->loc_offset);
	}
    }
    bail_with_error("Couldn't find %s for symtab_lexical_address!", name);
    return NULL;
}
*/
