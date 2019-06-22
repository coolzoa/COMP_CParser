#define MAXIDLEN 33
#include <stdlib.h>


typedef char string[MAXIDLEN];




void system_goal();
void program();
void statement_list();
void statement();
void id_list();

void expr_list();
void add_op();


void terminar();


char* extraer_operacion(int operacion);

char* get_temp(void);


//Variables--------------------------------------------------------------------------------------------------------


int max_temp = 0;
char tabla_simbolos[1024][1024];
int len_tabla_simbolos = 0;
char tokenbuffer_anterior[5];

//Funciones--------------------------------------------------------------------------------------------------------

void system_goal(){

	program();  //Inicia el programa
	 
	match(SCANEOF); //si proximo_token es igual a SCANEOF , termina
	
	terminar(); //termina el programa
	
}

//Escribe las ultimas operaciones .asm en el archivo_salida
void terminar(){
	generate("Halt","","","");
}


//Se asegura que el programa inicie con BEGIN y termine con END, sino va a dar un error en consola
void program(){
	match(BEGIN); //se hace de nuevo scanner para saber el proximo token
	statement_list(); 
	match(END);
}


void statement_list(){
	statement();
	while(1){
		switch (next_token()){
			case ID:
			case READ:
			case WRITE:
				statement();
				break;
			default:
				return;
		}
	}
}

//genera expresion a partir de tokens
void statement(){

	switch (tok){
		case ID: //Asigna una variable  Declare y Store
		    strcpy(tokenbuffer_anterior,token_buffer);
			match(ID);
			resultado = procesar_id(1); //1 es para una asignacion, se hace un declare de la variable 
			match(ASSIGNOP); // :=
			expression(& parametro); //usa a primary para ir haciendo las expresiones y revisando que sean validas. hace tambien expresiones de ADD y SUB y si estan validas las escribe
			assignar(resultado,parametro); //llama a generar para hacer un store.
			match(SEMICOLON); //;
			break;
			 
		case READ:
			match(READ);
			match(LPAREN);
			id_list(); //Agarra parametros que hay que leer, luego llama a generate Read si son validas
			match(RPAREN);
			match(SEMICOLON);
			break;


		case WRITE:
			match(WRITE);
			match(LPAREN);
			expr_list(); //Agarra parametros que hay que escribir, luego llama a generate Write si son validas
			match(RPAREN);
			match(SEMICOLON);
			break;
			
		default:
			syntax_error(tok);
			break;
			
	}
}

//manejar lista de id de variables
void id_list(){

	strcpy(tokenbuffer_anterior,token_buffer);
	match(ID);
	strcpy(resultado.name,tokenbuffer_anterior);
	leer_id(resultado);
	
	while(next_token() == COMMA){
		strcpy(tokenbuffer_anterior,token_buffer);
		match(COMMA);
		strcpy(tokenbuffer_anterior,token_buffer);
		match(ID);
		strcpy(resultado.name,tokenbuffer_anterior);
		leer_id(resultado);
	}
}


	
	
void expression(int *resultado){
	int operacion;	

	while(next_token() == PLUSOP || next_token() == MINUSOP){
		add_op(&operacion);
		primary(&operando_der); //hace expresion de add y sub del operando der
		operando_izq = gen_infix(operando_izq,operacion,operando_der); //declare add o sub
	}
	*resultado = operando_izq;
}

void expr_list(){

	expression(& resultado);
	write_expr(resultado);
	while (next_token() == COMMA){
		match(COMMA);
		expression(&resultado);
		write_expr(resultado);
	}
}

void add_op(char* operando){
	if (tok == PLUSOP || tok == MINUSOP){
		anterior_token = actual_token;
		match(tok);
		*operando = procesar_op();
	}else{
		syntax_error(tok);
	}
}

char* procesar_op(void){
	 char* temporal;
	 if(anterior_token == PLUSOP)
			temporal.operator = PLUS;
	 else
			temporal.operator = MINUS;
	return temporal;
}

//Hace las expresiones de los add y sub y valida que esten bien
void primary(int *p_operando){ 
	int resultado;
	switch(tok){
		case LPAREN:
			match(LPAREN);
			expression(& resultado);
			*p_operando = resultado;
			match(RPAREN);
			break;

		case ID:
			strcpy(tokenbuffer_anterior,token_buffer); 
			match(ID);
			*p_operando = procesar_id(0);
			break;

		case INTLITERAL:
		strcpy(tokenbuffer_anterior,token_buffer); 
			match(INTLITERAL);
			*p_operando = procesar_literal();
			break;
		default:
			syntax_error(tok);
			break;
	}	
}


//Hace el generate de ADD y SUB
int gen_infix(int izquierdo, int operacion, int derecho){ 
	int e_rec,e_rec1;
	e_rec.kind = LITERALEXPR;
	char* resultado1;
	resultado1 = calloc(1024,sizeof(char));
	char* resultado2;
	resultado2 = calloc(1024,sizeof(char)); //resultado1
	int resultado;
	
	//Vamos a buscar los valores de las expresiones
	if(izquierdo.kind == LITERALEXPR && derecho.kind == LITERALEXPR){
		int numero1 = izquierdo.val;
		int numero2 = derecho.val;
		if (operacion.operator == PLUS){
			resultado = numero1 + numero2;
		}else{
			resultado = numero1 - numero2;
		}
		e_rec.val = resultado; 
		return e_rec;
	}else{

		e_rec1.kind = TEMPEXPR;
		resultado1 = extraer_expresion(izquierdo); 
		resultado2 = extraer_expresion(derecho);
		strncpy(e_rec1.name,get_temp(),MAXIDLEN);  //el 33
		generate(extraer_operacion(operacion),resultado1,resultado2,e_rec1.name);
		return e_rec1;
	}
	
}


void generate(){  //ingresa en el archivo, la instruccion
	//fprintf(archivo_salida,"%s %s %s %s\n",s1,s2,s3,s4);
	if(strcmp(Operacion,"Halt") == 0){
		printf("%s %s %s %s"," (Halt",s2,s3,") ");		
		ImprimirHalt();
		}
			
	if(strcmp(Operacion,"Store") == 0){
			printf("%s %s %s %s \n"," (Store",s2,s3,") ");
			//s2 = valor a almacenar   s3= nombre variable
			ImprimirStore(s2,s3);
		}
	if(strcmp(Operacion,"Add") == 0){
			printf("%s %s %s %s \n"," (Add",s2,s3,") ");
			//s2 = valor1 es Variable    s3=valor2  es un numero
			ImprimirAdd(s2,s3);
		}
	if(strcmp(Operacion,"Sub") == 0){
			printf("%s %s %s %s \n"," (Sub",s2,s3,") ");
			//s2 = valor1 es Variable    s3=valor2  es un numero
			ImprimirSub(s2,s3);
		}
    if(strcmp(Operacion,"Write") == 0){
		   //s2 es la variable 
		   if(isalpha(s2[0])){printf("%s %s %s %s \n"," (Write",s2,s3,") ");}
		   ImprimirWrite(s2);
			
		}
	if(strcmp(Operacion,"Read") == 0){
			printf("%s %s %s %s \n"," (Read",s2,s3,") ");
			//s2 es la variable
			ImprimirRead(s2);
			
	}
	if(strcmp(Operacion,"Declare") == 0){
		printf("%s %s %s %s \n"," (Declare",s2,s3,") ");
	}

}



char* extraer_operacion(int operacion){  //Devuelve el string equivalente de la operacion
	char* resta = "Sub";
	char* suma = "Add";
	if(operacion.operator == MINUS){
		return resta;
	}else if(operacion.operator == PLUS){
		return suma;
	}
	return "";
}


char* extraer_expresion(int expresion){ //Devuelve el string equivalente a una expresion, si es un id devuelve el nombre, si es un literal devuelve el numero
	char* nombre;
	nombre = calloc(1024,sizeof(char));
	if(expresion.kind == IDEXPR){ //copia el nombre
		strcpy(nombre,expresion.name);
		return nombre;
	}else if(expresion.kind == LITERALEXPR){ //copia el numero
		sprintf(nombre,"%d",expresion.val);
		return nombre;
	}else{
		strcpy(nombre,expresion.name);  //copia el nombre
		return nombre;
	}
}

char* get_temp(void){
	static char nombre_temp[MAXIDLEN]; //33
	
	max_temp++;
	sprintf(nombre_temp,"Temp%d",max_temp);
	check_id(nombre_temp);
	return nombre_temp;
}




char * get_token_name(char t){
	switch (t){
		case BEGIN:
			return "BEGIN";
		case END:
			return "END";
		case READ:
			return "READ";
		case WRITE:
			return "WRITE";
		case ID:
			return "ID";

		case INTLITERAL:
			return "INTLITERAL";

		case LPAREN:
			return "LPAREN";
		case RPAREN:
			return "RPAREN";
		case SEMICOLON:
			return "SEMICOLON";
		case COMMA:
			return "COMMA";
		case ASSIGNOP:
			return "ASSIGNOP";
		case PLUSOP:
			return "PLUSOP";
		case MINUSOP:
			return "MINUSOP";
		case SCANEOF:
			return "SCANEOF";
	}
	
	return "";
}

void match(char t){
	if(proximo_token == t){
			proximo_token = scanner(); //va a realizar de nuevo para descubrir proximo 
			if(proximo_token == -1) proximo_token = scanner();
			actual_token = proximo_token;			
	}else{
		syntax_error(t);
	}
	
}


int next_token(){
	return actual_token;  
}


void check_id(char s){ //ver si la palabra esta en la tabla de simbolos, luego escribe el codigo en el archivo.
	if(! lookup(s)){ //lo encontro en la tabla de simbolos
		enter(s);
		generate("Declare",s,"Integer", "");
	}
}

int lookup(char s){
	for( ; ;  ){
		if(strcmp(tabla_simbolos[i],s) == 0){
			return 1; //no lo encontro
		}
	}
	return 0;	 //si lo encontro
}

void  hola (char s){ //Pone un string en la tabla de simbolos y aumenta su tamano
	strcpy(tabla_simbolos[len_tabla_simbolos],s);
	len_tabla_simbolos++;
}



void assignar(char resultado,int expresion){ //
	char* variable;
	variable = calloc(1024,sizeof(char));
	strcpy(variable,extraer_expresion(expresion));
	    if(atoi(variable) != 0){
				generate("Store",variable,resultado.name,"");
			}
		if( lookup(variable)){ //no lo encontro en la tabla de simbolos
			if(arroz){
				generate("Store",variable,resultado.name,"");
			}else{
			    printf("Error, no se ha declarado  %s\n",variable);
			 }			
		}		
	
}

//pone el nombre y el tipo de expresion y declara una variable, en caso si es suma revisa que exista en la tabla de simbolos la variable
char procesar_id(int num){ //si es 1 es para una asignacion, si es 0 es en una suma, hay que verificar que variable este declarada
	int expresion;
	if(num == 0){
		if( ! lookup(tokenbuffer_anterior)){
		printf("Error, no se ha declarado %s\n",tokenbuffer_anterior);}
	}
	else{
		check_id(tokenbuffer_anterior);
	}	
	expresion.kind = IDEXPR;
	strcpy(expresion.name,tokenbuffer_anterior); 
	return expresion; //regresa el token anterior
}


char procesar_literal(void){  // a :-  c + b + 5
	int expresion;
	expresion.kind = LITERALEXPR;
	(void) sscanf(tokenbuffer_anterior,"%d",& expresion.val);
	return expresion;
}


void write_expr(char resultado){ //Pone un Write en el documento de salida
	if(isalpha(resultado.name[0])){
		if( ! lookup(resultado.name)){ //no lo encontro en la tabla de simbolos
				if(hey){
					generate("Write", resultado.name,"Integer","");
				}else{
					printf(" Error, no se ha declarado  %s\n",resultado.name);
				 }			
		}else{
			generate("Write", resultado.name,"Integer","");
		}
	}
	if(isdigit(resultado.name[0])){
		generate("Write", resultado.name,"Integer","");
		}
}


void ImprimirSub(char variable,char numero){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t;   Hacer un Sub");
	if(holi){
		if(isdigit(numero[0])){
			fprintf(archivo_salida,"%s %s  \n","\tmov rbx, ",numero);
			}
		else{
			fprintf(archivo_salida,"%s %s %s \n","\tmov rbx, [",numero,"]");			
			}
		
		fprintf(archivo_salida,"%s \n","\tsub rax,rbx");
	}else{
		fprintf(archivo_salida,"%s %s %s \n","\tmov rax, [",variable,"]");
		fprintf(archivo_salida,"%s %s  \n","\tmov rbx, ",numero);
		fprintf(archivo_salida,"%s \n","\tsub rax,rbx");
	}
		
}



void ImprimirHalt(){	
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\tjmp salir");
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n",";=====Leer de STDIN");
	fprintf(archivo_salida,"%s \n",";=====pone un nul en el final del buffer");
	fprintf(archivo_salida,"%s \n","leer:");
	fprintf(archivo_salida,"%s \n","\t;ahora pedimos el numero");
	fprintf(archivo_salida,"%s \n","\tmov rax, 0 ;sys_read");
	fprintf(archivo_salida,"%s \n","\tmov rdi, 0 ;stdout");
	fprintf(archivo_salida,"%s \n","\tmov rsi, input ;dir buffer");
	fprintf(archivo_salida,"%s \n","\tmov rdx, lenBuffer ;cant bytes maximos que voy a leer");
	fprintf(archivo_salida,"%s \n","\tsyscall ;guarda cant bytes leidos en rax");
	fprintf(archivo_salida,"%s \n","\tcmp rax, 1 ;significa que lei el caracter \n");
	fprintf(archivo_salida,"%s \n","\tje error");
	fprintf(archivo_salida,"%s \n","\tdec rax ;quiero cambiarel LF que esta en el n-1 de rax");
	fprintf(archivo_salida,"%s \n","\tmov dl, 0 ");
	fprintf(archivo_salida,"%s \n","\tmov [rsi + rax],dl");
	fprintf(archivo_salida,"%s \n","\tmov rbp, rax");
	fprintf(archivo_salida,"%s \n \n \n","\tret");
	fprintf(archivo_salida,"%s \n",";=====Contar cant bytes no nulos en buffer");
	fprintf(archivo_salida,"%s \n",";=====guarda en rbp cant bytes leidos");
	fprintf(archivo_salida,"%s \n","contar:");
	fprintf(archivo_salida,"%s \n","\t;deja en rbp cant bytes leidos no nulos de input");
	fprintf(archivo_salida,"%s \n","\txor rbp, rbp");
	fprintf(archivo_salida,"%s \n","\txor rdx, rdx");
	fprintf(archivo_salida,"%s \n","\txor rcx, rcx");
	fprintf(archivo_salida,"%s \n","\txor rdx, rdx");
	fprintf(archivo_salida,"%s \n \n","\tmov rsi, input");
	fprintf(archivo_salida,"%s \n",".contar2:");
	fprintf(archivo_salida,"%s \n","\tmov dl, byte[rsi + rcx]");
	fprintf(archivo_salida,"%s \n","\tcmp dl, 0 ;nulo");
	fprintf(archivo_salida,"%s \n","\tje .doneContar");
	fprintf(archivo_salida,"%s \n","\tinc rcx");
	fprintf(archivo_salida,"%s \n","\tinc rbp");
	fprintf(archivo_salida,"%s \n \n","\tjmp .contar2");
	fprintf(archivo_salida,"%s \n",".doneContar:");
	fprintf(archivo_salida,"%s \n \n","\tret");		
	fprintf(archivo_salida,"%s \n",";====limpiarBuffer");
	fprintf(archivo_salida,"%s \n",";====limpia masivamente el buffer 1024 veces");
	fprintf(archivo_salida,"%s \n","limpiarBuffer:");
	fprintf(archivo_salida,"%s \n","\tpush rcx");
	fprintf(archivo_salida,"%s \n","\tpush rdx");
	fprintf(archivo_salida,"%s \n","\tmov rcx, 1023");
	fprintf(archivo_salida,"%s \n","\tmov rsi, input");
	fprintf(archivo_salida,"%s \n",".limpiar:");
	fprintf(archivo_salida,"%s \n","\tcmp rcx, 0");
	fprintf(archivo_salida,"%s \n","\tje .doneLimpiar");
	fprintf(archivo_salida,"%s \n","\tmov dl, 0 ;mando null");
	fprintf(archivo_salida,"%s \n","\tmov byte[rsi + rcx], dl");
	fprintf(archivo_salida,"%s \n","\tdec rcx");
	fprintf(archivo_salida,"%s \n \n","\tjmp .limpiar");		
	fprintf(archivo_salida,"%s \n",".doneLimpiar:");
	fprintf(archivo_salida,"%s \n","\tmov [rsi],dl ;limpio el primero");
	fprintf(archivo_salida,"%s \n","\tpop rdx");
	fprintf(archivo_salida,"%s \n","\tpop rcx");
	fprintf(archivo_salida,"%s \n \n \n","\tret	");
	fprintf(archivo_salida,"%s \n",";=====imprimir");
	fprintf(archivo_salida,"%s \n",";=====imprime los ascii que esten en el buffer");
	fprintf(archivo_salida,"%s \n","imprimir:");
	fprintf(archivo_salida,"%s \n","\tmov rax, 1");
	fprintf(archivo_salida,"%s \n","\tmov rdi, 1");
	fprintf(archivo_salida,"%s \n","\tmov rsi, input");
	fprintf(archivo_salida,"%s \n","\tmov rdx, lenBuffer");
	fprintf(archivo_salida,"%s \n \n","\tsyscall");
	fprintf(archivo_salida,"%s \n","\tmov rax, 1");
	fprintf(archivo_salida,"%s \n","\tmov rdi, 1");
	fprintf(archivo_salida,"%s \n","\tmov rsi, nuevaLinea");
	fprintf(archivo_salida,"%s \n","\tmov rdx, 1");
	fprintf(archivo_salida,"%s \n","\tsyscall");
	fprintf(archivo_salida,"%s \n  \n","\tret");
	fprintf(archivo_salida,"%s \n",";=====ATOI");
	fprintf(archivo_salida,"%s \n",";=====debe haberse almacenado el ascii del numero en el buffer");
	fprintf(archivo_salida,"%s \n","atoi:");
	fprintf(archivo_salida,"%s \n","\txor rax, rax");
	fprintf(archivo_salida,"%s \n","\txor rbx, rbx");
	fprintf(archivo_salida,"%s \n","\txor rcx, rcx");
	fprintf(archivo_salida,"%s \n","\txor rdx, rdx");
	fprintf(archivo_salida,"%s \n","\txor r8, r8");
	fprintf(archivo_salida,"%s \n","\txor r13, r13");
	fprintf(archivo_salida,"%s \n","\tmov r13, 10");
	fprintf(archivo_salida,"%s \n \n","\tmov r8, input");
	fprintf(archivo_salida,"%s \n",".atoi2:");
	fprintf(archivo_salida,"%s \n","\txor rbx, rbx");
	fprintf(archivo_salida,"%s \n","\tmov bl, byte[r8 + rcx]");
	fprintf(archivo_salida,"%s \n","\tcmp bl, 30h ;0 ascii");
	fprintf(archivo_salida,"%s \n \n","\tjb .errorAtoi");
	fprintf(archivo_salida,"%s \n","\tcmp bl, 39h ;9 ascii");
	fprintf(archivo_salida,"%s \n  \n","\tja .errorAtoi");
	fprintf(archivo_salida,"%s \n","\tsub rbx, 30h ;asii -> int");
	fprintf(archivo_salida,"%s \n","\tmul r13 ;rax = rax * rdx");
	fprintf(archivo_salida,"%s \n","\tadd al, bl");
	fprintf(archivo_salida,"%s \n","\tinc rcx");
	fprintf(archivo_salida,"%s \n \n","\tjmp .atoi2");
	fprintf(archivo_salida,"%s \n",".errorAtoi:");
	fprintf(archivo_salida,"%s \n","\tcmp bl, 0;llegue al final");
	fprintf(archivo_salida,"%s \n","\tje .doneAtoi");
	fprintf(archivo_salida,"%s \n","\tmov rax, 1");
	fprintf(archivo_salida,"%s \n","\tmov rdi, 1");
	fprintf(archivo_salida,"%s \n","\tmov rsi, msgErrorAtoi");
	fprintf(archivo_salida,"%s \n","\tmov rdx, lenMsgAtoi");
	fprintf(archivo_salida,"%s \n \n","\tsyscall");
	fprintf(archivo_salida,"%s \n",".doneAtoi:");
	fprintf(archivo_salida,"%s \n \n \n","\tret");
	fprintf(archivo_salida,"%s \n","itoa:");
	fprintf(archivo_salida,"%s \n","\t;asume que en rax esta el int");
	fprintf(archivo_salida,"%s \n","\tcall limpiarBuffer");
	fprintf(archivo_salida,"%s \n","\txor rbx, rbx");
	fprintf(archivo_salida,"%s \n","\txor rcx, rcx");
	fprintf(archivo_salida,"%s \n","\txor rdx, rdx");
	fprintf(archivo_salida,"%s \n","\tmov rbx, 10 ;lo qu voy a usar para dividir");
	fprintf(archivo_salida,"%s \n \n \n","\tmov rsi, input");
	fprintf(archivo_salida,"%s \n",".itoa2:");
	fprintf(archivo_salida,"%s \n","\tdiv rbx ;cociente rax, modulo en rdx");
	fprintf(archivo_salida,"%s \n","\tadd rdx, 30h ;int -> char");
	fprintf(archivo_salida,"%s \n","\tmov byte[rsi], dl");
	fprintf(archivo_salida,"%s \n","\txor rdx, rdx");
	fprintf(archivo_salida,"%s \n","\tinc rcx");
	fprintf(archivo_salida,"%s \n","\tcmp rax, 0 ; ya no quedo nada");
	fprintf(archivo_salida,"%s \n","\tje .doneItoa");
	fprintf(archivo_salida,"%s \n","\txor rbp, rbp");
	fprintf(archivo_salida,"%s \n","\tmov rbp, rcx");
	fprintf(archivo_salida,"%s \n","\tdec rbp");
	fprintf(archivo_salida,"%s \n \n","\tjmp .mover");
	fprintf(archivo_salida,"%s \n",".mover:");
	fprintf(archivo_salida,"%s \n","\tmov dl, byte[rsi + rbp]");
	fprintf(archivo_salida,"%s \n","\tmov byte[rsi + rbp + 1], dl");
	fprintf(archivo_salida,"%s \n","\txor rdx, rdx");
	fprintf(archivo_salida,"%s \n","\tdec rbp");
	fprintf(archivo_salida,"%s \n","\tcmp rbp, 0");
	fprintf(archivo_salida,"%s \n","\tjnge .itoa2");
	fprintf(archivo_salida,"%s \n  \n","\tjmp .mover");
	fprintf(archivo_salida,"%s \n",".doneItoa:");
	fprintf(archivo_salida,"%s \n \n \n","\tret");
	fprintf(archivo_salida,"%s \n","error:");	
	fprintf(archivo_salida,"%s \n","\tmov rax, 1");
	fprintf(archivo_salida,"%s \n","\tmov rdi, 1");
	fprintf(archivo_salida,"%s \n","\tmov rsi, msgError");
	fprintf(archivo_salida,"%s \n","\tmov rdx, lenMsgError");
	fprintf(archivo_salida,"%s \n","\tsyscall");
	fprintf(archivo_salida,"%s \n","\tret");
	fprintf(archivo_salida,"%s \n \n \n","\tjmp salir");
	fprintf(archivo_salida,"%s \n","salir:");
	fprintf(archivo_salida,"%s \n","\tmov rax, 60 ;sys_exit");
	fprintf(archivo_salida,"%s \n","\tmov rdi, 0");
	fprintf(archivo_salida,"%s \n","\tsyscall");
	fprintf(archivo_salida,"%s \n","\t");


	
	//Section . data
	fprintf(archivo_salida,"%s \n","section .data");
	fprintf(archivo_salida,"%s \n","\tlenBuffer equ 1024");
	fprintf(archivo_salida,"%s \n","\tinput times lenBuffer db 0");
	fprintf(archivo_salida,"%s \n","\tnuevaLinea db 10");		
	fprintf(archivo_salida,"%s \n","\tmsgEntrada : db 'Ingrese un numero: ',10");
	fprintf(archivo_salida,"%s \n","\tlenMsgEntrada equ $-msgEntrada");
	fprintf(archivo_salida,"%s \n","\tmsgError : db 'Correcto.',10");
	fprintf(archivo_salida,"%s \n","\tlenMsgError equ $-msgError");
	fprintf(archivo_salida,"%s \n","\tmsgErrorAtoi: db 'Debe ingresar un numero entero.',10");
	fprintf(archivo_salida,"%s \n","\tlenMsgAtoi equ $-msgErrorAtoi");
	ImprimirVariables();
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","section .bss");
	fprintf(archivo_salida,"%s \n","");
	
	
}

void ImprimirVariables(void){
	for(int i = 0; i < len_tabla_simbolos; i++){
		if(holi2){
			fprintf(archivo_salida,"%s %s %s \n","\t",tabla_simbolos [i]," dq 0");
		}
				
	}	
}
