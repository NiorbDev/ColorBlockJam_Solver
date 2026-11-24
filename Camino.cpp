#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <ctime>
#endif
#include "Camino.h"

// sube el cursor n lineas para imprimir el siguiente estado en el mismo lugar
void Camino::subirCursor(int lineas){
    for(int i = 0; i < lineas; i++){
        std::cout << "\033[A\033[2K";
    }
    std::cout << std::flush;
}

// espera ms milisegundos (usada para animar la aplicación del camino)
void Camino::esperar(int ms){
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, nullptr);
#endif
}


Camino::Camino(int capacidadInicial){
    // inicializamos variables del camino: arreglo de movimientos, cantidad y capacidad
    this->capacidad = capacidadInicial;
    this->cantidad = 0;
    this->movimientos = new Movimiento[this->capacidad];
}

Camino::~Camino(){
    delete[] this->movimientos;
}

// duplica la capacidad del arreglo de movimientos
void Camino::duplicarCapacidad(){
    int capacidadNueva = this->capacidad * 2;
    Movimiento* nuevos = new Movimiento[capacidadNueva];
    for(int i = 0; i < this->cantidad; i++){
        nuevos[i] = this->movimientos[i];
    }
    delete[] this->movimientos;
    this->movimientos = nuevos;
    this->capacidad = capacidadNueva;
}

void Camino::agregar(Movimiento m){
    //verificar si se llenó el arreglo
    if(this->cantidad == this->capacidad){
        duplicarCapacidad();
    }
    //agregar al final
    this->movimientos[this->cantidad] = m;
    this->cantidad++;
}

//imprime compactando movimientos consecutivos del mismo bloque y direccion.
//ejemplo: {R0,1 R0,1 R0,1 D2,1 D2,1} -> "R1,3 D3,2"
void Camino::imprimir(){
    //ver si no hay camino guardado
    if(this->cantidad == 0){
        std::cout << "(camino vacio)" << std::endl;
        return;
    }
    int i = 0;
    bool primero = true;
    //a cada movimieto imprimirlo en formato compacto "D1,2" (direccion, idBloque+1, cantidadCeldas)
    while(i < this->cantidad){
        //guardar su id (del bloque), direccion y cantidad de celdas que se movió
        uint8_t id = this->movimientos[i].idBloque;
        char dir = this->movimientos[i].direccion;
        int celdasAcum = this->movimientos[i].celdas;
        //juntar con los siguientes mientras sean iguales
        int j = i + 1;
        while(j < this->cantidad
              && this->movimientos[j].idBloque == id
              && this->movimientos[j].direccion == dir){
            celdasAcum += this->movimientos[j].celdas;
            j++;
        }
        //imprimir el grupo
        if(!primero) std::cout << " ";
        std::cout << dir << ((int)id + 1) << "," << celdasAcum;
        primero = false;
        i = j;
    }
    std::cout << std::endl;
}

//aplica los movimientos al tablero, animando cada paso en el mismo lugar.
void Camino::aplicar(Tablero* tablero, bool mostrarInicial){
    int alto = tablero->staticData->altoTablero;
    int lineasFrame = 1 + alto;

    std::cout << "\033[?25l" << std::flush; // ocultar cursor


    // mostrar estado inicial antes de aplicar movimientos
    if(mostrarInicial){
        std::cout << "Estado inicial:" << std::endl;
        tablero->imprimir();
        esperar(400);
    }
    // aplicar cada movimiento, mostrando el estado actualizado tras cada uno
    for(int i = 0; i < this->cantidad; i++){
        Movimiento m = this->movimientos[i];
        bool ok = tablero->moverBloque(m.idBloque, m.direccion);

        subirCursor(lineasFrame);

        // imprimir el movimiento aplicado y el estado del tablero
        std::cout << "Paso " << (i + 1) << "/" << this->cantidad << ": "
                  << m.direccion << ((int)m.idBloque + 1) << ",1";
        if(!ok) std::cout << " (INVALIDO)";
        std::cout << std::endl;
        tablero->imprimir();

        esperar(200);
    }

    std::cout << "\033[?25h" << std::flush; // restaurar cursor

    if(tablero->esSolucion()){
        std::cout << "\nSOLUCION ALCANZADA." << std::endl;
    }
}

//parsea "R1,2D2,1R1,10" produciendo movimientos de 1 celda expandidos.
//"R1,2" significa "bloque 1, 2 celdas a la derecha",
//lo que internamente son 2 movimientos R de 1 celda con idBloque=0.
//retorna false si el formato es invalido.
bool Camino::parsearDesdeString(const char* s){
    //limpiar contenido actual
    this->cantidad = 0;

    int i = 0;
    while(s[i] != '\0' && s[i] != '\n' && s[i] != '\r'){
        //saltar espacios
        if(s[i] == ' ' || s[i] == '\t'){ i++; continue; }

        //esperamos una direccion
        char dir = s[i];
        if(dir != 'U' && dir != 'D' && dir != 'L' && dir != 'R') return false;
        i++;

        //leer id (1-indexed)
        if(s[i] < '0' || s[i] > '9') return false;
        int idLeido = 0;
        while(s[i] >= '0' && s[i] <= '9'){
            idLeido = idLeido * 10 + (s[i] - '0');
            i++;
        }
        if(idLeido < 1) return false;
        uint8_t idInterno = (uint8_t)(idLeido - 1);

        //coma
        if(s[i] != ',') return false;
        i++;

        //leer cantidad de celdas
        if(s[i] < '0' || s[i] > '9') return false;
        int celdas = 0;
        while(s[i] >= '0' && s[i] <= '9'){
            celdas = celdas * 10 + (s[i] - '0');
            i++;
        }
        if(celdas < 1) return false;

        //expandir a 'celdas' movimientos de 1 celda cada uno (opcion A)
        for(int k = 0; k < celdas; k++){
            this->agregar(Movimiento(idInterno, dir, 1));
        }
    }
    return true;
}