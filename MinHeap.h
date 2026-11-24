/**
 * @file MinHeap.h
 * @brief Contiene la definición de la clase MinHeap.
 */

#pragma once

#include "Tablero.h"

/**
 * @class MinHeap
 * @brief Cola de prioridad implementada mediante un heap binario mínimo.
 * 
 * Esta estructura almacena punteros a Tablero asociados
 * a una prioridad entera.
 * 
 * En el algoritmo A*:
 * - la prioridad corresponde normalmente a f = g + h
 * - el tablero con menor prioridad queda en la raíz
 * 
 * =========================================================
 * COMPLEJIDAD
 * =========================================================
 * 
 * - push : O(log n)
 * - pop  : O(log n)
 * - peek : O(1)
 * 
 * =========================================================
 * GESTIÓN DE MEMORIA
 * =========================================================
 * 
 * El heap NO es dueño de los tableros almacenados:
 * - no libera los punteros Tablero*
 * - solo libera su arreglo interno
 * 
 * El llamador que obtiene un tablero mediante pop()
 * pasa a ser responsable de su liberación.
 */
class MinHeap {
public:

    /**
     * @struct Entrada
     * @brief Representa una entrada del heap.
     * 
     * Cada entrada asocia:
     * - un tablero
     * - una prioridad entera
     */
    struct Entrada {

        /**
         * @brief Puntero al tablero almacenado.
         */
        Tablero* tablero;

        /**
         * @brief Prioridad asociada al tablero.
         */
        int prioridad;
    };

    // =========================================================
    // ATRIBUTOS
    // =========================================================

    /**
     * @brief Arreglo dinámico que almacena el heap.
     */
    Entrada* datos;

    /**
     * @brief Cantidad actual de elementos almacenados.
     */
    int largo;

    /**
     * @brief Capacidad actual del arreglo dinámico.
     */
    int capacidad;

    // =========================================================
    // CONSTRUCTORES Y DESTRUCTOR
    // =========================================================

    /**
     * @brief Constructor del heap.
     * 
     * Inicializa el heap con una capacidad base.
     * 
     * @param capacidadInicial Tamaño inicial del arreglo.
     */
    MinHeap(int capacidadInicial = 64);

    /**
     * @brief Destructor del heap.
     * 
     * Libera únicamente el arreglo interno.
     * 
     * No elimina los tableros almacenados.
     */
    ~MinHeap();

    // =========================================================
    // OPERACIONES PRINCIPALES
    // =========================================================

    /**
     * @brief Inserta un tablero en el heap.
     * 
     * Complejidad: O(log n)
     * 
     * @param tablero Tablero a insertar.
     * @param prioridad Prioridad asociada.
     */
    void push(Tablero* tablero, int prioridad);

    /**
     * @brief Extrae el tablero con menor prioridad.
     * 
     * Complejidad: O(log n)
     * 
     * El tablero removido deja de pertenecer al heap.
     * 
     * @return Tablero* Tablero con menor prioridad.
     * @return nullptr Si el heap está vacío.
     */
    Tablero* pop();

    /**
     * @brief Retorna el tablero con menor prioridad.
     * 
     * No elimina el elemento del heap.
     * 
     * Complejidad: O(1)
     * 
     * @return Tablero* Tablero con menor prioridad.
     * @return nullptr Si el heap está vacío.
     */
    Tablero* peek();

    /**
     * @brief Verifica si el heap está vacío.
     * 
     * @return true Si no hay elementos.
     * @return false En caso contrario.
     */
    bool vacio();

    /**
     * @brief Obtiene la cantidad actual de elementos.
     * 
     * @return int Tamaño del heap.
     */
    int tamano();

private:

    // =========================================================
    // FUNCIONES AUXILIARES
    // =========================================================

    /**
     * @brief Duplica la capacidad del arreglo interno.
     * 
     * Se utiliza cuando el heap alcanza
     * su límite actual de almacenamiento.
     */
    void duplicarCapacidad();

    /**
     * @brief Reordena un elemento hacia arriba.
     * 
     * Mantiene la propiedad del heap mínimo.
     * 
     * @param i Índice inicial.
     */
    void siftUp(int i);

    /**
     * @brief Reordena un elemento hacia abajo.
     * 
     * Mantiene la propiedad del heap mínimo.
     * 
     * @param i Índice inicial.
     */
    void siftDown(int i);
};