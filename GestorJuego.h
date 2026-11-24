/**
 * @file GestorJuego.h
 * @brief Contiene la definicion de la clase GestorJuego.
 */

#pragma once

#include "StaticData.h"
#include "Tablero.h"
#include "Camino.h"

/**
 * @class GestorJuego
 * @brief Controla el flujo principal del programa Color Block Jam.
 *
 * Esta clase concentra la logica del menu interactivo y administra
 * el estado que se mantiene vivo mientras el programa esta abierto:
 * - el archivo de configuracion cargado
 * - los datos estaticos del nivel
 * - el tablero inicial del juego
 * - la ultima solucion calculada por A*
 *
 * Tambien contiene las rutinas auxiliares necesarias para liberar
 * memoria y recargar tableros limpios desde disco.
 */
class GestorJuego {
private:

    // =========================================================
    // ESTADO DEL PROGRAMA
    // =========================================================

    /**
     * @brief Ruta del archivo de configuracion cargado actualmente.
     *
     * Se guarda para poder reconstruir un tablero limpio cuando
     * se reproduce una solucion o una secuencia manual.
     */
    char nombreArchivo[512];

    /**
     * @brief Datos estaticos del nivel cargado.
     *
     * Incluye dimensiones, paredes, geometria de bloques,
     * salidas, compuertas y limite de movimientos.
     */
    StaticData* staticData;

    /**
     * @brief Tablero correspondiente al estado inicial del nivel.
     *
     * Este tablero se usa como punto de partida para ejecutar A*.
     */
    Tablero* tableroInicial;

    /**
     * @brief Ultima solucion encontrada por A*.
     *
     * Puede ser nullptr si todavia no se ha resuelto un nivel
     * o si el ultimo intento no encontro solucion.
     */
    Camino* ultimaSolucion;

    /**
     * @brief Tiempo de la ultima resolucion en milisegundos.
     */
    int ultimoTiempoMs;

    /**
     * @brief Indica si el ciclo principal debe terminar.
     */
    bool salir;

    // =========================================================
    // OPERACIONES DEL MENU
    // =========================================================

    /**
     * @brief Imprime el menu principal del programa.
     */
    void mostrarMenu();

    /**
     * @brief Lee y ejecuta una opcion ingresada por el usuario.
     *
     * Si la entrada no es numerica, descarta la linea y mantiene
     * el programa en ejecucion.
     */
    void procesarOpcion();

    /**
     * @brief Carga un archivo de configuracion desde disco.
     *
     * Si la carga es exitosa, reemplaza el nivel anterior,
     * reinicia la solucion guardada y muestra el estado inicial.
     */
    void cargarArchivo();

    /**
     * @brief Ejecuta A* sobre el tablero inicial cargado.
     *
     * Guarda la solucion encontrada para poder reproducirla
     * posteriormente desde el menu.
     */
    void resolverJuego();

    /**
     * @brief Reproduce paso a paso la ultima solucion encontrada.
     *
     * Para no modificar el tablero inicial, recarga el archivo
     * y aplica la secuencia sobre un tablero fresco.
     */
    void mostrarResultado();

    /**
     * @brief Aplica una secuencia manual de movimientos.
     *
     * La secuencia se ingresa con el formato compacto del enunciado,
     * por ejemplo: R2,2D2,1R1,5.
     */
    void aplicarSecuenciaManual();

    // =========================================================
    // LIMPIEZA Y FUNCIONES AUXILIARES
    // =========================================================

    /**
     * @brief Libera la solucion guardada, si existe.
     */
    void limpiarUltimaSolucion();

    /**
     * @brief Libera el nivel cargado actualmente.
     *
     * Borra la solucion, el tablero inicial, los datos estaticos
     * y limpia el nombre del archivo cargado.
     */
    void limpiarNivelActual();

    /**
     * @brief Libera todos los recursos asociados a StaticData.
     *
     * StaticData contiene varios arreglos dinamicos, por lo que
     * deben liberarse explicitamente antes de destruir la instancia.
     *
     * @param sd Datos estaticos a liberar.
     */
    static void liberarStaticData(StaticData* sd);

    /**
     * @brief Crea un tablero limpio recargando el archivo desde disco.
     *
     * Se utiliza al animar soluciones o secuencias manuales, porque
     * mover bloques modifica el tablero recibido.
     *
     * @param ruta Ruta del archivo de configuracion.
     * @param sdSalida Referencia donde se deja el StaticData recargado.
     *
     * @return Tablero* Tablero recargado, o nullptr si falla la carga.
     */
    static Tablero* recargarTableroDesdeArchivo(
        const char* ruta,
        StaticData*& sdSalida
    );

public:

    // =========================================================
    // CONSTRUCTOR Y DESTRUCTOR
    // =========================================================

    /**
     * @brief Constructor de la clase GestorJuego.
     *
     * Inicializa todos los punteros en nullptr y deja el programa
     * listo para entrar al menu principal.
     */
    GestorJuego();

    /**
     * @brief Destructor de la clase GestorJuego.
     *
     * Libera cualquier nivel o solucion que haya quedado cargado
     * antes de cerrar el programa.
     */
    ~GestorJuego();

    // =========================================================
    // OPERACION PRINCIPAL
    // =========================================================

    /**
     * @brief Ejecuta el ciclo principal del programa.
     *
     * Muestra el menu, recibe opciones y llama a la operacion
     * correspondiente hasta que el usuario seleccione salir.
     */
    void ejecutar();
};
