#include "Bloque.h"

/**
 * @brief Constructor de la clase Bloque.
 * 
 * Inicializa un bloque con su identificador,
 * posición inicial y referencia a los datos
 * estáticos del juego.
 * 
 * @param id Identificador único del bloque.
 * @param x Posición inicial en el eje X.
 * @param y Posición inicial en el eje Y.
 * @param staticData Referencia a los datos estáticos del juego.
 */
Bloque::Bloque(
    uint8_t id,
    int x,
    int y,
    StaticData* staticData
) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->staticData = staticData;
}

/**
 * @brief Mueve el bloque una celda hacia arriba.
 * 
 * Disminuye la coordenada Y del bloque en una unidad.
 */
void Bloque::moverArriba() {
    this->y -= 1;
}

/**
 * @brief Mueve el bloque una celda hacia abajo.
 * 
 * Aumenta la coordenada Y del bloque en una unidad.
 */
void Bloque::moverAbajo() {
    this->y += 1;
}

/**
 * @brief Mueve el bloque una celda hacia la izquierda.
 * 
 * Disminuye la coordenada X del bloque en una unidad.
 */
void Bloque::moverIzquierda() {
    this->x -= 1;
}

/**
 * @brief Mueve el bloque una celda hacia la derecha.
 * 
 * Aumenta la coordenada X del bloque en una unidad.
 */
void Bloque::moverDerecha() {
    this->x += 1;
}