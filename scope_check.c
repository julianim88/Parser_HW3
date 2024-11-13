#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "id_attrs.h"
#include "file_location.h"
#include "ast.h"
#include "utilities.h"
#include "symtab.h"
#include "scope_check.h"



block_t scope_check_program(block_t prog){
    symtab_enter_scope();

    scope_check_const_decls(prog.const_decls);
    scope_check_var_decls(prog.var_decls);
    scope_check_proc_decls(prog.proc_decls);
    scope_check_stmts(prog.stmts);
    symtab_leave_scope();
    return prog;
}

//scope checks const_decls
void scope_check_const_decls(const_decls_t decls){
    const_decl_t* constDecl = decls.start;
    while(constDecl != NULL){
        //chech each def in def_list
        const_def_t* constDef = constDecl->const_def_list.start;

        while(constDef != NULL){
            const char* name = constDef->ident.name;

            id_attrs *attrs = create_id_attrs(*constDef->ident.file_loc, constant_idk, symtab_scope_loc_count());

            //check for already declared error
            if(!symtab_declared_in_current_scope(name) && attrs != NULL){
                symtab_insert(name, attrs);
            }
            else{
                int kind = symtab_lookup(name)->attrs->kind;//gets the type of what the name was declared as originally

                //checks if the previously declared name was a constant or a variable
                if(kind == 0){ //constant
                    bail_with_prog_error(*constDef->file_loc, "constant \"%s\" is already declared as a constant", name);
                }
                else{ //variable 
                    bail_with_prog_error(*constDef->file_loc, "constant \"%s\" is already declared as a variable", name);
                }
            }
            //scope_check_const_delc(*constDecl->next); I don't think we need this
            constDef = constDef->next;
        }
        constDecl = constDecl->next;
    }
}

void scope_check_var_decls(var_decls_t decls)
{
    //implement kind of same as constant check
    var_decl_t* varDecl = decls.var_decls;

    while(varDecl != NULL){
        ident_t *identLi = varDecl->ident_list.start;

        while(identLi != NULL){
            const char* name = identLi->name;

            id_attrs *attrs = create_id_attrs(*identLi->file_loc, variable_idk, symtab_scope_loc_count());

             //check for already declared error
            if(!symtab_declared_in_current_scope(name) && attrs != NULL){
                symtab_insert(name, attrs);
            }
            else{
                int kind = symtab_lookup(name)->attrs->kind; //gets the type of what the name was declared as originally

                //checks if the previously declared name was a constant or a variable
                if(kind == 0){ //constant
                    bail_with_prog_error(*identLi->file_loc, "variable \"%s\" is already declared as a constant", name);
                }
                else{ //variable 
                    bail_with_prog_error(*identLi->file_loc, "variable \"%s\" is already declared as a variable", name);
                }
            }

            identLi=identLi->next;

        }
        varDecl = varDecl->next;
    }
    
}

void scope_check_stmts(stmts_t stmts){
    scope_check_stmt_list(stmts.stmt_list);
}

void scope_check_stmt_list(stmt_list_t stmtList){
    stmt_t* stmt = stmtList.start;
    while(stmt != NULL){
        scope_check_stmt(*stmt);
        stmt = stmt->next;
    }
}

void scope_check_ident_declared( file_location floc, const char *name){

    if(symtab_lookup(name) == NULL){
        bail_with_prog_error(floc, "identifier \"%s\" is not declared!", name);
    }




}


//scope checks expresion
void scope_check_expr(expr_t exp){
    
    switch (exp.expr_kind) {
        case expr_bin:
        scope_check_expr(*exp.data.binary.expr1);
        scope_check_expr(*exp.data.binary.expr2);
        break;
        case expr_negated:
        scope_check_expr(*exp.data.negated.expr);
        break;
        case expr_ident:
        scope_check_ident_declared(*exp.file_loc,exp.data.ident.name);
        break;
        case expr_number:
        break;
    }


}
//scope checks the assign statement
void scope_check_AssignStmt(assign_stmt_t stmt){
      
      scope_check_ident_declared(*(stmt.file_loc), stmt.name);
      scope_check_expr(*stmt.expr);
      
    }

 void scope_check_stmt(stmt_t stmt){
    //debug_print("In unparseStmt stmt.type_tag is %d\n", stmt.type_tag);
    
    switch (stmt.stmt_kind) {
        case assign_stmt:
        scope_check_AssignStmt(stmt.data.assign_stmt);
        break;
        case call_stmt:
        break;
        case if_stmt:
        
        scope_check_condition(stmt.data.if_stmt.condition);
        
        if(stmt.data.if_stmt.then_stmts != NULL){
            scope_check_stmts(*stmt.data.if_stmt.then_stmts);
        }
        if(stmt.data.if_stmt.else_stmts != NULL && stmt.data.if_stmt.then_stmts != NULL){
            scope_check_stmts(*stmt.data.if_stmt.else_stmts);
        }
        break;
        case while_stmt:
        scope_check_condition(stmt.data.while_stmt.condition);
        scope_check_stmt_list(stmt.data.while_stmt.body->stmt_list);
        break;
        case read_stmt:
        scope_check_ident_declared(*stmt.data.read_stmt.file_loc, stmt.data.read_stmt.name);
        break;
        case print_stmt:
        scope_check_expr(stmt.data.print_stmt.expr);
        break;
        case block_stmt:
        scope_check_program(*stmt.data.block_stmt.block);
        break;
        default:
        break;
    
    }
    
 }

void scope_check_condition(condition_t cond){
    scope_check_expr(cond.data.db_cond.dividend);
    scope_check_expr(cond.data.db_cond.divisor);
    scope_check_expr(cond.data.rel_op_cond.expr1);
    scope_check_expr(cond.data.rel_op_cond.expr2);
}

void scope_check_proc_decls(proc_decls_t decls){
    return;
}
