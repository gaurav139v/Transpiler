#ifndef DECLARATION_H_INCLUDED
#define DECLARATION_H_INCLUDED

// Declare all the Token type
enum TokenType {IDENTIFIER,KEYWORD,OPERATOR,DELIMITER,LITERAL,ST,INDENT,DEDENT};

// Declare all the Keywords
enum Keywords {FALSE,NONE,TRUE,AND,AS,ASSERT,BREAK,CLASS,CONTINUE,DEF,DEL,ELIF,ELSE,EXCEPT,FINALLY,FOR,
FROM,GLOBAL,IF,IMPORT,IN,IS,LAMBDA,NONLOCAL,NOT,OR,PASS,RAISE,RETURN,TRY,WHILE,WITH,YIELD};

// Declare all the Operators
enum Operators {PLUS,MINUS,MUL,DIV,FDIV,MOD,EXP,EQUAL,NOT_EQUAL,LESS_THAN,GREATER_THAN
,LT_EQUAL_TO,GT_EQUAL_TO,BINARY_AND,BINARY_OR,BINARY_1COMP,BINARY_XOR,BINARY_LS,BINARY_RS};

// Declare all the Delimiters
enum Delimiters {L_PAREN,R_PAREN,L_BRACK,R_BRACK,L_BRACE,R_BRACE,DOT,COMMA,COLON,AT,ASSIGN,PLUS_A,MINUS_A,
MUL_A,DIV_A,FDIV_A,MOD_A,EXP_A,B_AND_A,B_OR_A,XOR_A,LS_A,RS_A};

// Declare all the literals
enum Literals {INT,FLOAT,STR,IMG,BOOL,BYTES,NONETYPE,COMMENT};

// Data structure for the token
typedef struct Token{
    char *tokChar;
    int tokType;
    int tokName;
    struct Token *next;
}Token;

// singly link list to implement Stack
typedef struct Stack {
    int value;
    struct Stack *next;
}Stack;

// Doubly link list to implement Queue
typedef struct Queue {
    char val;
    struct Queue *next,*prev;
}Queue;

// push the value in the stack
Stack* pushStack(int v, Stack *top){
    Stack *temp;
    temp = (Stack*)malloc(sizeof(Stack));
    temp -> value = v;
    temp -> next = top;
    top = temp;
    return top;
}

// pop the value from the stack
Stack* popStack(Stack *top){
    top = top -> next;
    return top;
}

// push data in the queue
Queue* pushQueue(char data,Queue *rear){
    Queue *temp;
    temp = (Queue*)malloc(sizeof(Queue));
    rear -> prev = temp;
    temp -> next = rear;
    temp -> prev = NULL;
    temp -> val = data;
    rear = temp;
    return rear;
}

// display the data in the queue
void display_queue(Queue *f){
    f = f->prev;
    while(f != NULL){
        printf("%c",f -> val);
        f = f -> prev;
    }
}

//////////////////////////////////////////////////////////////////////////// SYNTAX ANALYSIS

// For storing the list of token for parsing
typedef struct Tokenlist{
    Token tok;
    struct tokenlist *next;
}Tokenlist;



// Define the list of the type of the statements
typedef enum Tag {global_stm=100,nonlocal_stm,var_exp,literal_exp,break_stm,continue_stm,func_call,temp_exp}Tag;

// Data structure for the Abstract syntax tree node
typedef struct AST{
   enum Tag tag; // to store the type of statement

   union {
        Token varExp,literalExp,breakStm,continueStm,tempExp; // for storing the identifier node

        // for global statement node
        struct declaration_Stmt {
            Token data; // to store the token data in it
            struct AstList *var_list; // to store the list of global identifier
        }globalStm,nonlocalStm;

        // for function call node
        struct function_call{
            Token name;
            struct AstList *arguments;
        }funcCall;

        // for storing the binary statement
        struct binary_Stmt{

        }binaryStm;

   }op;

}Ast;


typedef struct Ast_list{
    Ast ast;
    struct Ast_list *next;
}AstList;

////////////////////////////////////////////////////////////////////////////////////////////////

#endif // DECLARATION_H_INCLUDED

