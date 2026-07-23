#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MEMORIA 1000
#define MAX_SIMBOLOS 1000
#define MAX_LINEA 200
#define MAX_TOKENS 100
#define MAX_TOKEN 20

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
            if (posicion == -1) {
                return -1;
            }
            (*contadorDatos)--;
        }
        return tabla[posicion].ubicacion;
    }
    valor = strtol(token, &fin, 10);
    if (*fin != '\0' || valor < -99999 || valor > 99999) {
        return -1;
    }
    posicion = buscarSimbolo(tabla, *cantidad, (int)valor, 'C');
    if (posicion == -1) {
        posicion = agregarSimbolo(tabla, cantidad, (int)valor, 'C', *contadorDatos);
        if (posicion == -1) {
            return -1;
        }
        memory[*contadorDatos] = (int)valor;
        (*contadorDatos)--;
    }
    return tabla[posicion].ubicacion;
}

int esOperador(char token[]) {
    return strlen(token) == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/' || token[0] == '^');
}

int esApertura(char token[]) {
    return strcmp(token, "(") == 0 || strcmp(token, "[") == 0 || strcmp(token, "{") == 0;
}

int esCierre(char token[]) {
    return strcmp(token, ")") == 0 || strcmp(token, "]") == 0 || strcmp(token, "}") == 0;
}

int coincide(char apertura[], char cierre[]) {
    return (strcmp(apertura, "(") == 0 && strcmp(cierre, ")") == 0) || (strcmp(apertura, "[") == 0 && strcmp(cierre, "]") == 0) || (strcmp(apertura, "{") == 0 && strcmp(cierre, "}") == 0);
}

int jerarquia(char operador[]) {
    if (strcmp(operador, "^") == 0) {
        return 3;
    }
    if (strcmp(operador, "*") == 0 || strcmp(operador, "/") == 0) {
        return 2;
    }
    if (strcmp(operador, "+") == 0 || strcmp(operador, "-") == 0) {
        return 1;
    }
    return 0;
}

int convertirPostfijo(char tokens[][MAX_TOKEN], int cantidad, char postfija[][MAX_TOKEN]) {
    char pila[MAX_TOKENS][MAX_TOKEN];
    int tope = -1;
    int salida = 0;

    for (int i = 0; i < cantidad; i++) {
        if (!esOperador(tokens[i]) && !esApertura(tokens[i]) && !esCierre(tokens[i])) {
            strcpy(postfija[salida++], tokens[i]);
        } else if (esApertura(tokens[i])) {
            strcpy(pila[++tope], tokens[i]);
        } else if (esCierre(tokens[i])) {
            while (tope >= 0 && !esApertura(pila[tope])) {
                strcpy(postfija[salida++], pila[tope--]);
            }
            if (tope < 0 || !coincide(pila[tope], tokens[i])) {
                return -1;
            }
            tope--;
        } else {
            while (tope >= 0 && esOperador(pila[tope]) &&
                   (jerarquia(pila[tope]) > jerarquia(tokens[i]) ||
                   (jerarquia(pila[tope]) == jerarquia(tokens[i]) &&
                   strcmp(tokens[i], "^") != 0))) {
                strcpy(postfija[salida++], pila[tope--]);
            }
            strcpy(pila[++tope], tokens[i]);
        }
    }
    while (tope >= 0) {
        if (esApertura(pila[tope])) {
            return -1;
        }
        strcpy(postfija[salida++], pila[tope--]);
    }
    return salida;
}

int opcodeAritmetico(char operador[]) {
    if (strcmp(operador, "+") == 0) {
        return 30;
    }
    if (strcmp(operador, "-") == 0) {
        return 31;
    }
    if (strcmp(operador, "/") == 0) {
        return 32;
    }
    if (strcmp(operador, "*") == 0) {
        return 33;
    }
    if (strcmp(operador, "^") == 0) {
        return 35;
    }
    return -1;
}

int compilarExpresion(char tokens[][MAX_TOKEN], int cantidad, int destino, Entrada tabla[], int *cantidadSimbolos, int memory[], int *contadorInstrucciones, int *contadorDatos) {
    char postfija[MAX_TOKENS][MAX_TOKEN];
    int pila[MAX_TOKENS];
    int tope = -1;
    int cantidadPostfija = convertirPostfijo(tokens, cantidad, postfija);

    if (cantidadPostfija <= 0) {
        return 0;
    }
    for (int i = 0; i < cantidadPostfija; i++) {
        if (!esOperador(postfija[i])) {
            int ubicacion = obtenerDato(postfija[i], tabla, cantidadSimbolos, memory, contadorDatos);
            if (ubicacion == -1) {
                return 0;
            }
            pila[++tope] = ubicacion;
        } else {
            int izquierda;
            int derecha;
            int temporal;
            int opcode;
            if (tope < 1) {
                return 0;
            }
            derecha = pila[tope--];
            izquierda = pila[tope--];
            opcode = opcodeAritmetico(postfija[i]);
            if (opcode == -1 || *contadorInstrucciones + 3 > *contadorDatos) {
                return 0;
            }
            temporal = *contadorDatos;
            (*contadorDatos)--;
            memory[(*contadorInstrucciones)++] = 20000 + izquierda;
            memory[(*contadorInstrucciones)++] = opcode * 1000 + derecha;
            memory[(*contadorInstrucciones)++] = 21000 + temporal;
            pila[++tope] = temporal;
        }
    }
    if (tope != 0 || *contadorInstrucciones + 2 > *contadorDatos) {
        return 0;
    }
    memory[(*contadorInstrucciones)++] = 20000 + pila[tope];
    memory[(*contadorInstrucciones)++] = 21000 + destino;
    return 1;
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
            if (variable == NULL || strlen(variable) != 1 ||
                variable[0] < 'a' || variable[0] > 'z') {
                printf("*** Error: variable invalida en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            ubicacion = obtenerDato(variable, tabla, &cantidadSimbolos, memory, &contadorDatos);
            if (ubicacion == -1 || contadorInstrucciones > contadorDatos) {
                printf("*** Error: memoria insuficiente en la linea %d ***\n", numeroLinea);
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
        if (strcmp(comando, "let") == 0) {
            char *variable = strtok(NULL, " \t\n");
            char *igual = strtok(NULL, " \t\n");
            char expresion[MAX_TOKENS][MAX_TOKEN];
            int cantidadExpresion = 0;
            int ubicacion;
            if (variable == NULL || strlen(variable) != 1 ||
                variable[0] < 'a' || variable[0] > 'z' ||
                igual == NULL || strcmp(igual, "=") != 0) {
                printf("*** Error: enunciado let invalido en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            while ((token = strtok(NULL, " \t\n")) != NULL) {
                if (cantidadExpresion >= MAX_TOKENS || strlen(token) >= MAX_TOKEN) {
                    error = 1;
                    break;
                }
                strcpy(expresion[cantidadExpresion++], token);
            }
            if (error == 1 || cantidadExpresion == 0) {
                printf("*** Error: expresion invalida en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
            ubicacion = obtenerDato(variable, tabla, &cantidadSimbolos, memory, &contadorDatos);
            if (ubicacion == -1 ||!compilarExpresion(expresion, cantidadExpresion, ubicacion, tabla, &cantidadSimbolos, memory, &contadorInstrucciones, &contadorDatos)) {
                printf("*** Error: no se pudo compilar let en la linea %d ***\n", numeroLinea);
                error = 1;
                break;
            }
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
            if (izquierdo == NULL || operador == NULL || derecho == NULL ||
                palabraGoto == NULL || destino == NULL ||
                strcmp(palabraGoto, "goto") != 0) {
                printf("*** Error: condicion invalida en la linea %d ***\n", numeroLinea);
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
            memory[contadorInstrucciones++] = 43000;
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