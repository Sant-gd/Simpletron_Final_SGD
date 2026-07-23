# Requerimientos del Proyecto

## 1. Descripción general

Este proyecto consiste en el desarrollo de un sistema compuesto por dos partes principales:

1. Un **simulador de Simpletron** capaz de cargar y ejecutar programas en lenguaje máquina SML.
2. Un **compilador** capaz de traducir programas escritos en el lenguaje **Simple** a instrucciones **SML** ejecutables por el simulador.

Además, el proyecto incluye funciones auxiliares para:

- validación de profundidad de anidamiento,
- conversión de expresiones de infija a postfija,
- evaluación de expresiones postfijas.

---

## 2. Objetivo general

Desarrollar un sistema completo que permita escribir programas en lenguaje Simple, compilarlos a SML y ejecutarlos en el simulador Simpletron, demostrando el funcionamiento correcto de secuencia, selección, repetición, manejo de memoria y evaluación de expresiones.

---

## 3. Requerimientos funcionales

### 3.1 Simulador Simpletron base

El simulador debe:

- cargar instrucciones SML desde un archivo `programa.simp`,
- almacenar el programa en memoria,
- ejecutar instrucciones en orden secuencial,
- mantener y mostrar los registros principales,
- mostrar el vaciado de memoria al finalizar la ejecución.

### 3.2 Registros del simulador

El simulador debe manejar al menos los siguientes registros:

- `accumulator`
- `instructionCounter`
- `instructionRegister`
- `operationCode`
- `operand`

### 3.3 Instrucciones base de SML

El simulador debe reconocer y ejecutar las siguientes instrucciones:

#### Entrada / salida
- `10` READ
- `11` WRITE

#### Carga / almacenamiento
- `20` LOAD
- `21` STORE

#### Aritmética
- `30` ADD
- `31` SUBTRACT
- `32` DIVIDE
- `33` MULTIPLY

#### Transferencia de control
- `40` BRANCH
- `41` BRANCHNEG
- `42` BRANCHZERO
- `43` HALT

---

## 4. Requerimientos de las mejoras del simulador

### M1. Carga desde archivo
El simulador debe cargar automáticamente un programa desde `programa.simp`.

### M2. Memoria extendida
El simulador debe ampliar la memoria de 100 a 1000 posiciones.

Esto implica:

- direcciones de memoria de `000` a `999`,
- formato de instrucción `OOAAA`,
- rango de palabra de `-99999` a `+99999`.

### M3. Operación de residuo
El simulador debe incorporar la instrucción:

- `34` MODULO

### M4. Operación de exponenciación
El simulador debe incorporar la instrucción:

- `35` POWER

### M5. Salto de línea
El simulador debe incorporar la instrucción:

- `12` NEWLINE

### M6. Entrada de cadenas
El simulador debe incorporar la instrucción:

- `13` READ STRING

La cadena debe almacenarse con el siguiente formato:

- en la dirección base se almacena la longitud,
- en las direcciones siguientes se almacena la posición del carácter y su código ASCII.

Ejemplo general:

- `04000` → longitud 4
- `01104` → carácter 1, código ASCII 104
- `02111` → carácter 2, código ASCII 111

### M7. Salida de cadenas
El simulador debe incorporar la instrucción:

- `14` WRITE STRING

Debe reconstruir la cadena desde memoria usando la longitud y los códigos ASCII almacenados.

### M8. Soporte de punto flotante
El simulador debe admitir datos en punto flotante usando `double`.

Debe permitir:

- lectura de números decimales,
- almacenamiento de números decimales,
- suma, resta, multiplicación y división con decimales.

En esta implementación:

- la operación de residuo requiere operandos enteros,
- la exponenciación se mantiene con exponente entero no negativo.

---

## 5. Manejo de errores en el simulador

El simulador debe detectar y reportar errores fatales como:

- división entre cero,
- intento de residuo entre cero,
- código de operación inválido,
- contador de instrucción fuera de rango,
- operando fuera de rango,
- desbordamiento del acumulador,
- palabra fuera del rango permitido,
- entrada inválida.

---

## 6. Requerimientos de funciones auxiliares

### 6.1 Profundidad de anidamiento
Debe validar expresiones con:

- `()`
- `[]`
- `{}`

Debe indicar si la expresión está balanceada o no.

### 6.2 Conversión infija a postfija
Debe convertir expresiones infijas a postfijas respetando:

- precedencia de operadores,
- agrupadores,
- operadores aritméticos `+`, `-`, `*`, `/`, `^`.

### 6.3 Evaluación postfija
Debe evaluar expresiones postfijas correctamente.

---

## 7. Requerimientos del compilador

El compilador debe:

- leer un programa fuente en `programa.simple`,
- analizarlo línea por línea,
- traducirlo a instrucciones SML,
- generar como salida un archivo `programa.simp`,
- trabajar con una arquitectura de **dos pasadas**,
- construir y usar una **tabla de símbolos**,
- resolver referencias hacia adelante mediante una estructura de banderas (`flags`).

### 7.1 Tabla de símbolos

La tabla de símbolos debe registrar:

- `symbol`
- `type`
- `location`

Tipos requeridos:

- `L` → número de línea
- `V` → variable
- `C` → constante

### 7.2 Comandos del lenguaje Simple

El compilador debe reconocer y compilar:

- `rem`
- `input`
- `print`
- `goto`
- `if ... goto`
- `let`
- `end`

### 7.3 Reglas del lenguaje Simple

- Los números de línea deben ir en orden ascendente.
- Las variables deben ser de una sola letra.
- Las constantes deben ser enteras dentro del rango permitido.
- Las expresiones de `let` deben escribirse con espacios entre operandos, operadores y agrupadores.
- Los comandos deben escribirse en minúsculas.

### 7.4 Expresiones en `let`

El compilador debe:

- convertir la expresión infija a postfija,
- generar instrucciones SML usando temporales,
- soportar:
  - suma,
  - resta,
  - multiplicación,
  - división,
  - potencia,
  - agrupadores.

### 7.5 Condiciones en `if goto`

El compilador debe soportar:

- `<`
- `<=`
- `>`
- `>=`
- `==`
- `!=`

### 7.6 Salida del compilador

El compilador debe generar un archivo `programa.simp` compatible con el simulador.

---

## 8. Requerimientos de prueba

El proyecto debe incluir pruebas para demostrar:

- funcionamiento de entrada y salida,
- asignaciones con `let`,
- precedencia y agrupadores,
- potencia,
- saltos con `goto`,
- condiciones con `if/goto`,
- ciclos,
- factorial,
- expresiones complejas,
- mejoras M1–M8,
- errores del compilador,
- errores del simulador.

---

## 9. Requerimientos de organización del repositorio

El repositorio debe contener, al menos:

- código fuente del simulador,
- código fuente del compilador,
- funciones auxiliares,
- programas de prueba,
- evidencias de ejecución,
- documentación en `docs/`,
- archivo `README.md`.

---

## 10. Criterios de cumplimiento

Se considera que el proyecto cumple sus requerimientos si:

- el compilador genera correctamente `programa.simp`,
- el simulador ejecuta correctamente dicho archivo,
- las pruebas producen las salidas esperadas,
- los errores se detectan y reportan adecuadamente,
- la documentación describe la arquitectura, el diseño y el uso del sistema.
