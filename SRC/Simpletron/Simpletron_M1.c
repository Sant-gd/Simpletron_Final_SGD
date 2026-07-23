#include <stdio.h>
#include <stdlib.h>

int main() {
    int memory[100] = {0};
    int instruccion;
    int contador = 0;
    int accumulator = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;
    int ejecutando = 1;
    FILE *archivo;
    int cargaDesdeArchivo = 0;
    int errorArchivo = 0;

    printf("** ¡Bienvenido a Simpletron!\t\t\t***\n");
    printf("** Introduzca su programa una instruccion\t***\n");
    printf("** (o palabra de datos) a la vez en la linea\t***\n");
    printf("** de texto de entrada. Yo indicara el numero\t***\n");
    printf("** de posición y una interrogacion (?). Usted\t***\n");
    printf("** tecleara entonces la palabra para esa\t***\n");
    printf("** posicion. Escriba 9999 para terminar.\t***\n");

    archivo = fopen("programa.simp", "r");

    if (archivo != NULL) {
        cargaDesdeArchivo = 1;
        printf("** Archivo programa.simp encontrado.\t\t***\n");
        printf("** Cargando programa desde archivo...\t\t***\n\n");
        while (contador < 100 && fscanf(archivo, "%d", &instruccion) == 1) {
            if (instruccion == 9999) {
                break;
            }
            if (instruccion < -9999 || instruccion > 9999) {
                printf("\n*** Error: instruccion invalida en archivo ***\n");
                printf("*** Posicion: %02d | Valor: %d ***\n", contador, instruccion);
                printf("*** La carga del programa se cancelo ***\n");
                errorArchivo = 1;
                break;
            }
            memory[contador] = instruccion;
            printf("%02d ? %+05d\n", contador, memory[contador]);
            contador++;
        }
        if (!feof(archivo) && errorArchivo == 0) {
        printf("\n*** Error: el archivo contiene una linea invalida ***\n");
        printf("*** La carga del programa se cancelo ***\n");
        errorArchivo = 1;
        }
        fclose(archivo);
    } else {
        printf("** No se encontro programa.simp.\t\t***\n");
        printf("** Se usara carga interactiva.\t\t\t***\n\n");
        while (contador < 100) {
            printf("%02d ? ", contador);
            scanf("%d", &instruccion);
            if (instruccion == 9999) {
                break;
            }
            if (instruccion < -9999 || instruccion > 9999) {
                printf("\n*** Error: instruccion invalida ***\n");
                printf("*** Debe estar entre -9999 y +9999 ***\n");
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
        instructionRegister = memory[instructionCounter];
        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;
        switch (operationCode) {
            case 10:
                printf("? ");
                scanf("%d", &memory[operand]);
                instructionCounter++;
                break;
            case 11:
                printf("Salida: %+05d\n", memory[operand]);
                instructionCounter++;
                break;
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
                    printf("*** La ejecucion de Simpletron termino anormalmente ***\n");
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
                printf("*** La ejecucion de Simpletron termino anormalmente ***\n");
                ejecutando = 0;
                break;
        }
    }

    printf("\nREGISTROS:\n");
    printf("accumulator:            %+05d\n", accumulator);
    printf("instructionCounter:     %02d\n", instructionCounter);
    printf("instructionRegister:    %+05d\n", instructionRegister);
    printf("operationCode:          %02d\n", operationCode);
    printf("operand:                %02d\n", operand);
    printf("\nMEMORIA:\n\n");
    printf("       ");
    for (int i = 0; i < 10; i++) {
        printf("%5d ", i);
    }
    printf("\n");
    for (int i = 0; i < 10; i++) {
        printf("%2d    ", i * 10);
        for (int j = 0; j < 10; j++) {
            printf("%+05d ", memory[i * 10 + j]);
        }
    printf("\n");
    }
    return 0;
}