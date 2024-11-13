#ifndef _SCOPE_CHECK_H
#define _SCOPE_CHECK_H
#include "ast.h"
#include "id_use.h"

extern block_t scope_check_program(block_t prog);

extern void scope_check_const_decls(const_decls_t decls);

extern void scope_check_var_decls(var_decls_t decls);

extern void scope_check_proc_decls(proc_decls_t decls);
   

extern void scope_check_stmts(stmts_t stmts);

extern void scope_check_AssignStmt(assign_stmt_t stmt);
extern void scope_check_expr(expr_t expr);
extern void scope_check_ident_declared( file_location floc, const char *name);
extern void scope_check_stmt_list(stmt_list_t stmtList);
extern void scope_check_stmt(stmt_t stmt);
extern void scope_check_condition(condition_t cond);
#endif
