#include <iostream>
#include "Tablero.h"

// Constructor inicial: recibe punteros ya construidos por el parser
Tablero::Tablero(StaticData* staticData,
        Bloque** bloques,
        Salida** salidas,
        Compuerta** compuertas){
    this->staticData = staticData;   // datos inmutables del nivel, compartidos por todos los tableros
    this->bloques    = bloques;       // arreglo de bloques vivos en este estado
    this->salidas    = salidas;       // arreglo de salidas (pueden cambiar de largo)
    this->compuertas = compuertas;    // arreglo de compuertas (pueden cambiar de color)

    // el estado inicial parte con todos los bloques en juego
    this->cantidadBloques    = staticData->cantidadBloques;
    this->contadorMovimientos = 0;   // 0 pasos realizados

    // estado inicial no tiene padre ni movimiento previo (es la raíz del árbol A*)
    this->padre           = nullptr;
    this->movimientoOrigen = Movimiento();

    // reservar cuadricula vacía; se llena con reconstruirCuadricula()
    int alto  = staticData->altoTablero;
    int ancho = staticData->anchoTablero;
    this->cuadricula = new char*[alto];
    for(int i = 0; i < alto; i++){
        this->cuadricula[i] = new char[ancho];
    }

    reconstruirCuadricula();
}

Tablero::~Tablero(){
    // liberar cuadricula fila por fila
    // (puede haber sido liberada antes por liberarCuadricula() para ahorrar memoria en A*)
    if(this->cuadricula != nullptr){
        int alto = staticData->altoTablero;
        for(int i = 0; i < alto; i++){
            delete[] this->cuadricula[i]; // liberar cada fila
        }
        delete[] this->cuadricula;        // liberar el arreglo de punteros
        this->cuadricula = nullptr;
    }

    // liberar los bloques que siguen vivos en este estado
    for(int i = 0; i < this->cantidadBloques; i++){
        delete this->bloques[i];
    }
    delete[] this->bloques;

    // liberar salidas (existen aunque no queden bloques adentro)
    for(int i = 0; i < this->staticData->cantidadSalidas; i++){
        delete this->salidas[i];
    }
    delete[] this->salidas;

    // liberar compuertas
    for(int i = 0; i < this->staticData->cantidadCompuertas; i++){
        delete this->compuertas[i];
    }
    delete[] this->compuertas;

    // staticData NO se libera aquí: lo comparten todos los tableros del árbol A*
    // padre NO se libera aquí: la gestión del árbol de nodos la hace A*
}

// Rellena la cuadricula desde cero con el estado actual:
// primero pega las paredes base, luego pinta salidas, compuertas y bloques encima.
void Tablero::reconstruirCuadricula(){
    int alto  = this->staticData->altoTablero;
    int ancho = this->staticData->anchoTablero;

    // si la cuadricula fue liberada para ahorrar memoria, la reservamos de nuevo
    if(this->cuadricula == nullptr){
        this->cuadricula = new char*[alto];
        for(int i = 0; i < alto; i++){
            this->cuadricula[i] = new char[ancho];
        }
    }

    // copiar paredes base desde staticData: '#' es pared, ' ' es vacío
    for(int i = 0; i < alto; i++){
        for(int j = 0; j < ancho; j++){
            this->cuadricula[i][j] = this->staticData->paredesTablero[i][j];
        }
    }

    // pintar salidas según su largo actual (la), que puede cambiar cada turno
    // orientación 'H' (horizontal): fila fija, se extiende en columnas
    // orientación 'V' (vertical):   columna fija, se extiende en filas
    for(int s = 0; s < this->staticData->cantidadSalidas; s++){
        Salida* salida = this->salidas[s];
        int  x0    = this->staticData->xSalidas[s];        // columna de origen
        int  y0    = this->staticData->ySalidas[s];        // fila de origen
        char color  = this->staticData->coloresSalidas[s]; // carácter que representa la salida
        char orient = this->staticData->orientacionSalidas[s];
        int  largoActual = salida->la;                     // puede ser menor que el largo máximo

        for(int k = 0; k < largoActual; k++){
            int fila, col;
            if(orient == 'H'){
                fila = y0;      // avanza en columnas
                col  = x0 + k;
            } else {
                fila = y0 + k;  // avanza en filas
                col  = x0;
            }
            // seguridad: solo pintar dentro del tablero
            if(fila >= 0 && fila < alto && col >= 0 && col < ancho){
                this->cuadricula[fila][col] = color;
            }
        }
    }

    // pintar compuertas con su coloractual; también puede cambiar con el tiempo
    for(int c = 0; c < this->staticData->cantidadCompuertas; c++){
        int  x0     = this->staticData->xCompuertas[c];
        int  y0     = this->staticData->yCompuertas[c];
        int  largo  = this->staticData->largoCompuertas[c]; // largo fijo, no cambia
        char orient = this->staticData->orientacionCompuertas[c];
        char color  = this->compuertas[c]->coloractual;     // color dinámico

        for(int k = 0; k < largo; k++){
            int fila, col;
            if(orient == 'H'){ fila = y0; col = x0 + k; }
            else              { fila = y0 + k; col = x0; }
            this->cuadricula[fila][col] = color;
        }
    }

    // pintar bloques según su geometría y posición actual
    // geometriaBloques[id] es un arreglo plano de anchoB*altoB uint8_t:
    //   1 = celda ocupada por el bloque, 0 = celda vacía dentro del bounding box
    // índice: fila i, columna j  =>  i * anchoB + j
    for(int b = 0; b < this->cantidadBloques; b++){
        Bloque* bloque = this->bloques[b];
        int     id     = bloque->id;
        int     anchoB = this->staticData->anchoBloques[id];
        int     altoB  = this->staticData->altoBloques[id];
        char    colorB = this->staticData->coloresBloques[id];
        uint8_t* geom  = this->staticData->geometriaBloques[id];

        for(int i = 0; i < altoB; i++){
            for(int j = 0; j < anchoB; j++){
                if(geom[i * anchoB + j] == 1){
                    int fila = bloque->y + i; // coordenada absoluta en el tablero
                    int col  = bloque->x + j;
                    this->cuadricula[fila][col] = colorB;
                }
            }
        }
    }
}

// Imprime la cuadricula carácter a carácter para debug/visualización
void Tablero::imprimir(){
    int alto  = this->staticData->altoTablero;
    int ancho = this->staticData->anchoTablero;

    for(int i = 0; i < alto; i++){
        for(int j = 0; j < ancho; j++){
            std::cout << this->cuadricula[i][j];
        }
        std::cout << std::endl;
    }
}

// ─── Métodos usados para movimientos y A* ───────────────────────────────────

// El tablero es solución cuando no queda ningún bloque vivo
bool Tablero::esSolucion(){
    return this->cantidadBloques == 0;
}

// Constructor usado por A* para clonar un estado padre
// Recibe arreglos ya copiados y el tablero padre como puntero
Tablero::Tablero(StaticData* staticData,
        int cantidadBloques,
        Bloque** bloques,
        Salida** salidas,
        Compuerta** compuertas,
        int contadorMovimientos,
        Tablero* padre){
    this->staticData          = staticData;          // compartido (no se clona)
    this->cantidadBloques     = cantidadBloques;     // puede ser menor si bloques ya salieron
    this->bloques             = bloques;
    this->salidas             = salidas;
    this->compuertas          = compuertas;
    this->contadorMovimientos = contadorMovimientos; // copiado del padre antes de aplicar el movimiento
    this->padre               = padre;               // enlace al nodo anterior en el árbol A*
    this->movimientoOrigen    = Movimiento();        // se llenará con el movimiento aplicado

    // reservar cuadricula vacía; se llena con reconstruirCuadricula()
    int alto  = staticData->altoTablero;
    int ancho = staticData->anchoTablero;
    this->cuadricula = new char*[alto];
    for(int i = 0; i < alto; i++){
        this->cuadricula[i] = new char[ancho];
    }
    reconstruirCuadricula();
}

// Crea una copia profunda del tablero actual.
// Se usa en generarVecinos para no mutar el estado padre al explorar movimientos.
Tablero* Tablero::clonar(){
    // copiar cada bloque vivo con su id y posición actuales
    Bloque** nuevosBloques = new Bloque*[this->cantidadBloques];
    for(int i = 0; i < this->cantidadBloques; i++){
        Bloque* orig    = this->bloques[i];
        nuevosBloques[i] = new Bloque(orig->id, orig->x, orig->y, this->staticData);
    }

    // copiar salidas (todas, aunque no haya bloques de ese color siguen activas)
    int numSalidas     = this->staticData->cantidadSalidas;
    Salida** nuevasSalidas = new Salida*[numSalidas];
    for(int i = 0; i < numSalidas; i++){
        Salida* orig  = this->salidas[i];
        Salida* copia = new Salida(orig->id, orig->la, this->staticData);
        copia->contadorpasos = orig->contadorpasos; // preservar el tiempo transcurrido
        copia->direccion     = orig->direccion;     // preservar si está creciendo o encogiendo
        nuevasSalidas[i] = copia;
    }

    // copiar compuertas con su color actual y contador de pasos
    int numCompuertas        = this->staticData->cantidadCompuertas;
    Compuerta** nuevasCompuertas = new Compuerta*[numCompuertas];
    for(int i = 0; i < numCompuertas; i++){
        Compuerta* orig  = this->compuertas[i];
        Compuerta* copia = new Compuerta(orig->id, orig->coloractual, this->staticData);
        copia->contadorpasos = orig->contadorpasos; // preservar el ciclo de color
        nuevasCompuertas[i] = copia;
    }

    // crear el nuevo nodo con 'this' como padre
    Tablero* clon = new Tablero(
        this->staticData,
        this->cantidadBloques,
        nuevosBloques,
        nuevasSalidas,
        nuevasCompuertas,
        this->contadorMovimientos,
        this);
    return clon;
}

// Retorna el índice en bloques[] del bloque con ese id, o -1 si ya salió
int Tablero::buscarBloque(uint8_t idBloque){
    for(int i = 0; i < this->cantidadBloques; i++){
        if((uint8_t)this->bloques[i]->id == idBloque){
            return i;
        }
    }
    return -1;
}

// Comprueba si el bloque puede desplazarse 1 celda en la dirección dada
// sin salirse del tablero ni colisionar con otra pieza.
// No modifica el estado; solo consulta la cuadricula actual.
bool Tablero::comprobarMovimiento(uint8_t idBloque, char direccion){
    int idx = buscarBloque(idBloque);
    if(idx == -1) return false; // el bloque ya salió, movimiento inválido

    Bloque*  bloque = this->bloques[idx];
    int      id     = bloque->id;
    int      anchoB = this->staticData->anchoBloques[id];
    int      altoB  = this->staticData->altoBloques[id];
    uint8_t* geom   = this->staticData->geometriaBloques[id];

    // calcular el desplazamiento (dx, dy) según la dirección
    int dx = 0, dy = 0;
    if     (direccion == 'U') dy = -1;
    else if(direccion == 'D') dy =  1;
    else if(direccion == 'L') dx = -1;
    else if(direccion == 'R') dx =  1;
    else return false; // dirección desconocida

    // posición del bounding box tras el desplazamiento
    int nuevoX = bloque->x + dx;
    int nuevoY = bloque->y + dy;

    // recorrer cada celda ocupada de la geometría en la NUEVA posición
    for(int i = 0; i < altoB; i++){
        for(int j = 0; j < anchoB; j++){
            if(geom[i * anchoB + j] != 1) continue; // celda vacía del bounding box

            int fila = nuevoY + i; // coordenada absoluta destino
            int col  = nuevoX + j;

            char contenido = this->cuadricula[fila][col];
            if(contenido == ' ') continue; // celda vacía, sin colisión

            // la celda está ocupada; verificar si pertenece al mismo bloque en su pos actual
            // (cuando el bloque se desplaza 1 celda, parte de su geometría puede solapar su propia huella)
            int localI = fila - bloque->y; // coordenada relativa dentro del bounding box actual
            int localJ = col  - bloque->x;
            bool esMismoBloque = false;
            if(localI >= 0 && localI < altoB && localJ >= 0 && localJ < anchoB){
                if(geom[localI * anchoB + localJ] == 1){
                    esMismoBloque = true; // la celda destino es parte de su propio bounding box
                }
            }
            if(!esMismoBloque) return false; // choca con algo ajeno
        }
    }
    return true; // ninguna celda destino colisiona con algo externo
}


// Intenta que el bloque salga por alguna salida compatible con su color y tamaño.
// Si lo logra, elimina el bloque del arreglo (compacta hacia la izquierda) y devuelve true.
bool Tablero::intentarSalida(uint8_t idBloque){
    int idx = buscarBloque(idBloque);
    if(idx == -1) return false; // el bloque ya salió antes

    Bloque* bloque = this->bloques[idx];
    int     id     = bloque->id;
    int     anchoB = this->staticData->anchoBloques[id];
    int     altoB  = this->staticData->altoBloques[id];
    char    colorB = this->staticData->coloresBloques[id];

    // revisar todas las salidas en busca de una compatible
    for(int s = 0; s < this->staticData->cantidadSalidas; s++){
        if(this->staticData->coloresSalidas[s] != colorB) continue; // color distinto, no aplica

        int  xs          = this->staticData->xSalidas[s];
        int  ys          = this->staticData->ySalidas[s];
        char orient      = this->staticData->orientacionSalidas[s];
        int  largoActual = this->salidas[s]->la; // largo actual (puede haber cambiado)

        // salida vertical (V): está en una pared lateral; el bloque sale por la derecha o izquierda
        if(orient == 'V'){
            // la salida ocupa filas ys..ys+largoActual-1 en la columna xs
            bool adyDerecha   = (bloque->x + anchoB == xs); // borde derecho del bloque toca la salida
            bool adyIzquierda = (bloque->x - 1 == xs);      // borde izquierdo del bloque toca la salida
            if(!adyDerecha && !adyIzquierda) continue;

            // el bloque debe caber verticalmente dentro de la apertura de la salida
            if(altoB > largoActual) continue;

            // las filas del bloque deben estar dentro del rango de la salida
            if(bloque->y < ys) continue;
            if(bloque->y + altoB > ys + largoActual) continue;

            // todo ok: eliminar el bloque compactando el arreglo
            delete this->bloques[idx];
            for(int k = idx; k < this->cantidadBloques - 1; k++){
                this->bloques[k] = this->bloques[k + 1];
            }
            this->cantidadBloques--;
            return true;
        }
        // salida horizontal (H): está en una pared superior o inferior; el bloque sale por arriba o abajo
        else {
            // la salida ocupa columnas xs..xs+largoActual-1 en la fila ys
            bool adyAbajo  = (bloque->y + altoB == ys); // borde inferior del bloque toca la salida
            bool adyArriba = (bloque->y - 1 == ys);     // borde superior del bloque toca la salida
            if(!adyAbajo && !adyArriba) continue;

            // el bloque debe caber horizontalmente dentro de la apertura de la salida
            if(anchoB > largoActual) continue;

            // las columnas del bloque deben estar dentro del rango de la salida
            if(bloque->x < xs) continue;
            if(bloque->x + anchoB > xs + largoActual) continue;

            // todo ok: eliminar el bloque compactando el arreglo
            delete this->bloques[idx];
            for(int k = idx; k < this->cantidadBloques - 1; k++){
                this->bloques[k] = this->bloques[k + 1];
            }
            this->cantidadBloques--;
            return true;
        }
    }
    return false; // ninguna salida recibió al bloque
}

// Intenta teleportar el bloque a través de una compuerta del mismo color.
// La compuerta lo desplaza anchoB+1 (o altoB+1) celdas de golpe cruzando la pared.
// Devuelve true y actualiza la posición del bloque si hubo espacio al otro lado.
bool Tablero::intentarCompuerta(uint8_t idBloque){
    int idx = buscarBloque(idBloque);
    if(idx == -1) return false;

    Bloque*  bloque = this->bloques[idx];
    int      id     = bloque->id;
    int      anchoB = this->staticData->anchoBloques[id];
    int      altoB  = this->staticData->altoBloques[id];
    char     colorB = this->staticData->coloresBloques[id];
    uint8_t* geom   = this->staticData->geometriaBloques[id];

    int alto  = this->staticData->altoTablero;
    int ancho = this->staticData->anchoTablero;

    for(int c = 0; c < this->staticData->cantidadCompuertas; c++){
        if(this->compuertas[c]->coloractual != colorB) continue; // color distinto

        int  xc     = this->staticData->xCompuertas[c];
        int  yc     = this->staticData->yCompuertas[c];
        int  largo  = this->staticData->largoCompuertas[c];
        char orient = this->staticData->orientacionCompuertas[c];

        int  dx = 0, dy = 0;
        bool adyacente = false;

        if(orient == 'V'){
            // compuerta vertical: ocupa filas yc..yc+largo-1 en la columna xc
            // bloque adyacente por la derecha (entrará por la izquierda de la compuerta)
            if(bloque->x + anchoB == xc
               && bloque->y >= yc && bloque->y + altoB <= yc + largo
               && altoB <= largo){
                dx = anchoB + 1; adyacente = true; // salta la compuerta hacia la derecha
            }
            // bloque adyacente por la izquierda (entrará por la derecha de la compuerta)
            else if(bloque->x - 1 == xc
                    && bloque->y >= yc && bloque->y + altoB <= yc + largo
                    && altoB <= largo){
                dx = -(anchoB + 1); adyacente = true; // salta la compuerta hacia la izquierda
            }
        } else {
            // compuerta horizontal: ocupa columnas xc..xc+largo-1 en la fila yc
            // bloque adyacente por abajo (entrará por la parte superior de la compuerta)
            if(bloque->y + altoB == yc
               && bloque->x >= xc && bloque->x + anchoB <= xc + largo
               && anchoB <= largo){
                dy = altoB + 1; adyacente = true; // salta la compuerta hacia abajo
            }
            // bloque adyacente por arriba
            else if(bloque->y - 1 == yc
                    && bloque->x >= xc && bloque->x + anchoB <= xc + largo
                    && anchoB <= largo){
                dy = -(altoB + 1); adyacente = true; // salta la compuerta hacia arriba
            }
        }

        if(!adyacente) continue; // el bloque no está pegado a esta compuerta

        // posición destino tras cruzar la compuerta
        int nuevoX = bloque->x + dx;
        int nuevoY = bloque->y + dy;

        // verificar que todas las celdas del bloque en la posición destino estén libres
        bool hayEspacio = true;
        for(int i = 0; i < altoB && hayEspacio; i++){
            for(int j = 0; j < anchoB && hayEspacio; j++){
                if(geom[i * anchoB + j] != 1) continue;
                int fila = nuevoY + i;
                int col  = nuevoX + j;
                if(fila < 0 || fila >= alto || col < 0 || col >= ancho){
                    hayEspacio = false; break; // fuera del tablero
                }
                if(this->cuadricula[fila][col] != ' ') hayEspacio = false; // celda ocupada
            }
        }
        if(!hayEspacio) continue; // no cabe al otro lado, probar siguiente compuerta

        // hay espacio: teletransportar el bloque
        bloque->x = nuevoX;
        bloque->y = nuevoY;
        return true;
    }
    return false; // ninguna compuerta admitió al bloque
}

// Aplica un movimiento de 1 celda al bloque indicado.
// Actualiza compuertas, salidas, contador de pasos y reconstruye la cuadricula.
// Al final comprueba si el bloque puede salir o cruzar una compuerta (sin coste extra).
bool Tablero::moverBloque(uint8_t idBloque, char direccion){
    if(!comprobarMovimiento(idBloque, direccion)){
        return false; // movimiento ilegal, el estado no cambia
    }

    int     idx    = buscarBloque(idBloque);
    Bloque* bloque = this->bloques[idx];

    // aplicar el desplazamiento de 1 celda
    if     (direccion == 'U') bloque->y -= 1;
    else if(direccion == 'D') bloque->y += 1;
    else if(direccion == 'L') bloque->x -= 1;
    else if(direccion == 'R') bloque->x += 1;

    // avanzar el ciclo de compuertas: cada una sabe cuántos pasos necesita para cambiar de color
    for(int c = 0; c < this->staticData->cantidadCompuertas; c++){
        this->compuertas[c]->actualizarCompuerta(
            this->staticData->colorICompuertas[c],
            this->staticData->colorFCompuertas[c],
            this->staticData->pasosCompuertas[c]);
    }
    // avanzar el ciclo de salidas: cada una sabe cuántos pasos necesita para cambiar de largo
    for(int s = 0; s < this->staticData->cantidadSalidas; s++){
        this->salidas[s]->actualizarSalida(
            this->staticData->largoISalidas[s],
            this->staticData->largoFSalidas[s],
            this->staticData->pasosSalidas[s]);
    }

    this->contadorMovimientos += 1; // 1 paso = 1 celda deslizada

    // registrar el movimiento que llevó a este estado (usado al reconstruir la solución)
    this->movimientoOrigen = Movimiento(idBloque, direccion, 1);

    // reconstruir cuadricula con la nueva posición y el nuevo estado de tiempo
    reconstruirCuadricula();

    // comprobar eventos post-movimiento (no tienen coste adicional de tiempo)
    if(intentarSalida(idBloque)){
        reconstruirCuadricula(); // se eliminó el bloque, actualizar visual
        return true;
    }
    if(intentarCompuerta(idBloque)){
        reconstruirCuadricula(); // el bloque cambió de posición, actualizar visual
        return true;
    }

    return true;
}

// Heurística auxiliar: suma de distancias Manhattan de cada bloque a la salida
// de su color más cercana. Se usa en contarMovimientosNecesarios (versión simplificada).
int Tablero::contarMovimientosNecesarios(){
    int total = 0;
    for(int b = 0; b < this->cantidadBloques; b++){
        Bloque* bloque = this->bloques[b];
        int  id     = bloque->id;
        char color  = this->staticData->coloresBloques[id];
        int  anchoB = this->staticData->anchoBloques[id];
        int  altoB  = this->staticData->altoBloques[id];

        int mejorDist = -1; // -1 = no encontramos salida del mismo color aún
        for(int s = 0; s < this->staticData->cantidadSalidas; s++){
            if(this->staticData->coloresSalidas[s] != color) continue;

            int sx = this->staticData->xSalidas[s];
            int sy = this->staticData->ySalidas[s];

            // distancia desde el BORDE del bloque más cercano a la salida
            // si la salida queda a la derecha del bloque: recorre sx-(x+anchoB)
            // si queda a la izquierda: recorre x-sx
            // si la salida queda dentro del rango horizontal del bloque: dx = 0
            int dx;
            if     (sx >= bloque->x + anchoB) dx = sx - (bloque->x + anchoB);
            else if(sx <  bloque->x)          dx = bloque->x - sx;
            else                               dx = 0;

            int dy;
            if     (sy >= bloque->y + altoB) dy = sy - (bloque->y + altoB);
            else if(sy <  bloque->y)         dy = bloque->y - sy;
            else                              dy = 0;

            int dist = dx + dy;
            if(mejorDist == -1 || dist < mejorDist) mejorDist = dist;
        }

        // si no hay salida del color del bloque, el tablero no tiene solución válida;
        // devolvemos un valor alto para que A* descarte esta rama rápidamente
        if(mejorDist == -1) mejorDist = 1000;
        total += mejorDist;
    }
    return total;
}

// Genera todos los tableros vecinos (hijos) del estado actual.
// Por cada bloque vivo y cada dirección válida: clona el tablero y aplica el movimiento.
Tablero** Tablero::generarVecinos(int& cantidadVecinos){
    int capacidad = this->cantidadBloques * 4; // máximo posible: 4 direcciones por bloque
    if(capacidad == 0){
        cantidadVecinos = 0;
        return nullptr; // sin bloques, sin vecinos
    }

    Tablero** vecinos = new Tablero*[capacidad]; // arreglo de hijos generados
    int n = 0;                                   // contador real de vecinos válidos

    char direcciones[4] = {'U', 'D', 'L', 'R'};

    for(int b = 0; b < this->cantidadBloques; b++){
        uint8_t idBloque = this->bloques[b]->id;
        for(int d = 0; d < 4; d++){
            // filtrar primero sobre el estado actual (sin clonar) para no desperdiciar memoria
            if(!this->comprobarMovimiento(idBloque, direcciones[d])) continue;

            // clonar y aplicar: moverBloque setea movimientoOrigen y padre del clon
            Tablero* hijo = this->clonar();
            hijo->moverBloque(idBloque, direcciones[d]);
            vecinos[n] = hijo;
            n++;
        }
    }

    cantidadVecinos = n;
    return vecinos;
}

// Dos tableros son iguales si tienen los mismos bloques vivos en las mismas posiciones,
// las mismas compuertas con los mismos colores y contadores, y las mismas salidas
// con los mismos largos y contadores. Se usa para detectar estados repetidos en A*.
bool Tablero::esIgual(Tablero* otro){
    if(this->cantidadBloques != otro->cantidadBloques) return false;

    // comparar bloques por id (el índice en el arreglo puede diferir si algunos ya salieron)
    for(int i = 0; i < this->cantidadBloques; i++){
        Bloque* b1       = this->bloques[i];
        bool    encontrado = false;
        for(int j = 0; j < otro->cantidadBloques; j++){
            Bloque* b2 = otro->bloques[j];
            if(b1->id == b2->id){
                if(b1->x != b2->x || b1->y != b2->y) return false; // mismo bloque, posición distinta
                encontrado = true;
                break;
            }
        }
        if(!encontrado) return false; // bloque presente en uno pero no en el otro
    }

    // compuertas: mismo orden garantizado por staticData
    int nc = this->staticData->cantidadCompuertas;
    for(int i = 0; i < nc; i++){
        if(this->compuertas[i]->coloractual   != otro->compuertas[i]->coloractual)   return false;
        if(this->compuertas[i]->contadorpasos != otro->compuertas[i]->contadorpasos) return false;
    }

    // salidas: comparar largo actual, contador de pasos y dirección de cambio
    int ns = this->staticData->cantidadSalidas;
    for(int i = 0; i < ns; i++){
        if(this->salidas[i]->la            != otro->salidas[i]->la)            return false;
        if(this->salidas[i]->contadorpasos != otro->salidas[i]->contadorpasos) return false;
        if(this->salidas[i]->direccion     != otro->salidas[i]->direccion)     return false;
    }

    return true; // todos los campos relevantes coinciden
}

// Hash del estado: combina posiciones de bloques, colores/pasos de compuertas y largos/pasos de salidas.
// Se multiplica por un primo grande en cada iteración para distribuir los valores y minimizar colisiones.
unsigned long Tablero::hash(){
    unsigned long h = 0;
    const unsigned long PRIMO = 1000003UL; // primo grande para mezclar bits del hash

    // iterar por todos los ids posibles (incluyendo los que ya salieron) para mantener
    // el hash consistente independientemente del orden en que se eliminan los bloques
    int nb = this->staticData->tamArrayBloques;
    for(int id = 0; id < nb; id++){
        int x = -1, y = -1; // -1 indica que el bloque con este id ya salió del tablero
        for(int i = 0; i < this->cantidadBloques; i++){
            if((int)this->bloques[i]->id == id){
                x = this->bloques[i]->x;
                y = this->bloques[i]->y;
                break;
            }
        }
        // sumar posición al hash (offset +1000 para evitar valores negativos con -1)
        h = h * PRIMO + (unsigned long)(x + 1000);
        h = h * PRIMO + (unsigned long)(y + 1000);
    }

    // incorporar estado de compuertas (color y ciclo)
    for(int i = 0; i < this->staticData->cantidadCompuertas; i++){
        h = h * PRIMO + (unsigned long)this->compuertas[i]->coloractual;
        h = h * PRIMO + (unsigned long)this->compuertas[i]->contadorpasos;
    }

    // incorporar estado de salidas (largo y ciclo)
    for(int i = 0; i < this->staticData->cantidadSalidas; i++){
        h = h * PRIMO + (unsigned long)this->salidas[i]->la;
        h = h * PRIMO + (unsigned long)this->salidas[i]->contadorpasos;
    }

    return h;
}

// Heurística admisible para A*: suma de distancias Manhattan de cada bloque vivo
// a la salida de su mismo color más cercana, con penalización por obstrucciones.
// Subestima la distancia real (admisible) => A* garantiza solución óptima si existe.
int Tablero::heuristica(){
    // necesitamos la cuadricula para detectar bloqueos en el camino
    if(this->cuadricula == nullptr) reconstruirCuadricula();

    int total = 0;
    int alto  = this->staticData->altoTablero;
    int ancho = this->staticData->anchoTablero;

    for(int b = 0; b < this->cantidadBloques; b++){
        Bloque* bloque = this->bloques[b];
        int     id     = bloque->id;
        char    color  = this->staticData->coloresBloques[id];
        int     anchoB = this->staticData->anchoBloques[id];
        int     altoB  = this->staticData->altoBloques[id];
        int     bx     = bloque->x;
        int     by     = bloque->y;

        // buscar la salida del mismo color más cercana, guardando también su posición
        // para luego escanear el camino en busca de bloqueos
        int mejorDist = -1;
        int mejorSx = -1, mejorSy = -1;
        for(int s = 0; s < this->staticData->cantidadSalidas; s++){
            if(this->staticData->coloresSalidas[s] != color) continue;
            int sx = this->staticData->xSalidas[s];
            int sy = this->staticData->ySalidas[s];
            int dx = bx - sx; if(dx < 0) dx = -dx;
            int dy = by - sy; if(dy < 0) dy = -dy;
            int dist = dx + dy;
            if(mejorDist == -1 || dist < mejorDist){
                mejorDist = dist;
                mejorSx = sx;
                mejorSy = sy;
            }
        }

        if(mejorDist == -1){ total += 1000; continue; } // sin salida del color: rama sin solución
        total += mejorDist;

        // penalización +2 por cada bloque que obstruye el camino horizontal hacia la salida
        // (se rompe al primer obstáculo por fila para no sobreestimar)
        int dxExit = mejorSx - bx;
        if(dxExit > 0){
            // la salida está a la derecha: escanear desde el borde derecho del bloque hasta la salida
            for(int row = by; row < by + altoB && row < alto; row++){
                for(int col = bx + anchoB; col < mejorSx && col < ancho; col++){
                    char c = this->cuadricula[row][col];
                    if(c != ' ' && c != '#'){ total += 2; break; } // obstáculo encontrado
                }
            }
        } else if(dxExit < 0){
            // la salida está a la izquierda: escanear desde el borde izquierdo hasta la salida
            for(int row = by; row < by + altoB && row < alto; row++){
                for(int col = mejorSx + 1; col < bx && col < ancho; col++){
                    char c = this->cuadricula[row][col];
                    if(c != ' ' && c != '#'){ total += 2; break; }
                }
            }
        }

        // penalización +2 por cada bloque que obstruye el camino vertical hacia la salida
        int dyExit = mejorSy - by;
        if(dyExit > 0){
            // la salida está abajo: escanear desde el borde inferior hasta la salida
            for(int col = bx; col < bx + anchoB && col < ancho; col++){
                for(int row = by + altoB; row < mejorSy && row < alto; row++){
                    char c = this->cuadricula[row][col];
                    if(c != ' ' && c != '#'){ total += 2; break; }
                }
            }
        } else if(dyExit < 0){
            // la salida está arriba: escanear desde el borde superior hasta la salida
            for(int col = bx; col < bx + anchoB && col < ancho; col++){
                for(int row = mejorSy + 1; row < by; row++){
                    char c = this->cuadricula[row][col];
                    if(c != ' ' && c != '#'){ total += 2; break; }
                }
            }
        }
    }
    return total;
}

// Libera el arreglo de cuadricula para ahorrar memoria durante A*.
// Al mover un tablero al ClosedSet solo se necesita hash/esIgual y el puntero padre.
// Si después se llama imprimir() o moverBloque(), reconstruirCuadricula() la regenera.
void Tablero::liberarCuadricula(){
    if(this->cuadricula == nullptr) return;
    int alto = this->staticData->altoTablero;
    for(int i = 0; i < alto; i++){
        delete[] this->cuadricula[i]; // liberar cada fila
    }
    delete[] this->cuadricula;        // liberar el arreglo de punteros
    this->cuadricula = nullptr;
}
