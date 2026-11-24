#include <iostream>
#include "Parser.h"
#include "Tablero.h"

//auxiliar para borrar el StaticData
void liberarStaticData(StaticData* sd){
    if(sd->paredesTablero){
        for(int i = 0; i < sd->altoTablero; i++) delete[] sd->paredesTablero[i];
        delete[] sd->paredesTablero;
    }
    delete[] sd->coloresBloques;
    delete[] sd->anchoBloques;
    delete[] sd->altoBloques;
    if(sd->geometriaBloques){
        for(int i = 0; i < sd->tamArrayBloques; i++) delete[] sd->geometriaBloques[i];
        delete[] sd->geometriaBloques;
    }
    delete[] sd->xSalidas;
    delete[] sd->ySalidas;
    delete[] sd->coloresSalidas;
    delete[] sd->orientacionSalidas;
    delete[] sd->largoISalidas;
    delete[] sd->largoFSalidas;
    delete[] sd->pasosSalidas;
    delete[] sd->xCompuertas;
    delete[] sd->yCompuertas;
    delete[] sd->colorICompuertas;
    delete[] sd->colorFCompuertas;
    delete[] sd->pasosCompuertas;
    delete sd;
}

//leer mapa.txt
void probar(const char* ruta){
    std::cout << "Cargando: " << ruta << std::endl;

    //crear parser y verificar si existe la ruta
    Parser* parser = new Parser();
    if(!parser->cargar(ruta)){
        std::cout<<"Fallo al cargar" << std::endl;
        delete parser;
        return;
    }

    
    StaticData* sd = parser->staticData;
    std::cout<<"Dimensiones: "<<sd->anchoTablero<<"x"<<sd->altoTablero<<std::endl;
    std::cout<<"Limite movimientos: "<<sd->limiteMovimientos<<std::endl;
    std::cout<<"Bloques: " << sd->cantidadBloques<<" (tamArray="<<sd->tamArrayBloques<<")"<<std::endl;
    int bIdx = 0;
    for(int i = 0; i < sd->tamArrayBloques; i++){
        if(!sd->geometriaBloques || !sd->geometriaBloques[i]) continue; //hueco (ID no existe)
        std::cout<<"  bloque id="<<(i+1)
                  <<" color="<<sd->coloresBloques[i]
                  <<" "<<sd->anchoBloques[i]<<"x"<<sd->altoBloques[i]
                  <<" en ("<<parser->bloques[bIdx]->x<<","<<parser->bloques[bIdx]->y<<")"
                  <<" geom: ";
        for(int k = 0; k < sd->anchoBloques[i] * sd->altoBloques[i]; k++){
            std::cout << (int)sd->geometriaBloques[i][k] << " ";
        }
        std::cout << std::endl;
        bIdx++;
    }
    std::cout << "Salidas: " << sd->cantidadSalidas << std::endl;
    for(int i = 0; i < sd->cantidadSalidas; i++){
        std::cout << "  salida " << i
                  << " color=" << sd->coloresSalidas[i]
                  << " en (" << sd->xSalidas[i] << "," << sd->ySalidas[i] << ")"
                  << " orient=" << sd->orientacionSalidas[i]
                  << " li=" << sd->largoISalidas[i]
                  << " lf=" << sd->largoFSalidas[i]
                  << " step=" << sd->pasosSalidas[i] << std::endl;
    }
    std::cout << "Compuertas: " << sd->cantidadCompuertas << std::endl;
    for(int i = 0; i < sd->cantidadCompuertas; i++){
        std::cout << "  compuerta " << i
                  << " en (" << sd->xCompuertas[i] << "," << sd->yCompuertas[i] << ")"
                  << " ci=" << sd->colorICompuertas[i]
                  << " cf=" << sd->colorFCompuertas[i]
                  << " step=" << sd->pasosCompuertas[i] << std::endl;
    }

    //ahora construimos el Tablero y lo imprimimos
    Tablero* tablero = new Tablero(sd, parser->bloques, parser->salidas, parser->compuertas);
    std::cout << "\nEstado inicial visual:" << std::endl;
    tablero->imprimir();

    delete tablero;
    liberarStaticData(sd);
    delete parser;
    std::cout << std::endl;
}

int main(int argc, char** argv){
    if(argc >= 2){
        //modo: probar el archivo pasado como argumento
        probar(argv[1]);
    } else {
        std::cout<<"Para probar el test el mapa debe estár en la misma carpeta que este test."<<std::endl;
        std::cout<<"Además ejecutar el archivo con ./testParser archivo.txt"<<std::endl;
    }
    return 0;
}