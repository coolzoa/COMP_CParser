
%option noyywrap yylineno

%x COMMENT
%x IFILE

/*Universal char name*/
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})

/*float exponente*/
EXP ([Ee][-+]?[0-9]+)

/*int length*/
ILEN ([Uu](L|l|LL|ll)?|(L|l|LL|ll)[Uu]?)

/*whitespace*/
WS [ \t\v\n\f]

%{
	#include "CCode.tab.h"
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	
	#define YYDECL int yylex()	

	extern void yyerror(const char *);
%}


%%

"/*"			{BEGIN(COMMENT);}
<COMMENT>"*/"		{BEGIN(INITIAL);}
<COMMENT>([^*]|\n)+|.	{}
<COMMENT><<EOF>>	{ yyerror("Comentario sin terminar\n"); }
"//".*\n		{}


"auto"			{ return AUTO; }
"break"			{ return BREAK; }
"case"			{ return CASE; }
"char"			{ return CHAR; }
"const"			{ return CONST; }
"continue"		{ return CONTINUE; }
"default"		{ return DEFAULT; }
"do"			{ return DO; }
"double"		{ return DOUBLE; }
"else"			{ return ELSE; }
"enum"			{ return ENUM; }
"extern"		{ return EXTERN; }
"float"			{ return FLOAT; }
"for"			{ return FOR; }
"goto"			{ return GOTO; }
"if"			{ return IF; }
"inline"		{ return INLINE; }
"int"			{ return INT; }
"long"			{ return LONG; }
"register"		{ return REGISTER; }
"restrict"		{ return RESTRICT; }
"return"		{ return RETURN; }
"short"			{ return SHORT; }
"signed"		{ return SIGNED; }
"sizeof"		{ return SIZEOF; }
"static"		{ return STATIC; }
"struct"		{ return STRUCT; }
"switch"		{ return SWITCH; }
"typedef"		{ return TYPEDEF; }
"union"			{ return UNION; }
"unsigned"		{ return UNSIGNED; }
"void"			{ return VOID; }
"volatile"		{ return VOLATILE; }
"while"			{ return WHILE; }

"_Alignas"		{ return ALIGNAS; }
"_Alignof"		{ return ALIGNOF; }
"_Atomic"		{ return ATOMIC; }
"_Bool"			{ return BOOL; }
"_Complex"		{ return COMPLEX; }
"_Generic"		{ return GENERIC; }
"_Imaginary"		{ return IMAGINARY; }
"_Noreturn"		{ return NORETURN; }
"_Static_assert"	{ return STATIC_ASSERT; }
"_Thread_local"		{ return THREAD_LOCAL; }
"__func__"		{ return FUNC_NAME; }


0[0-7]*{ILEN}?		{ yylval.intval = (int)strtol(yytext,NULL,8); return I_CONSTANT; }
[1-9][0-9]*{ILEN}?	{ yylval.intval = atoi(yytext); return I_CONSTANT; }

0[Xx][0-9a-fA-F]+{ILEN}? {yylval.intval = (int)strtol(yytext,NULL,0); return I_CONSTANT; }


([0-9]*\.[0-9]+|[0-9]+\.){EXP}?[flFL]?	{yylval.floatval = atof(yytext); return F_CONSTANT; }
[0-9]+{EXP}[flFL]?	{ yylval.floatval = atof(yytext); return F_CONSTANT; }


\'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9a-fA-F]+|{UCN})+\' { yylval.strval = strdup(yytext); return STRING_LITERAL; }

L?\"([^"\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9a-fA-F]+|{UCN})*\" { yylval.strval = strdup(yytext); return STRING_LITERAL; }

"..."			{ return ELLIPSIS; }
">>="			{ return RIGHT_ASSIGN; }
"<<="			{ return LEFT_ASSIGN; }
"+="			{ return ADD_ASSIGN; }
"-="			{ return SUB_ASSIGN; }
"*="			{ return MUL_ASSIGN; }
"/="			{ return DIV_ASSIGN; }
"%="			{ return MOD_ASSIGN; }
"&="			{ return AND_ASSIGN; }
"^="			{ return XOR_ASSIGN; }
"|="			{ return OR_ASSIGN; }
">>"			{ return RIGHT_OP; }
"<<"			{ return LEFT_OP; }
"++"			{ return INC_OP; }
"--"			{ return DEC_OP; }
"->"			{ return PTR_OP; }
"&&"			{ return AND_OP; }
"||"			{ return OR_OP; }
"<="			{ return LE_OP; }
">="			{ return GE_OP; }
"=="			{ return EQ_OP; }
"!="			{ return NE_OP; }
";"			{ return ';'; }
("{"|"<%")		{ return '{'; }
("}"|"%>")		{ return '}'; }
","			{ return ','; }
":"			{ return ':'; }
"="			{ return '='; }
"("			{ return '('; }
")"			{ return ')'; }
("["|"<:")		{ return '['; }
("]"|":>")		{ return ']'; }
"."			{ return '.'; }
"&"			{ return '&'; }
"!"			{ return '!'; }
"~"			{ return '~'; }
"-"			{ return '-'; }
"+"			{ return '+'; }
"*"			{ return '*'; }
'/'			{ return '/'; }
"%"			{ return '%'; }
"<"			{ return '<'; }
">"			{ return '>'; }
"^"			{ return '^'; }
"|"			{ return '|'; }
"?"			{ return '?'; }
{WS}+			{/* ignoramos los espacios */}


([_a-zA-Z]|{UCN})([_a-zA-Z0-9]|{UCN})* {yylval.strval = strdup(yytext); return IDENTIFIER;}


"#"" "*if.*\n 	{/*no haremos nada con el preprocesador*/}
"#"" "*else.*\n	{/*no haremos nada con el preprocesador*/}
"#"" "*endif.*\n {/*no haremos nada con el preprocesador*/}
"#"" "*define.*\n {/*no haremos nada con el preprocesador*/}
"#"" "*line.*\n	   {/*no haremos nada con el preprocesador*/}

{WS}*"#"[ \t]*include[ \t]*[\"<] { BEGIN IFILE; }

<IFILE>[^>\"]+ {
			{ int c;
			  int indice = 0;
			  char filename[1024] = {0};
		
			  while((c = input()) && c != '\n');
			   }
				
			//printf("Archivo include es: %s\n",yytext);
			BEGIN INITIAL;
			}

<IFILE>.|\n	{printf("Error en include\n");
			BEGIN INITIAL;
		}

.			{ } //printf("syntax error, found:'%c'\n",*yytext); 

%%



