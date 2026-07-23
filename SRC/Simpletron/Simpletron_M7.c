#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MEMORIA 1000
#define FIN_CARGA 99999
#define MIN_PALABRA -99999
#define MAX_PALABRA 99999

int main() {
    int memory[TAM_MEMORIA] = {0};
    int instruccion;
    int contador = 0;
    int accumulator = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;
    int ejecutando = 1;
    int errorArchivo = 0;
    FILE *archivo;

    printf("** Bienvenido a Simpletron!\t\t\t***\n");
    printf("** Introduzca su programa una instruccion\t***\n");
    printf("** (o palabra de datos) a la vez en la linea\t***\n");
    printf("** de texto de entrada. Yo indicare el numero\t***\n");
    printf("** de posicion y una interrogacion (?). Usted\t***\n");
    printf("** tecleara entonces la palabra para esa\t***\n");
    printf("** posicion. Escriba 99999 para terminar.\t***\n\n");
    archivo = fopen("programa.simp", "r");

    if (archivo != NULL) {
        printf("** Archivo programa.simp encontrado.\t\t***\n");
        printf("** Cargando programa desde archivo...\t\t***\n\n");
        while (contador < TAM_MEMORIA) {
            int lectura = fscanf(archivo, "%d", &instruccion);
            if (lectura == EOF) {
                break;
            }
            if (lectura != 1) {
                printf("\n*** Error: el archivo contiene una linea invalida ***\n");
                printf("*** La carga del programa se cancelo ***\n");
                errorArchivo = 1;
                break;
            }
            if (instruccion == FIN_CARGA) {
                break;
            }
            if (instruccion < MIN_PALABRA || instruccion > MAX_PALABRA) {
                printf("\n*** Error: instruccion invalida en archivo ***\n");
                printf("*** Posicion: %03d | Valor: %d ***\n", contador, instruccion);
                printf("*** Debe estar entre -99999 y +99999 ***\n");
                printf("*** La carga del programa se cancelo ***\n");
                errorArchivo = 1;
                break;
            }
            memory[contador] = instruccion;
            printf("%03d ? %+06d\n", contador, memory[contador]);
            contador++;
        }
        if (contador == TAM_MEMORIA && errorArchivo == 0) {
            if (fscanf(archivo, "%d", &instruccion) == 1 && instruccion != FIN_CARGA) {
                printf("\n*** Error: la memoria esta llena ***\n");
                printf("*** El archivo contiene mas de 1000 palabras ***\n");
                errorArchivo = 1;
            }
        }
        fclose(archivo);
    } else {
        printf("** No se encontro programa.simp.\t\t***\n");
        printf("** Se usara carga interactiva.\t\t\t***\n\n");
        while (contador < TAM_MEMORIA) {
            printf("%03d ? ", contador);
            if (scanf("%d", &instruccion) != 1) {
                int caracter;
                printf("\n*** Error: debe introducir un numero entero ***\n");
                while ((caracter = getchar()) != '\n' && caracter != EOF) {
                }
                continue;
            }
            if (instruccion == FIN_CARGA) {
                break;
            }
            if (instruccion < MIN_PALABRA || instruccion > MAX_PALABRA) {
                printf("\n*** Error: instruccion invalida ***\n");
                printf("*** Debe estar entre -99999 y +99999 ***\n");
                continue;
            }
            memory[contador] = instruccion;
            contador++;
        }
    }
    if (errorArchivo == 1) {
        return 1;
    }
    printf("\n** Se termino de cargar el programa\t\t***\n");
    printf("** Comienza la ejecucion del programa\t\t***\n\n");
    while (ejecutando == 1) {
        if (instructionCounter < 0 || instructionCounter >= TAM_MEMORIA) {
            printf("\n*** Error: contador de instruccion fuera de rango ***\n");
            ejecutando = 0;
            break;
        }
        instructionRegister = memory[instructionCounter];
        operationCode = instructionRegister / 1000;
        operand = instructionRegister % 1000;
        if (operand < 0) {
            operand = -operand;
        }
        if (operand >= TAM_MEMORIA) {
            printf("\n*** Error: operando fuera de rango ***\n");
            ejecutando = 0;
            break;
        }
        switch (operationCode) {
            case 10:
                printf("? ");
                if (scanf("%d", &memory[operand]) != 1) {
                    printf("\n*** Error: entrada invalida ***\n");
                    ejecutando = 0;
                    break;
                }
                if (memory[operand] < MIN_PALABRA || memory[operand] > MAX_PALABRA) {
                    printf("\n*** Error: dato fuera del rango permitido ***\n");
                    ejecutando = 0;
                    break;
                }
                instructionCounter++;
                break;
            case 11:
                printf("Salida: %+06d", memory[operand]);
                instructionCounter++;
                break;
            case 12:
                printf("\n");
                instructionCounter++;
                break;
            case 13: {
                char cadena[100];
                int longitud;
                printf("Cadena: ");
                if (fgets(cadena, sizeof(cadena), stdin) == NULL) {
                    printf("\n*** Error: no se pudo leer la cadena ***\n");
                    ejecutando = 0;
                    break;
                }
                longitud = strlen(cadena);
                if (longitud > 0 && cadena[longitud - 1] == '\n') {
                    cadena[longitud - 1] = '\0';
                    longitud--;
                }
                if (longitud > 99) {
                    printf("\n*** Error: la cadena supera los 99 caracteres ***\n");
                    ejecutando = 0;
                    break;
                }
                if (operand + longitud >= TAM_MEMORIA) {
                    printf("\n*** Error: la cadena excede la memoria disponible ***\n");
                    ejecutando = 0;
                    break;
                }
                memory[operand] = longitud * 1000;
                for (int i = 0; i < longitud; i++) {
                    memory[operand + i + 1] = (i + 1) * 1000 + (unsigned char)cadena[i];
                }
                instructionCounter++;
                break;
            }
            case 14: {
                int longitud = memory[operand] / 1000;
                if (longitud < 0 || longitud > 99 || operand + longitud >= TAM_MEMORIA) {
                    printf("\n*** Error: cadena almacenada incorrectamente ***\n");
                    ejecutando = 0;
                    break;
                }
                printf("Salida: ");
                for (int i = 1; i <= longitud; i++) {
                    int posicion = memory[operand + i] / 1000;
                    int codigoASCII = memory[operand + i] % 1000;
                    if (posicion != i || codigoASCII < 0 || codigoASCII > 255) {
                        printf("\n*** Error: caracter de cadena invalido ***\n");
                        ejecutando = 0;
                        break;
                    }
                    printf("%c", codigoASCII);
                }
                if (ejecutando == 1) {
                    instructionCounter++;
                }
                break;
            }
            case 20:
                accumulator = memory[operand];
                instructionCounter++;
                break;
            case 21:
                memory[operand] = accumulator;
                instructionCounter++;
                break;
            case 30:
                accumulator = accumulator + memory[operand];
                instructionCounter++;
                break;
            case 31:
                accumulator = accumulator - memory[operand];
                instructionCounter++;
                break;
            case 32:
                if (memory[operand] == 0) {
                    printf("\n*** Error: intento de division entre cero ***\n");
                    ejecutando = 0;
                } else {
                    accumulator = accumulator / memory[operand];
                    instructionCounter++;
                }
                break;
            case 33:
                accumulator = accumulator * memory[operand];
                instructionCounter++;
                break;
            case 34:
                if (memory[operand] == 0) {
                    printf("\n*** Error: intento de calcular residuo entre cero ***\n");
                    ejecutando = 0;
                } else {
                    accumulator = accumulator % memory[operand];
                    instructionCounter++;
                }
                break;
            case 35: {
                int exponente = memory[operand];
                long long potencia = 1;
                if (exponente < 0) {
                    printf("\n*** Error: exponente negativo no permitido ***\n");
                    ejecutando = 0;
                    break;
                }
                for (int i = 0; i < exponente; i++) {
                    potencia = potencia * accumulator;
                    if (potencia < MIN_PALABRA || potencia > MAX_PALABRA) {
                        printf("\n*** Error: desbordamiento durante la exponenciacion ***\n");
                        ejecutando = 0;
                        break;
                    }
                }
                if (ejecutando == 1) {
                    accumulator = (int)potencia;
                    instructionCounter++;
                }
                break;
            }
            case 40:
                instructionCounter = operand;
                break;
            case 41:
                if (accumulator < 0) {
                    instructionCounter = operand;
                } else {
                    instructionCounter++;
                }
                break;
            case 42:
                if (accumulator == 0) {
                    instructionCounter = operand;
                } else {
                    instructionCounter++;
                }
                break;
            case 43:
                printf("\n*** Termino la ejecucion de Simpletron ***\n");
                ejecutando = 0;
                break;
            default:
                printf("\n*** Error: codigo de operacion invalido ***\n");
                printf("*** Codigo recibido: %02d ***\n", operationCode);
                printf("*** Posicion de memoria: %03d ***\n", instructionCounter);
                ejecutando = 0;
                break;
        }
        if (ejecutando == 1 && (accumulator < MIN_PALABRA || accumulator > MAX_PALABRA)) {
            printf("\n*** Error: desbordamiento del acumulador ***\n");
            ejecutando = 0;
        }
    }
    printf("\nREGISTROS:\n");
    printf("accumulator:            %+06d\n", accumulator);
    printf("instructionCounter:     %03d\n", instructionCounter);
    printf("instructionRegister:    %+06d\n", instructionRegister);
    printf("operationCode:          %02d\n", operationCode);
    printf("operand:                %03d\n", operand);
    printf("\nMEMORIA:\n\n");
    printf("        ");
    for (int i = 0; i < 10; i++) {
        printf("%6d ", i);
    }
    printf("\n");
    for (int i = 0; i < 100; i++) {
        printf("%03d    ", i * 10);
        for (int j = 0; j < 10; j++) {
            printf("%+06d ", memory[i * 10 + j]);
        }
        printf("\n");
    }
    return 0;
}