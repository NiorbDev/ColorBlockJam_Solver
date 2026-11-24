// Con pragma once se evita la inclusión múltiple del mismo archivo
#pragma once

// Agregamos libreria que permite asignar tipos de
// datos con tamaños especificos
#include <cstdint>

/**
 * @class Movimiento
 * @brief Representa el movimiento realizado por un bloque dentro del tablero.
 * 
 * Esta clase almacena la información necesaria para describir
 * un movimiento realizado durante la ejecución del programa.
 * 
 * Sirve para reconstruir el camino de la solución y registrar
 * cómo se desplazó cada bloque dentro del tablero.
 */
class Movimiento {
public:

    /**
     * @brief Identificador del bloque movido.
     * 
     * Solo acepta números positivos entre 0 y 255,
     * gracias al uso del tipo uint8_t.
     */
    uint8_t idBloque;

    /**
     * @brief Dirección en la que se movió el bloque.
     * 
     * Valores posibles:
     * - 'U' : arriba (Up)
     * - 'D' : abajo (Down)
     * - 'L' : izquierda (Left)
     * - 'R' : derecha (Right)
     */
    char direccion;

    /**
     * @brief Cantidad de celdas que se desplazó el bloque.
     */
    int celdas;

    /**
     * @brief Constructor vacío de la clase Movimiento.
     * 
     * Es obligatorio si se quiere crear un arreglo de movimientos.
     */
    Movimiento();

    /**
     * @brief Constructor con parámetros.
     * 
     * Inicializa un movimiento con toda la información necesaria.
     * 
     * @param idBloque Identificador del bloque movido.
     * @param direccion Dirección del movimiento.
     * @param celdas Cantidad de celdas desplazadas.
     */
    Movimiento(uint8_t idBloque, char direccion, int celdas);
};