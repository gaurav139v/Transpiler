#include "errorHandler.c"
#include "parser.c"

// not main file..........................
#include "declaration.h"
#include <string.h>


extern Token *tokFront , *tokRear;

//.........................................
Token *tempTok;

Tokenlist *toklist;

// to create the list which need to be send to the parser
int addTokList(Token *tok){
    Tokenlist  *temp2 = toklist,*temp;
    temp = (Tokenlist*)malloc(sizeof(Tokenlist));
    temp -> next = NULL;
    temp -> tok = *tok;

    while(temp2 -> next != NULL){
        temp2 = temp2 -> next;
    }
    temp2 -> next = temp;
    return 1;
}

// file_input : (NEWLINE | stmt)* ENDMARKER
int file_input(){
	printf("\n\ninside file_input");
	if(stmt()){
        printf("\ninside file_input stmt success : %s",tempTok->tokChar);
		if(tempTok->tokType == ST && tempTok->tokName== ST || tempTok -> tokName == -1){
		    printf("\nStatement terminate ");
            tempTok = tempTok -> next;
            if(tempTok == NULL || tempTok->tokName == -1){
                printf("\nfile end");
                return 1;
            }

			file_input();
			return 1;
		}else{
			// error terminating missing
		}
	}
	error(1);
	return 0;
}

// stmt : simple_stmt | compound_stmt
int stmt(){
	printf("\ninside stmt");
	if(compound_stmt()){
        printf("\ninside stmt compound_stmt success");
		return 1;
	}else{
		if(simple_stmt()){
		    printf("\ninside stmt sumple_stmt success");
			return 1;
		}else{
			// erron no statement
		}
	}
//	printf(" fail");
	return 0;
}

// simple_stmt : small_stmt
int simple_stmt(){
	printf("\ninside simple_stmt");
    if(small_stmt()){
        printf("\ninside simple_stmt small_stmt success");
        return 1;
    }
 //   printf(" fail");
    return 0;
}

// small_stmt : expr_stmt | flow_stmt | global_stmt | nonlocal_stmt
int small_stmt(){
	printf("\ninside small_stmt");
	if(expr_stmt()){
        printf("\ninside small_stmt expr_stmt success");
        return 1;
    }else{
        if(flow_stmt()){
            printf("\ninside small_stmt flow_stmt success");
            return 1;
        }else{
            if(global_stmt()){
                printf("\ninside small_stmt global_stmt success");
                parser(global_stm);
                return 1;
            }else{
                if(nonlocal_stmt()){
                    printf("\ninside small_stmt non_local success");
                    parser(nonlocal_stm);
                    return 1;
                }else{
                    // error msg
                }
            }
        }
    }
//    printf(" fail");
    return 0;
}


//expr_stmt : annassign
int expr_stmt(){
    printf("\ninside expr_stmt");
    if(annassign()){
        printf("\ninside expr_stmt annassign success");
        return 1;
    }
//    printf(" fail");
    return 0;
}

// annassign : test ['=' test]
int annassign(){
    printf("\ninside annassign");
    if(test()){
        printf("\ninside annassign test success");
        if(tempTok->tokType ==DELIMITER && tempTok->tokName ==ASSIGN){
            addTokList(tempTok);
            tempTok = tempTok -> next;
            printf("\ninside annassign ASSIGN FOUND");
            if(test()){
                //
                printf("\ninside annassign inner test success");
            }else{
                // error msg 
                return 0;
            }
        }
        return 1;
    }
//    printf(" fail");
    return 0;
}

// flow_stmt : break_stmt | continue_stmt | return_stmt
int flow_stmt(){
	printf("\ninside flow_stmt");
    if(break_stmt()){
        printf("\ninside flow_stmt break_stmt success");
        parser(break_stm);
    	return 1;
    }else{
    	if(continue_stmt()){
    	    printf("\ninside flow_stmt continue success");
    	    parser(continue_stm);
    		return 1;
    	}else{
    		if(return_stmt()){
    		    printf("\ninside flow_stmt return_stmt success");
    			return 1;
	   		}else{
	   			// error msg
	   		}
    	}
    }
//    printf(" fail");
    return 0;
}

// break_stmt : 'break'
int break_stmt(){
	printf("\ninsdie break_stmt");
	if(strcmp(tempTok->tokChar,"break")==0){
        printf("\ninsdie break_stmt BREAK FOUND");
        addTokList(tempTok);
		tempTok = tempTok -> next;
		return 1;
	}
//	printf(" fail");
	return 0;
}

// continue_stmt : 'continue'
int continue_stmt(){
	printf("\ninside continue_stmt");
	if(strcmp(tempTok->tokChar,"continue")==0){
        printf("\ninside continue_stmt CONTINUE FOUND");
        addTokList(tempTok);
        tempTok = tempTok -> next;
        return 1;
	}
//	printf(" fail");
	return 0;
}

// return_stmt : 'return' testlist
int return_stmt(){
	printf("\ninside return_stmt");
	if(strcmp(tempTok->tokChar,"return")==0){
        printf("\ninside return_stmt REUTURN FOUND");
        addTokList(tempTok);
		tempTok = tempTok -> next;
		if(testlist()){
            printf("\ninside the return_stmt testlist success");
			return 1;
		}
	}
//	printf(" fail");
	return 0;
}

// global_stmt : 'global' NAME (',' NAME)*
int global_stmt(){
	printf("\ninside global_stmt");
	if(strcmp(tempTok->tokChar,"global")==0){
        printf("\ninside global_stmt GLOBAL FOUND");
        addTokList(tempTok);
		tempTok = tempTok -> next;
		while(tempTok->tokType == 0 && tempTok -> tokName == 0){
		    printf("\ninside global_stmt IDENTIFIER FOUND");
		    addTokList(tempTok);
			tempTok = tempTok -> next;
			if(tempTok->tokType == DELIMITER && tempTok->tokName == COMMA){
			    printf("\ninside global_stmt COMMA FOUND");
				tempTok = tempTok -> next;
			}else{
			    return 1;
			}
		}
	}
//	printf(" fail");
	return 0;
}

// nonlocal_stmt : 'nonlocal' NAME (',' NAME)*
int nonlocal_stmt(){
	printf("\ninside nonlocal_stmt");
	if(strcmp(tempTok->tokChar,"nonlocal")==0){
        printf("\ninside nonlocal_stmt NONLOCAL FOUND");
        addTokList(tempTok);
		tempTok = tempTok -> next;
		while(tempTok->tokType == 0 && tempTok -> tokName == 0){
		    printf("\ninside nonlocal_stmt IDENTIFIER FOUND");
		    addTokList(tempTok);
			tempTok = tempTok -> next;
			if(tempTok->tokType == DELIMITER && tempTok->tokName == COMMA){
			    printf("\ninside nonlocal_stmt COMMA FOUND");
				tempTok = tempTok -> next;
			}else{
				return 1;
			}
		}
	}
//	printf(" fail");
	return 0;
}


// testlist : test
int testlist(){
	printf("\ninside testlist");
	if(test()){
        printf("\ninside testlist test success");
        return 1;
	}
//	printf(" fail");
	return 0;
}

//testlist_comp : test
int testlist_comp(){
	printf("\ninside testlist_comp");
	if(test()){
        printf("\ninside testlist_comp test success");
        return 1;
	}
//	printf(" fail");
	return 0;
}

// test : or_test
int test(){
    printf("\ninside test");
    if(or_test()){
        printf("\ninside test or_test success");
        return 1;
    }
//    printf(" fail");
    return 0;
}

// or_test : and_test ('or' and_test)*
int or_test(){
    printf("\ninside or_test");
    if(and_test()){
        //tempTok = tempTok -> next;
        while(tempTok->tokType == KEYWORD && tempTok->tokName == OR){
            printf("\ninside or_test OR FOUND");
            addTokList(tempTok);
            tempTok = tempTok -> next;
            if(and_test()){
                printf("\ninside or_test and_test success");
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
 //   printf(" fail");
    return 0;
}

// and_test : not_test ('and' not_test)*
int and_test(){
    printf("\ninside and_test");
    if(not_test()){
        //tempTok = tempTok -> next;
        while(tempTok->tokType == KEYWORD && tempTok->tokName == AND){
            printf("\ninside and_test AND FOUND");
            addTokList(tempTok);
            tempTok = tempTok -> next;
            if(not_test()){
                printf("\ninside and_test not_test success");
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
  //  printf(" fail");
    return 0;
}

// not_test : 'not' not_test | comparison
int not_test(){
    printf("\ninside not_test");
	if(tempTok->tokType == KEYWORD && tempTok->tokName == NOT){
        printf("\ninside not_test NOT FOUND");
        addTokList(tempTok);
        tempTok = tempTok -> next;
        if(not_test()){
            printf("\ninside not_test not_test success");
            return 1;
        }
	}else{
        if(comparison()){
            printf("\ninside not_test comparison success");
            return 1;
        }
	}
//	printf(" fail");
	return 0;
}

// comparison : expr (comp_op expr)*
int comparison(){
    printf("\ninside comparison");
    if(expr()){
        //tempTok = tempTok -> next;
        while(comp_op()){
            printf("\ninside comparison comp_op success");
            //tempTok = tempTok -> next;
            if(expr()){
                printf("\ninside comparison expr success");
                //tempTok = tempTok -> next;
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
 //   printf(" fail");
    return 0;
}

// comp_op : '<' | '>' | '==' | '>=' | '<=" | '!=' | 'in' | 'not' 'in' | 'is' | 'is' 'not'
int comp_op(){
    printf("\ninside comp_op");
    int type,name;
    type = tempTok -> tokType;
    name = tempTok -> tokName;

    if(type == OPERATOR){
        //tempTok = tempTok -> next;
        printf("\n%s",tempTok->tokChar);
        switch(name){
            case LESS_THAN:
                printf("\ninside comp_op LESS_THAN FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

            case GREATER_THAN:
                printf("\ninside comp_op GREATER_THAN FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

            case EQUAL:
                printf("\ninside comp_op EQUAL FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

            case GT_EQUAL_TO:
                printf("\ninside comp_op GT_EQUAL_TO FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

            case LT_EQUAL_TO:
                printf("\ninside comp_op LT_EQUAL_TO FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

            case NOT_EQUAL:
                printf("\ninside comp_op NOT_EQUAL FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

        }
    }else{
        if(type == KEYWORD){
            switch(name){
                case IN:
                    printf("\ninside comp_op IN FOUND");
                    addTokList(tempTok);
                    tempTok = tempTok -> next;
                    return 1;

                case NOT:
                    addTokList(tempTok);
                    tempTok = tempTok -> next;
                    if(tempTok->tokType == KEYWORD && tempTok->tokName == IN){
                        printf("\ninside comp_op NOT IN FOUND");
                        addTokList(tempTok);
                        tempTok = tempTok -> next;
                        return 1;
                    }else{
                        // error
                    }
                    return 0;

                case IS:
                     addTokList(tempTok);
                    tempTok = tempTok -> next;
                    if(tempTok->tokType == KEYWORD && tempTok->tokName == NOT){
                        printf("\ninside comp_op IS NOT FOUND");
                        addTokList(tempTok);
                        tempTok = tempTok -> next;
                        return 1;
                    }
                    printf("\ninside comp_op IS FOUND");
                    return 1;
            }
        }
    }
 //   printf(" fail");
    return 0;
}

// expr : xor_expr ('|' xor_expr)*
int expr(){
    printf("\ninside expr");
    if(xor_expr()){
        //tempTok = tempTok -> next;
        printf("\ninside expr xor_expr success");
        while(tempTok->tokType == OPERATOR && tempTok->tokName == BINARY_OR){
            printf("\ninside expr BINARY_OR FOUND");
            addTokList(tempTok);
            tempTok = tempTok -> next;
            if(xor_expr()){
                printf("\ninside expr xor_expr success");
               // tempTok = tempTok -> next;
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
 //   printf(" fail");
    return 0;
}

// xor_expr : and_expr ('^' and_expr)*
int xor_expr(){
    printf("\ninside xor_expr");
    if(and_expr()){
        //tempTok = tempTok -> next;
        printf("\ninside xor_expr and_expr success");
        while(tempTok->tokType == OPERATOR && tempTok->tokName == BINARY_XOR){
            printf("\ninside xor_expr BINARY_XOR FOUND");
            addTokList(tempTok);
            tempTok = tempTok -> next;
            if(and_expr()){
                printf("\ninside xor_expr and_expr success");
                //tempTok = tempTok -> next;
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
//    printf(" fail");
    return 0;
}

// and_expr : shift_expr ('&' shift_expr)*
int and_expr(){
    printf("\ninside and_expr");
     if(shift_expr()){
        printf("\ninside and_expr shift_expr success");

        while(tempTok->tokType == OPERATOR && tempTok->tokName == BINARY_AND){
            printf("\ninside and_expr BINARY_AND FOUND");
            addTokList(tempTok);
            tempTok = tempTok -> next;
            if(shift_expr()){
                printf("\ninside and_expr shift_expr success");
                //tempTok = tempTok -> next;
                //return 1;
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
//    printf(" fail");
    return 0;
}

// shift_expr : arith_expr (('<<' | '>>') arith_expr)*
int shift_expr(){
    printf("\ninside shift_expr");
    if(arith_expr()){
        //tempTok = tempTok -> next;
        printf("\ninside shift_expr arith_expr success");
        while(tempTok->tokType == OPERATOR && tempTok->tokName == BINARY_LS ||
              tempTok->tokType == OPERATOR && tempTok->tokName == BINARY_RS){
            addTokList(tempTok);
            tempTok = tempTok -> next;
            printf("\ninside shift_expr BINARY_LS | BINAR_RS FOUND");
            if(arith_expr()){
                printf("\ninside shift_expr arith_expr success");
                //tempTok = tempTok -> next;
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
//    printf(" fail");
    return 0;
}

// arith_expr : term (('+' | '-') term )*
int arith_expr(){
    printf("\ninside arith_expr");

    if(term()){
        printf("\ninside arith_expr term success");
        while(tempTok->tokType == OPERATOR && tempTok->tokName == PLUS ||
              tempTok->tokType == OPERATOR && tempTok->tokName == MINUS){
            addTokList(tempTok);
            tempTok = tempTok -> next;
            printf("\ninside arith_expr PLUS | MINUS FOUND");
            if(term()){
                printf("\ninside arith_expr term success");
            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
//    printf(" fail");
    return 0;
}

// term : factor (('*' | '@' | '/' | '%' | '//') factor)*
int term(){
    printf("\ninside term");
    if(factor()){
        //tempTok = tempTok -> next;
        printf("\ninside term factor success");
        printf("\nfac : %s",tempTok->tokChar);
        while(tempTok->tokType == OPERATOR && tempTok->tokName == MUL ||
              tempTok->tokType == DELIMITER && tempTok->tokName == AT ||
              tempTok->tokType == OPERATOR && tempTok->tokName == DIV ||
              tempTok->tokType == OPERATOR && tempTok->tokName == MOD ||
              tempTok->tokType == OPERATOR && tempTok->tokName == FDIV){
            addTokList(tempTok);
            tempTok = tempTok -> next;
            printf("\ninside term MUL | AT | DIV | MOD | FDIV FOUND");
            if(factor()){
                printf("\ninside term factor success");

            }else{
                // error msg
                return 0;
            }
        }
        return 1;
    }
//    printf(" fail");
    return 0;
}

// factor :  ('+' | '-' | '~' ) factor | power
int factor(){
    printf("\ninside factor");
    if(tempTok->tokType == OPERATOR && tempTok->tokName == PLUS  ||
       tempTok->tokType == OPERATOR && tempTok->tokName == MINUS ||
       tempTok->tokType == OPERATOR && tempTok->tokName == BINARY_1COMP){
        printf("\ninside factor PLUS | MINUS | BINARY_1COMP FOUND");
        addTokList(tempTok);
        tempTok = tempTok -> next;
        if(factor()){
            printf("\ninside factor factor success");
            return 1;
        }
    }else{
        if(power()){
            printf("\ninside factor power success");
            return 1;
        }
    }
//    printf(" fail");
    return 0;
}

// power : atom_expr ['**' factor]
int power(){
    printf("\ninside power");
    if(atom_expr()){
        printf("\ninside power atom_expr success");
        if(tempTok->tokType == OPERATOR && tempTok ->tokName == EXP){
            addTokList(tempTok);
            tempTok = tempTok -> next;
            printf("\ninside power EXP FOUND");
            if(factor()){
                printf("\ninside power factor success");
                return 1;
            }else{
                // error msg 
                return 0;
            }
        }
        return 1;
    }
//    printf(" fail");
    return 0;
}

// atom_expr : atom (trailer)*
int atom_expr(){
    printf("\ninside atom_expr");
    if(atom()){
        printf("\ninside atom_expr atom success");
        if(trailer()){
            printf("\ninside atom_expr trailer success");
        }
        return 1;
    }
//    printf("\n fail");
    return 0;
}

// trailer : '(' [arglist] ')'
int trailer(){
    printf("\ninside trailer");
    if(tempTok->tokType == DELIMITER && tempTok->tokName == L_PAREN){
        printf("\ninside trailer L_PAREN FOUND");
        addTokList(tempTok);
        tempTok = tempTok -> next;
        //printf("\n%s",tempTok->tokChar);
        if(arglist()){
            printf("\ninside trailer arglist success");
        }
        if(tempTok->tokType == DELIMITER && tempTok->tokName == R_PAREN){
            printf("\ninside trailer R_PAREN FOUND");
            addTokList(tempTok);
            tempTok = tempTok -> next;
            return 1;
        }
    }
//    printf("\n fail");
    return 0;
}

// arglist : argument (',' argument)*
int arglist(){
    int flag = 0;
	printf("\ninside arglist");
    while(argument()){
	    printf("\ninside arglist argument success");
        flag = 1;
       	if(tempTok->tokType== DELIMITER && tempTok->tokName == COMMA){
		    printf("\ninside arglist COMMA FOUND");
            addTokList(tempTok);
		    tempTok = tempTok -> next;

		}else{
		    return 1;
		}
	}
//	printf(" fail");
    if (flag == 1) error(1); // extra comma present
	return 0;
}

// argument : test
int argument(){
    printf("\ninside argument");
    if(test()){
        return 1;
    }
    printf("\nfail");
    return 0;
}


// atom : ( '(' [testlist_comp] ')' | '[' [testlist_comp] ']' | NAME | NUMBER | STRING | 'None' | 'True' | 'False'
int atom(){
    printf("\ninside atom");
    int temp;
    if(tempTok->tokType == DELIMITER && tempTok -> tokName == L_PAREN){
        printf("\ninside atom L_PAREN FOUND");
        addTokList(tempTok);
        tempTok = tempTok -> next;
        if(testlist_comp()){
            printf("\ninside atom testlist_comp success");
            if(tempTok->tokType == DELIMITER && tempTok -> tokName == R_PAREN){
                printf("\ninside atom R_PAREN FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;
           }
        }
    }

    if(tempTok->tokType == DELIMITER && tempTok -> tokName == L_BRACK){
        printf("\ninside atom L_BRACK FOUND");
        addTokList(tempTok);
        tempTok = tempTok -> next;
        if(testlist_comp()){
            printf("\ninside atom testlist_comp success");
            if(tempTok->tokType == DELIMITER && tempTok -> tokName == R_BRACK){
                printf("\ninside atom R_BRACK FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;
            }
        }
    }

    if(tempTok->tokType == 0 && tempTok->tokName == 0){
        printf("\ninside atom IDENTIFIER FOUND");
        addTokList(tempTok);
        tempTok = tempTok -> next;
        return 1;
    }

    if(tempTok->tokType == LITERAL){
        temp = tempTok ->tokName;

        switch(temp){
            case INT:
                printf("\ninside atom INT FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;                
                return 1;

            case FLOAT:
                printf("\ninside atom FLOAT FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;                
                return 1;

            case STR:
                printf("\ninside atom STR FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;                
                return 1;

        }
   }

    if(tempTok->tokType == KEYWORD){
        temp = tempTok ->tokName;
        switch(temp){
            case NONE:
                printf("\ninside atom NONE FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

            case TRUE:
                printf("\ninside atom TRUE FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

            case FALSE:
                printf("\ninside atom FALSE FOUND");
                addTokList(tempTok);
                tempTok = tempTok -> next;
                return 1;

        }
    }
//    printf(" fail");
    return 0;
}



int compound_stmt(){
	printf("\ninside compound_stmt");
	printf(" fail");
    return 0;
}
