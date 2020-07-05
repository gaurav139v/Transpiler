#include<stdio.h>

#include "lexicalAnalyzer.h"
#include "syntaxAnalysis.c"

Token *tokRear, *tokFront;
///////////////////////////
extern Token *tempTok;
extern Tokenlist *toklist;


int main(){

    char *file_name = {"sample.txt"};
    tokenizer(file_name);

    tempTok = tokFront;

    printf("\nfirst token : %s",tokFront->tokChar);
    //Tokenlist *toklist;
    toklist = (Tokenlist*)malloc(sizeof(Tokenlist));
    //toklist -> tok = 0;
    toklist -> next = NULL;
/*
    addTokList(tokFront);
    addTokList(tokFront->next);
    addTokList(tokFront->next->next);
    addTokList(tokFront->next->next->next);

    while(toklist != NULL){
        printf("\n%s %d",toklist->tok.tokChar,toklist->next);
        toklist = toklist -> next;
    }
*/
    file_input();



}
