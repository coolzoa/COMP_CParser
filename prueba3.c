#define hola 33

//Declaracion Funciones-----------------------------------------------------------------------------------------------

void clear_buffer();
void buffer_char(char c);

void lexical_error(int character);
void leer_archivo();
void cerrar_archivo();
void crear_archivo_salida();
void cerrar_archivo_salida();
void abrir_archivo();


//Funciones-----------------------------------------------------------------------------------------------------------

void abrir_archivo(){
	char nombre_archivo[100]="";
	printf("Ingrese el nombre del archivo: ");
	scanf("%s",nombre_archivo);
	archivo = fopen(nombre_archivo,"r");

	}


void leer_archivo(){
	abrir_archivo();
	if(archivo){
		largo_archivo = ftell(archivo);  //Posicion actual del archivo, 0
	}else{
		printf(" Error abriendo el archivo, intente de nuevo ");
		
		
	}
	
}

void crear_archivo_salida(){
	archivo_salida = fopen("archivo_salida.asm","w"); //w+
	fprintf(archivo_salida,"%s \n",";   Inicio codigo");
	fprintf(archivo_salida,"%s \n","section .text");
	fprintf(archivo_salida,"%s \n \n","\tglobal _start");
	fprintf(archivo_salida,"%s \n","\t_start:");
}

void cerrar_archivo_salida(){
	fclose(archivo_salida);
}

void cerrar_archivo(){
	fclose(archivo);
}


//funcion que retorna el tipo de token que va detectando 
// Si es un numero o un ID los guarda en el buffer =token_buffer y aumenta tamano buffer
int scanner(){
	int in_char;
	int c;
	clear_buffer();
	
	if (feof(archivo)){
		//printf("%s"," SCANOF");
		return SCANEOF;	
	}
    largo_buffer = 0; 
    
	//mientras no encuentre EOF
	while ((in_char = getc(archivo)) != EOF) {
		if (isspace(in_char)){
			continue;
		}else if (isalpha(in_char)){ 
			// si es una letra
			buffer_char(in_char);	//la mete en el buffer = token_buffer
			largo_buffer++;
			for (c = getc(archivo); isalnum(c) || c == '_'; c = getc(archivo)){ // si es una letra, numero o _
				buffer_char(c); //mete buffer
				largo_buffer++;
			}
			ungetc(c, archivo); //para no comerse el siguiente ch despues de haber llenado el buffer
			return check_reserved(); //ve a ver si la palabra encontrada es reservada por micro

		}else if (isdigit(in_char)){
			buffer_char(in_char);
			for (c = getc(archivo); isdigit(c); c = getc(archivo)){
				buffer_char(c);
			}
			ungetc(c,archivo);
			//printf("%s"," INTLITERAL");
			return INTLITERAL;

		//casos char particular de token type
		}else if (in_char == '('){
			//printf("%s"," LPAREN");
			return LPAREN;
		}else if (in_char == ')'){
			//printf("%s"," RPAREN");
			return RPAREN;
		}else if (in_char == ';'){
			//printf("%s \n"," SEMICOLON");
			return SEMICOLON;
		}else if (in_char == ','){
			//printf("%s"," COMMA");
			return COMMA;
		}else if (in_char == '+'){
			//printf("%s"," PLUSOP");
			return PLUSOP;

		//caso :=
		}else if (in_char == ':'){
			c = getc(archivo);
			if (c == '='){
				//printf("%s"," ASSIGNOP");
				return ASSIGNOP;
			}else{
				ungetc(c,archivo);
				lexical_error(in_char);
			}
		//caso --
		}else if(in_char == '-'){
			c = getc(archivo);
			if (c == '-'){
				do{
					in_char = getc(archivo);
				}while (in_char != '\n');
			}else{
				ungetc(c,archivo);
				//printf("%s"," MINUSOP");
				return MINUSOP;
			}
		}else{
			lexical_error(in_char);
		}
	}
	
}

//funcion para limpiar buffer
void clear_buffer(){
	memset(token_buffer,0,5);
	posicion_char = 0;
}

//funcion para insertar caracter en el buffer
void buffer_char(char c){
	token_buffer[posicion_char++]= c;
	switch(c){

	case 1:
		
		break;

	case 2:
		break;

	case 3: 
		break;
	}
}

////Ve si la palabra en  el token_buffer es una palabra reservada de micro, o un ID y retorna su tipo
char check_reserved(){
	int i;
	//hacer todo upper
	for (i = 0; i <= MAXCAR; i++){
		token_buffer[i] = toupper(token_buffer[i]);
	}
	if (token_buffer[0] == 'B'){
		//printf("%d\n",strcmp(token_buffer,"BEGIN"));
		if (strcmp(token_buffer,"BEGIN") == 0){
			//printf("%s"," BEGIN");
			return BEGIN;
		}else{
			//printf("%s"," ID");
			return ID;
		}
	}else if (token_buffer[0] == 'E'){
		if (strcmp(token_buffer,"END") == 0){
			//printf("%s"," END");
			return END;
		}else{
			//printf("%s"," ID");
			return ID;
		}
	}else if (token_buffer[0] == 'W'){
		if (strcmp(token_buffer,"WRITE") == 0){
			//printf("%s"," WRITE");
			return WRITE;
		}else{
			//printf("%s"," ID");
			return ID;
		}
	}else if (token_buffer[0] == 'R'){
		if (strcmp(token_buffer,"READ") == 0){
			//printf("%s"," READ");
			return READ;
		}else{
			//printf("%s"," ID");
			return ID;
		}
	}else{
		//printf("%s"," ID");
		return ID;
	}
	
}



void lexical_error(int character){
	printf("LEXICAL ERROR %d\n",character);
}


void ImprimirRead(char valor){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t;   Hacer un Read");
	fprintf(archivo_salida,"%s \n","\tcall limpiarBuffer");
	fprintf(archivo_salida,"%s \n","\tcall leer ");
	fprintf(archivo_salida,"%s \n","\tcall atoi");
	fprintf(archivo_salida,"%s %s %s \n","\tmov [",valor,"], rax");
	
}

void ImprimirAdd(char variable,char numero){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t-;   Hacer un Add");
	fprintf(archivo_salida,"%s %s %s \n","\tmov rax, [",variable,"]");
	fprintf(archivo_salida,"%s %s  \n","\tmov rbx, ",numero);
	fprintf(archivo_salida,"%s \n","\tadd rax,rbx");
	
}

void ImprimirSub(char variable,char numero){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t-;   Hacer un Sub");
	fprintf(archivo_salida,"%s %s %s \n","\tmov rax, [",variable,"]");
	fprintf(archivo_salida,"%s %s  \n","\tmov rbx, ",numero);
	fprintf(archivo_salida,"%s \n","\tsub rax,rbx");
	
}

void ImprimirStore(char valor,char variable){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","kj");
	if(hola){
		fprintf(archivo_salida,"%s %s \n","\tmov rax, ", valor);
		fprintf(archivo_salida,"%s %s %s \n","\tmov [",variable,"], rax");
	}	
}

void ImprimirWrite(char variable){
	if(isalpha(variable[0])){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t;   Hacer un Write");
	fprintf(archivo_salida,"%s \n","\tcall limpiarBuffer");
	fprintf(archivo_salida,"%s %s %s %s\n","\tmov rax, [",variable,"]","");
	fprintf(archivo_salida,"%s \n","\tcall itoa");
	fprintf(archivo_salida,"%s \n\n","\tcall imprimir");
	}
	
}


void ImprimirHalt(){	
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\tjmp salir");
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n",";-----------ATOI");
	//falto poner el atoi
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
	fprintf(archivo_salida,"%s \n","\tmov rdx, lenBuffe");
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
		
				
	}	
}

void leer_id(int resultado){
	
		if(isalpha(resultado.name[0])){
		if( ! lookup(resultado.name)){ //no lo encontro en la tabla de simbolos
				if(hey){
					generate("Read",resultado.name,"Integer","");
				}else{
					printf(" Error, no se ha declarado  %s\n",resultado);
				 }			
		}else{
			generate("Read",resultado.name,"Integer","");
		}
	}

}


//  Funciones que escriben en el archivo --------------------------------------------------------------------------------------------------------



void ImprimirRead(char valor){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t;   Hacer un Read");
	fprintf(archivo_salida,"%s \n","\tcall limpiarBuffer");
	fprintf(archivo_salida,"%s \n","\tcall leer ");
	fprintf(archivo_salida,"%s \n","\tcall atoi");
	fprintf(archivo_salida,"%s %s %s \n","\tmov [",valor,"], rax");
	
}

void ImprimirAdd(char variable,char numero){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t;   Hacer un Add");
	if(arroz){
		if(isdigit(numero[0])){
			fprintf(archivo_salida,"%s %s  \n","\tmov rbx, ",numero);
			}
		else{
			fprintf(archivo_salida,"%s %s %s \n","\tmov rbx, [",numero,"]");			
			}
		
		fprintf(archivo_salida,"%s \n","\tadd rax,rbx");
	}else{
		fprintf(archivo_salida,"%s %s %s \n","\tmov rax, [",variable,"]");
		fprintf(archivo_salida,"%s %s  \n","\tmov rbx, ",numero);
		fprintf(archivo_salida,"%s \n","\tadd rax,rbx");
	}

	
}




void ImprimirStore(char valor,char variable){
	fprintf(archivo_salida,"%s \n","");
	fprintf(archivo_salida,"%s \n","\t;   Hacer un Store");
	if(store == true || true)  {
		fprintf(archivo_salida,"%s %s %s \n","\tmov [",variable,"], rax");
	}	
}

void ImprimirWrite(int variable){
	if(isalpha(variable[0])){
		fprintf(archivo_salida,"%s \n","");
		fprintf(archivo_salida,"%s \n","\t;   Hacer un Write");
		if(imprimir){
			fprintf(archivo_salida,"%s \n","\tcall itoa");    //*****
			fprintf(archivo_salida,"%s \n\n","\tcall imprimir");
		}else{

			do{ 
			fprintf(archivo_salida,"%s \n","\tcall limpiarBuffer");
			fprintf(archivo_salida,"%s %s %s %s\n","\tmov rax, [",variable,"]","");
			fprintf(archivo_salida,"%s \n","\tcall itoa");
			fprintf(archivo_salida,"%s \n\n","\tcall imprimir");
			}while(true);

			
		}
	}
	
}




