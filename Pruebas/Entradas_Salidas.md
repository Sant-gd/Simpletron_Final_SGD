# Salidas esperadas

## Programas Simple

| Archivo | Entradas | Salida esperada |
|---|---|---|
| 01_entrada_salida.simple | 25 | Salida: 25 |
| 02_asignacion.simple | Ninguna | Salida: 15 |
| 03_precedencia.simple | Ninguna | Salida: 11 |
| 04_agrupadores.simple | Ninguna | Salida: 14 |
| 05_potencia.simple | Ninguna | Salida: 8 |
| 06_variables.simple | 3, 4 | Salida: 11 |
| 07_goto_adelante.simple | 18 | Salida: 18; no solicita b |
| 08_mayor.simple | 8, 20 | Salida: 20 |
| 08_mayor.simple | 25, 10 | Salida: 25 |
| 09_igualdad.simple | 5, 5 | Salida: 1 |
| 09_igualdad.simple | 5, 8 | Salida: 0 |
| 10_diferente.simple | 5, 8 | Salida: 1 |
| 10_diferente.simple | 5, 5 | Salida: 0 |
| 11_menor_igual.simple | 4, 8 | Salida: 1 |
| 11_menor_igual.simple | 8, 8 | Salida: 1 |
| 11_menor_igual.simple | 9, 8 | Salida: 0 |
| 12_ciclo.simple | 5 | Salida: 1Salida: 2Salida: 3Salida: 4Salida: 5 |
| 13_factorial.simple | 5 | Salida: 120 |
| 14_expresion_compleja.simple | 5, 6 | Salida: 240 |

## Programas SML

| Archivo | Entradas | Salida esperada |
|---|---|---|
| 01_suma.simp | 25, 17 | Salida: 42 |
| 02_modulo.simp | 17, 5 | Salida: 2 |
| 03_potencia.simp | 2, 3 | Salida: 8 |
| 04_salto_linea.simp | 25, 48 | Cada valor aparece en una línea distinta |
| 05_entrada_salida_cadena.simp | Hola mundo | Salida: Hola mundo |
| 06_punto_flotante.simp | 5.5, 2 | Salida: 7.5 y Salida: 2.75 |
| 07_memoria_extendida.simp | 25, 17 | Salida: 42, usando posiciones 120–122 |

## Errores del compilador

Cada archivo debe detener la compilación y mostrar un mensaje relacionado con el error de su nombre.

## Errores de Simpletron

| Archivo | Resultado esperado |
|---|---|
| 01_division_cero.simp | Error de división entre cero |
| 02_opcode_invalido.simp | Error de código de operación inválido |
| 03_overflow.simp | Error de desbordamiento del acumulador |
| 04_valor_fuera_rango.simp | Error de palabra fuera del rango permitido |
