#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int obtenerDato(char token[], Entrada tabla[], int *cantidad, int memory[], int *contadorDatos) {
    int posicion;
    char *fin;
    long valor;

    if (strlen(token) == 1 && token[0] >= 'a' && token[0] <= 'z') {
        posicion = buscarSimbolo(tabla, *cantidad, token[0], 'V');
        if (posicion == -1) {
            posicion = agregarSimbolo(tabla, cantidad, token[0], 'V', *contadorDatos);
            (*contadorDatos)--;
        }
        return tabla[posicion].ubicacion;
    }
    valor = strtol(token, &fin, 10);
    if (*fin != '\0') {
        return -1;
    }
    posicion = buscarSimbolo(tabla, *cantidad, (int)valor, 'C');
    if (posicion == -1) {
        posicion = agregarSimbolo(tabla, cantidad, (int)valor, 'C', *contadorDatos);
        memory[*contadorDatos] = (int)valor;
        (*contadorDatos)--;
    }
    return tabla[posicion].ubicacion;
}

void generarSalto(int memory[], int flags[], int *contadorInstrucciones, int opcode, int destino, Entrada tabla[], int cantidad) {
    int posicion = buscarSimbolo(tabla, cantidad, destino, 'L');
    memory[*contadorInstrucciones] = opcode * 1000;
    if (posicion == -1) {
        flags[*contadorInstrucciones] = destino;
    } else {
        memory[*contadorInstrucciones] += tabla[posicion].ubicacion;
    }
    (*contadorInstrucciones)++;
}

int main() {
    int memory[TAM_MEMORIA] = {0};
    int flags[TAM_MEMORIA];
    Entrada tabla[MAX_SIMBOLOS];
    int cantidadSimbolos = 0;
    int contadorInstrucciones = 0;
    int contadorDatos = TAM_MEMORIA - 1;
    int ultimaLinea = -1;
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
        char *comando;
        int numeroLinea;
        if (token == NULL) {
            continue;
        }
        numeroLinea = atoi(token);
        if (numeroLinea <= ultimaLinea) {
            printf("*** Error: los numeros de linea deben estar en orden ascendente ***\n");
            error = 1;
            break;
        }
        ultimaLinea = numeroLinea;
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
            int ubicacion;
            if (variable == NULL || strlen(variable) != 1 || variable[0] < 'a' || variable[0] > 'z') {
                printf("*** Error: variable invalida en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            ubicacion = obtenerDato(variable, tabla, &cantidadSimbolos, memory, &contadorDatos);
            if (contadorInstrucciones > contadorDatos) {
                printf("*** Error: memoria insuficiente ***\n");
                error = 1;
                break;
            }
            if (strcmp(comando, "input") == 0) {
                memory[contadorInstrucciones] = 10000 + ubicacion;
            } else {
                memory[contadorInstrucciones] = 11000 + ubicacion;
            }
            contadorInstrucciones++;
            continue;
        }
        if (strcmp(comando, "goto") == 0) {
            char *destino = strtok(NULL, " \t\n");
            if (destino == NULL) {
                printf("*** Error: falta destino en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            generarSalto(memory, flags, &contadorInstrucciones, 40, atoi(destino), tabla, cantidadSimbolos);
            continue;
        }
        if (strcmp(comando, "if") == 0) {
            char *izquierdo = strtok(NULL, " \t\n");
            char *operador = strtok(NULL, " \t\n");
            char *derecho = strtok(NULL, " \t\n");
            char *palabraGoto = strtok(NULL, " \t\n");
            char *destino = strtok(NULL, " \t\n");
            int ubicacionIzquierda;
            int ubicacionDerecha;
            int numeroDestino;
            if (izquierdo == NULL || operador == NULL || derecho == NULL || palabraGoto == NULL || destino == NULL) {
                printf("*** Error: condicion incompleta en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            if (strcmp(palabraGoto, "goto") != 0) {
                printf("*** Error: se esperaba goto en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            ubicacionIzquierda = obtenerDato(izquierdo, tabla, &cantidadSimbolos, memory, &contadorDatos);
            ubicacionDerecha = obtenerDato(derecho, tabla, &cantidadSimbolos, memory, &contadorDatos);
            numeroDestino = atoi(destino);
            if (ubicacionIzquierda == -1 || ubicacionDerecha == -1) {
                printf("*** Error: operando invalido en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            if (strcmp(operador, ">") == 0 || strcmp(operador, ">=") == 0) {
                memory[contadorInstrucciones++] = 20000 + ubicacionDerecha;
                memory[contadorInstrucciones++] = 31000 + ubicacionIzquierda;
            } else {
                memory[contadorInstrucciones++] = 20000 + ubicacionIzquierda;
                memory[contadorInstrucciones++] = 31000 + ubicacionDerecha;
            }
            if (strcmp(operador, "<") == 0 || strcmp(operador, ">") == 0) {
                generarSalto(memory, flags, &contadorInstrucciones, 41, numeroDestino, tabla, cantidadSimbolos);
            } else if (strcmp(operador, "<=") == 0 || strcmp(operador, ">=") == 0) {
                generarSalto(memory, flags, &contadorInstrucciones, 41, numeroDestino, tabla, cantidadSimbolos);
                generarSalto(memory, flags, &contadorInstrucciones, 42, numeroDestino, tabla, cantidadSimbolos);
            } else if (strcmp(operador, "==") == 0) {
                generarSalto(memory, flags, &contadorInstrucciones, 42, numeroDestino, tabla, cantidadSimbolos);
            } else if (strcmp(operador, "!=") == 0) {
                memory[contadorInstrucciones] = 42000 + contadorInstrucciones + 2;
                contadorInstrucciones++;
                generarSalto(memory, flags, &contadorInstrucciones, 40, numeroDestino, tabla, cantidadSimbolos);
            } else {
                printf("*** Error: operador relacional invalido en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            if (contadorInstrucciones > contadorDatos) {
                printf("*** Error: memoria insuficiente ***\n");
                error = 1;
                break;
            }
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
    for (int i = 0; i < TAM_MEMORIA; i++) {
        fprintf(salida, "%d\n", memory[i]);
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