#include "declaration.h"
#include "display.c"
///////////////////
extern Tokenlist *toklist;

// create a identifier node
Ast* make_var_node(Token t){
    Ast *temp;
    temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = var_exp; // store the tag value =  id
    temp -> op.varExp = t; // store the token in the union id
    return temp;
}

Ast* make_literal_node(Token t){
    Ast *temp;
    temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = literal_exp; // store the tag value =  id
    temp -> op.literalExp = t; // store the token in the union id
    return temp;
}

Ast* make_temp_node(Token t){
    Ast *temp;
    temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = temp_exp; // store the tag value =  id
    temp -> op.tempExp = t; // store the token in the union id
    return temp;
}

AstList* make_var_list(Tokenlist *start){
    AstList *temp , *next_id;
    Ast *temp2;

    // for create the first node of the list
    temp = (AstList*)malloc(sizeof(AstList));
    temp2 = make_var_node(start->tok);
    temp -> ast = *temp2;
    temp -> next = NULL;
    start = start -> next;

    // if there are more then one identifier
    AstList *temp1 = temp;
    while(start  != NULL){
        AstList *next_id = (AstList*)malloc(sizeof(AstList));

        temp1 -> next = next_id;
        temp1 = next_id;

        temp2 = make_var_node(start->tok);
        next_id -> ast = *temp2;
        next_id -> next = NULL;

        start = start -> next;
    }

    return temp;
}

Ast* make_global_node(Tokenlist *start){
    Ast *temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = global_stm; // store the tag data in global node
    temp -> op.globalStm.data = start -> tok; // store the tok data in union globalStm
    start = start -> next;
    temp -> op.globalStm.var_list = make_var_list(start); // for creating the list of identifiers in global statement
    return temp;
}

Ast* make_nonlocal_node(Tokenlist *start){
    Ast *temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = nonlocal_stm; // store the tag data in nonlocal node
    temp -> op.nonlocalStm.data = start -> tok; // store the tok data in union nonlocalStm
    start = start -> next;
    temp -> op.nonlocalStm.var_list = make_var_list(start); // for creating the list of identifiers in global statement
    return temp;
}

Ast* make_break_node(Tokenlist  *start){
    Ast *temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = break_stm;
    temp -> op.breakStm = start -> tok;
    return temp;
}

Ast* make_continue_node(Tokenlist  *start){
    Ast *temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = continue_stm;
    temp -> op.continueStm = start -> tok;
    return temp;
}

Ast* make_funcCall_node(Tokenlist *start){
    Ast *temp = (Ast*)malloc(sizeof(Ast));
    temp -> tag = func_call;
    temp -> op.funcCall.name = start -> tok;

    Ast *next_arg = (Ast*)malloc(sizeof(Ast));
    start = start -> next;
    while(start != NULL){

    }
}

int add_Ast_list(Ast *node, AstList *first){
    AstList *temp = (AstList*)malloc(sizeof(AstList));
    temp -> ast = *node;
    temp -> next = NULL;

    while(first -> next != NULL){
        first = first -> next;
    }
    first -> next = temp;
    return 1;
}



Ast* make_expr_node(Tokenlist *start){
    Ast *temp = (Ast*)malloc(sizeof(Ast));
    AstList *list_first = (AstList*)malloc(sizeof(AstList));
   // list -> ast = NULL;
    list_first -> next = NULL;

    Ast *temp2, *temp_ast;
    Tokenlist *s = start,*p;
    while(s != NULL){
        if(s -> tok.tokType == 0 && s -> tok.tokName == 0){
            p = s->next;
            if(p->tok.tokType == DELIMITER && p->tok.tokName == L_PAREN){
                // FUNCTION CALL
            }else{
                // var expression
                temp2 = make_var_node(s -> tok);
                // add to list
                add_Ast_list(temp2,list_first);
            }
        }else{
            if(s -> tok.tokType == LITERAL){
                // Literal expression
                temp2 = make_literal_node(s -> tok);
                add_Ast_list(temp2,list_first);
            }else{
                // rest of the token except function , variable , literal in the expression
                temp2 = make_temp_node(s -> tok);
                add_Ast_list(temp2,list_first);
            }
        }

        s = s -> next;
    }



}


int parser(Tag stm){
    Tokenlist *start= toklist -> next;
    Ast *statement;
    printf("\n----------------------------------------------Here");
    switch(stm){

        // create a global node
        case global_stm:
            statement = make_global_node(start);
            disp_global(statement);
            break;

        case nonlocal_stm:
            statement = make_nonlocal_node(start);
            disp_nonlocal(statement);
            break;

        case break_stm:
            statement = make_break_node(start);
            disp_break(statement);
            break;

        case continue_stm:
            statement = make_continue_node(start);
            disp_continue(statement);
            break;

    }
    printf("\n--------------------------------------------------");
    // make the toklist empty
    toklist->next = NULL;
}


