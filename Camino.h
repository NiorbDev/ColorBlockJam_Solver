/**
 * @file Camino.h
 * @brief Contiene la definición de la clase Camino.
 */

#pragma once

#include "Movimiento.h"
#include "Tablero.h"

/**
 * @class Camino
 * @brief Representa una secuencia de movimientos aplicada a un tablero.
 * 
 * Esta clase almacena y administra la secuencia de movimientos
 * necesarios para llegar desde un estado inicial hasta una solución.
 * 
 * Además:
 * - reconstruye caminos generados por A*
 * - compacta movimientos consecutivos
 * - permite imprimir la solución
 * - permite reproducir los movimientos paso a paso
 * - permite parsear secuencias ingresadas por el usuario
 */
class Camino {
public:

    // =========================================================
    // ATRIBUTOS
    // =========================================================

    /**
     * @brief Arreglo dinámico de movimientos.
     */
    Movimiento* movimientos;

    /**
     * @brief Cantidad actual de movimientos almacenados.
     */
    int cantidad;

    /**
     * @brief Capacidad actual del arreglo.
     * 
     * Se duplica automáticamente cuando
     * el arreglo está próximo a llenarse.
     */
    int capacidad;

    // =========================================================
    // CONSTRUCTOR Y DESTRUCTOR
    // =========================================================

    /**
     * @brief Constructor de la clase Camino.
     * 
     * Inicializa el arreglo dinámico de movimientos.
     * 
     * @param capacidadInicial Tamaño inicial del arreglo.
     */
    Camino(int capacidadInicial = 16);

    /**
     * @brief Destructor de la clase Camino.
     * 
     * Libera el arreglo dinámico de movimientos.
     */
    ~Camino();

    // =========================================================
    // OPERACIONES PRINCIPALES
    // =========================================================

    /**
     * @brief Agrega un movimiento al final de la secuencia.
     * 
     * Si el arreglo está lleno, aumenta automáticamente
     * su capacidad.
     * 
     * @param m Movimiento a agregar.
     */
    void agregar(Movimiento m);

    /**
     * @brief Imprime la secuencia de movimientos.
     * 
     * Utiliza el formato compacto definido
     * por el enunciado del proyecto.
     */
    void imprimir();

    /**
     * @brief Aplica la secuencia de movimientos a un tablero.
     * 
     * El tablero recibido se modifica progresivamente
     * para visualizar el avance en tiempo real.
     * 
     * @param tablero Tablero sobre el cual aplicar los movimientos.
     * @param mostrarInicial Indica si debe mostrarse el estado inicial.
     */
    void aplicar(
        Tablero* tablero,
        bool mostrarInicial = true
    );

    /**
     * @brief Parsea una secuencia de movimientos desde texto.
     * 
     * Ejemplo de formato válido:
     * 
     * R1,2D2,1R1,10
     * 
     * @param s Cadena a parsear.
     * 
     * @return true Si el formato es válido.
     * @return false Si ocurre un error de parseo.
     */
    bool parsearDesdeString(const char* s);

    /**
     * @brief Duplica la capacidad del arreglo dinámico.
     * 
     * Se utiliza cuando el arreglo alcanza
     * su límite actual.
     */
    void duplicarCapacidad();

private:

    // =========================================================
    // FUNCIONES AUXILIARES
    // =========================================================

    /**
     * @brief Mueve el cursor de consola hacia arriba.
     * 
     * Utilizado para actualizar la visualización
     * del tablero en tiempo real.
     * 
     * @param lineas Cantidad de líneas a subir.
     */
    static void subirCursor(int lineas);

    /**
     * @brief Pausa la ejecución durante un tiempo.
     * 
     * Utilizado para animar la reproducción
     * de movimientos.
     * 
     * @param ms Tiempo de espera en milisegundos.
     */
    static void esperar(int ms);
};