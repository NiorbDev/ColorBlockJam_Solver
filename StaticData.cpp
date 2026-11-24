#include "StaticData.h"

// El constructor inicializa todos los atributos
// con valores por defecto para evitar referencias
// a memoria no inicializada.
StaticData::StaticData() {

    // =========================================================
    // DATOS DEL TABLERO
    // =========================================================

    this->anchoTablero = 0;
    this->altoTablero = 0;
    this->limiteMovimientos = 0;
    this->paredesTablero = nullptr;

    // =========================================================
    // DATOS DE LOS BLOQUES
    // =========================================================

    this->cantidadBloques = 0;
    this->tamArrayBloques = 0;
    this->coloresBloques = nullptr;
    this->altoBloques = nullptr;
    this->anchoBloques = nullptr;
    this->geometriaBloques = nullptr;

    // =========================================================
    // DATOS DE LAS SALIDAS
    // =========================================================

    this->cantidadSalidas = 0;
    this->xSalidas = nullptr;
    this->ySalidas = nullptr;
    this->coloresSalidas = nullptr;
    this->orientacionSalidas = nullptr;
    this->largoISalidas = nullptr;
    this->largoFSalidas = nullptr;
    this->pasosSalidas = nullptr;

    // =========================================================
    // DATOS DE LAS COMPUERTAS
    // =========================================================

    this->cantidadCompuertas = 0;
    this->xCompuertas = nullptr;
    this->yCompuertas = nullptr;
    this->largoCompuertas = nullptr;
    this->orientacionCompuertas = nullptr;
    this->colorICompuertas = nullptr;
    this->colorFCompuertas = nullptr;
    this->pasosCompuertas = nullptr;
}