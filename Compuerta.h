/**
 * @file Compuerta.h
 * @brief Contiene la definición de la clase Compuerta.
 */

#pragma once

#include <string>
#include "StaticData.h"

/**
 * @class Compuerta
 * @brief Representa una compuerta del tablero en Color Block Jam.
 * 
 * Una compuerta permite transportar una pieza hacia el otro
 * extremo de la misma cuando la pieza posee el mismo color.
 * 
 * Además, las compuertas pueden cambiar dinámicamente de color
 * con el tiempo, independientemente de si una pieza las atraviesa
 * o no.
 * 
 * El cambio de color puede ocurrir cada cierta cantidad de pasos:
 * - si pasos = 0, el color permanece fijo
 * - en otro caso, alterna entre dos colores
 */
class Compuerta {
public:

    // =========================================================
    // ATRIBUTOS
    // =========================================================

    /**
     * @brief Identificador único de la compuerta.
     */
    int id;

    /**
     * @brief Color actual de la compuerta.
     * 
     * Determina qué piezas pueden atravesarla.
     */
    char coloractual;

    /**
     * @brief Contador de pasos para actualizar el color.
     * 
     * Lleva el control de cuántos pasos han transcurrido
     * antes de realizar un cambio de color.
     */
    int contadorpasos;

    /**
     * @brief Referencia a los datos estáticos del juego.
     */
    StaticData* staticData;

    // =========================================================
    // CONSTRUCTOR
    // =========================================================

    /**
     * @brief Constructor de la clase Compuerta.
     * 
     * Inicializa la compuerta con su identificador,
     * color actual y referencia a los datos estáticos.
     * 
     * @param id Identificador de la compuerta.
     * @param coloractual Color inicial de la compuerta.
     * @param staticData Referencia a los datos estáticos del juego.
     */
    Compuerta(
        int id,
        char coloractual,
        StaticData* staticData
    );

    // =========================================================
    // MÉTODOS
    // =========================================================

    /**
     * @brief Actualiza dinámicamente el color de la compuerta.
     * 
     * La compuerta alterna entre un color inicial y un color final
     * según la cantidad de pasos configurados.
     * 
     * Si pasos es igual a 0, el color permanece fijo.
     * 
     * @param colorInicial Color inicial de la compuerta.
     * @param colorFinal Color alternativo de la compuerta.
     * @param pasos Cantidad de pasos necesarios para cambiar el color.
     */
    void actualizarCompuerta(
        char colorInicial,
        char colorFinal,
        int pasos
    );
};