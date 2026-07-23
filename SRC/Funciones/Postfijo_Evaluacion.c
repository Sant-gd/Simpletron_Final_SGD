#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 200

int esOperador(char c);
int esOperando(char c);
int esApertura(char c);
int esCierre(char c);
int coincide(char apertura, char cierre);
int jerarquia(char op);
int prec(char op1, char op2);

float convierte(char numero);
float evalua(float opnd1, char sign, float opnd2);
float evaluar(char expresion[]);

int convertirPostfijo(char expresion[], char resultado[]);
int esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
int esOperando(char c) {
    return isdigit(c);
}
int esApertura(char c) {
    return c == '(' || c == '[' || c == '{';
}
int esCierre(char c) {
    return c == ')' || c == ']' || c == '}';
}
int coincide(char apertura, char cierre) {
    return (apertura == '(' && cierre == ')') ||
           (apertura == '[' && cierre == ']') ||
           (apertura == '{' && cierre == '}');
}
int jerarquia(char op) {
    if (op == '(' || op == '[' || op == '{') {
        return 1;
    } else if (op == '^') {
        return 4;
    } else if (op == '*' || op == '/') {
        return 3;
    } else if (op == '+' || op == '-') {
        return 2;
    } else {
        return 0;
    }
}
int prec(char op1, char op2) {
    if (esApertura(op1)) {
        return 0;
    }

    if (esCierre(op2)) {
        return 1;
    }

    if (jerarquia(op1) >= jerarquia(op2)) {
        return 1;
    }

    return 0;
}
float convierte(char numero) {
    return (float)(numero - '0');
}
float evalua(float opnd1, char sign, float opnd2) {
    switch (sign) {
        case '+':
            return opnd1 + opnd2;
        case '-':
            return opnd1 - opnd2;
        case '*':
            return opnd1 * opnd2;
        case '/':
            return opnd1 / opnd2;
        case '^':
            return pow(opnd1, opnd2);
        default:
            return 0;
    }
}
float evaluar(char expresion[]) {
    float pila[MAX];
    float opnd1, opnd2, value;
    char symb;
    int tope = -1;
    int i = 0;
    while (expresion[i] != '\0') {
        symb = expresion[i];
        if (symb == ' ' || symb == '\n') {
            i++;
            continue;
        }
        if (esOperando(symb)) {
            tope++;
            pila[tope] = convierte(symb);
        }
        else if (esOperador(symb)) {
            if (tope < 1) {
                printf("Error: expresion postfija invalida.\n");
                return 0;
            }
            opnd2 = pila[tope];
            tope--;
            opnd1 = pila[tope];
            tope--;
            value = evalua(opnd1, symb, opnd2);
            tope++;
            pila[tope] = value;
        }
        else {
            printf("Error: caracter invalido en la expresion postfija.\n");
            return 0;
        }
        i++;
    }
    if (tope != 0) {
        printf("Error: expresion postfija invalida.\n");
        return 0;
    }
    return pila[tope];
}
int convertirPostfijo(char expresion[], char resultado[]) {
    char pila[MAX];
    char symb, topSymb;
    int tope = -1;
    int i = 0;
    int j = 0;
    while (expresion[i] != '\0') {
        symb = expresion[i];
        if (symb == ' ' || symb == '\n') {
            i++;
            continue;
        }
        if (esOperando(symb)) {
            resultado[j] = symb;
            j++;
        }
        else if (esApertura(symb)) {
            tope++;
            pila[tope] = symb;
        }
        else if (esOperador(symb) || esCierre(symb)) {
            while (tope != -1 && prec(pila[tope], symb)) {
                topSymb = pila[tope];
                tope--;
                if (!esApertura(topSymb) && !esCierre(topSymb)) {
                    resultado[j] = topSymb;
                    j++;
                }
            }
            if (esCierre(symb)) {
                if (tope != -1 && coincide(pila[tope], symb)) {
                    tope--;
                } else {
                    printf("Error: agrupadores no balanceados.\n");
                    return 0;
                }
            } else {
                tope++;
                pila[tope] = symb;
            }
        }
        else {
            printf("Error: caracter invalido en la expresion.\n");
            return 0;
        }
        i++;
    }
    while (tope != -1) {
        topSymb = pila[tope];
        tope--;
        if (esApertura(topSymb) || esCierre(topSymb)) {
            printf("Error: agrupadores no balanceados.\n");
            return 0;
        }
        resultado[j] = topSymb;
        j++;
    }
    resultado[j] = '\0';
    return 1;
}

int main() {
    char expresion[MAX];
    char postfija[MAX];
    float resultado;
    while (1) {
        printf("\nIngrese una expresion infija o escriba 'salir' para terminar:\n");
        fgets(expresion, MAX, stdin);
        expresion[strcspn(expresion, "\n")] = '\0';
        if (strcmp(expresion, "salir") == 0) {
            printf("Programa finalizado.\n");
            break;
        }
        if (convertirPostfijo(expresion, postfija)) {
            printf("Expresion postfija: %s\n", postfija);
            resultado = evaluar(postfija);
            printf("Resultado: %.1f\n", resultado);
        }
    }
    return 0;
}