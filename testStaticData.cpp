/*
 * testStaticData.cpp
 * Prueba unitaria para la clase StaticData.
 *
 * StaticData almacena todos los datos estaticos del tablero: dimensiones,
 * geometrias de bloques, configuracion de salidas y compuertas. Estos datos
 * son compartidos por todos los Tablero durante A* y nunca cambian.
 *
 * Lo que se prueba:
 * 1. Constructor: todos los punteros se inicializan en nullptr y contadores en 0.
 * 2. Asignacion manual de campos escalares (ancho, alto, limiteMovimientos).
 * 3. Asignacion y lectura de arreglos de bloques (colores, anchos, altos).
 * 4. Asignacion y lectura de arreglos de salidas.
 * 5. Asignacion y lectura de arreglos de compuertas.
 * 6. Destructor: libera toda la memoria sin crash (incluye paredesTablero y geometriaBloques).
 */

#include <iostream>
#include <cstdint>
#include "StaticData.h"

int main(){

    std::cout << "=== Test StaticData ===" << std::endl;

    // ------------------------------------------------------------------
    // TEST 1: constructor deja todo en nullptr/0
    // ------------------------------------------------------------------
    std::cout << "\n[1] Constructor inicializa correctamente..." << std::endl;

    StaticData* sd = new StaticData();

    // verificar valores escalares
    bool ok = (sd->anchoTablero == 0 &&
               sd->altoTablero  == 0 &&
               sd->limiteMovimientos == 0 &&
               sd->cantidadBloques == 0 &&
               sd->cantidadSalidas == 0 &&
               sd->cantidadCompuertas == 0);

    // verificar punteros nulos
    ok = ok && (sd->paredesTablero    == nullptr &&
                sd->coloresBloques    == nullptr &&
                sd->anchoBloques      == nullptr &&
                sd->altoBloques       == nullptr &&
                sd->geometriaBloques  == nullptr &&
                sd->xSalidas          == nullptr &&
                sd->ySalidas          == nullptr &&
                sd->coloresSalidas    == nullptr &&
                sd->xCompuertas       == nullptr &&
                sd->yCompuertas       == nullptr);

    std::cout << (ok ? "  PASS: todos los campos en 0/nullptr" : "  FAIL: algun campo no se inicializo") << std::endl;

    // ------------------------------------------------------------------
    // TEST 2: asignacion de campos escalares del tablero
    // ------------------------------------------------------------------
    std::cout << "\n[2] Asignacion de dimensiones y limite..." << std::endl;

    sd->anchoTablero      = 8;
    sd->altoTablero       = 8;
    sd->limiteMovimientos = 50;

    std::cout << "  anchoTablero      = " << sd->anchoTablero      << " (esperado: 8)"  << std::endl;
    std::cout << "  altoTablero       = " << sd->altoTablero       << " (esperado: 8)"  << std::endl;
    std::cout << "  limiteMovimientos = " << sd->limiteMovimientos  << " (esperado: 50)" << std::endl;

    bool ok2 = (sd->anchoTablero == 8 && sd->altoTablero == 8 && sd->limiteMovimientos == 50);
    std::cout << (ok2 ? "  PASS" : "  FAIL") << std::endl;

    // ------------------------------------------------------------------
    // TEST 3: arreglos de bloques (colores, anchos, altos)
    // ------------------------------------------------------------------
    std::cout << "\n[3] Arreglos de bloques..." << std::endl;

    int nb = 3;
    sd->cantidadBloques  = nb;
    sd->coloresBloques   = new char[nb];
    sd->anchoBloques     = new int[nb];
    sd->altoBloques      = new int[nb];

    // bloque 0: color 'a', 2x2
    sd->coloresBloques[0] = 'a'; sd->anchoBloques[0] = 2; sd->altoBloques[0] = 2;
    // bloque 1: color 'b', 1x3
    sd->coloresBloques[1] = 'b'; sd->anchoBloques[1] = 1; sd->altoBloques[1] = 3;
    // bloque 2: color 'c', 2x1
    sd->coloresBloques[2] = 'c'; sd->anchoBloques[2] = 2; sd->altoBloques[2] = 1;

    // geometrias: arreglo plano de ancho*alto booleans
    sd->geometriaBloques = new uint8_t*[nb];
    // bloque 0: 2x2 lleno
    sd->geometriaBloques[0] = new uint8_t[4]{1,1,1,1};
    // bloque 1: 1x3 lleno
    sd->geometriaBloques[1] = new uint8_t[3]{1,1,1};
    // bloque 2: 2x1 lleno
    sd->geometriaBloques[2] = new uint8_t[2]{1,1};

    std::cout << "  Bloque 0: color='" << sd->coloresBloques[0]
              << "' ancho=" << sd->anchoBloques[0]
              << " alto=" << sd->altoBloques[0]
              << " (esperado: a 2 2)" << std::endl;

    std::cout << "  Bloque 1: color='" << sd->coloresBloques[1]
              << "' ancho=" << sd->anchoBloques[1]
              << " alto=" << sd->altoBloques[1]
              << " (esperado: b 1 3)" << std::endl;

    std::cout << "  Bloque 2: color='" << sd->coloresBloques[2]
              << "' ancho=" << sd->anchoBloques[2]
              << " alto=" << sd->altoBloques[2]
              << " (esperado: c 2 1)" << std::endl;

    bool ok3 = (sd->coloresBloques[0] == 'a' && sd->anchoBloques[0] == 2 && sd->altoBloques[0] == 2 &&
                sd->coloresBloques[1] == 'b' && sd->anchoBloques[1] == 1 && sd->altoBloques[1] == 3 &&
                sd->coloresBloques[2] == 'c' && sd->anchoBloques[2] == 2 && sd->altoBloques[2] == 1);
    std::cout << (ok3 ? "  PASS" : "  FAIL") << std::endl;

    // ------------------------------------------------------------------
    // TEST 4: arreglos de salidas
    // ------------------------------------------------------------------
    std::cout << "\n[4] Arreglos de salidas..." << std::endl;

    int ns = 2;
    sd->cantidadSalidas      = ns;
    sd->xSalidas             = new int[ns];
    sd->ySalidas             = new int[ns];
    sd->coloresSalidas       = new char[ns];
    sd->orientacionSalidas   = new char[ns];
    sd->largoISalidas        = new int[ns];
    sd->largoFSalidas        = new int[ns];
    sd->pasosSalidas         = new int[ns];

    // salida 0: color 'a', horizontal, largo fijo 2
    sd->xSalidas[0] = 3; sd->ySalidas[0] = 0; sd->coloresSalidas[0] = 'a';
    sd->orientacionSalidas[0] = 'H'; sd->largoISalidas[0] = 2; sd->largoFSalidas[0] = 2; sd->pasosSalidas[0] = 0;

    // salida 1: color 'b', vertical, largo variable de 1 a 3 cada 2 pasos
    sd->xSalidas[1] = 7; sd->ySalidas[1] = 2; sd->coloresSalidas[1] = 'b';
    sd->orientacionSalidas[1] = 'V'; sd->largoISalidas[1] = 1; sd->largoFSalidas[1] = 3; sd->pasosSalidas[1] = 2;

    std::cout << "  Salida 0: x=" << sd->xSalidas[0] << " y=" << sd->ySalidas[0]
              << " color='" << sd->coloresSalidas[0] << "'"
              << " orient=" << sd->orientacionSalidas[0]
              << " li=" << sd->largoISalidas[0] << " lf=" << sd->largoFSalidas[0]
              << " paso=" << sd->pasosSalidas[0]
              << " (esperado: 3 0 a H 2 2 0)" << std::endl;

    std::cout << "  Salida 1: x=" << sd->xSalidas[1] << " y=" << sd->ySalidas[1]
              << " color='" << sd->coloresSalidas[1] << "'"
              << " orient=" << sd->orientacionSalidas[1]
              << " li=" << sd->largoISalidas[1] << " lf=" << sd->largoFSalidas[1]
              << " paso=" << sd->pasosSalidas[1]
              << " (esperado: 7 2 b V 1 3 2)" << std::endl;

    bool ok4 = (sd->xSalidas[0] == 3 && sd->coloresSalidas[0] == 'a' && sd->orientacionSalidas[0] == 'H' &&
                sd->xSalidas[1] == 7 && sd->coloresSalidas[1] == 'b' && sd->orientacionSalidas[1] == 'V');
    std::cout << (ok4 ? "  PASS" : "  FAIL") << std::endl;

    // ------------------------------------------------------------------
    // TEST 5: arreglos de compuertas
    // ------------------------------------------------------------------
    std::cout << "\n[5] Arreglos de compuertas..." << std::endl;

    int nc = 1;
    sd->cantidadCompuertas     = nc;
    sd->xCompuertas            = new int[nc];
    sd->yCompuertas            = new int[nc];
    sd->largoCompuertas        = new int[nc];
    sd->orientacionCompuertas  = new char[nc];
    sd->colorICompuertas       = new char[nc];
    sd->colorFCompuertas       = new char[nc];
    sd->pasosCompuertas        = new int[nc];

    // compuerta 0: vertical, cicla de 'a' a 'c' cada 3 pasos
    sd->xCompuertas[0] = 4; sd->yCompuertas[0] = 2; sd->largoCompuertas[0] = 2;
    sd->orientacionCompuertas[0] = 'V'; sd->colorICompuertas[0] = 'a';
    sd->colorFCompuertas[0] = 'c'; sd->pasosCompuertas[0] = 3;

    std::cout << "  Compuerta 0: x=" << sd->xCompuertas[0]
              << " y=" << sd->yCompuertas[0]
              << " largo=" << sd->largoCompuertas[0]
              << " orient=" << sd->orientacionCompuertas[0]
              << " ci='" << sd->colorICompuertas[0] << "'"
              << " cf='" << sd->colorFCompuertas[0] << "'"
              << " pasos=" << sd->pasosCompuertas[0]
              << " (esperado: 4 2 2 V a c 3)" << std::endl;

    bool ok5 = (sd->xCompuertas[0] == 4 && sd->largoCompuertas[0] == 2 &&
                sd->colorICompuertas[0] == 'a' && sd->colorFCompuertas[0] == 'c' &&
                sd->pasosCompuertas[0] == 3);
    std::cout << (ok5 ? "  PASS" : "  FAIL") << std::endl;

    // ------------------------------------------------------------------
    // TEST 6: destructor libera todo sin crash
    // ------------------------------------------------------------------
    std::cout << "\n[6] Destructor libera memoria sin crash..." << std::endl;

    // paredesTablero necesita estar inicializado para que el destructor lo libere
    sd->paredesTablero = new char*[sd->altoTablero];
    for(int i = 0; i < sd->altoTablero; i++){
        sd->paredesTablero[i] = new char[sd->anchoTablero];
        for(int j = 0; j < sd->anchoTablero; j++){
            sd->paredesTablero[i][j] = '#';
        }
    }

    delete sd; // si no crashea, el destructor funciona correctamente
    std::cout << "  PASS: delete completo sin crash" << std::endl;

    // ------------------------------------------------------------------
    // TEST 7: StaticData vacia se destruye sin crash (punteros en nullptr)
    // ------------------------------------------------------------------
    std::cout << "\n[7] Destructor con StaticData vacia (todo nullptr)..." << std::endl;
    StaticData* sdVacio = new StaticData();
    delete sdVacio;
    std::cout << "  PASS: delete con nullptr sin crash" << std::endl;

    // ------------------------------------------------------------------
    // resumen
    // ------------------------------------------------------------------
    std::cout << "\n=== Fin testStaticData ===" << std::endl;
    return 0;
}
