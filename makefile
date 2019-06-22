all: ccode programa2

ccode: CCode.l CCode.y 
	bison -d  CCode.y	
	flex CCode.l 
	cc -o $@ CCode.tab.c lex.yy.c -lfl

programa2: preprocesador.c
	   gcc -o programa2 preprocesador.c
	

