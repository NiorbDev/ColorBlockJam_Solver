/**
 * @file HashSet.h
 * @brief Contiene la definición de la clase HashSet.
 */

#pragma once

#include "Tablero.h"

/**
 * @class HashSet
 * @brief Estructura hash para almacenar estados Tablero*.
 * 
 * Esta estructura funciona como un conjunto de tableros,
 * utilizado principalmente por el algoritmo A* para:
 * - evitar estados repetidos
 * - almacenar estados visitados (ClosedSet)
 * 
 * La comparación entre estados se realiza usando:
 * - hash()
 * - esIgual()
 * 
 * =========================================================
 * COMPLEJIDAD
 * =========================================================
 * 
 * - insertar : O(1) amortizado
 * - contiene : O(1) promedio
 * - tamaño   : O(1)
 * 
 * =========================================================
 * GESTIÓN DE MEMORIA
 * =========================================================
 * 
 * El HashSet ES dueño de los tableros almacenados:
 * - elimina los tableros en su destructor
 * 
 * Por ello:
 * - A* debe reconstruir el camino solución antes
 *   de destruir el HashSet
 */
class HashSet {
public:

    /**
     * @struct Nodo
     * @brief Nodo de lista enlazada utilizado en cada bucket.
     * 
     * Cada bucket de la tabla hash almacena
     * una lista enlazada de tableros.
     */
    struct Nodo {

        /**
         * @brief Tablero almacenado en el nodo.
         */
        Tablero* tablero;

        /**
         * @brief Siguiente nodo de la lista enlazada.
         */
        Nodo* siguiente;
    };

    // =========================================================
    // ATRIBUTOS
    // =========================================================

    /**
     * @brief Arreglo de buckets de la tabla hash.
     * 
     * Cada posición apunta a la cabeza
     * de una lista enlazada.
     */
    Nodo** lista;

    /**
     * @brief Cantidad total de buckets.
     */
    int capacidad;

    /**
     * @brief Cantidad total de tableros almacenados.
     */
    int cantidad;

    // =========================================================
    // CONSTRUCTOR Y DESTRUCTOR
    // =========================================================

    /**
     * @brief Constructor del HashSet.
     * 
     * Inicializa la tabla hash con una capacidad base.
     * 
     * @param capacidadInicial Tamaño inicial de la tabla.
     */
    HashSet(int capacidadInicial = 101);

    /**
     * @brief Destructor del HashSet.
     * 
     * Libera:
     * - todos los nodos
     * - todos los tableros almacenados
     */
    ~HashSet();

    // =========================================================
    // OPERACIONES PRINCIPALES
    // =========================================================

    /**
     * @brief Inserta un tablero en el conjunto.
     * 
     * Si ya existe un tablero equivalente
     * según esIgual(), no se inserta.
     * 
     * @param tablero Tablero a insertar.
     * 
     * @return true Si el tablero fue insertado.
     * @return false Si ya existía un estado equivalente.
     */
    bool insertar(Tablero* tablero);

    /**
     * @brief Verifica si existe un tablero equivalente.
     * 
     * @param tablero Tablero a buscar.
     * 
     * @return true Si el estado ya existe.
     * @return false En caso contrario.
     */
    bool contiene(Tablero* tablero);

    /**
     * @brief Obtiene la cantidad total de tableros.
     * 
     * @return int Número de elementos almacenados.
     */
    int tamano();

private:

    // =========================================================
    // FUNCIONES AUXILIARES
    // =========================================================

    /**
     * @brief Redimensiona la tabla hash.
     * 
     * Se ejecuta cuando el factor de carga
     * supera el límite esperado.
     * 
     * Reorganiza todos los elementos manteniendo
     * complejidad amortizada O(1).
     */
    void rehashear();
};