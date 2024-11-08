#include <stdio.h>
#include <stdlib.h>
#include "parser.h"//not sure if it will be needed here, but left it in case
#include "id_attrs.h"
#include "ast.h"
#include "symtab.h"
#include "scope_check.h"
#include "utilities.h"
#include "id_use.h"

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