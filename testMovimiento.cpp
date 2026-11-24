#include <iostream>
#include "Movimiento.h"

static int passed = 0;
static int failed = 0;

static void check(const char* nombre, bool ok){
    if(ok){ std::cout << "  PASS: " << nombre << std::endl; passed++; }
    else   { std::cout << "  FAIL: " << nombre << std::endl; failed++; }
}

int main(){
    std::cout << "=== Test Movimiento ===" << std::endl;

    // ------------------------------------------------------------------
    // TEST 1: constructor por defecto
    // ------------------------------------------------------------------
    std::cout << "\n[1] Constructor por defecto..." << std::endl;
    Movimiento m0;
    check("idBloque por defecto es 0",   m0.idBloque  == 0);
    check("direccion por defecto es ' '", m0.direccion == ' ');
    check("celdas por defecto es 0",     m0.celdas    == 0);

    // ------------------------------------------------------------------
    // TEST 2: constructor con valores
    // ------------------------------------------------------------------
    std::cout << "\n[2] Constructor con valores..." << std::endl;
    Movimiento m1(3, 'R', 5);
    check("idBloque == 3",   m1.idBloque  == 3);
    check("direccion == 'R'", m1.direccion == 'R');
    check("celdas == 5",     m1.celdas    == 5);

    // ------------------------------------------------------------------
    // TEST 3: todas las direcciones validas
    // ------------------------------------------------------------------
    std::cout << "\n[3] Direcciones U, D, L, R..." << std::endl;
    Movimiento mU(0, 'U', 1);
    Movimiento mD(0, 'D', 1);
    Movimiento mL(0, 'L', 1);
    Movimiento mR(0, 'R', 1);
    check("direccion U", mU.direccion == 'U');
    check("direccion D", mD.direccion == 'D');
    check("direccion L", mL.direccion == 'L');
    check("direccion R", mR.direccion == 'R');

    // ------------------------------------------------------------------
    // TEST 4: id maximo (uint8_t = 255)
    // ------------------------------------------------------------------
    std::cout << "\n[4] idBloque maximo (255)..." << std::endl;
    Movimiento mMax(255, 'U', 100);
    check("idBloque == 255", mMax.idBloque == 255);
    check("celdas == 100",   mMax.celdas   == 100);

    // ------------------------------------------------------------------
    // TEST 5: copia por valor
    // ------------------------------------------------------------------
    std::cout << "\n[5] Copia por valor..." << std::endl;
    Movimiento original(7, 'D', 3);
    Movimiento copia = original;
    check("copia idBloque == 7",   copia.idBloque  == 7);
    check("copia direccion == 'D'", copia.direccion == 'D');
    check("copia celdas == 3",     copia.celdas    == 3);
    //modificar copia no afecta al original
    copia.celdas = 99;
    check("original.celdas sigue siendo 3", original.celdas == 3);

    // ------------------------------------------------------------------
    // TEST 6: asignacion con =
    // ------------------------------------------------------------------
    std::cout << "\n[6] Asignacion con operador =..." << std::endl;
    Movimiento a(1, 'L', 2);
    Movimiento b;
    b = a;
    check("b.idBloque == 1",   b.idBloque  == 1);
    check("b.direccion == 'L'", b.direccion == 'L');
    check("b.celdas == 2",     b.celdas    == 2);

    std::cout << "\n=== Fin testMovimiento: " << passed << " PASS, " << failed << " FAIL ===" << std::endl;
    return (failed > 0) ? 1 : 0;
}
