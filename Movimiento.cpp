#include "Movimiento.h"

// Constructor vacío de la clase Movimiento.
// Se utiliza principalmente para permitir la creación
// de arreglos o estructuras dinámicas de movimientos.
Movimiento::Movimiento() {
    this->idBloque = 0;
    this->direccion = ' ';
    this->celdas = 0;
}

// Constructor con parámetros de la clase Movimiento.
// Inicializa toda la información asociada
// al desplazamiento de un bloque.
Movimiento::Movimiento(uint8_t idBloque, char direccion, int celdas) {
    this->idBloque = idBloque;
    this->direccion = direccion;
    this->celdas = celdas;
}