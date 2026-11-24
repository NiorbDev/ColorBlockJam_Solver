/**
 * @file Salida.h
 * @brief Contiene la definición de la clase Salida.
 */

#pragma once

#include <iostream>
#include <string>
#include "StaticData.h"

/**
 * @class Salida
 * @brief Representa una salida del tablero en Color Block Jam.
 * 
 * Una salida permite eliminar una pieza del tablero cuando:
 * - la pieza toca una salida del mismo color
 * - el tamaño de la pieza coincide con el tamaño actual de la salida
 * 
 * Además, las salidas pueden variar dinámicamente su tamaño
 * con el tiempo, creciendo o disminuyendo según una cantidad
 * determinada de pasos.
 */
class Salida {
public:

    // =========================================================
    // ATRIBUTOS
    // =========================================================

    /**
     * @brief Identificador único de la salida.
     */
    int id;

    /**
     * @brief Tamaño actual de la salida.
     * 
     * Representa el largo actual que posee la salida
     * en el tablero.
     */
    int la;

    /**
     * @brief Contador de pasos para variar el tamaño.
     * 
     * Indica cuántos pasos faltan para modificar el tamaño
     * actual entre los límites establecidos.
     */
    int contadorpasos;

    /**
     * @brief Dirección actual del cambio de tamaño.
     * 
     * Valores posibles:
     * - 1  : la salida debe crecer
     * - -1 : la salida debe achicarse
     */
    char direccion;

    /**
     * @brief Referencia a los datos estáticos del juego.
     */
    StaticData* staticData;

    // =========================================================
    // CONSTRUCTOR
    // =========================================================

    /**
     * @brief Constructor de la clase Salida.
     * 
     * Inicializa una salida con su identificador,
     * tamaño inicial y referencia a los datos estáticos.
     * 
     * @param id Identificador de la salida.
     * @param la Tamaño actual inicial de la salida.
     * @param staticData Referencia a los datos estáticos del juego.
     */
    Salida(
        int id,
        int la,
        StaticData* staticData
    );

    // =========================================================
    // MÉTODOS
    // =========================================================

    /**
     * @brief Actualiza el tamaño dinámico de la salida.
     * 
     * La salida puede crecer o disminuir entre un tamaño
     * mínimo y máximo dependiendo de la cantidad de pasos
     * configurados.
     * 
     * @param li Tamaño mínimo permitido.
     * @param lf Tamaño máximo permitido.
     * @param pasos Cantidad de pasos necesarios para cambiar el tamaño.
     */
    void actualizarSalida(int li, int lf, int pasos);
};