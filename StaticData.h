/**
 * @file StaticData.h
 * @brief Contiene la definición de la clase StaticData.
 */

#pragma once
#include <cstdint>

/**
 * @class StaticData
 * @brief Almacena todos los datos estáticos del juego.
 * 
 * Esta clase contiene la información fija del tablero,
 * bloques, salidas y compuertas.
 * 
 * Los datos almacenados aquí no representan un estado dinámico
 * de la partida, sino la configuración base cargada desde
 * el nivel o archivo de entrada.
 */
class StaticData {
public:

    // =========================================================
    // DATOS DEL TABLERO
    // =========================================================

    /**
     * @brief Ancho del tablero.
     */
    int anchoTablero;

    /**
     * @brief Alto del tablero.
     */
    int altoTablero;

    /**
     * @brief Matriz que representa las paredes del tablero.
     * 
     * Cada celda indica si existe o no una pared.
     */
    char** paredesTablero;

    /**
     * @brief Límite máximo de movimientos permitidos.
     */
    int limiteMovimientos;

    // =========================================================
    // DATOS DE LOS BLOQUES
    // =========================================================

    /**
     * @brief Cantidad de bloques presentes al inicio del juego.
     */
    int cantidadBloques;

    /**
     * @brief Tamaño total de los arreglos asociados a bloques.
     * 
     * Puede ser mayor o igual a cantidadBloques cuando
     * existen IDs no consecutivos.
     */
    int tamArrayBloques;

    /**
     * @brief Color asociado a cada bloque.
     */
    char* coloresBloques;

    /**
     * @brief Alto de cada bloque.
     */
    int* altoBloques;

    /**
     * @brief Ancho de cada bloque.
     */
    int* anchoBloques;

    /**
     * @brief Geometría de cada bloque.
     * 
     * Representa la forma del bloque mediante una matriz
     * de 0s y 1s con dimensiones alto x ancho.
     */
    uint8_t** geometriaBloques;

    // =========================================================
    // DATOS DE LAS SALIDAS
    // =========================================================

    /**
     * @brief Cantidad total de salidas.
     */
    int cantidadSalidas;

    /**
     * @brief Coordenadas X de las salidas.
     */
    int* xSalidas;

    /**
     * @brief Coordenadas Y de las salidas.
     */
    int* ySalidas;

    /**
     * @brief Color asociado a cada salida.
     */
    char* coloresSalidas;

    /**
     * @brief Orientación de cada salida.
     */
    char* orientacionSalidas;

    /**
     * @brief Inicio del rango ocupado por cada salida.
     */
    int* largoISalidas;

    /**
     * @brief Fin del rango ocupado por cada salida.
     */
    int* largoFSalidas;

    /**
     * @brief Cantidad de pasos requeridos para cada salida.
     */
    int* pasosSalidas;

    // =========================================================
    // DATOS DE LAS COMPUERTAS
    // =========================================================

    /**
     * @brief Cantidad total de compuertas.
     */
    int cantidadCompuertas;

    /**
     * @brief Coordenadas X de las compuertas.
     */
    int* xCompuertas;

    /**
     * @brief Coordenadas Y de las compuertas.
     */
    int* yCompuertas;

    /**
     * @brief Largo de cada compuerta.
     */
    int* largoCompuertas;

    /**
     * @brief Orientación de cada compuerta.
     */
    char* orientacionCompuertas;

    /**
     * @brief Color inicial asociado a cada compuerta.
     */
    char* colorICompuertas;

    /**
     * @brief Color final asociado a cada compuerta.
     */
    char* colorFCompuertas;

    /**
     * @brief Cantidad de pasos asociados a cada compuerta.
     */
    int* pasosCompuertas;

    // =========================================================
    // CONSTRUCTOR
    // =========================================================

    /**
     * @brief Constructor de la clase StaticData.
     * 
     * Inicializa la estructura que almacena toda la
     * información estática del juego.
     */
    StaticData();
};