#include <iostream>
#include "GestorJuego.h"

int main(){
    //Prueba basica de construccion y destruccion.
    //No se llama a ejecutar() porque ese metodo abre el menu interactivo.
    GestorJuego gestor;

    std::cout << "testGestorJuego OK" << std::endl;
    return 0;
}
