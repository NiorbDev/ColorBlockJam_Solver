#include <iostream>
#include <string>
#include "Compuerta.h"

// Constructor de la clase Compuerta.
// Inicializa la compuerta y el contador utilizado
// para controlar los cambios dinámicos de color.
Compuerta::Compuerta(
    int id,
    char coloractual,
    StaticData* staticData
) {

    this->id = id;
    this->coloractual = coloractual;
    this->contadorpasos = 1;
    this->staticData = staticData;
}

// El cambio de color es cíclico:
// cuando alcanza el color final vuelve al inicial.
void Compuerta::actualizarCompuerta(
    char colorInicial,
    char colorFinal,
    int pasos
) {

    // Si la compuerta tiene color fijo,
    // no se actualiza.
    if (pasos == 0) {
        return;
    }

    // Cambia el color cuando se alcanza
    // la cantidad de pasos requerida.
    if (this->contadorpasos == pasos) {

        this->contadorpasos = 1;

        // Si alcanza el color final,
        // vuelve nuevamente al color inicial.
        if (this->coloractual == colorFinal) {

            this->coloractual = colorInicial;

        } else {

            // Si aún no llega al color final,
            // continúa avanzando en la secuencia.
            this->coloractual++;
        }

    } else {

        // Si aún no corresponde cambiar el color,
        // aumenta el contador de pasos.
        this->contadorpasos++;
    }
}