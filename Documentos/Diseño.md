# Diseño del Proyecto

## 1. Introducción

El proyecto se diseñó como un sistema modular compuesto por:

1. **Simulador Simpletron**
2. **Funciones auxiliares para expresiones**
3. **Compilador de Simple a SML**

La idea principal es que el usuario escriba un programa en lenguaje **Simple**, el compilador lo traduzca a **SML** y después el simulador ejecute ese código.

Flujo general del sistema:

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

---

## 2. Arquitectura general

## 2.1 Módulos principales

### a) Simulador Simpletron
Se encarga de:

- cargar el archivo `programa.simp`,
- almacenar las palabras en memoria,
- decodificar cada instrucción,
- ejecutar la operación correspondiente,
- mostrar registros y memoria al finalizar.

### b) Funciones auxiliares
Se desarrollaron funciones independientes para:

- profundidad de anidamiento,
- conversión infija a postfija,
- evaluación postfija.

Estas funciones sirvieron como base conceptual para la compilación de expresiones `let`.

### c) Compilador
Se encarga de:

- leer `programa.simple`,
- construir la tabla de símbolos,
- generar instrucciones SML,
- resolver referencias hacia adelante,
- escribir el archivo `programa.simp`.

---

## 3. Diseño del simulador Simpletron

## 3.1 Formato de palabra e instrucción

Después de la mejora M2, el sistema trabaja con memoria de 1000 posiciones.

Esto obliga a usar el formato:

```text
OOAAA
```

donde:

- `OO` = código de operación
- `AAA` = dirección de memoria

Ejemplo:

```text
20120
```

significa:

- `20` = LOAD
- `120` = dirección de memoria

El rango de palabra permitido es:

```text
-99999 a +99999
```

---

## 3.2 Memoria

La memoria del simulador se diseñó con:

- **1000 posiciones**
- direcciones desde `000` hasta `999`

Se representa mediante:

```c
memory[1000]
```

En la mejora 8 la memoria pasó a ser de tipo `double` para permitir soporte de punto flotante.

---

## 3.3 Registros

El simulador utiliza cinco registros principales:

- `accumulator`
- `instructionCounter`
- `instructionRegister`
- `operationCode`
- `operand`

Su función es:

- `instructionCounter`: apunta a la dirección actual.
- `instructionRegister`: contiene la palabra de instrucción actual.
- `operationCode`: contiene el opcode decodificado.
- `operand`: contiene la dirección objetivo.
- `accumulator`: almacena resultados intermedios.

---

## 3.4 Ciclo de ejecución

El ciclo general del simulador es:

1. Cargar programa desde archivo.
2. Inicializar registros.
3. Leer instrucción actual.
4. Obtener `operationCode` y `operand`.
5. Ejecutar la operación correspondiente.
6. Actualizar el contador de instrucción.
7. Finalizar con `HALT` o por error fatal.

---

## 3.5 Instrucciones soportadas

### Entrada / salida
- `10` READ
- `11` WRITE
- `12` NEWLINE
- `13` READ STRING
- `14` WRITE STRING

### Carga / almacenamiento
- `20` LOAD
- `21` STORE

### Aritmética
- `30` ADD
- `31` SUBTRACT
- `32` DIVIDE
- `33` MULTIPLY
- `34` MODULO
- `35` POWER

### Transferencia de control
- `40` BRANCH
- `41` BRANCHNEG
- `42` BRANCHZERO
- `43` HALT

---

## 3.6 Diseño de las mejoras

### M1. Carga desde archivo
Se sustituyó la carga completamente interactiva por lectura desde `programa.simp`.

### M2. Memoria extendida
Se cambió la memoria a 1000 posiciones y el formato `OOAA` a `OOAAA`.

### M3. Residuo
Se agregó un nuevo `case 34` en el `switch` del simulador.

### M4. Exponenciación
Se agregó un nuevo `case 35`.

### M5. Salto de línea
Se agregó el opcode `12`.

### M6. Entrada de cadenas
La cadena se almacena con longitud y códigos ASCII.

Ejemplo para `hola` desde la dirección `120`:

```text
120 -> 04000
121 -> 01104
122 -> 02111
123 -> 03108
124 -> 04097
```

### M7. Salida de cadenas
Se reconstruye la cadena recorriendo la longitud almacenada y leyendo ASCII desde memoria.

### M8. Punto flotante
Se cambió `memory` y `accumulator` a `double`.

Se decidió mantener:

- módulo solo para enteros,
- potencia con exponente entero no negativo,

para conservar una evolución consistente del código.

---

## 3.7 Manejo de errores

El simulador se diseñó para detener la ejecución si ocurre un error fatal, por ejemplo:

- división entre cero,
- residuo entre cero,
- opcode inválido,
- dirección inválida,
- desbordamiento,
- valor fuera de rango.

Esto evita resultados indefinidos y facilita la depuración.

---

## 4. Diseño de funciones auxiliares

## 4.1 Profundidad de anidamiento

Se implementó con una pila de caracteres.

Funcionamiento:

1. Si encuentra un agrupador de apertura, lo apila.
2. Si encuentra uno de cierre, verifica coincidencia.
3. Si al final la pila está vacía, la expresión está balanceada.

---

## 4.2 Conversión de infija a postfija

Se usa una pila para operadores y agrupadores.

La lógica es:

- operandos → salida
- operadores → se comparan por jerarquía
- agrupadores → controlan el vaciado de la pila

Se respetan las prioridades:

1. `^`
2. `*`, `/`
3. `+`, `-`

---

## 4.3 Evaluación postfija

Se usa una pila de operandos.

La lógica es:

- si se lee un operando, se apila;
- si se lee un operador, se desapilan dos operandos;
- se evalúa la operación;
- se apila el resultado.

---

## 5. Diseño del compilador

## 5.1 Objetivo

El compilador traduce un programa escrito en Simple a un archivo `programa.simp` ejecutable por Simpletron.

---

## 5.2 Arquitectura de dos pasadas

Se eligió una arquitectura de **dos pasadas** porque el lenguaje Simple permite referencias a líneas que aún no han sido definidas.

### Primera pasada
Durante la primera pasada, el compilador:

- lee cada línea de `programa.simple`,
- registra números de línea en la tabla de símbolos,
- registra variables y constantes,
- genera instrucciones SML preliminares,
- marca referencias hacia adelante en `flags`.

### Segunda pasada
Durante la segunda pasada, el compilador:

- recorre las instrucciones generadas,
- localiza los saltos pendientes,
- sustituye la referencia por la dirección real.

---

## 5.3 Tabla de símbolos

Se diseñó una estructura con tres campos:

- `simbolo`
- `tipo`
- `ubicacion`

Tipos usados:

- `L` para línea,
- `V` para variable,
- `C` para constante.

Ejemplo conceptual:

| Símbolo | Tipo | Ubicación |
|---|---|---|
| 10 | L | 000 |
| a | V | 999 |
| 5 | C | 998 |

---

## 5.4 Distribución de memoria del compilador

El compilador genera instrucciones desde el inicio de la memoria y datos desde el final.

```text
Instrucciones → 000, 001, 002, ...
Datos         ← 999, 998, 997, ...
```

Esta decisión permite evitar que las instrucciones y los datos se mezclen mientras se compila.

Se usan dos contadores:

- `contadorInstrucciones`
- `contadorDatos`

Si ambos se cruzan, se reporta memoria insuficiente.

---

## 5.5 Comandos soportados

### `rem`
No genera código SML. Solo sirve como comentario en el programa fuente.

### `input`
Genera una instrucción `READ`.

### `print`
Genera una instrucción `WRITE`.

### `goto`
Genera una instrucción `BRANCH`.

### `if ... goto`
Genera una secuencia de:

- `LOAD`
- `SUBTRACT`
- `BRANCHNEG` y/o `BRANCHZERO`
- `BRANCH`

según el operador relacional.

### `let`
Genera instrucciones SML para evaluar una expresión y almacenar el resultado.

### `end`
Genera `HALT`.

---

## 5.6 Compilación de expresiones `let`

Para compilar expresiones `let`, el diseño sigue estas etapas:

1. Leer la expresión infija por tokens.
2. Convertirla a postfija.
3. Recorrer la expresión postfija.
4. Generar temporales para resultados intermedios.
5. Al final, almacenar el resultado en la variable destino.

Ejemplo:

```text
let c = a + b * 2
```

Conversión a postfija:

```text
a b 2 * +
```

Generación conceptual:

```text
LOAD b
MULTIPLY 2
STORE temp1
LOAD a
ADD temp1
STORE temp2
LOAD temp2
STORE c
```

---

## 5.7 Soporte de condiciones

El compilador soporta:

- `<`
- `<=`
- `>`
- `>=`
- `==`
- `!=`

La estrategia consiste en traducir la condición a una resta y luego decidir el salto con:

- `BRANCHNEG`
- `BRANCHZERO`
- `BRANCH`

Por ejemplo:

```text
if a > b goto 60
```

se traduce conceptualmente como:

```text
LOAD b
SUBTRACT a
BRANCHNEG 60
```

---

## 5.8 Manejo de errores del compilador

El compilador valida:

- líneas en orden ascendente,
- presencia de comando,
- variables válidas,
- constantes válidas,
- referencias a líneas existentes,
- operadores relacionales válidos,
- expresiones `let` bien formadas,
- agrupadores balanceados,
- memoria suficiente.

Si ocurre un error, la compilación se detiene y se muestra un mensaje explicativo.

---

## 6. Organización del repositorio

La organización general propuesta es:

```text
src/
├── simpletron/
├── expresiones/
└── compilador/

programas/
├── simple/
└── sml/

pruebas/
├── errores_compilador/
├── errores_simpletron/
└── salidas_esperadas/

evidencias/
└── ejecuciones/

docs/
├── requerimientos.md
└── diseno.md
```

Esta estructura separa claramente:

- implementación,
- casos de prueba,
- resultados esperados,
- evidencias,
- documentación.

---

## 7. Decisiones de diseño relevantes

Las decisiones más importantes fueron:

1. **Separar compilador y simulador**  
   Se mantienen como programas independientes para respetar la arquitectura solicitada.

2. **Usar memoria extendida de 1000 posiciones**  
   Esto facilita programas más completos y permite direcciones de tres dígitos.

3. **Asignar datos desde el final de memoria**  
   Evita colisiones con las instrucciones durante la compilación.

4. **Compilar expresiones con postfijo**  
   Simplifica la generación de código SML.

5. **Mantener el estilo del código incremental**  
   Cada mejora se construyó como evolución de la anterior, conservando coherencia visual y funcional.

---

## 8. Conclusión de diseño

El diseño del proyecto permite:

- compilar programas fuente Simple,
- traducirlos a SML,
- ejecutarlos en un simulador funcional,
- ampliar el sistema mediante mejoras bien definidas.

La combinación de simulador, funciones auxiliares y compilador produce un flujo completo de desarrollo y ejecución, cumpliendo con los objetivos académicos del proyecto.
