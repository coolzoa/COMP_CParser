
/*Parser para lenguaje C*/

%{
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#define red "\033[0;31m"
#define cyan "\033[1;36m"
#define green "\033[4;32m"
#define blue "\033[9;34m]"
#define magenta "\033[0;35m"
#define none "\033[0m"

extern int yylex();
extern int yyparse();
void yyerror(char const *s);



%}

%union{
	int intval;
	double floatval;
	char* strval;
}

%define parse.error verbose

%token IDENTIFIER I_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN

%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token CONST RESTRICT VOLATILE
%token BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token COMPLEX IMAGINARY
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL

%start translation_unit

%%

primary_expression
	: IDENTIFIER { printf("Entre a primary exp 1\n"); }
	| constant   { printf("Entre a primary exp 2\n"); }
	| string     { printf("Entre a primary exp 3\n"); }
	| '(' expression ')' {printf("Entre a primary expression 4\n"); }
	| generic_selection { printf("Entre a primary expression 5\n"); }


	;

constant
	: I_CONSTANT { printf("Entre a integer constant\n"); }
	| F_CONSTANT { printf("Entre a float constant\n"); }
	;

enumeration_constant
	: IDENTIFIER { printf("Entre a enumeration constant con identifier\n"); }
	;

string
	: STRING_LITERAL { printf("Entre a string con string literal\n"); }
	| FUNC_NAME { printf("Entre a string con func name\n"); }
	;

generic_selection
	: GENERIC '(' assignment_expression ',' generic_assoc_list ')' { printf("Entre a generic selection con generic\n"); }


	;

generic_assoc_list
	: generic_association { printf("Entre a generic assoc list\n"); }
	| generic_assoc_list ',' generic_association { printf("Entre a generic assoc 2\n"); }
	;

generic_association
	: type_name ':' assignment_expression { printf("Entre a generic association 1\n"); }
	| DEFAULT ':' assignment_expression { printf("Entre a generic association 2\n"); }
	;

postfix_expression
	: primary_expression { printf("Entre a postfix 1\n"); }
	| postfix_expression '[' expression ']' { printf("Entre a postfix 2\n"); }
	| postfix_expression '(' ')' { printf("Entre a postfix 3\n"); }
	| postfix_expression '(' argument_expression_list ')' { printf("Entre a postfix 4\n"); }
	| postfix_expression '.' IDENTIFIER { printf("Entre a postfix 5\n"); }
	| postfix_expression PTR_OP IDENTIFIER { printf("Entre a postfix 6\n"); }
	| postfix_expression INC_OP { printf("Entre a postfix 7\n"); }
	| postfix_expression DEC_OP { printf("Entre a postfix 8\n"); }
	| '(' type_name ')' '{' initializer_list '}' { printf("Entre a postfix 9"); }
	| '(' type_name ')' '{' initializer_list ',' '}' { printf("Entre a postfix 10"); }


	;

argument_expression_list
	: assignment_expression { printf("Entre a argument expression list 1\n"); }
	| argument_expression_list ',' assignment_expression { printf("Entre a argument expression list 2\n"); }
	;

unary_expression
	: postfix_expression { printf("Entre a unary 1\n"); }
	| INC_OP unary_expression { printf("Entre a unary 2\n"); }
	| DEC_OP unary_expression { printf("Entre a unary 3\n"); }
	| unary_operator cast_expression { printf("Entre a unary 4\n"); }
	| SIZEOF unary_expression { printf("Entre a unary 5\n"); }
	| SIZEOF '(' type_name ')' { printf("Entre a unary 6\n"); }
	| ALIGNOF '(' type_name ')' { printf("Entre a unary 7\n"); }


	;

unary_operator
	: '&' { printf("Entre a unary operator 1\n"); }
	| '*' { printf("Entre a unary operator 2\n"); }
	| '+' { printf("Entre a unary operator 3\n"); }
	| '-' { printf("Entre a unary operator 4\n"); }
	| '~' { printf("Entre a unary operator 5\n"); }
	| '!' { printf("Entre a unary operator 6\n"); }
	;

cast_expression
	: unary_expression { printf("Entre a cast expresion 1\n"); }
	| '(' type_name ')' cast_expression { printf("Entre a cast expression 2\n"); }
	;

multiplicative_expression
	: cast_expression { printf("Entre a mult expression 1\n"); }
	| multiplicative_expression '*' cast_expression { printf("Entre a mult expression 2\n"); }
	| multiplicative_expression '/' cast_expression { printf("Entre a mult expression 3\n"); }
	| multiplicative_expression '%' cast_expression { printf("Entre a mult expression 4\n"); }
	;

additive_expression
	: multiplicative_expression { printf("Entre a add expression 1\n"); }
	| additive_expression '+' multiplicative_expression { printf("Entre a add expression 2\n"); }
	| additive_expression '-' multiplicative_expression { printf("Entre a add expression 3\n"); }
	;

shift_expression
	: additive_expression { printf("Entre a shift exp 1\n"); }
	| shift_expression LEFT_OP additive_expression { printf("Entre a shift exp 2\n"); }
	| shift_expression RIGHT_OP additive_expression {printf("Entre a shift exp 3\n"); }
	;

relational_expression
	: shift_expression { printf("Entre a rel exp 1\n"); }
	| relational_expression '<' shift_expression { printf("Entre a rel exp 2\n"); }
	| relational_expression '>' shift_expression { printf("Entre a rel exp 3\n"); }
	| relational_expression LE_OP shift_expression { printf("Entre a rel exp 4\n"); }
	| relational_expression GE_OP shift_expression { printf("Entre a rel exp 5\n"); }
	;

equality_expression
	: relational_expression { printf("Entre a eq exp 1\n"); }
	| equality_expression EQ_OP relational_expression { printf("Entre a eq exp 2\n"); }
	| equality_expression NE_OP relational_expression { printf("Entre a eq exp 3\n"); }
	;

and_expression
	: equality_expression { printf("Entre a and exp 1\n"); }
	| and_expression '&' equality_expression { printf("Entre a and exp 2\n"); }
	;

exclusive_or_expression
	: and_expression { printf("Entre a exor exp 1\n"); }
	| exclusive_or_expression '^' and_expression { printf("Entre a exor exp 2\n"); }
	;

inclusive_or_expression
	: exclusive_or_expression { printf("Entre a inc or exp 1\n"); }
	| logical_and_expression '|' exclusive_or_expression { printf("Entre a inc or exp 2\n"); }
	;

logical_and_expression
	: inclusive_or_expression { printf("Entre a log and exp 1\n"); }
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression { printf("Entre a log or exp 1\n"); }
	| logical_or_expression OR_OP logical_and_expression { printf("Entre a log or exp 2\n"); }
	;

conditional_expression
	: logical_or_expression { printf("Entre a cond exp 1\n"); }
	| logical_or_expression '?' expression ':' conditional_expression { printf("Entre a cond exp 2\n"); }

	;

assignment_expression
	: conditional_expression { printf("Entre a ass exp 1\n"); }
	| unary_expression assignment_operator assignment_expression { printf("Entre a ass exp 2\n"); }
	;

assignment_operator
	: '=' { printf("Entre a ass op 1\n"); }
	| MUL_ASSIGN { printf("Entre a ass op 2\n"); }
	| DIV_ASSIGN { printf("Entre a ass op 3\n"); }
	| MOD_ASSIGN { printf("Entre a ass op 4\n"); }
	| ADD_ASSIGN { printf("Entre a ass op 5\n"); }
	| SUB_ASSIGN { printf("Entre a ass op 6\n"); }
	| LEFT_ASSIGN { printf("Entre a ass op 7\n"); }
	| RIGHT_ASSIGN { printf("ENtre a ass op 8\n"); }
	| AND_ASSIGN {printf("Entre a ass op 9\n"); }
	| XOR_ASSIGN {printf("Entre a ass op 10\n"); }
	| OR_ASSIGN {printf("Entre a ass op 11\n"); }
	;

expression
	: assignment_expression { printf("Entre a exp 1\n"); }
	| expression ',' assignment_expression { printf("Entre a exp 2\n"); }
	;

constant_expression
	: conditional_expression { printf("Entre a cons exp 1\n"); }
	;

declaration
	: declaration_specifiers ';' { printf("Entre a dec 1\n"); }
	| declaration_specifiers init_declarator_list ';' { printf("Entre a dec 2\n"); }
	| static_assert_declaration { printf("Entre a dec 3\n"); }

	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers { printf("Entre a dec spec 1\n"); }
	| storage_class_specifier { printf("Entre a dec spec 2\n"); }
	| type_specifier declaration_specifiers { printf("Entre a dec spec 3\n"); }
	| type_specifier { printf("Entre a dec spec 4\n"); }
	| type_qualifier declaration_specifiers { printf("Entre a dec spec 5\n"); }
	| type_qualifier { printf("Entre a dec spec 6\n"); }
	| function_specifier declaration_specifiers { printf("Entre a dec spec 6\n"); }
	| function_specifier { printf("Entre a dec spec 7\n"); }
	| alignment_specifier declaration_specifiers { printf("Entre a dec spec 7\n"); }
	| alignment_specifier { printf("Entre a dec spec 8\n"); }
	;

init_declarator_list
	: init_declarator { printf("Entre a init dec 1\n"); }
	| init_declarator_list ',' init_declarator { printf("Entre a init dec 2\n"); }
	;

init_declarator
	: declarator '=' initializer { printf("Entre a init dec 1\n"); }
	| declarator { printf("Entre a init dec 2\n"); }


	;

storage_class_specifier
	: TYPEDEF { printf("Entre a storage class spec 1\n"); }
	| EXTERN { printf("Entre a storage class spec 2\n"); }
	| STATIC { printf("Entre a storage class spec 3\n"); }
	| THREAD_LOCAL { printf("Entre a storage class spec 4\n"); }
	| AUTO { printf("Entre a storage class spec 4\n"); }
	| REGISTER { printf("Entre a storage class spec 5\n"); }
	;

type_specifier
	: VOID { printf("Entre a type spec 1\n"); }
	| CHAR { printf("Entre a type spec 2\n"); }
	| SHORT { printf("Entre a type spec 3\n"); }
	| INT { printf("Entre a type spec 4\n"); }
	| LONG { printf("Entre a type spec 5\n"); }
	| FLOAT { printf("Entre a type spec 6\n"); }
	| DOUBLE { printf("Entre a type spec 7\n"); }
	| SIGNED { printf("Entre a type spec 8\n"); }
	| UNSIGNED { printf("Entre a type spec 9\n"); }
	| BOOL { printf("Entre a type spec 10\n"); }
	| COMPLEX { printf("Entre a type spec 11\n"); }
	| IMAGINARY { printf("Entre a type spec 12\n"); }
	| atomic_type_specifier { printf("Entre a type spec 13\n"); }
	| struct_or_union_specifier { printf("Entre a type spec 14\n"); }
	| enum_specifier { printf("Entre a type spec 15\n"); }
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}' { printf("Entre a struc o union spec 1\n"); }
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}' { printf("Entre a struc o union spec 2\n"); }
	| struct_or_union IDENTIFIER { printf("Entre a struct o union spec 3\n"); }


	;

struct_or_union
	: STRUCT { printf("Entre a struct o union 1\n"); }
	| UNION { printf("Entre a struct o union 2\n"); }
	;

struct_declaration_list
	: struct_declaration { printf("Entre a struct dec list 1\n"); }
	| struct_declaration_list ',' struct_declaration { printf("Entre a struct dec list 2\n"); }
	| struct_declaration_list ';' struct_declaration { printf("Entre a struct dec list 2\n"); }

	
	;

struct_declaration
	: specifier_qualifier_list ';' {printf("Entre a struct dec 1\n"); }
	| specifier_qualifier_list struct_declarator_list { printf("Entre a struct dec 2\n"); }
	| static_assert_declaration { printf("Entre a struct dec 3\n"); }

	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list { printf("Entre a spec qual list 1\n"); }
	| type_specifier	{ printf("Entre a spec equal list 2\n"); }
	| type_qualifier specifier_qualifier_list { printf("Entre a spec qual list 3\n"); }
	| type_qualifier	{ printf("Entre a spec equal list 4\n"); }
	;

struct_declarator_list
	: struct_declarator { printf("Entre a struct dec list 1\n"); }
	| struct_declarator_list ',' struct_declarator { printf("Entre a struct dec list 2\n"); }
	;

struct_declarator
	: ':' constant_expression { printf("Entre a struct dec 1\n"); }
	| declarator ':' constant_expression { printf("Entre a struct dec 2\n"); }
	| declarator	{ printf("Entre a struct dec 3\n"); }

	;

enum_specifier
	: ENUM '{' enumerator_list '}' { printf("Entre a enum spec 1\n"); }
	| ENUM '{' enumerator_list ',' '}' { printf("Entre a enum spec 2\n"); }
	| ENUM IDENTIFIER '{' enumerator_list '}'{ printf("Entre a enum spec 3\n"); }
	| ENUM IDENTIFIER '{' enumerator_list ',' '}' { printf("Entre a enum spec 4\n"); }
	| ENUM IDENTIFIER


	;

enumerator_list
	: enumerator	{ printf("Entre a enumerator list 1\n"); }
	| enumerator_list ',' enumerator { printf("Entre a enumerator list 2\n"); }
	;

enumerator
	: enumeration_constant '=' constant_expression { printf("Entre a enumerator 1\n"); }
	| enumeration_constant { printf("Entre a enumerator 2\n"); }


	;

atomic_type_specifier
	: ATOMIC '(' type_name ')' { printf("Entre a atomic spec 1\n"); }


	;

type_qualifier
	: CONST { printf("Entre a type qual 1\n"); }
	| RESTRICT { printf("Entre a type qual 2\n"); }
	| VOLATILE { printf("Entre a type qual 3\n"); }
	| ATOMIC { printf("Entre a type qual 4\n"); }
	;

function_specifier
	: INLINE { printf("Entre a func spec 1\n"); }
	| NORETURN { printf("Entre a func spec 2\n"); }
	;

alignment_specifier
	: ALIGNAS '(' type_name ')' { printf("Entre a all spec 1\n"); }
	| ALIGNAS '(' constant_expression ')' { printf("Entre a all spec 2\n"); }

	;

declarator
	: pointer direct_declarator { printf("Entre a declarator 1\n"); }
	| direct_declarator { printf("Entre a declarator 2\n"); }
	| error  { printf("Soy el culpable 3\n"); }
	
	;

direct_declarator
	: IDENTIFIER { printf("Entre a dir declarator 1\n"); }
	| '(' declarator ')' { printf("Entre a dir declarator 2\n"); }
	| direct_declarator '[' ']' { printf("Entre a dir declarator 3\n"); }
	| direct_declarator '[' '*' ']' { printf("Entre a dir declarator 4\n"); }
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']' { printf("Entre a dir declarator 5\n"); }
	| direct_declarator '[' STATIC assignment_expression ']' { printf("Entre a dir declarator 6\n"); }
	| direct_declarator '[' type_qualifier_list '*' ']' { printf("Entre a dir declarator 7\n"); }
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']' { printf("Entre a dir declarator 8\n"); }
	| direct_declarator '[' type_qualifier_list assignment_expression ']' { printf("Entre a dir declarator 9\n"); }
	| direct_declarator '[' type_qualifier_list ']' { printf("dir declarator 10\n"); }
	| direct_declarator '[' assignment_expression ']' { printf("Entre a dir declarator 11\n"); }
	| direct_declarator '(' parameter_type_list ')' { printf("Entre a dir declarator 12\n"); }
	| direct_declarator '(' ')' { printf("Entre a dir declarator 13\n"); }
	| direct_declarator '(' identifier_list ')' { printf("Entre a dir declarator 14\n"); }


	;

pointer
	: '*' type_qualifier_list pointer {printf("Entre a pointer 1\n"); }
	| '*' type_qualifier_list {printf("Entre a pointer 2\n"); }
	| '*' pointer {printf("Entre a pointer 3\n"); }
	| '*' {printf("Entre a pointer 4\n"); }
	;

type_qualifier_list
	: type_qualifier {printf("Entre a type qual list 1\n"); }
	| type_qualifier_list type_qualifier {printf("Entre a type qual list 2\n"); }
	;

parameter_type_list
	: parameter_list ',' ELLIPSIS {printf("Entre a paramter type list 1\n"); }
	| parameter_list {printf("Entre a paramter type list 2\n"); }
	| parameter_list error parameter_declaration {printf("Soy el culpable 4\n");}

	;

parameter_list
	: parameter_declaration { printf("Entre a parameter list 1\n"); }
	| parameter_list ',' parameter_declaration { printf("Entre a parameter list 2\n"); }
	| parameter_list error parameter_declaration {printf("Soy el culpable 5\n");}
	;

parameter_declaration
	: declaration_specifiers declarator { printf("Entre a parameter dec  1\n"); }
	| declaration_specifiers abstract_declarator { printf("Entre a parameter dec 2\n"); }
	| declaration_specifiers { printf("Entre a parameter dec 3\n"); }
	;

identifier_list
	: IDENTIFIER { printf("Entre a identifier list 1\n"); }
	| identifier_list ',' IDENTIFIER { printf("Entre a identifier list 2\n"); }


	;

type_name
	: specifier_qualifier_list abstract_declarator {printf("Entre a type name 1\n"); }
	| specifier_qualifier_list {printf("Entre a type name 2\n"); }
	;

abstract_declarator
	: pointer direct_abstract_declarator { printf("Entre a abstract dec 1\n"); }
	| pointer { printf("Entre a abstract dec 2\n"); }
	| direct_abstract_declarator { printf("Entre a abstract dec 3\n"); }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' { printf("Entre a dir abs dec 1\n"); }
	| '[' ']' { printf("Entre a dir abs dec 2\n"); }
	| '[' '*' ']' { printf("Entre a dir abs dec 3\n"); }
	| '[' STATIC type_qualifier_list assignment_expression ']' { printf("Entre a dir abs dec 4\n"); }
	| '[' STATIC assignment_expression ']' { printf("Entre a dir abs dec 5\n"); }
	| '[' type_qualifier_list STATIC assignment_expression ']' { printf("Entre a dir abs dec 6\n"); }
	| '[' type_qualifier_list assignment_expression ']' { printf("Entre a dir abs dec 7\n"); }
	| '[' type_qualifier_list ']' { printf("Entre a dir abs dec 8\n"); }
	| '[' assignment_expression ']' { printf("Entre a dir abs dec 9\n"); }
	| direct_abstract_declarator '[' ']' { printf("Entre a dir abs dec 10\n"); }
	| direct_abstract_declarator '[' '*' ']' { printf("Entre a dir abs dec 11\n"); }
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']' { printf("Entre a dir abs dec 12\n"); }
	| direct_abstract_declarator '[' STATIC assignment_expression ']' { printf("Entre a dir abs dec 13\n"); }
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']' { printf("Entre a dir abs dec 14\n"); }
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']' { printf("Entre a dir abs dec 15\n"); }
	| direct_abstract_declarator '[' type_qualifier_list ']' { printf("Entre a dir abs dec 16\n"); }
	| direct_abstract_declarator '[' assignment_expression ']' { printf("Entre a dir abs dec 17\n"); }
	| '(' ')' { printf("Entre a dir abs dec 18\n"); }
	| '(' parameter_type_list ')' { printf("Entre a dir abs dec 19\n"); }
	| direct_abstract_declarator '(' ')' { printf("Entre a dir abs dec 20\n"); }
	| direct_abstract_declarator '(' parameter_type_list ')' { printf("Entre a dir abs dec 21\n"); }

	;

initializer
	: '{' initializer_list '}' { printf("Entre a initializer 1\n"); }
	| '{' initializer_list ',' '}' { printf("Entre a initializer 2\n"); }
	| assignment_expression { printf("Entre a initializer 3\n"); }

	;

initializer_list
	: designation initializer { printf("Entre a initializer_list 1\n"); }
	| initializer designation initializer { printf("Entre a initializer_list 2\n"); }
	| initializer_list ',' designation initializer designation initializer { printf("Entre a initializer_list 3\n"); }
	| initializer_list ',' initializer designation initializer { printf("Entre a initializer_list 4\n"); }

	;

designation
	: designator_list '=' { printf("Entre a designation 1\n"); }

	;

designator_list
	: designator { printf("Entre a designator_list 1\n"); }
	| designator_list designator { printf("Entre a designator_list 2\n"); }
	;

designator
	: '[' constant_expression ']' { printf("Entre a designator 1\n"); }
	| '.' IDENTIFIER { printf("Entre a designator 2\n"); }


	;

static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';' { printf("Entre a static assert dec 1\n"); }

	
	;

statement
	: labeled_statement { printf("Entre a statement 1\n"); }
	| compound_statement { printf("Entre a statement 2\n"); }
	| expression_statement { printf("Entre a statement 3\n"); }
	| selection_statement { printf("Entre a statement 4\n"); }
	| iteration_statement { printf("Entre a statement 5\n"); }
	| jump_statement { printf("Entre a statement 6\n"); }
	;

labeled_statement
	: IDENTIFIER':' statement { printf("labeled statement 1\n"); }
	| CASE constant_expression ':' statement { printf("labeled statement 2\n"); }
	| DEFAULT ':' statement { printf("labeled statement 3\n"); }
	

	;

compound_statement
	: '{' '}' { printf("Entre a copound statement 1\n"); }
	| '{' block_item_list '}' { printf("Entre a copound statement 2\n"); }


	
	;

block_item_list
	: block_item { printf("Entre a block item list 1\n"); }
	| block_item_list block_item { printf("Entre a block item list 2\n"); }
	;

block_item
	: declaration { printf("Entre a block item 1\n"); }
	| statement { printf("Entre a block item 2\n"); }
	;

expression_statement
	: ';' { printf("Entre a exp stat 1\n"); }
	| expression ';' { printf("Entre a exp stat 2\n"); }
	| error  { printf("Soy el culpable 1\n"); }
	/* | expression error  { printf("Soy el culpable 2\n"); } */
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement { printf("Entre a selection statement 1\n"); }
	| IF '(' expression ')' statement { printf("Entre a selection statement 2\n"); }
	| SWITCH '(' expression ')' statement { printf("Entre a selection statement 3\n"); }

	
	;

iteration_statement
	: WHILE '(' expression ')' statement { printf("Entre a iteration statement 1\n"); }
	| DO statement WHILE '(' expression ')' ';' { printf("Entre a iteration statement 2\n"); }
	| FOR '(' expression_statement expression_statement ')' statement { printf("Entre a iteration statement 3\n"); }
	| FOR '(' expression_statement expression_statement expression ')' statement { printf("Entre a iteration statement 4\n"); }
	| FOR '(' declaration expression_statement ')' statement { printf("Entre a iteration statement 5\n"); }
	| FOR '(' declaration expression_statement expression ')' statement { printf("Entre a iteration statement 6\n"); }

	;

jump_statement
	: GOTO IDENTIFIER ';' { printf("Entre a jmp statement 1\n"); }
	| CONTINUE ';' { printf("Entre a jmp statement 2\n"); }
	| BREAK ';' { printf("Entre a jmp statement 3\n"); }
	| RETURN ';' { printf("Entre a jmp statement 4\n"); }
	| RETURN expression ';' { printf("Entre a jmp statement 5\n"); }

	;

translation_unit
	: external_declaration { printf("Entre a translation unit 1\n"); }
	| translation_unit external_declaration { printf("Entre a translation unit 2\n"); }
	| error {printf("Soy el culpable 6\n");}

	;

external_declaration
	: function_definition { printf("Entre a external dec 1\n"); }
	| declaration { printf("Entre a external dec 2\n"); }
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement { printf("Entre a func def 1\n"); }
	| declaration_specifiers declarator compound_statement { printf("Entre a func def 2\n"); }
	;

declaration_list
	: declaration { printf("Entre a dec list 1\n"); }
	| declaration_list declaration { printf("Entre a dec list 2\n"); }
	;

%%

void emit (char *s)
{
	
}
void yyerror (char const *s)
{
	extern int yylineno;
	extern char* yytext;
	

	printf(" In line %s  %d %s %s found: %s %s \n", green, yylineno, red, s, cyan, yytext);
	printf("%s",none);
	
}

int main(int ac, char **av)
{
	if (!yyparse()){
		printf("El parser ha completado\n");
	}else{
		printf("El parser ha fallado\n");
	}
	return 0;
}
	
	












	










