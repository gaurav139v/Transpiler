#ifndef LEXICALANALYZER_H_INCLUDED
#define LEXICALANALYZER_H_INCLUDED

#include<string.h>
#include "declaration.h"

extern Token *tokRear, *tokFront;

Queue*  gen_str(Queue *firstChar, Queue *lastChar){
    char *tChar;
    int count = 0,i=0;
    Queue *temp;
    firstChar = firstChar -> prev;
    temp = firstChar;

    while(temp != lastChar){
        temp = temp -> prev;
        count++;
    }
    tChar = (char*)malloc(sizeof(char)*count);

    while(i<=count){
        *(tChar+i) = firstChar -> val;
        firstChar = firstChar -> prev;
        i++;
    }
    *(tChar + i) = '\0';
    generateToken(tChar,LITERAL,STR);
    return lastChar;
}

/*
* FSM for different Non-terminals
* lower <-- [a-z]
* upper <-- [A-Z]
* digit <-- [0-9]
* ordinary <-- _|(|)|[|]|{|}|+|-|*|/|%|!|&|||~|^|<|=|>|,|.|:|;|$|?|#
* graphic <-- lower | upper | digit | ordinary
*/
int isLower(char c){
    if(c>=97 && c<=122){
        return 1;
    }else{
        return 0;
    }
}

int isUpper(char c){
    if(c>=65 && c<=90){
        return 1;
    }else{
        return 0;
    }
}

int isDigit(char c){
    if(c>=48 && c<=57){
        return 1;
    }else{
        return 0;
    }
}

int isOrdinary(char c){
    if(c > 34 && c < 39 || c > 39 && c < 48 || c > 57 && c < 64 || c > 92 && c < 96 || c > 122 && c < 127 || c == 91 || c == 33  ){
        return 1;
    }else{
        return 0;
    }
}

int isGraphic(char c){
    if(isLower(c) || isUpper(c) || isDigit(c) || isOrdinary(c)){
        return 1;
    }else{
        return 0;
    }
}

/*
* Token which contain the token,token-type,token-name
* Add the token to the token-list it's a global variable
*/
int generateToken(char tChar[],int tType,int tName){
    int tokenLength,i;
    Token *newtok;
    tokenLength = strlen(tChar)+1;

    newtok = (Token*)malloc(sizeof(Token));
    newtok -> tokChar = (char*)malloc(sizeof(char)*tokenLength);
    newtok -> next = NULL;
    newtok -> tokType = tType;
    newtok -> tokName = tName;
    for(i=0; i<tokenLength; i++){
        *((newtok -> tokChar)+i) = tChar[i];
    }

    tokRear -> next = newtok;
    tokRear = newtok;
    return 1;
}


/*
* To check if the generated token is identifier or keyword
* Pass the token information to the token-list
*/
int isKeyword(char token[]){
    int i=0;
    char *keyword[33] = {"False","None","True","and","as","assert","break","class","continue","def","del","elif",
    "else","except","finally","for","from","global","if","import","in","is","lambda","nonlocal","not","or",
    "pass","raise","return","try","while","with","yield"};

    char *t;
    t = token;
    for(i=0;i<33;i++){
        if(strcmpi(t,keyword[i])==0){
            generateToken(token,KEYWORD,i);
            return 0;
        }
    }
    // type of the identifier variable or function remain to be coded
    generateToken(token,IDENTIFIER,IDENTIFIER);
    return 1;
}

/*
* For checking the token is Identifier
* Starting character of the identifier (id_start)
* id_start <-- lower | upper | _
* identifier <-- id_start{id_start | digit }
*/

int id_start(char c){
    if(isLower(c) || isUpper(c) || c =='_'){
        return 1;
    }else{
        return 0;
    }
}

Queue* isIdentifier(Queue *firstChar){
    int i=0;
    char tChar[32];
    int rear = 0;

    /*
    * evaluate until the identifier is detected
    */
    while(firstChar != NULL){
        if(id_start(firstChar -> val)){
            // first character of identifier must be char or _
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;

            /*
            * character in the identifier can be DIGIT or ALPHABETS
            */
            while(firstChar != NULL){
                if(id_start(firstChar -> val) || isDigit(firstChar -> val)){
                    // valid identifier
                    tChar[rear++] = firstChar -> val;
                    firstChar = firstChar -> prev;
                }else{
                    if(firstChar != NULL){
                        // Seperate partial valid identifier from the input string
                        tChar[rear++] = '\0';
                        isKeyword(tChar);
                        return firstChar;
                    }
                }
            }
            // Entire the string is the identifier
            tChar[rear++] = '\0';
            isKeyword(tChar);
        }
        return firstChar;
    }
    return NULL;
}

/*
* To generate the token of type symbol [delimiter, operator]
* operator <-- +|-|*|/|//|%|**|==|!=||<|>|<=|>=|&|||~|^|<<|>>
* delimiter <-- (|)|[|]|{|}|.|,|:|;|@|=|+=|-+|*=|/=|//=|%=|**=|&=||=|^=|<<=|>>=
*/
Queue* isSymbol(Queue *firstChar){
    char tChar[4];
    int rear =0,i=0;
    char sym[10]={'(',')','[',']','{','}','.',',',':','@'};

    switch(firstChar->val){
        // Check the symbol = ==
        case '=':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar->val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,OPERATOR,EQUAL);
                return firstChar;
            }
            tChar[rear++] = '\0';
            generateToken(tChar,DELIMITER,ASSIGN);
            return firstChar;

        // Check the symbol + +=
        case '+':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar->val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,PLUS_A);
                return firstChar;
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,PLUS);
            return firstChar;

        // Check the symbol - -=
        case '-':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar -> val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,MINUS_A);
                return firstChar;
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,MINUS);
            return firstChar;

        // Check the symbol * ** *= **=
        case '*':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar -> val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,MUL_A);
                return firstChar;
            }else{
                if(firstChar -> val == '*'){
                    tChar[rear++] = firstChar -> val;
                    firstChar = firstChar -> prev;
                    if(firstChar -> val == '='){
                        tChar[rear++] = firstChar -> val;
                        tChar[rear++] = '\0';
                        firstChar = firstChar -> prev;
                        generateToken(tChar,DELIMITER,EXP_A);
                        return firstChar;
                    }
                    tChar[rear++] = '\0';
                    generateToken(tChar,OPERATOR,EXP);
                    return firstChar;
                }
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,MUL);
            return firstChar;

        // Check the symbol / // /= //=
        case '/':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar -> val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,DIV_A);
                return firstChar;
            }else{
                if(firstChar -> val == '/'){
                    tChar[rear++] = firstChar -> val;
                    firstChar = firstChar -> prev;
                    if(firstChar -> val == '='){
                        tChar[rear++] = firstChar -> val;
                        tChar[rear++] = '\0';
                        firstChar = firstChar -> prev;
                        generateToken(tChar,DELIMITER,FDIV_A);
                        return firstChar;
                    }
                    tChar[rear++] = '\0';
                    generateToken(tChar,OPERATOR,FDIV);
                    return firstChar;
                }
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,DIV);
            return firstChar;

        // Check the symbol % %=
        case '%':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar->val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,MOD_A);
                return firstChar;
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,MOD);
            return firstChar;

        // Check the symbol < <= << <<=
        case '<':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar -> val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,OPERATOR,LT_EQUAL_TO);
                return firstChar;
            }else{
                if(firstChar -> val == '<'){
                    tChar[rear++] = firstChar -> val;
                    firstChar = firstChar -> prev;
                    if(firstChar -> val == '='){
                        tChar[rear++] = firstChar -> val;
                        tChar[rear++] = '\0';
                        firstChar = firstChar -> prev;
                        generateToken(tChar,DELIMITER,LS_A);
                        return firstChar;
                    }
                    tChar[rear++] = '\0';
                    generateToken(tChar,OPERATOR,BINARY_LS);
                    return firstChar;
                }
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,LESS_THAN);
            return firstChar;

        // Check the symbol > >= >> >>=
        case '>':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar -> val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,OPERATOR,GT_EQUAL_TO);
                return firstChar;
            }else{
                if(firstChar -> val == '>'){
                    tChar[rear++] = firstChar -> val;
                    firstChar = firstChar -> prev;
                    if(firstChar -> val == '='){
                        tChar[rear++] = firstChar -> val;
                        tChar[rear++] = '\0';
                        firstChar = firstChar -> prev;
                        generateToken(tChar,DELIMITER,RS_A);
                        return firstChar;
                    }
                    tChar[rear++] = '\0';
                    generateToken(tChar,OPERATOR,BINARY_RS);
                    return firstChar;
                }
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,GREATER_THAN);
            return firstChar;

        // Check the symbol & &=
        case '&':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar->val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,B_AND_A);
                return firstChar;
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,BINARY_AND);
            return firstChar;

        // Check the symbol | |=
        case '|':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar->val == '='){
                tChar[rear++] = firstChar -> val;

                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,B_OR_A);
                return firstChar;
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,BINARY_OR);
            return firstChar;

        // Check the symbol ^ ^=
        case '^':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar->val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,DELIMITER,XOR_A);
                return firstChar;
            }
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,XOR_A);
            return firstChar;

        // Check the symbol !=
        case '!':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar->val == '='){
                tChar[rear++] = firstChar -> val;
                tChar[rear++] = '\0';
                firstChar = firstChar -> prev;
                generateToken(tChar,OPERATOR,NOT_EQUAL);
                return firstChar;
            }
            // error();

        // Check the symbol ~
        case '~':
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            tChar[rear++] = '\0';
            generateToken(tChar,OPERATOR,XOR_A);
            return firstChar;


    }

    // Check the symbol ( ) [ ] { } . , : @
    for(i=0;i<11;i++){
        if(firstChar -> val == sym[i]){
            tChar[rear++] = firstChar -> val;
            tChar[rear++] = '\0';
            firstChar = firstChar -> prev;
            generateToken(tChar,DELIMITER,i);
            return firstChar;
        }
    }
    return firstChar;
}


/*
* To generate the literal token [integral , float]

* decimal_literal <-- digit {digit}*
* mantissa <-- decimal_literal | . decimal_literal
* exponent <-- e [+|-] decimal_literal | E [+|-] decimal_literal
* float_literal <-- mantissa [exponent] | decimal_literal exponent
*/
Queue* isLitereal(Queue *firstChar){
    char tChar[32];
    int rear = 0,flag=0;

    while(firstChar -> val != NULL){
        if(isDigit(firstChar -> val)){
            tChar[rear++] = firstChar -> val;
            firstChar = firstChar -> prev;
            if(firstChar == 0) break;
                if(firstChar -> val == 'e' || firstChar -> val == 'E'){
                    tChar[rear++] = firstChar -> val;
                    firstChar = firstChar -> prev;
                    if(firstChar -> val == '+' || firstChar -> val == '-'){
                        tChar[rear++] = firstChar -> val;
                        firstChar = firstChar -> prev;
                        flag = 1; // float
                    }else{
                        // error(); To be coded
                    }
                }else{
                    if(flag != 1)
                        flag = 2; // int
                }

        }else{
            if(firstChar -> val == '.'){
                tChar[rear++] = firstChar -> val;
                firstChar = firstChar -> prev;
                // flag for float
                flag = 1;
            }else{
                break;
            }
        }
    }

    tChar[rear++] = '\0';
    if(flag == 1){
        generateToken(tChar,LITERAL,FLOAT);
    }else{
        if(flag == 2){
            generateToken(tChar,LITERAL,INT);
        }
    }
    return firstChar;
}

/*
* To generate the stream of token
* input is the sequence of character between to spaces
*/
Queue* tokenize(Queue *firstChar,Queue *lastChar){

    // for testing the string which is to be tokenize
    Queue *temp;
    temp = firstChar;
    printf("\nstring--- ");
    pushQueue(0,lastChar);
    while(temp != NULL){
        printf("%d ",temp -> val);
        temp = temp -> prev;
    }

    firstChar = firstChar -> prev;
    pushQueue(0,lastChar);
    if(firstChar->val == 32) firstChar = firstChar ->prev;

    while(firstChar -> val != 0){
        firstChar = isLitereal(isSymbol(isIdentifier(firstChar)));
        if(firstChar->val == 32) break;
    }
    return lastChar;
}

/*
* This function take the input file name which contain python code
*/
int tokenizer(const char *file_name){
    int indentCount = 0;  // number of spaces in the line start
    char ch;
    FILE *file;
    file = fopen(file_name,"r");

    // variables for the indentation list uses [stack]
    // create a stack with 0 value
    Stack *temp, *top;
    top = (Stack*)malloc(sizeof(Stack));
    top -> value = 0;
    top -> next = NULL;

    // create a queue with 0 value
    // tokFront and tokRear are global variable
    tokFront = (Token*)malloc(sizeof(Token));
    tokFront -> tokChar = 'a';
    tokFront -> tokName = 0;
    tokFront -> tokType = 0;
    tokFront -> next = NULL;
    tokRear = tokFront;

    // variables for the statement tokenization  uses [queue]
    // queue initialization
    Queue *rear,*front;
    front = (Queue*)malloc(sizeof(Queue));
    front -> next = NULL;
    front -> prev = NULL;
    front -> val = 0;
    rear = front;


    ch = fgetc(file); // fetch first char from the file
    // keep reading file until the end
    while(ch != EOF){
        // To skip the new line character
        if(ch == '\n'){
            ch = fgetc(file);
            // For not generating the termination token in blank lines
            if(ch != '\n'){
                generateToken("\\n",ST,ST);
            }
            indentCount = 0;
            continue;
        }

        //  To count the tab in the start of the line TAB ascii value is 9
        while(ch == 9){
            indentCount += 4;  // one tab = 8 spaces
            ch = fgetc(file);
        }

        // To count the spaces in the start of the line sapce ascii value is 32
        while(ch == 32){
            indentCount += 1;
            ch = fgetc(file);
        }

        // To check if the actual statement of the program is start or not
        // when it's is not a new line or space
        if(ch != '\n' || ch != 32){
            /*
            * generate the INDENT token
            * Which shows the start of the new scope
            */
            if(indentCount > (top -> value)){
                top = pushStack(indentCount,top);
                generateToken("INDENT",INDENT,INDENT);

            }else{
                /*
                * Generate the DEDENT token
                * Which shows the end of the scope
                */
                if(indentCount < top -> value ){
                    while(indentCount != top -> value){
                        top = popStack(top);
                        generateToken("DEDENT",DEDENT,DEDENT);
                        /*
                        // To show error in Indentation
                        if(top -> value == 0){
                            error();
                            break;
                        }
                        */
                    }
                }

            }

            // For Tokenization of the statements
            while(ch != '\n'){

                /*
                * Tokenize the program
                */
                if(ch == 32 || ch =='"' || ch == '\''){
                    front = tokenize(front,rear);  // tokenize the characters

                    /*
                    * To tokenize the string token between "" and ''
                    * white_space <-- \t | space
                    * single_quote_str <-- "{graphic | white_space | ' | \ }" |
                                           '{graphic | white_space | " | \ }'
                    */
                    if(ch=='"'){
                        ch = fgetc(file);
                        while(ch!='"'){
                            if(isGraphic(ch) || ch == 32 || ch == '\t'|| ch == '\'' || ch=='\\'){
                                rear = pushQueue(ch,rear);
                                ch = fgetc(file);
                            }else{
                                // error in string
                            }
                        }
                        front = gen_str(front,rear);
                        ch = fgetc(file);
                    }else{
                        if(ch == '\'') {
                            ch = fgetc(file);
                            while(ch!='\''){
                                if(isGraphic(ch) || ch == 32 || ch == '\t'|| ch == '"' || ch == '\\'){
                                    rear = pushQueue(ch,rear);
                                    ch = fgetc(file);
                                }else{
                                    // error in string
                                }
                            }
                            front = gen_str(front,rear);
                            ch = fgetc(file);
                        }
                    }

                }
                rear = pushQueue(ch,rear);
                ch = fgetc(file);
                if (ch==EOF)
                    break;
            }
            //if(ch != -1)
            front = tokenize(front,rear);
        }
    }
    generateToken("",-1,-1);
    fclose(file);

    file = fopen("Token.txt","w");
    fprintf(file,"Type\tName\tToken\n");

    tokFront = tokFront -> next;
    Token *temp3;
    temp3 = tokFront;

    while(temp3!= NULL){
        printf("\ntoken %s",temp3->tokChar);
        printf("\ntype %d",temp3->tokType);
        printf("\nName %d",temp3->tokName);
        fprintf(file,"%d\t%d\t%s\n",temp3->tokType,temp3->tokName,temp3->tokChar);
        temp3 = temp3->next;
        printf("\n");
    }
    printf("\nello");
}

#endif // LEXICALANALYZER_H_INCLUDED
