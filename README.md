# Proyecto Final Simpletron

Santiago Gutiérrez Durán

26320346

Verano 2026

Programación Avanzada

## Descripción

Este proyecto implementa un sistema completo basado en la computadora virtual **Simpletron**. Incluye:

- un simulador de Simpletron;
- el lenguaje máquina SML;
- ocho mejoras al simulador;
- funciones para validar y evaluar expresiones;
- un lenguaje de alto nivel llamado **Simple**;
- un compilador de dos pasadas que traduce programas Simple a SML;
- programas de prueba y evidencias de ejecución.

El flujo general del sistema es:

```text
programa.simple
      ↓
compilador
      ↓
programa.simp
      ↓
simpletron
      ↓
resultado
```

## Objetivo

Desarrollar un sistema capaz de:

1. recibir programas escritos en lenguaje Simple;
2. convertirlos a instrucciones SML;
3. generar un archivo `programa.simp`;
4. cargar y ejecutar dicho archivo en el simulador Simpletron;
5. mostrar resultados, registros y vaciado de memoria.

## Estructura del repositorio

```text
.
├── SRC/
│   ├── Simpletron/
│   │   ├── Simpletron_M0.c
│   │   ├── Simpletron_M1.c
│   │   ├── Simpletron_M2.c
│   │   ├── Simpletron_M3.c
│   │   ├── Simpletron_M4.c
│   │   ├── Simpletron_M5.c
│   │   ├── Simpletron_M6.c
│   │   ├── Simpletron_M7.c
│   │   └── Simpletron_M8.c
│   ├── Funciones/
│   │   ├── profundidad_anidamiento.c
│   │   ├── conversion_postfija.c
│   │   └── evaluacion_postfija.c
│   └── Compilador/
│       ├── Compilador_C0.c
│       ├── Compilador_C1.c
│       └── Compilador_C2.c
├── programas/
│   ├── simple/
│   └── sml/
├── pruebas/
│   ├── errores_compilador/
│   ├── errores_simpletron/
│   └── salidas_esperadas/
├── evidencias/
│   └── ejecuciones/
├── docs/
│   ├── requerimientos.md
│   └── diseno.md
└── README.md
```

> Los nombres exactos pueden variar ligeramente según la organización final del repositorio.

## Componentes principales

### 1. Simulador Simpletron

El simulador ejecuta instrucciones SML almacenadas en memoria.

Registros principales:

- `accumulator`
- `instructionCounter`
- `instructionRegister`
- `operationCode`
- `operand`

Formato de instrucción:

```text
OOAAA
```

donde:

- `OO` es el código de operación;
- `AAA` es la dirección de memoria.

La memoria contiene 1000 posiciones, de `000` a `999`.

### 2. Instrucciones SML

#### Entrada y salida

| Opcode | Instrucción | Descripción |
|---:|---|---|
| 10 | READ | Lee un valor |
| 11 | WRITE | Imprime un valor |
| 12 | NEWLINE | Imprime un salto de línea |
| 13 | READ STRING | Lee una cadena |
| 14 | WRITE STRING | Imprime una cadena |

#### Carga y almacenamiento

| Opcode | Instrucción | Descripción |
|---:|---|---|
| 20 | LOAD | Carga un valor al acumulador |
| 21 | STORE | Guarda el acumulador en memoria |

#### Operaciones aritméticas

| Opcode | Instrucción | Descripción |
|---:|---|---|
| 30 | ADD | Suma |
| 31 | SUBTRACT | Resta |
| 32 | DIVIDE | División |
| 33 | MULTIPLY | Multiplicación |
| 34 | MODULO | Residuo |
| 35 | POWER | Exponenciación |

#### Transferencia de control

| Opcode | Instrucción | Descripción |
|---:|---|---|
| 40 | BRANCH | Salto incondicional |
| 41 | BRANCHNEG | Salta si el acumulador es negativo |
| 42 | BRANCHZERO | Salta si el acumulador es cero |
| 43 | HALT | Finaliza la ejecución |

### 3. Mejoras del simulador

| Mejora | Descripción |
|---|---|
| M1 | Carga desde `programa.simp` |
| M2 | Memoria extendida a 1000 posiciones |
| M3 | Operación de residuo |
| M4 | Operación de exponenciación |
| M5 | Salto de línea |
| M6 | Entrada de cadenas |
| M7 | Salida de cadenas |
| M8 | Soporte básico de punto flotante |

### 4. Funciones de expresiones

El proyecto incluye funciones para:

- validar `()`, `[]` y `{}`;
- convertir expresiones infijas a postfijas;
- evaluar expresiones postfijas;
- respetar precedencia y asociatividad;
- manejar suma, resta, multiplicación, división y potencia.

Prioridad de operadores:

1. `^`
2. `*`, `/`
3. `+`, `-`

### 5. Lenguaje Simple

El compilador reconoce:

```text
rem
input
let
print
goto
if ... goto
end
```

Reglas principales:

- números de línea en orden ascendente;
- variables de una sola letra;
- comandos en minúsculas;
- constantes enteras;
- expresiones separadas por espacios;
- operadores relacionales `<`, `<=`, `>`, `>=`, `==` y `!=`.

Ejemplo:

```text
10 input a
20 input b
30 let c = a + b * 2
40 if c > 10 goto 60
50 print b
60 print c
70 end
```

### 6. Compilador

El compilador trabaja en dos pasadas.

#### Primera pasada

- lee `programa.simple`;
- construye la tabla de símbolos;
- registra líneas, variables y constantes;
- genera instrucciones SML preliminares;
- marca referencias hacia adelante.

#### Segunda pasada

- resuelve referencias pendientes;
- completa instrucciones inconclusas;
- genera `programa.simp`.

Tipos de símbolo:

| Tipo | Significado |
|---|---|
| L | Número de línea |
| V | Variable |
| C | Constante |

Distribución de memoria:

```text
Instrucciones → 000, 001, 002, ...
Datos         ← 999, 998, 997, ...
```

## Requisitos

- Compilador de C compatible con C99 o superior.
- GCC o MinGW-w64.
- Git Bash, PowerShell o terminal equivalente.
- Windows, Linux o macOS.

## Compilación

### Simulador final

```bash
gcc SRC/Simpletron/Simpletron_M8.c -o simpletron.exe -lm
```

### Compilador final

```bash
gcc SRC/Compilador/Compilador_C2.c -o compilador.exe
```

### Funciones auxiliares

```bash
gcc SRC/Funciones/conversion_postfija.c -o postfijo.exe -lm
```

> Ajusta los nombres de archivo si en tu repositorio tienen una escritura diferente.

## Ejecución del compilador

1. Coloca `programa.simple` junto al ejecutable del compilador.
2. Ejecuta:

```bash
./compilador.exe
```

3. El compilador generará:

```text
programa.simp
```

## Ejecución del simulador

1. Coloca `programa.simp` junto al ejecutable de Simpletron.
2. Ejecuta:

```bash
./simpletron.exe
```

3. Introduce los valores solicitados.
4. Al terminar se mostrarán la salida, los registros y el vaciado de memoria.

## Ejemplo completo

Archivo `programa.simple`:

```text
10 input n
20 let r = 1
30 let i = 1
40 if i > n goto 80
50 let r = r * i
60 let i = i + 1
70 goto 40
80 print r
90 end
```

Entrada:

```text
5
```

Salida esperada:

```text
Salida: 120
```

## Programas de prueba

Las pruebas están organizadas en:

```text
programas/simple/
programas/sml/
pruebas/errores_compilador/
pruebas/errores_simpletron/
```

Se incluyen casos para:

- entrada y salida;
- asignaciones;
- precedencia;
- agrupadores;
- potencia;
- variables;
- referencias hacia adelante;
- condiciones;
- ciclos;
- factorial;
- expresiones complejas;
- división entre cero;
- opcode inválido;
- overflow;
- valores fuera de rango.

Las entradas y salidas esperadas se encuentran en:

```text
pruebas/salidas_esperadas/README.md
```

## Evidencias

Las capturas de ejecución se encuentran en:

```text
evidencias/ejecuciones/
```

Las evidencias muestran:

- código fuente Simple;
- tabla de símbolos;
- SML generado;
- ejecución del simulador;
- entradas;
- salidas;
- memory dump cuando es relevante.

## Manejo de errores

El simulador detecta:

- división entre cero;
- residuo entre cero;
- opcode inválido;
- dirección inválida;
- contador fuera de rango;
- overflow;
- palabra fuera del rango permitido;
- entrada inválida.

El compilador detecta:

- líneas desordenadas;
- comandos desconocidos;
- variables inválidas;
- referencias inexistentes;
- agrupadores desbalanceados;
- operadores inválidos;
- expresiones `let` incompletas;
- memoria insuficiente.

## Historial de desarrollo

El repositorio utiliza commits incrementales para mostrar la evolución del proyecto:

```text
M0: Código base de Simpletron
M1: Agregar carga de programa desde archivo
M2: Extender memoria de Simpletron a 1000 posiciones
M3: Agregar operación de residuo
M4: Agregar operación de exponenciación
M5: Agregar instrucción de salto de línea
M6: Agregar entrada de cadenas
M7: Agregar salida de cadenas
M8: Agregar soporte de punto flotante
F1: Agregar validación de profundidad de anidamiento
F2: Agregar conversión de infija a postfija
F3: Agregar evaluación de expresiones postfijas
C0: Agregar compilador base de Simple a SML
C1: Agregar condiciones if goto
C2: Agregar compilación de expresiones let
```

## Autor

**Nombre:** [Agregar nombre completo]  
**Matrícula:** [Agregar matrícula]  
**Materia:** [Agregar nombre de la materia]  
**Profesor:** [Agregar nombre del profesor]  
**Fecha:** Julio de 2026

## Estado del proyecto

- [x] Simulador Simpletron
- [x] Instrucciones SML base
- [x] Ocho mejoras
- [x] Funciones de expresiones
- [x] Lenguaje Simple
- [x] Compilador de dos pasadas
- [x] Programas de prueba
- [x] Evidencias de ejecución
- [x] Documentación de requerimientos
- [x] Documentación de diseño
- [x] Repositorio GitHub
