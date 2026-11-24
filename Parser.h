/**
 * @file Parser.h
 * @brief Contiene la definición de la clase Parser.
 */

#pragma once

#include "StaticData.h"
#include "Bloque.h"
#include "Salida.h"
#include "Compuerta.h"

/**
 * @class Parser
 * @brief Responsable de leer y construir un nivel de Color Block Jam.
 * 
 * Esta clase procesa un archivo de configuración y construye:
 * - los datos estáticos del juego (StaticData)
 * - los bloques iniciales
 * - las salidas
 * - las compuertas
 * 
 * Los datos generados quedan listos para ser utilizados
 * por el constructor de Tablero.
 * 
 * =========================================================
 * FORMATO ESPERADO DEL ARCHIVO
 * =========================================================
 * 
 * [META]
 * NAME, WIDTH, HEIGHT, STEP_LIMIT
 * 
 * [BLOCK]
 * ID COLOR=c WIDTH=w HEIGHT=h INIT_X=x INIT_Y=y GEOMETRY=...
 * 
 * [WALL]
 * Matriz de '#' y ' ' del tamaño WIDTH x HEIGHT
 * 
 * [EXIT]
 * COLOR=c X=x Y=y ORIENTATION=H|V LI=li LF=lf STEP=p
 * 
 * [GATE]
 * COLOR=c X=x Y=y ORIENTATION=H|V LI=li CI=ci CF=cf STEP=p
 * 
 * =========================================================
 * CONVENCIÓN DE COORDENADAS
 * =========================================================
 * 
 * - X representa columnas
 * - Y representa filas
 * - Internamente: cuadricula[fila][columna]
 */
class Parser {
public:

    // =========================================================
    // DATOS PARSEADOS
    // =========================================================

    /**
     * @brief Datos estáticos del nivel.
     */
    StaticData* staticData;

    /**
     * @brief Arreglo de bloques iniciales.
     */
    Bloque** bloques;

    /**
     * @brief Arreglo de salidas iniciales.
     */
    Salida** salidas;

    /**
     * @brief Arreglo de compuertas iniciales.
     */
    Compuerta** compuertas;

    // =========================================================
    // CONSTRUCTORES Y DESTRUCTOR
    // =========================================================

    /**
     * @brief Constructor de la clase Parser.
     */
    Parser();

    /**
     * @brief Destructor de la clase Parser.
     * 
     * Libera la memoria asociada a los datos cargados.
     */
    ~Parser();

    // =========================================================
    // MÉTODOS PRINCIPALES
    // =========================================================

    /**
     * @brief Carga un archivo de configuración.
     * 
     * Tras una carga exitosa:
     * - staticData queda inicializado
     * - bloques queda poblado
     * - salidas queda poblado
     * - compuertas queda poblado
     * 
     * @param ruta Ruta del archivo de configuración.
     * 
     * @return true Si el archivo se cargó correctamente.
     * @return false Si ocurrió un error durante la carga.
     */
    bool cargar(const char* ruta);

private:

    // =========================================================
    // FUNCIONES AUXILIARES
    // =========================================================

    /**
     * @brief Verifica si una línea está vacía.
     * 
     * @param s Línea a verificar.
     * @return true Si la línea está vacía.
     * @return false En caso contrario.
     */
    static bool lineaVacia(const char* s);

    /**
     * @brief Busca el valor asociado a una clave.
     * 
     * @param linea Línea completa.
     * @param key Clave buscada.
     * @return const char* Puntero al valor encontrado.
     */
    static const char* buscarValor(
        const char* linea,
        const char* key
    );

    /**
     * @brief Lee un valor entero desde una cadena.
     * 
     * @param s Referencia al puntero de lectura.
     * @return int Valor leído.
     */
    static int leerInt(const char*& s);

    /**
     * @brief Lee un carácter desde una cadena.
     * 
     * @param s Referencia al puntero de lectura.
     * @return char Carácter leído.
     */
    static char leerChar(const char*& s);
};