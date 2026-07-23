#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MEMORIA 1000
#define MAX_SIMBOLOS 1000
#define MAX_LINEA 200

typedef struct {
    int simbolo;
    char tipo;
    int ubicacion;
} Entrada;

int buscarSimbolo(Entrada tabla[], int cantidad, int simbolo, char tipo) {
    for (int i = 0; i < cantidad; i++) {
        if (tabla[i].simbolo == simbolo && tabla[i].tipo == tipo) {
            return i;
        }
    }
    return -1;
}

int agregarSimbolo(Entrada tabla[], int *cantidad, int simbolo, char tipo, int ubicacion) {
    int posicion = buscarSimbolo(tabla, *cantidad, simbolo, tipo);
    if (posicion != -1) {
        return posicion;
    }
    if (*cantidad >= MAX_SIMBOLOS) {
        return -1;
    }
    tabla[*cantidad].simbolo = simbolo;
    tabla[*cantidad].tipo = tipo;
    tabla[*cantidad].ubicacion = ubicacion;
    (*cantidad)++;

    return *cantidad - 1;
}

int main() {
    int memory[TAM_MEMORIA] = {0};
    int flags[TAM_MEMORIA];
    Entrada tabla[MAX_SIMBOLOS];
    int cantidadSimbolos = 0;
    int contadorInstrucciones = 0;
    int contadorDatos = TAM_MEMORIA - 1;
    int error = 0;
    char linea[MAX_LINEA];
    FILE *entrada;
    FILE *salida;

    for (int i = 0; i < TAM_MEMORIA; i++) {
        flags[i] = -1;
    }
    entrada = fopen("programa.simple", "r");
    if (entrada == NULL) {
        printf("*** Error: no se encontro programa.simple ***\n");
        return 1;
    }
    while (fgets(linea, MAX_LINEA, entrada) != NULL) {
        char *token = strtok(linea, " \t\n");
        int numeroLinea;
        char *comando;
        if (token == NULL) {
            continue;
        }
        numeroLinea = atoi(token);
        if (numeroLinea <= 0) {
            printf("*** Error: numero de linea invalido ***\n");
            error = 1;
            break;
        }
        if (buscarSimbolo(tabla, cantidadSimbolos, numeroLinea, 'L') != -1) {
            printf("*** Error: linea %d repetida ***\n", numeroLinea);
            error = 1;
            break;
        }
        if (agregarSimbolo(tabla, &cantidadSimbolos, numeroLinea, 'L', contadorInstrucciones) == -1) {
            printf("*** Error: tabla de simbolos llena ***\n");
            error = 1;
            break;
        }
        comando = strtok(NULL, " \t\n");
        if (comando == NULL) {
            printf("*** Error: falta comando en la linea %d ***\n", numeroLinea);
            error = 1;
            break;
        }
        if (strcmp(comando, "rem") == 0) {
            continue;
        }
        if (strcmp(comando, "input") == 0 || strcmp(comando, "print") == 0) {
            char *variable = strtok(NULL, " \t\n");
            int posicion;
            if (variable == NULL || strlen(variable) != 1 || variable[0] < 'a' || variable[0] > 'z') {
                printf("*** Error: variable invalida en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            posicion = buscarSimbolo(tabla, cantidadSimbolos, variable[0], 'V');
            if (posicion == -1) {
                posicion = agregarSimbolo(tabla, &cantidadSimbolos, variable[0], 'V', contadorDatos);
                contadorDatos--;
            }
            if (contadorInstrucciones > contadorDatos) {
                printf("*** Error: memoria insuficiente ***\n");
                error = 1;
                break;
            }
            if (strcmp(comando, "input") == 0) {
                memory[contadorInstrucciones] = 10000 + tabla[posicion].ubicacion;
            } else {
                memory[contadorInstrucciones] = 11000 + tabla[posicion].ubicacion;
            }
            contadorInstrucciones++;
            continue;
        }
        if (strcmp(comando, "goto") == 0) {
            char *destino = strtok(NULL, " \t\n");
            int numeroDestino;
            int posicion;
            if (destino == NULL) {
                printf("*** Error: falta destino en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            numeroDestino = atoi(destino);
            posicion = buscarSimbolo(tabla, cantidadSimbolos, numeroDestino, 'L');
            if (posicion == -1) {
                memory[contadorInstrucciones] = 40000;
                flags[contadorInstrucciones] = numeroDestino;
            } else {
                memory[contadorInstrucciones] = 40000 + tabla[posicion].ubicacion;
            }
            contadorInstrucciones++;
            continue;
        }
        if (strcmp(comando, "end") == 0) {
            memory[contadorInstrucciones] = 43000;
            contadorInstrucciones++;
            continue;
        }
        printf("*** Error: comando desconocido en la linea %d ***\n", numeroLinea);
        error = 1;
        break;
    }
    fclose(entrada);
    if (error == 1) {
        return 1;
    }
    for (int i = 0; i < contadorInstrucciones; i++) {
        if (flags[i] != -1) {
            int posicion = buscarSimbolo(tabla, cantidadSimbolos, flags[i], 'L');

            if (posicion == -1) {
                printf("*** Error: la linea %d no existe ***\n", flags[i]);
                return 1;
            }

            memory[i] += tabla[posicion].ubicacion;
        }
    }
    salida = fopen("programa.simp", "w");
    if (salida == NULL) {
        printf("*** Error: no se pudo crear programa.simp ***\n");
        return 1;
    }
    for (int i = 0; i < contadorInstrucciones; i++) {
        fprintf(salida, "%05d\n", memory[i]);
    }
    fprintf(salida, "99999\n");
    fclose(salida);
    printf("\nTABLA DE SIMBOLOS:\n");
    printf("SIMBOLO\tTIPO\tUBICACION\n");
    for (int i = 0; i < cantidadSimbolos; i++) {
        if (tabla[i].tipo == 'V') {
            printf("%c\t%c\t%03d\n", tabla[i].simbolo, tabla[i].tipo, tabla[i].ubicacion);
        } else {
            printf("%d\t%c\t%03d\n", tabla[i].simbolo, tabla[i].tipo, tabla[i].ubicacion);
        }
    }
    printf("\nCODIGO SML:\n");
    for (int i = 0; i < contadorInstrucciones; i++) {
        printf("%03d: %+06d\n", i, memory[i]);
    }
    printf("\n*** Compilacion terminada ***\n");
    printf("*** Archivo generado: programa.simp ***\n");
    return 0;
}