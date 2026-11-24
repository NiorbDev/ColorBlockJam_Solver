/**
 * @file Bloque.h
 * @brief Contiene la definición de la clase Bloque.
 */

#pragma once

#include <cstdint>
#include "StaticData.h"

/**
 * @class Bloque
 * @brief Representa una pieza móvil dentro del juego Color Block Jam.
 * 
 * La clase Bloque almacena la posición y el identificador
 * de una pieza del tablero.
 * 
 * Cada bloque utiliza la información contenida en StaticData
 * para acceder a sus propiedades estáticas, como geometría,
 * tamaño y color.
 */
class Bloque {
public:

    // =========================================================
    // ATRIBUTOS
    // =========================================================

    /**
     * @brief Identificador único del bloque.
     * 
     * Se utiliza para acceder a los datos asociados
     * dentro de StaticData.
     */
    uint8_t id;

    /**
     * @brief Posición horizontal del bloque en el tablero.
     */
    int x;

    /**
     * @brief Posición vertical del bloque en el tablero.
     */
    int y;

    /**
     * @brief Referencia a los datos estáticos del juego.
     * 
     * Permite acceder a información global como:
     * - geometría de bloques
     * - dimensiones
     * - colores
     * - configuración del tablero
     */
    StaticData* staticData;

    // =========================================================
    // CONSTRUCTOR
    // =========================================================

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
    Bloque(
        uint8_t id,
        int x,
        int y,
        StaticData* staticData
    );

    // =========================================================
    // MOVIMIENTOS
    // =========================================================

    /**
     * @brief Desplaza el bloque una celda hacia arriba.
     */
    void moverArriba();

    /**
     * @brief Desplaza el bloque una celda hacia abajo.
     */
    void moverAbajo();

    /**
     * @brief Desplaza el bloque una celda hacia la izquierda.
     */
    void moverIzquierda();

    /**
     * @brief Desplaza el bloque una celda hacia la derecha.
     */
    void moverDerecha();
};