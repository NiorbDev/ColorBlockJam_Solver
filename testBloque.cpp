#include <iostream>
#include "Bloque.h"

static int passed = 0;
static int failed = 0;

static void check(const char* nombre, bool ok){
    if(ok){ std::cout << "  PASS: " << nombre << std::endl; passed++; }
    else   { std::cout << "  FAIL: " << nombre << std::endl; failed++; }
}

int main(){
    std::cout << "=== Test Bloque ===" << std::endl;

    // ------------------------------------------------------------------
    // TEST 1: constructor guarda id y posicion correctamente
    // ------------------------------------------------------------------
    std::cout << "\n[1] Constructor..." << std::endl;
    Bloque* b = new Bloque(5, 3, 7, nullptr);
    check("id correcto",     b->id == 5);
    check("x correcto",     b->x  == 3);
    check("y correcto",     b->y  == 7);
    check("staticData nulo", b->staticData == nullptr);

    // ------------------------------------------------------------------
    // TEST 2: moverArriba decrementa y
    // ------------------------------------------------------------------
    std::cout << "\n[2] moverArriba..." << std::endl;
    b->moverArriba();
    check("y disminuye en 1 (3->7-1=6)", b->y == 6);
    b->moverArriba();
    b->moverArriba();
    check("y disminuye 3 veces (7-3=4)", b->y == 4);
    check("x no cambia tras moverArriba", b->x == 3);

    // ------------------------------------------------------------------
    // TEST 3: moverAbajo incrementa y
    // ------------------------------------------------------------------
    std::cout << "\n[3] moverAbajo..." << std::endl;
    b->moverAbajo();
    check("y aumenta en 1", b->y == 5);
    b->moverAbajo();
    check("y aumenta en 1 otra vez", b->y == 6);
    check("x no cambia tras moverAbajo", b->x == 3);

    // ------------------------------------------------------------------
    // TEST 4: moverIzquierda decrementa x
    // ------------------------------------------------------------------
    std::cout << "\n[4] moverIzquierda..." << std::endl;
    b->moverIzquierda();
    check("x disminuye en 1 (3->2)", b->x == 2);
    b->moverIzquierda();
    b->moverIzquierda();
    check("x disminuye 3 veces (3-3=0)", b->x == 0);
    check("y no cambia tras moverIzquierda", b->y == 6);

    // ------------------------------------------------------------------
    // TEST 5: moverDerecha incrementa x
    // ------------------------------------------------------------------
    std::cout << "\n[5] moverDerecha..." << std::endl;
    b->moverDerecha();
    check("x aumenta en 1 (0->1)", b->x == 1);
    b->moverDerecha();
    b->moverDerecha();
    check("x aumenta 3 veces (0+3=3)", b->x == 3);
    check("y no cambia tras moverDerecha", b->y == 6);

    // ------------------------------------------------------------------
    // TEST 6: movimientos se acumulan correctamente
    // ------------------------------------------------------------------
    std::cout << "\n[6] Secuencia combinada..." << std::endl;
    Bloque* b2 = new Bloque(0, 0, 0, nullptr);
    b2->moverDerecha(); b2->moverDerecha(); b2->moverDerecha(); //x=3
    b2->moverAbajo();   b2->moverAbajo();                       //y=2
    b2->moverIzquierda();                                       //x=2
    b2->moverArriba();                                          //y=1
    check("posicion final x=2", b2->x == 2);
    check("posicion final y=1", b2->y == 1);

    delete b;
    delete b2;

    std::cout << "\n=== Fin testBloque: " << passed << " PASS, " << failed << " FAIL ===" << std::endl;
    return (failed > 0) ? 1 : 0;
}
