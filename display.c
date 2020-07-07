extern Tokenlist *toklist;

void disp_toklist(){
    Tokenlist *temp=toklist;
    while(temp != NULL){
        printf("\n%s\t%d",temp->tok.tokChar,temp->next);
        temp = temp -> next;
    }
}

void disp_global(Ast *s){
    printf("\ntag : %d",s->tag);
    printf("\ndata : %s",s->op.globalStm.data.tokChar);

    AstList * temp = s->op.globalStm.var_list;
    while(temp != NULL){
        printf("\nvarExp : %s",temp->ast.op.varExp.tokChar);
        temp = temp -> next;
        
    }
}

void disp_nonlocal(Ast *s){
    printf("\ntag : %d",s->tag);
    printf("\ndata : %s",s->op.nonlocalStm.data.tokChar);

    AstList * temp = s->op.nonlocalStm.var_list;
    while(temp != NULL){
        printf("\nvarExp : %s",temp->ast.op.varExp.tokChar);
        temp = temp -> next;
    }
}

void disp_break(Ast *s){
    printf("\ntag : %d",s->tag);
    printf("\nbreak_stm : %s",s->op.breakStm.tokChar);
}

void disp_continue(Ast *s){
    printf("\ntag : %d",s->tag);
    printf("\nbreak_stm : %s",s->op.continueStm.tokChar);
}
