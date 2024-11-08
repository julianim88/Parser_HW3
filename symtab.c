#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "id_attrs.h"
#include "ast.h"
#include "symtab.h"
#include "scope_check.h"
#include "utilities.h"
#include "id_use.h"

static scope_t *symtab[MAX_NESTING];
static int symtab_top_idx = -1;

// Initialize the symbol table
void symtab_initialize() {
    symtab_top_idx = -1;
    for (int i = 0; i < MAX_NESTING; i++) {
        symtab[i] = NULL;
    }
}

// Return the number of scopes in the symbol table
unsigned int symtab_size() {
    return symtab_top_idx + 1;
}

// Check if the symbol table is empty
bool symtab_empty() {
    return symtab_top_idx == -1;
}

// Check if the symbol table is full
bool symtab_full() {
    return symtab_top_idx >= MAX_NESTING - 1;
}

// Start a new scope (for a block or procedure)
void symtab_enter_scope() {
    if (symtab_full()) {
        bail_with_error("Symbol table is full; cannot enter new scope!");
    }
    symtab[++symtab_top_idx] = scope_create();
}

// Exit the current scope, freeing its resources
void symtab_leave_scope() {
    if (symtab_empty()) {
        bail_with_error("No scope to leave; symbol table is empty!");
    }
    scope_destroy(symtab[symtab_top_idx--]);
}

// Return the count of variable and constant declarations in the current scope
unsigned int symtab_scope_loc_count() {
    return scope_loc_count(symtab[symtab_top_idx]);
}

// Return the size of the current scope
unsigned int symtab_scope_size() {
    return scope_size(symtab[symtab_top_idx]);
}

// Check if the current scope is full
bool symtab_scope_full() {
    return scope_full(symtab[symtab_top_idx]);
}

// Return the current nesting level
unsigned int symtab_current_nesting_level() {
    return symtab_top_idx;
}

// Check if an identifier is declared in any scope (backwards through stack)
bool symtab_declared(const char *name) {
    return symtab_lookup(name) != NULL;
}

// Check if an identifier is declared in the current scope only
bool symtab_declared_in_current_scope(const char *name) {
    return scope_declared(symtab[symtab_top_idx], name);
}

// Insert an identifier with attributes into the current scope
void symtab_insert(const char *name, id_attrs *attrs) {
    scope_insert(symtab[symtab_top_idx], name, attrs);
}

// Lookup an identifier in all scopes, returning an id_use pointer if found
id_use *symtab_lookup(const char *name) {
    for (int level = symtab_top_idx; level >= 0; level--) {
        id_attrs *attrs = scope_lookup(symtab[level], name);
        if (attrs != NULL) {
            return id_use_create(attrs, symtab_top_idx - level);
        }
    }
    return NULL;
}
Updated scope_check.c
This file traverses the AST and checks for undeclared or duplicate identifiers, creating the symbol table as it processes variable declarations.

c
Copy code
#include "scope_check.h"
#include "symtab.h"
#include "error.h"

void scope_check_program(program_t prog) {
    symtab_enter_scope();
    scope_check_varDecls(prog.var_decls);
    prog.stmts = scope_check_stmts(prog.stmts);
    symtab_leave_scope();
}

void scope_check_varDecls(var_decls_t vds) {
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL) {
        scope_check_varDecl(*vdp);
        vdp = vdp->next;
    }
}

void scope_check_varDecl(var_decl_t vd) {
    scope_check_idents(vd.idents, vd.type);
}

void scope_check_idents(idents_t ids, type_exp_e t) {
    ident_t *idp = ids.idents;
    while (idp != NULL) {
        scope_check_declare_ident(*idp, t);
        idp = idp->next;
    }
}

void scope_check_declare_ident(ident_t id, type_exp_e t) {
    if (symtab_declared_in_current_scope(id.name)) {
        bail_with_prog_error(*(id.file_loc), "Variable \"%s\" has already been declared!", id.name);
    } else {
        int loc_count = symtab_scope_loc_count();
        id_attrs *attrs = id_attrs_loc_create(*(id.file_loc), t, loc_count);
        symtab_insert(id.name, attrs);
    }
}

stmt_t scope_check_stmt(stmt_t stmt) {
    switch (stmt.stmt_kind) {
        case assign_stmt:
            stmt.data.assign_stmt = scope_check_assignStmt(stmt.data.assign_stmt);
            break;
        // Other statement types
        default:
            bail_with_error("Invalid statement in scope_check_stmt!");
    }
    return stmt;
}

assign_stmt_t scope_check_assignStmt(assign_stmt_t stmt) {
    stmt.idu = scope_check_ident_declared(*(stmt.file_loc), stmt.name);
    *stmt.expr = scope_check_expr(*(stmt.expr));
    return stmt;
}

id_use *scope_check_ident_declared(file_location floc, const char *name) {
    id_use *use = symtab_lookup(name);
    if (!use) {
        bail_with_prog_error(floc, "Identifier \"%s\" is not declared!", name);
    }
    return use;
}