/**
 * @file Tablero.h
 * @brief Contiene la definición de la clase Tablero.
 */

#pragma once

#include <iostream>
#include <string>

#include "StaticData.h"
#include "Bloque.h"
#include "Salida.h"
#include "Compuerta.h"
#include "Movimiento.h"

/**
 * @class Tablero
 * @brief Representa el estado completo del juego en un instante dado.
 * 
 * Esta clase es utilizada por el algoritmo A* como representación
 * de un estado del problema.
 * 
 * Cada tablero almacena únicamente información dinámica:
 * - bloques vivos y sus posiciones
 * - estado actual de salidas
 * - estado actual de compuertas
 * - contador de movimientos
 * 
 * Toda la información estática del nivel se mantiene en StaticData.
 * 
 * Además, cada tablero conoce:
 * - el estado padre desde el cual fue generado
 * - el movimiento aplicado para llegar a él
 * 
 * Esto permite reconstruir posteriormente el camino solución.
 */
class Tablero {
public:

    // =========================================================
    // DATOS ESTÁTICOS
    // =========================================================

    /**
     * @brief Referencia a los datos estáticos del juego.
     */
    StaticData* staticData;

    // =========================================================
    // DATOS DINÁMICOS
    // =========================================================

    /**
     * @brief Cantidad de bloques actualmente en juego.
     */
    int cantidadBloques;

    /**
     * @brief Arreglo de punteros a los bloques vivos.
     */
    Bloque** bloques;

    /**
     * @brief Arreglo de punteros a las salidas activas.
     */
    Salida** salidas;

    /**
     * @brief Arreglo de punteros a las compuertas activas.
     */
    Compuerta** compuertas;

    /**
     * @brief Cantidad de movimientos realizados.
     * 
     * Comienza en 0 para el estado inicial.
     */
    int contadorMovimientos;

    // =========================================================
    // DATOS PARA RECONSTRUCCIÓN DEL CAMINO
    // =========================================================

    /**
     * @brief Puntero al tablero padre.
     * 
     * Representa el estado desde el cual se generó
     * este tablero.
     */
    Tablero* padre;

    /**
     * @brief Movimiento aplicado para llegar a este tablero.
     */
    Movimiento movimientoOrigen;

    // =========================================================
    // REPRESENTACIÓN VISUAL
    // =========================================================

    /**
     * @brief Cuadrícula actual del tablero.
     * 
     * Se reconstruye dinámicamente usando:
     * - paredes
     * - bloques
     * - salidas
     * - compuertas
     */
    char** cuadricula;

    // =========================================================
    // CONSTRUCTORES Y DESTRUCTOR
    // =========================================================

    /**
     * @brief Constructor principal del tablero.
     * 
     * Inicializa un estado del juego usando los
     * elementos dinámicos proporcionados.
     * 
     * @param staticData Datos estáticos del juego.
     * @param bloques Bloques activos.
     * @param salidas Salidas activas.
     * @param compuertas Compuertas activas.
     */
    Tablero(
        StaticData* staticData,
        Bloque** bloques,
        Salida** salidas,
        Compuerta** compuertas
    );

    /**
     * @brief Destructor del tablero.
     * 
     * Libera la memoria asociada a:
     * - bloques
     * - salidas
     * - compuertas
     * - cuadrícula
     */
    ~Tablero();

    /**
     * @brief Constructor utilizado para clonar estados en A*.
     * 
     * @param staticData Datos estáticos del juego.
     * @param cantidadBloques Cantidad de bloques activos.
     * @param bloques Arreglo de bloques.
     * @param salidas Arreglo de salidas.
     * @param compuertas Arreglo de compuertas.
     * @param contadorMovimientos Cantidad de movimientos realizados.
     * @param padre Estado padre del tablero clonado.
     */
    Tablero(
        StaticData* staticData,
        int cantidadBloques,
        Bloque** bloques,
        Salida** salidas,
        Compuerta** compuertas,
        int contadorMovimientos,
        Tablero* padre
    );

    // =========================================================
    // REPRESENTACIÓN DEL TABLERO
    // =========================================================

    /**
     * @brief Reconstruye la cuadrícula del tablero.
     * 
     * Genera la representación visual utilizando:
     * - paredes
     * - bloques
     * - salidas
     * - compuertas
     */
    void reconstruirCuadricula();

    /**
     * @brief Imprime la cuadrícula actual en consola.
     */
    void imprimir();

    /**
     * @brief Libera la memoria de la cuadrícula.
     * 
     * Se utiliza para ahorrar memoria en estados
     * almacenados dentro del ClosedSet de A*.
     * 
     * Si posteriormente se requiere imprimir o mover
     * bloques, la cuadrícula puede regenerarse usando
     * reconstruirCuadricula().
     */
    void liberarCuadricula();

    // =========================================================
    // LÓGICA DEL JUEGO
    // =========================================================

    /**
     * @brief Verifica si el tablero corresponde a una solución.
     * 
     * @return true Si todos los bloques han salido.
     * @return false En caso contrario.
     */
    bool esSolucion();

    /**
     * @brief Busca un bloque según su identificador.
     * 
     * @param idBloque ID del bloque buscado.
     * @return int Índice del bloque en el arreglo.
     * @return -1 Si el bloque no existe.
     */
    int buscarBloque(uint8_t idBloque);

    /**
     * @brief Comprueba si un bloque puede moverse.
     * 
     * Verifica colisiones con:
     * - paredes
     * - otros bloques
     * - salidas bloqueadas
     * 
     * @param idBloque ID del bloque.
     * @param direccion Dirección del movimiento:
     * - 'U'
     * - 'D'
     * - 'L'
     * - 'R'
     * 
     * @return true Si el movimiento es válido.
     * @return false En caso contrario.
     */
    bool comprobarMovimiento(uint8_t idBloque, char direccion);

    /**
     * @brief Aplica el movimiento de un bloque.
     * 
     * Después del movimiento:
     * - se verifica si el bloque puede salir
     * - se verifica si puede atravesar una compuerta
     * 
     * Además:
     * - aumenta contadorMovimientos
     * - no tiene costo adicional usar salidas o compuertas
     * 
     * @param idBloque ID del bloque.
     * @param direccion Dirección del movimiento.
     * 
     * @return true Si el movimiento fue realizado.
     * @return false Si no pudo ejecutarse.
     */
    bool moverBloque(uint8_t idBloque, char direccion);

    /**
     * @brief Calcula movimientos aproximados necesarios.
     * 
     * Retorna la suma de distancias entre bloques
     * y sus respectivas salidas.
     * 
     * @return int Cantidad estimada de movimientos.
     */
    int contarMovimientosNecesarios();

    /**
     * @brief Intenta sacar un bloque por una salida válida.
     * 
     * Si el bloque puede salir:
     * - se elimina del arreglo de bloques
     * - retorna true
     * 
     * @param idBloque ID del bloque.
     * @return true Si el bloque salió.
     * @return false En caso contrario.
     */
    bool intentarSalida(uint8_t idBloque);

    /**
     * @brief Intenta atravesar una compuerta.
     * 
     * Si el bloque puede cruzar:
     * - se teletransporta
     * - retorna true
     * 
     * @param idBloque ID del bloque.
     * @return true Si cruzó la compuerta.
     * @return false En caso contrario.
     */
    bool intentarCompuerta(uint8_t idBloque);

    // =========================================================
    // FUNCIONES PARA A*
    // =========================================================

    /**
     * @brief Genera todos los estados vecinos válidos.
     * 
     * Para cada bloque y dirección:
     * - verifica si el movimiento es legal
     * - clona el tablero
     * - aplica el movimiento
     * - agrega el nuevo estado al arreglo resultado
     * 
     * Cada vecino:
     * - tiene padre = this
     * - registra movimientoOrigen
     * 
     * El llamador es responsable de liberar la memoria
     * de los vecinos generados.
     * 
     * @param cantidadVecinos Cantidad total de vecinos válidos.
     * @return Tablero** Arreglo de vecinos generados.
     */
    Tablero** generarVecinos(int& cantidadVecinos);

    /**
     * @brief Compara si dos tableros representan el mismo estado.
     * 
     * Compara:
     * - bloques vivos y posiciones
     * - colores de compuertas
     * - tamaños de salidas
     * 
     * Ignora:
     * - padre
     * - movimientoOrigen
     * - contadorMovimientos
     * 
     * @param otro Tablero a comparar.
     * @return true Si representan el mismo estado.
     * @return false En caso contrario.
     */
    bool esIgual(Tablero* otro);

    /**
     * @brief Genera un hash del estado actual.
     * 
     * Estados iguales generan el mismo hash.
     * 
     * Utilizado por el ClosedSet de A*.
     * 
     * @return unsigned long Hash del estado.
     */
    unsigned long hash();

    /**
     * @brief Estima la distancia restante a la solución.
     * 
     * Utiliza una heurística basada en:
     * - distancias Manhattan
     * - salida más cercana del mismo color
     * 
     * La heurística es admisible:
     * nunca sobreestima el costo real.
     * 
     * @return int Valor heurístico estimado.
     */
    int heuristica();

    /**
     * @brief Crea una copia profunda del tablero.
     * 
     * El nuevo tablero tendrá como padre
     * al tablero original.
     * 
     * @return Tablero* Copia del tablero.
     */
    Tablero* clonar();
};