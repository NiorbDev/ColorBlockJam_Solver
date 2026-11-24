#include <string>
#include <iostream>
#include "Salida.h"

// Constructor de la clase Salida.
// Inicializa la salida y determina automáticamente
// si debe comenzar creciendo o achicando.
Salida::Salida(
    int id,
    int la,
    StaticData* staticData
) {

    // Inicializamos el ID y el tamaño actual
    this->id = id;
    this->la = la;

    // Si el largo inicial es mayor que el final,
    // la salida debe comenzar achicando.
    if (staticData->largoISalidas[id] >
        staticData->largoFSalidas[id]) {

        this->direccion = -1;

    } else {

        this->direccion = 1;
    }

    this->contadorpasos = 1;
    this->staticData = staticData;
}

// El cambio de tamaño funciona con efecto rebote:
// cuando alcanza el tamaño mínimo o máximo,
// cambia automáticamente de dirección.
void Salida::actualizarSalida(int li, int lf, int pasos) {

    // Si la salida tiene tamaño fijo,
    // no se actualiza.
    if (pasos == 0 || li == lf) {
        return;
    }

    // Cambia el tamaño cuando se alcanza
    // la cantidad de pasos requerida.
    if (this->contadorpasos == pasos) {

        this->contadorpasos = 1;

        // Crece o disminuye según la dirección actual
        if (this->direccion == 1) {
            this->la += 1;
        } else {
            this->la -= 1;
        }

        // Si alcanza el tamaño mínimo o máximo,
        // invierte la dirección.
        if (this->la == lf || this->la == li) {
            this->direccion = this->direccion * -1;
        }

    } else {

        // Si aún no corresponde cambiar el tamaño,
        // aumenta el contador de pasos.
        this->contadorpasos++;
    }
}