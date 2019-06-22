#define _GNU_SOURCE 
#include <ctype.h>
#include <string.h>
#define maximoLetras 20
#define maximoCaracteres 100





int largoLista;
int lecturaCompleta;


int* primero = NULL;
char* ultimo = NULL;
int numLinea;
int cantElementos = 0;

//
void agregar(char* n,int repetidoLinea);
void leerArchivo();
int esReservada(char palabra[]);
int esta(char* n);
void imprimir();
void menu();
int revisar(char palabra[]);
int aparece(char palabra[]);




//


void agregar(char* n, int repetidoLinea){
    n->siguiente = NULL;

    if (primero == NULL){
        primero = n;
        ultimo = n;
        cantElementos++;
        //printf("Lo agregue al principio de la lista\n");

    }else{

        //si solo hay una palabra
        if (repetidoLinea == 1){
            char* actual = primero;
            char* previo = actual;

            while (actual != NULL && strcmp(actual->palabra, n->palabra) <= 0){
                    previo = actual;
                    actual = actual->siguiente;
            }

            if (actual == NULL){
                previo->siguiente = n;
                ultimo = n;
                cantElementos++;
                //printf("Lo agregue al final del arreglo \n");
            }else{
                if (strcmp(previo->palabra,n->palabra) <= 0){
                    previo->siguiente = n;
                    n->siguiente = actual;
                    cantElementos++;
                }else{
                    char* auxActual = primero;
                    char* auxPRevio = actual->siguiente;
                    while (auxActual != NULL){
                        if (auxActual == primero && strcmp(auxActual->palabra,n->palabra) > 0){
                            char* temp = primero;
                            primero = n;
                            cantElementos++;
                            n->siguiente = auxActual;
                            auxActual = auxActual->siguiente;
                        }else{
                            auxActual = auxActual->siguiente;
                        }
                    }
                }
            }

        }else{
            char* actual = primero;
            while (actual != NULL){
                if (strcmp(n->palabra,actual->palabra) == 0 && n->linea == actual->linea){
                    actual->apariciones++;
                    actual = actual->siguiente;
                }else{
                    actual = actual->siguiente;
                }
                //printf("Incremente las apariciones de esta palabra \n");
            }
        }
    }
}

int esta(char* n){
    if (primero == NULL){
        return 1;
    }else{
        char* actual = primero;
        while (actual != NULL){
            if (strcmp(n->palabra,actual->palabra) == 0 && n->linea == actual->linea){
                return 0;
            }else{
                actual = actual->siguiente;
            }
        }
        return 1;
    }
}

void imprimir(){
    FILE *archivo;
    archivo = fopen("resultado.txt","w");
    fputs("Palabras reservadas: \n",archivo);

    int lineaActual = 1;
    while (lineaActual <= numLinea){
        char* actual = primero;
        while (actual != NULL){
           int indice = 0;
            if (actual->linea == lineaActual){
                while (actual->palabra[indice] != '\0'){
                    if ('Z' - actual->palabra[indice] >= 0){
                        fputc(actual->palabra[indice],archivo);
                        indice++;
                    }else{
                        indice++;
                    }

                }

                fputs(" ",archivo);
                if (actual->apariciones == 1){
                    fprintf(archivo, "%s %d %c","Aparece 1 vez en la linea ",actual->linea,'\n');
                }else{
                    fprintf(archivo, "%s %d %s %d %c","Aparece: ",actual->apariciones, "veces en la linea ",actual->linea,'\n');
                }
                actual = actual->siguiente;
            }else{
                actual = actual->siguiente;
            }

        }
        lineaActual++;
    }

    fputs("--------------\n",archivo);
    fputs("Totales: \n",archivo);
    char* actual2 = primero;
    int contador = 0;
    char* ultimo = NULL;
    while (actual2 != NULL){
        if (ultimo == NULL || strcmp(actual2->palabra,ultimo->palabra) != 0){
             char* siguiente = actual2->siguiente;
            while (siguiente != NULL){
                if (strcmp(actual2->palabra, siguiente->palabra) == 0){
                    contador = contador + siguiente->apariciones;
                    siguiente = siguiente->siguiente;
                }else{
                    siguiente = siguiente->siguiente;
                }
            }
            contador = contador + actual2->apariciones;
            int indice = 0;
            fputs("La palabra: ",archivo);
            while (actual2->palabra[indice] != '\0'){
                    if ('Z' - actual2->palabra[indice] >= 0){
                        fputc(actual2->palabra[indice],archivo);
                        indice++;
                    }else{
                        indice++;
                    }

            }
            fputs(" ",archivo);
            if (contador == 1){
                fprintf(archivo, "%s %c", "Aparece 1 vez \n", '\n');
            }else{
                fprintf(archivo, "%s %d %s %c", "Aparece ", contador, " veces en el archivo", '\n');
            }

            ultimo = actual2;
            actual2 = actual2->siguiente;
            contador = 0;
        }else{
            contador = 0;
            actual2 = actual2->siguiente;
        }

    }
    fclose(archivo);
}




int esReservada(char palabra[]){
    int esValida = 1;

    switch (palabra[0]){
    case 'A':
        if (strcmp(palabra,"AUTO") == 0){
            esValida = 0;
            break;
        }
    case 'B':
        if (strcmp(palabra,"BREAK") == 0){
            esValida = 0;
            break;
        }
    case 'C':
        if (strcmp(palabra,"CASE") == 0 || strcmp(palabra,"CHAR") == 0 || strcmp(palabra,"CONST") == 0 || strcmp(palabra,"CONTINUE") == 0){
            esValida = 0;
            break;
        }
    case 'D':
        if (strcmp(palabra,"DEFAULT") == 0 || strcmp(palabra,"DO") == 0 || strcmp(palabra,"DOUBLE") == 0){
            esValida = 0;
            break;
        }
    case 'E':
        if (strcmp(palabra,"ELSE") == 0 || strcmp(palabra,"ENUM") == 0 || strcmp(palabra,"EXTERN") == 0){
            esValida = 0;
            break;
        }
    case 'F':
        if (strcmp(palabra,"FLOAT") == 0 || strcmp(palabra,"FOR") == 0){
            esValida = 0;
            break;
        }
    case 'G':
        if (strcmp(palabra,"GOTO") == 0){
            esValida = 0;
            break;
        }
    case 'I':
        if (strcmp(palabra,"IF") == 0 || strcmp(palabra,"INT") == 0){
            esValida = 0;
            break;
        }
    case 'L':
        if (strcmp(palabra,"LONG") == 0){
            esValida = 0;
            break;
        }
    case 'R':
        if (strcmp(palabra,"REGISTER") == 0 || strcmp(palabra,"RETURN") == 0){
            esValida = 0;
            break;
        }
    case 'S':
        if (strcmp(palabra,"SHORT") == 0 || strcmp(palabra,"SIGNED") == 0 || strcmp(palabra,"SIZEOF") == 0 || strcmp(palabra,"STATIC") == 0 || strcmp(palabra,"STRUCT") == 0 || strcmp(palabra,"SWITCH") == 0){
            esValida = 0;
            break;
        }
    case 'T':
        if (strcmp(palabra,"TYPEDEF") == 0){
            esValida = 0;
            break;
        }
    case 'U':
        if (strcmp(palabra,"UNION") == 0){
            esValida = 0;
            break;
        }
    case 'V':
        if (strcmp(palabra,"VOID") == 0 || strcmp(palabra,"VOLATILE") == 0){
            esValida = 0;
            break;
        }
    case 'W':
        if (strcmp(palabra,"WHILE") == 0){
            esValida = 0;
            break;
        }
    case '_':
        if (strcmp(palabra,"_PACKED") == 0){
            esValida = 0;
            break;
        }
    }

    return esValida;
}

void menu(){
    int seleccion;
    char palabra [8];

    printf("-----Menu-----\n");
    printf("1. consultar palabra\n");
    printf("2. Salir.\n");
    printf("Seleccione una operacion: ");
    scanf("%d",&seleccion);
    switch (seleccion){
        case 1:
            printf("\n Ingrese su palabra en mayuscula: ");
            scanf("%s",palabra);
            if (esReservada(palabra) == 0 && aparece(palabra) == 0){
                int lineaActual = 1;
                while (lineaActual <= numLinea){
                    char* actual = primero;
                    while (actual != NULL){
                        int indice = 0;
                        if (actual->linea == lineaActual && strcmp(actual->palabra,palabra) == 0){
                            while (actual->palabra[indice] != '\0'){
                                if ('Z' - actual->palabra[indice] >= 0){
                                    printf("%c",actual->palabra[indice]);
                                    indice++;
                                }else{
                                    indice++;
                                }

                            }
                            printf(" ");
                            if (actual->apariciones == 1){
                                printf("Aparece 1 vez en la linea %d \n",actual->linea);
                            }else{
                                printf("Aparece %d veces en la linea %d \n",actual->apariciones,actual->linea);
                            }
                            actual = actual->siguiente;
                        }else{
                            actual = actual->siguiente;
                        }

                    }
                    lineaActual++;
                }
                menu();
            }else{
                printf("NO esta esa palabra\n");
                menu();
            }

        case 2:
            break;

        default:
            printf("Opcion invalida \n");
            menu();

    }

}





int aparece(char palabra[]){
    char* actual = primero;
    int encontrada = 1;
    while (actual != NULL){
        if (strcmp(actual->palabra, palabra) == 0){
            encontrada = 0;
            break;
        }else{
            actual = actual->siguiente;
        }
    }
    return encontrada;
}






void imprimir2(){
    FILE *archivo;
    archivo = fopen("resultado.txt","w");
    fputs("Palabras reservadas: \n",archivo);

    int lineaActual = 1;
    while (lineaActual <= numLinea){
        int* actual = primero;
        while (actual != NULL){
           int indice = 0;
            if (actual->linea == lineaActual){
                while (actual->palabra[indice] != '\0'){
                    if ('Z' - actual->palabra[indice] >= 0){
                        fputc(actual->palabra[indice],archivo);
                        indice++;
                    }else{
                        indice++;
                    }

                }

                fputs(" ",archivo);
                if (actual->apariciones == 1){
                    fprintf(archivo, "%s %d %c","Aparece 1 vez en la linea ",actual->linea,'\n');
                }else{
                    fprintf(archivo, "%s %d %s %d %c","Aparece: ",actual->apariciones, "veces en la linea ",actual->linea,'\n');
                }
                actual = actual->siguiente;
            }else{
                actual = actual->siguiente;
            }

        }
        lineaActual++;
    }

    fputs("--------------\n",archivo);
    fputs("Totales: \n",archivo);
    int* actual2 = primero;
    int contador = 0;
    int* ultimo = NULL;
    while (actual2 != NULL){
        if (ultimo == NULL || strcmp(actual2->palabra,ultimo->palabra) != 0){
            
            while (siguiente != NULL){
                if (strcmp(actual2->palabra, siguiente->palabra) == 0){
                    contador = contador + siguiente->apariciones;
                    siguiente = siguiente->siguiente;
                }else{
                    siguiente = siguiente->siguiente;
                }
            }
            contador = contador + actual2->apariciones;
            int indice = 0;
            fputs("La palabra: ",archivo);
            while (actual2->palabra[indice] != '\0'){
                    if ('Z' - actual2->palabra[indice] >= 0){
                        fputc(actual2->palabra[indice],archivo);
                        indice++;
                    }else{
                        indice++;
                    }

            }
            fputs(" ",archivo);
            if (contador == 1){
                fprintf(archivo, "%s %c", "Aparece 1 vez \n", '\n');
            }else{
                fprintf(archivo, "%s %d %s %c", "Aparece ", contador, " veces en el archivo", '\n');
            }

            ultimo = actual2;
            actual2 = actual2->siguiente;
            contador = 0;
        }else{
            contador = 0;
            actual2 = actual2->siguiente;
        }

    }
    fclose(archivo);
}







int esta2(int* n){
    if (primero == NULL){
        return 1;

    }


	else{

        int* actual = primero;
        while (actual != NULL){
            if (strcmp(n->palabra,actual->palabra) == 0 && n->linea == actual->linea){
                return 0;
            }


		else{
                actual = actual->siguiente;
            }

		
        }
        return 1;
    }
}




int main()
{

    leerArchivo();
    imprimir();
    printf("Analisis de documento completado \n");
    menu();


    return 0;
}

