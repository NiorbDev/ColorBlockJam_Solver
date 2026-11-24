# Color Block Jam Solver

Solver de consola en C++ para describir niveles de **Color Block Jam** en archivos de texto y buscar una secuencia de movimientos con A*.

El proyecto también permite revisar una solución paso a paso o ingresar movimientos manuales. No incluye imágenes ni recursos del juego; los niveles de ejemplo son archivos de configuración de este repositorio.

## Qué incluye

- Búsqueda A* con `f(n) = g(n) + h(n)`.
- Costo acumulado medido en celdas deslizadas.
- Bloques de geometría irregular, muros y salidas asociadas a colores.
- Compuertas que transportan bloques y pueden cambiar de color según su ciclo.
- Límite de movimientos por nivel mediante `STEP_LIMIT`.
- Cola de prioridad (`MinHeap`) y conjunto de estados visitados (`HashSet`) propios.
- Menú interactivo para resolver y animar una secuencia en la terminal.

## Requisitos

- GCC y GNU Make.
- En Windows, MinGW/MSYS2; el `Makefile` añade la biblioteca `psapi` para medir memoria.
- En Linux, el programa lee el uso de memoria desde `/proc`.

No usa bibliotecas externas.

## Compilar y ejecutar

En Linux con GCC y GNU Make:

```bash
make main
./main
```

En Windows con MinGW/MSYS2:

```powershell
mingw32-make main
.\main.exe
```

En el menú, elige **1) Cargar archivo de configuración**, escribe una ruta como `ejemplos/simple1.txt` y luego elige **2) Resolver el juego con A***. También puedes mostrar la solución animada o ingresar una secuencia manual.

`make` compila el ejecutable y los programas de prueba incluidos en el repositorio; `make clean` elimina los binarios y archivos objeto.

## Formato de niveles

Cada nivel es un archivo `.txt` con secciones `[META]`, `[BLOCK]`, `[WALL]`, `[EXIT]` y, si corresponde, `[GATE]`. `ejemplos/` contiene tableros pequeños, niveles con varios colores y casos con compuertas.

Ejemplo mínimo:

```ini
[META]
NAME = SIMPLE1
WIDTH = 8
HEIGHT = 8
STEP_LIMIT = 50

[BLOCK]
1 COLOR=a WIDTH=2 HEIGHT=2 INIT_X=4 INIT_Y=4 GEOMETRY=1 1 1 1

[WALL]
########
#      #
#      #
#      #
#      #
#      #
#      #
########

[EXIT]
COLOR=a X=4 Y=7 ORIENTATION=V LI=2 LF=2 STEP=0
```

Las coordenadas y la geometría se interpretan según el formato que valida `Parser.cpp`; revisa los niveles de ejemplo para ver compuertas y bloques irregulares.

## Estructura

- `Aestrella.*`: búsqueda A*.
- `Tablero.*`, `Bloque.*`, `Salida.*` y `Compuerta.*`: estado y reglas del tablero.
- `Parser.*`: lectura de niveles.
- `MinHeap.*` y `HashSet.*`: estructuras usadas por la búsqueda.
- `Camino.*` y `Movimiento.*`: reconstrucción, reproducción y animación.
- `test*.cpp`: programas de prueba por componente.
- `ejemplos/`: configuraciones de niveles.

## Licencia y alcance

El código de este repositorio se publica bajo la licencia MIT. Color Block Jam es un juego de terceros; este proyecto es un solver independiente y no está afiliado a sus creadores.
