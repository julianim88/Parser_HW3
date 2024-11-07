#ifndef _SCOPE_CHECK_H
#define _SCOPE_CHECK_H


// Function prototypes for declaration checking and AST traversal

// Build the symbol table for the given program AST
// and check the AST for duplicate declarations or undeclared identifiers
program_t scope_check_program(program_t prog);

// Check declarations in varDecls and build symbol table
void scope_check_varDecls(var_decls_t vds);

// Add declarations for a single variable declaration, checking for duplicates
void scope_check_varDecl(var_decl_t vd);

// Add declarations for all identifiers in the list to the symbol table,
// reporting any duplicate declarations
void scope_check_idents(idents_t ids, type_exp_e t);

// Add a declaration for a single identifier,
// checking for duplicate declarations
void scope_check_declare_ident(ident_t id, type_exp_e t);

// Check that all identifiers referenced in the statement are declared,
// return the modified AST with id_use pointers
stmt_t scope_check_stmt(stmt_t stmt);

// Check that all identifiers referenced in the assignment statement are declared,
// return the modified AST with id_use pointers
assign_stmt_t scope_check_assignStmt(assign_stmt_t stmt);

// Check that all identifiers referenced in the begin statement are declared
// and check for duplicate declarations, return the modified AST
begin_stmt_t scope_check_beginStmt(begin_stmt_t stmt);

// Check that all identifiers referenced in the statement list are declared,
// return the modified AST with id_use pointers
stmts_t scope_check_stmts(stmts_t stmts);

// Check that all identifiers referenced in the if statement are declared,
// return the modified AST with id_use pointers
if_stmt_t scope_check_ifStmt(if_stmt_t stmt);

// Check that the identifier used in the read statement is declared,
// return the modified AST with id_use pointers
read_stmt_t scope_check_readStmt(read_stmt_t stmt);

// Check that the expression in the write statement has all identifiers declared,
// return the modified AST with id_use pointers
write_stmt_t scope_check_writeStmt(write_stmt_t stmt);

// Check that all identifiers in the expression are declared,
// return the modified AST with id_use pointers
expr_t scope_check_expr(expr_t exp);

// Check identifiers in a binary operation expression for declarations,
// return the modified AST with id_use pointers
binary_op_expr_t scope_check_binary_op_expr(binary_op_expr_t exp);

// Check that the identifier in an identifier expression is declared,
// return the modified AST with id_use pointers
ident_t scope_check_ident_expr(ident_t id);

// Check that the given identifier has been declared, produce error if not,
// and return id_use with its attributes
id_use *scope_check_ident_declared(file_location floc, const char *name);

#endif