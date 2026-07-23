#include <stdio.h>
#include <string.h>

#define MAX 200

int profundidad(char expresion[]) {
    char pila[MAX];
    char symb, temp;
    int tope = -1;

    for (int i = 0; expresion[i] != '\0'; i++) {
        symb = expresion[i];

        if (symb == '(' || symb == '[' || symb == '{') {
            tope++;
            pila[tope] = symb;
        }

        else if (symb == ')' || symb == ']' || symb == '}') {
            if (tope == -1) {
                return 0;
            } else {
                temp = pila[tope];
                tope--;

                if ((symb == ')' && temp != '(') ||
                    (symb == ']' && temp != '[') ||
                    (symb == '}' && temp != '{')) {
                    return 0;
                }
            }
        }
    }

    if (tope != -1) {
        return 0;
    }

    return 1;
}

int main() {
    char expresion[MAX];

    while (1) {
        printf("\nIngrese una expresion aritmetica o escriba 'salir' para terminar:\n");
        fgets(expresion, MAX, stdin);

        expresion[strcspn(expresion, "\n")] = '\0';

        if (strcmp(expresion, "salir") == 0) {
            printf("\nPrograma finalizado.\n");
            break;
        }

        if (profundidad(expresion) == 1) {
            printf("1\n");
            printf("La expresion esta balanceada.\n");
        } else {
            printf("0\n");
            printf("La expresion NO esta balanceada.\n");
        }
    }

    return 0;
}