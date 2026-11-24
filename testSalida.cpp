#include <iostream>
#include "Salida.h"
#include "StaticData.h"

static int passed = 0;
static int failed = 0;

static void check(const char* nombre, bool ok){
    if(ok){ std::cout << "  PASS: " << nombre << std::endl; passed++; }
    else   { std::cout << "  FAIL: " << nombre << std::endl; failed++; }
}

static StaticData* hacerSD(int id, int li, int lf, int pasos){
    StaticData* sd = new StaticData();
    sd->cantidadSalidas  = id + 1;
    sd->largoISalidas    = new int[id + 1];
    sd->largoFSalidas    = new int[id + 1];
    sd->pasosSalidas     = new int[id + 1];
    sd->xSalidas         = new int[id + 1];
    sd->ySalidas         = new int[id + 1];
    sd->coloresSalidas   = new char[id + 1];
    sd->orientacionSalidas = new char[id + 1];
    sd->largoISalidas[id]  = li;
    sd->largoFSalidas[id]  = lf;
    sd->pasosSalidas[id]   = pasos;
    return sd;
}

static void liberarSD(StaticData* sd){
    delete[] sd->largoISalidas;
    delete[] sd->largoFSalidas;
    delete[] sd->pasosSalidas;
    delete[] sd->xSalidas;
    delete[] sd->ySalidas;
    delete[] sd->coloresSalidas;
    delete[] sd->orientacionSalidas;
    delete sd;
}

int main(){
    std::cout << "=== Test Salida ===" << std::endl;

    // ------------------------------------------------------------------
    // TEST 1: constructor inicializa correctamente (salida creciente)
    // ------------------------------------------------------------------
    std::cout << "\n[1] Constructor salida creciente (li=2, lf=4)..." << std::endl;
    StaticData* sd1 = hacerSD(0, 2, 4, 2);
    Salida* s1 = new Salida(0, 2, sd1);
    check("id correcto",          s1->id == 0);
    check("la inicial == li",     s1->la == 2);
    check("contadorpasos == 1",   s1->contadorpasos == 1);
    check("direccion == 1 (crece)", s1->direccion == 1);
    delete s1;
    liberarSD(sd1);

    // ------------------------------------------------------------------
    // TEST 2: constructor inicializa correctamente (salida decreciente)
    // ------------------------------------------------------------------
    std::cout << "\n[2] Constructor salida decreciente (li=4, lf=2)..." << std::endl;
    StaticData* sd2 = hacerSD(0, 4, 2, 2);
    Salida* s2 = new Salida(0, 4, sd2);
    check("direccion == -1 (achica)", s2->direccion == -1);
    check("la inicial == 4",          s2->la == 4);
    delete s2;
    liberarSD(sd2);

    // ------------------------------------------------------------------
    // TEST 3: salida fija (li == lf) nunca cambia
    // ------------------------------------------------------------------
    std::cout << "\n[3] Salida de tamano fijo (li==lf==3)..." << std::endl;
    StaticData* sd3 = hacerSD(0, 3, 3, 1);
    Salida* s3 = new Salida(0, 3, sd3);
    s3->actualizarSalida(3, 3, 1);
    s3->actualizarSalida(3, 3, 1);
    check("la sigue siendo 3 tras actualizar", s3->la == 3);
    delete s3;
    liberarSD(sd3);

    // ------------------------------------------------------------------
    // TEST 4: salida con pasos==0 nunca cambia
    // ------------------------------------------------------------------
    std::cout << "\n[4] Salida con pasos==0 nunca cambia..." << std::endl;
    StaticData* sd4 = hacerSD(0, 2, 4, 0);
    Salida* s4 = new Salida(0, 2, sd4);
    s4->actualizarSalida(2, 4, 0);
    s4->actualizarSalida(2, 4, 0);
    check("la sigue siendo 2 con pasos=0", s4->la == 2);
    delete s4;
    liberarSD(sd4);

    // ------------------------------------------------------------------
    // TEST 5: ciclo completo de rebote (li=2, lf=4, pasos=1)
    // cada actualizarSalida dispara el cambio porque pasos=1
    // 2 -> 3 -> 4 -> 3 -> 2 -> 3 -> 4
    // ------------------------------------------------------------------
    std::cout << "\n[5] Ciclo de rebote (li=2, lf=4, pasos=1)..." << std::endl;
    StaticData* sd5 = hacerSD(0, 2, 4, 1);
    Salida* s5 = new Salida(0, 2, sd5);
    s5->actualizarSalida(2, 4, 1); check("paso1: la==3", s5->la == 3);
    s5->actualizarSalida(2, 4, 1); check("paso2: la==4", s5->la == 4);
    s5->actualizarSalida(2, 4, 1); check("paso3: la==3", s5->la == 3);
    s5->actualizarSalida(2, 4, 1); check("paso4: la==2", s5->la == 2);
    s5->actualizarSalida(2, 4, 1); check("paso5: la==3 (rebote)", s5->la == 3);
    delete s5;
    liberarSD(sd5);

    // ------------------------------------------------------------------
    // TEST 6: pasos=2 - solo cambia cada 2 actualizaciones
    // ------------------------------------------------------------------
    std::cout << "\n[6] Cadencia pasos=2 (li=1, lf=3)..." << std::endl;
    StaticData* sd6 = hacerSD(0, 1, 3, 2);
    Salida* s6 = new Salida(0, 1, sd6);
    s6->actualizarSalida(1, 3, 2); check("actualizacion1: la==1 (sin cambio)", s6->la == 1);
    s6->actualizarSalida(1, 3, 2); check("actualizacion2: la==2 (cambio)",     s6->la == 2);
    s6->actualizarSalida(1, 3, 2); check("actualizacion3: la==2 (sin cambio)", s6->la == 2);
    s6->actualizarSalida(1, 3, 2); check("actualizacion4: la==3 (cambio)",     s6->la == 3);
    delete s6;
    liberarSD(sd6);

    std::cout << "\n=== Fin testSalida: " << passed << " PASS, " << failed << " FAIL ===" << std::endl;
    return (failed > 0) ? 1 : 0;
}
