#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 200

int esOperador(char c);
int esOperando(char c);
int esApertura(char c);
int esCierre(char c);
int coincide(char apertura, char cierre);
int jerarquia(char op);
int prec(char op1, char op2);
void postfijo(char expresion[]);

int esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int esOperando(char c) {
    return isalnum(c);
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

void postfijo(char expresion[]) {
    char pila[MAX];
    char resultado[MAX];
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
                    return;
                }
            } else {
                tope++;
                pila[tope] = symb;
            }
        }

        i++;
    }

    while (tope != -1) {
        topSymb = pila[tope];
        tope--;

        if (esApertura(topSymb) || esCierre(topSymb)) {
            printf("Error: agrupadores no balanceados.\n");
            return;
        }

        resultado[j] = topSymb;
        j++;
    }

    resultado[j] = '\0';

    printf("Expresion postfija: %s\n", resultado);
}

int main() {
    char expresion[MAX];

    while (1) {
        printf("\nIngrese una expresion infija o escriba 'salir' para terminar:\n");
        fgets(expresion, MAX, stdin);

        expresion[strcspn(expresion, "\n")] = '\0';

        if (strcmp(expresion, "salir") == 0) {
            printf("Programa finalizado.\n");
            break;
        }

        postfijo(expresion);
    }

    return 0;
}