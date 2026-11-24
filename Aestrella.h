/**
 * @file Aestrella.h
 * @brief Contiene la definición de la clase Aestrella.
 */

#pragma once

#include "Tablero.h"
#include "Camino.h"
#include "MinHeap.h"
#include "HashSet.h"

/**
 * @class Aestrella
 * @brief Implementa el algoritmo A* para resolver Color Block Jam.
 * 
 * Este algoritmo realiza una búsqueda informada utilizando:
 * - MinHeap como OpenSet
 * - HashSet como ClosedSet
 * 
 * =========================================================
 * ESTRUCTURAS UTILIZADAS
 * =========================================================
 * 
 * OpenSet:
 * - cola de prioridad basada en heap mínimo
 * - almacena estados pendientes por explorar
 * - complejidad O(log n)
 * 
 * ClosedSet:
 * - tabla hash de estados visitados
 * - detección de repetidos en O(1) promedio
 * 
 * =========================================================
 * REPRESENTACIÓN DE ESTADOS
 * =========================================================
 * 
 * Cada estado corresponde a un objeto Tablero.
 * 
 * La prioridad utilizada por A* es:
 * 
 * f = g + h
 * 
 * donde:
 * - g = contadorMovimientos
 * - h = heurística estimada hasta la solución
 * 
 * =========================================================
 * FUNCIONAMIENTO DEL ALGORITMO
 * =========================================================
 * 
 * 1) Insertar el tablero inicial en OpenSet
 * 
 * 2) Mientras OpenSet no esté vacío:
 * 
 *    a) Extraer el estado con menor f
 * 
 *    b) Si ya fue visitado, ignorarlo
 * 
 *    c) Si es solución:
 *       - reconstruir camino
 *       - terminar
 * 
 *    d) Si supera STEP_LIMIT:
 *       - podar el estado
 * 
 *    e) Insertar en ClosedSet
 * 
 *    f) Generar vecinos:
 *       - ignorar repetidos
 *       - insertar nuevos estados en OpenSet
 * 
 * 3) Si OpenSet queda vacío:
 *    - no existe solución
 * 
 * =========================================================
 * EJEMPLO DE USO
 * =========================================================
 * 
 * Aestrella* a = new Aestrella(tableroInicial);
 * Camino* sol = a->resolver();
 * int ms = a->tiempoMs();
 * delete a;
 * delete sol;
 */
class Aestrella {
public:

    // =========================================================
    // ATRIBUTOS
    // =========================================================

    /**
     * @brief Tablero inicial del problema.
     * 
     * La clase NO es dueña de este puntero.
     */
    Tablero* inicial;

    /**
     * @brief Tiempo de ejecución de la última búsqueda.
     * 
     * Expresado en milisegundos.
     */
    int ultimoTiempo;

    /**
     * @brief Cantidad de estados explorados.
     * 
     * Utilizado para estadísticas.
     */
    int estadosExplorados;

    /**
     * @brief Cantidad de estados generados.
     * 
     * Utilizado para estadísticas.
     */
    int estadosGenerados;

    // =========================================================
    // CONSTRUCTOR Y DESTRUCTOR
    // =========================================================

    /**
     * @brief Constructor del algoritmo A*.
     * 
     * @param tableroInicial Estado inicial del problema.
     */
    Aestrella(Tablero* tableroInicial);

    /**
     * @brief Destructor de la clase Aestrella.
     */
    ~Aestrella();

    // =========================================================
    // MÉTODOS PRINCIPALES
    // =========================================================

    /**
     * @brief Busca una solución para el tablero inicial.
     * 
     * Ejecuta el algoritmo A* completo.
     * 
     * @return Camino* Camino solución encontrado.
     * @return nullptr Si no existe solución.
     * 
     * El llamador es responsable de liberar
     * el Camino retornado.
     */
    Camino* resolver();

    /**
     * @brief Retorna el tiempo de la última ejecución.
     * 
     * @return int Tiempo en milisegundos.
     */
    int tiempoMs();

private:

    // =========================================================
    // FUNCIONES AUXILIARES
    // =========================================================

    /**
     * @brief Reconstruye el camino solución.
     * 
     * Utiliza los punteros padre de cada tablero
     * desde la solución hasta el estado inicial.
     * 
     * @param solucion Estado solución.
     * @return Camino* Camino reconstruido.
     */
    Camino* reconstruirCamino(Tablero* solucion);

    /**
     * @brief Obtiene la memoria utilizada actualmente.
     * 
     * @return size_t Memoria usada en MB.
     */
    static size_t memoriaActualMB();

    /**
     * @brief Habilita secuencias ANSI si la consola lo requiere.
     *
     * En Linux normalmente no hace nada porque la terminal ya entiende ANSI.
     * En Windows intenta activar ENABLE_VIRTUAL_TERMINAL_PROCESSING para que
     * funcionen codigos como borrar linea y ocultar cursor.
     *
     * @return true Si se pueden usar secuencias ANSI.
     * @return false Si se debe usar el respaldo con espacios.
     */
    static bool prepararConsola();

    /**
     * @brief Borra la linea actual de la consola.
     *
     * Se usa antes de imprimir el progreso de A* para que la misma linea
     * se sobrescriba limpiamente en Ubuntu y Windows.
     */
    static void limpiarLineaActual();

    /**
     * @brief Oculta el cursor mientras se actualiza el progreso.
     */
    static void ocultarCursor();

    /**
     * @brief Restaura el cursor al finalizar la busqueda.
     */
    static void mostrarCursor();

    /**
     * @brief Imprime estadísticas del algoritmo.
     * 
     * @param explorados Estados explorados.
     * @param generados Estados generados.
     * @param bloques Cantidad de bloques restantes.
     * @param ms Tiempo transcurrido.
     */
    static void imprimirEstado(
        int explorados,
        int generados,
        int bloques,
        double ms
    );
};
