#include <iostream>
#include <fstream>
#include <cstring>
#include "Parser.h"

Parser::Parser(){
    this->staticData = nullptr;
    this->bloques = nullptr;
    this->salidas = nullptr;
    this->compuertas = nullptr;
}

Parser::~Parser(){
    //el Parser NO libera staticData/bloques/salidas/compuertas
    //esos punteros se entregan al Tablero, que es su nuevo dueño.
}


bool Parser::lineaVacia(const char* s){
    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] != ' ' && s[i] != '\t' && s[i] != '\r' && s[i] != '\n') return false;
    }
    return true;
}

const char* Parser::buscarValor(const char* linea, const char* key){
    int klen = std::strlen(key);
    int llen = std::strlen(linea);
    for(int i = 0; i + klen <= llen; i++){
        if(i > 0){
            char prev = linea[i - 1];
            bool esAlfa = (prev >= 'A' && prev <= 'Z') || (prev >= 'a' && prev <= 'z')
                       || (prev >= '0' && prev <= '9') || prev == '_';
            if(esAlfa) continue;
        }
        if(std::strncmp(linea + i, key, klen) != 0) continue;
        int j = i + klen;
        while(j < llen && (linea[j] == ' ' || linea[j] == '\t')) j++;
        if(j >= llen || linea[j] != '=') continue;
        j++;
        while(j < llen && (linea[j] == ' ' || linea[j] == '\t')) j++;
        return linea + j;
    }
    return nullptr;
}

int Parser::leerInt(const char*& s){
    while(*s == ' ' || *s == '\t') s++;
    int signo = 1;
    if(*s == '-'){ signo = -1; s++; }
    int v = 0;
    while(*s >= '0' && *s <= '9'){
        v = v * 10 + (*s - '0');
        s++;
    }
    return signo * v;
}

char Parser::leerChar(const char*& s){
    while(*s == ' ' || *s == '\t') s++;
    char c = *s;
    if(c != '\0') s++;
    return c;
}

//comenzar la carga de datos
bool Parser::cargar(const char* ruta){
    std::ifstream f(ruta);
    if(!f.is_open()){
        std::cout << "Error: no se pudo abrir " << ruta << std::endl;
        return false;
    }

    //creamos el StaticData y vamos rellenándolo
    this->staticData = new StaticData();
    StaticData* sd = this->staticData;

    //buffer para línea actual
    char linea[1024];

    //pasada 1: contar bloques/salidas/compuertas y hallar el ID maximo de bloque
    int countBloques = 0, countSalidas = 0, countCompuertas = 0;
    int maxIdArchivo = 0; //ID mas alto visto en [BLOCK] (1-indexed en el archivo)
    int wallStart = -1, wallEnd = -1;
    int seccion = 0;
    int numLinea = 0;
    while(f.getline(linea, sizeof(linea))){
        numLinea++;
        if(linea[0] == '['){
            if(std::strncmp(linea, "[META]", 6) == 0) seccion = 0;
            else if(std::strncmp(linea, "[BLOCK]", 7) == 0) seccion = 1;
            else if(std::strncmp(linea, "[WALL]", 6) == 0) seccion = 2;
            else if(std::strncmp(linea, "[EXIT]", 6) == 0) seccion = 3;
            else if(std::strncmp(linea, "[GATE]", 6) == 0) seccion = 4;
            continue;
        }
        if(lineaVacia(linea)) continue;
        if(seccion == 1){
            countBloques++;
            //leer el ID del archivo para saber el maximo
            const char* p = linea;
            int idLeido = leerInt(p);
            if(idLeido > maxIdArchivo) maxIdArchivo = idLeido;
        }
        else if(seccion == 2){
            if(wallStart == -1) wallStart = numLinea;
            wallEnd = numLinea;
        }
        else if(seccion == 3){ countSalidas++; }
        else if(seccion == 4){ countCompuertas++; }
    }

    //los arrays de StaticData se indexan por ID interno (idArchivo-1).
    //el tamaño necesario es maxIdArchivo (IDs van de 1..maxId → indices 0..maxId-1).
    int tamArrayBloques = (maxIdArchivo > 0) ? maxIdArchivo : countBloques;

    sd->cantidadBloques = countBloques;
    sd->tamArrayBloques = tamArrayBloques;
    sd->cantidadSalidas = countSalidas;
    sd->cantidadCompuertas = countCompuertas;

    if(tamArrayBloques > 0){
        sd->coloresBloques = new char[tamArrayBloques];
        sd->anchoBloques = new int[tamArrayBloques];
        sd->altoBloques = new int[tamArrayBloques];
        sd->geometriaBloques = new uint8_t*[tamArrayBloques];
        for(int i = 0; i < tamArrayBloques; i++) sd->geometriaBloques[i] = nullptr;
    }
    if(countSalidas > 0){
        sd->xSalidas = new int[countSalidas];
        sd->ySalidas = new int[countSalidas];
        sd->coloresSalidas = new char[countSalidas];
        sd->orientacionSalidas = new char[countSalidas];
        sd->largoISalidas = new int[countSalidas];
        sd->largoFSalidas = new int[countSalidas];
        sd->pasosSalidas = new int[countSalidas];
    }
    if(countCompuertas > 0){
        sd->xCompuertas = new int[countCompuertas];
        sd->yCompuertas = new int[countCompuertas];
        sd->largoCompuertas = new int[countCompuertas];
        sd->orientacionCompuertas = new char[countCompuertas];
        sd->colorICompuertas = new char[countCompuertas];
        sd->colorFCompuertas = new char[countCompuertas];
        sd->pasosCompuertas = new int[countCompuertas];
    }

    //arreglos para datos dinamicos iniciales (bloques en orden de aparicion)
    Bloque** bloques = (countBloques > 0) ? new Bloque*[countBloques] : nullptr;
    int* initX  = (countBloques > 0) ? new int[countBloques] : nullptr;
    int* initY  = (countBloques > 0) ? new int[countBloques] : nullptr;
    int* initID = (countBloques > 0) ? new int[countBloques] : nullptr;
    Salida** salidas = (countSalidas > 0) ? new Salida*[countSalidas] : nullptr;
    Compuerta** compuertas = (countCompuertas > 0) ? new Compuerta*[countCompuertas] : nullptr;

    //pasada 2: rellenar todo
    f.clear();
    f.seekg(0);
    seccion = 0;
    int idxBloque = 0, idxSalida = 0, idxCompuerta = 0;
    int wallRow = 0;

    while(f.getline(linea, sizeof(linea))){
        if(linea[0] == '['){
            if(std::strncmp(linea, "[META]", 6) == 0) seccion = 0;
            else if(std::strncmp(linea, "[BLOCK]", 7) == 0) seccion = 1;
            else if(std::strncmp(linea, "[WALL]", 6) == 0){
                seccion = 2;
                sd->paredesTablero = new char*[sd->altoTablero];
                for(int i = 0; i < sd->altoTablero; i++){
                    sd->paredesTablero[i] = new char[sd->anchoTablero];
                    for(int j = 0; j < sd->anchoTablero; j++){
                        sd->paredesTablero[i][j] = ' ';
                    }
                }
                wallRow = 0;
            }
            else if(std::strncmp(linea, "[EXIT]", 6) == 0) seccion = 3;
            else if(std::strncmp(linea, "[GATE]", 6) == 0) seccion = 4;
            continue;
        }

        if(seccion == 2){
            if(wallRow >= sd->altoTablero) continue;
            for(int j = 0; j < sd->anchoTablero; j++){
                char c = linea[j];
                if(c == '\0' || c == '\n' || c == '\r') c = ' ';
                sd->paredesTablero[wallRow][j] = c;
            }
            wallRow++;
            continue;
        }

        if(lineaVacia(linea)) continue;

        if(seccion == 0){
            const char* v;
            if((v = buscarValor(linea, "WIDTH"))) sd->anchoTablero = leerInt(v);
            else if((v = buscarValor(linea, "HEIGHT"))) sd->altoTablero = leerInt(v);
            else if((v = buscarValor(linea, "STEP_LIMIT"))) sd->limiteMovimientos = leerInt(v);
            continue;
        }

        if(seccion == 1){
            //leer el ID del archivo (1-indexed) y convertir a 0-indexed interno
            const char* p = linea;
            int idArchivo = leerInt(p);
            int idInterno = idArchivo - 1; //indice en los arrays de StaticData

            const char* v;
            char color = '?';
            int w = 0, h = 0, ix = 0, iy = 0;
            if((v = buscarValor(linea, "COLOR"))) color = leerChar(v);
            if((v = buscarValor(linea, "WIDTH"))) w = leerInt(v);
            if((v = buscarValor(linea, "HEIGHT"))) h = leerInt(v);
            if((v = buscarValor(linea, "INIT_X"))) ix = leerInt(v);
            if((v = buscarValor(linea, "INIT_Y"))) iy = leerInt(v);

            sd->coloresBloques[idInterno] = color;
            sd->anchoBloques[idInterno] = w;
            sd->altoBloques[idInterno] = h;

            uint8_t* geom = new uint8_t[w * h];
            for(int k = 0; k < w * h; k++) geom[k] = 0;
            if((v = buscarValor(linea, "GEOMETRY"))){
                for(int k = 0; k < w * h; k++){
                    int g = leerInt(v);
                    geom[k] = (uint8_t)(g != 0 ? 1 : 0);
                }
            }
            sd->geometriaBloques[idInterno] = geom;

            initX[idxBloque]  = ix;
            initY[idxBloque]  = iy;
            initID[idxBloque] = idInterno;
            idxBloque++;
            continue;
        }

        if(seccion == 3){
            //EXIT: COLOR=c X=x Y=y ORIENTATION=H|V LI=li LF=lf STEP=p
            const char* v;
            char color = '?', orient = 'H';
            int x = 0, y = 0, li = 0, lf = 0, step = 0;
            if((v = buscarValor(linea, "COLOR"))) color = leerChar(v);
            if((v = buscarValor(linea, "X"))) x = leerInt(v);
            if((v = buscarValor(linea, "Y"))) y = leerInt(v);
            if((v = buscarValor(linea, "ORIENTATION"))) orient = leerChar(v);
            if((v = buscarValor(linea, "LI"))) li = leerInt(v);
            if((v = buscarValor(linea, "LF"))) lf = leerInt(v);
            if((v = buscarValor(linea, "STEP"))) step = leerInt(v);

            sd->xSalidas[idxSalida] = x;
            sd->ySalidas[idxSalida] = y;
            sd->coloresSalidas[idxSalida] = color;
            sd->orientacionSalidas[idxSalida] = orient;
            sd->largoISalidas[idxSalida] = li;
            sd->largoFSalidas[idxSalida] = lf;
            sd->pasosSalidas[idxSalida] = step;
            idxSalida++;
            continue;
        }

        if(seccion == 4){
            const char* v;
            char ci = '?', cf = '?', orient = 'V';
            int x = 0, y = 0, step = 0, li = 1;
            if((v = buscarValor(linea, "X"))) x = leerInt(v);
            if((v = buscarValor(linea, "Y"))) y = leerInt(v);
            if((v = buscarValor(linea, "ORIENTATION"))) orient = leerChar(v);
            if((v = buscarValor(linea, "LI"))) li = leerInt(v);
            if((v = buscarValor(linea, "CI"))) ci = leerChar(v);
            if((v = buscarValor(linea, "CF"))) cf = leerChar(v);
            if((v = buscarValor(linea, "STEP"))) step = leerInt(v);
            
            sd->xCompuertas[idxCompuerta] = x;
            sd->yCompuertas[idxCompuerta] = y;
            sd->largoCompuertas[idxCompuerta] = li;
            sd->orientacionCompuertas[idxCompuerta] = orient;
            sd->colorICompuertas[idxCompuerta] = ci;
            sd->colorFCompuertas[idxCompuerta] = cf;
            sd->pasosCompuertas[idxCompuerta] = step;
            idxCompuerta++;
            continue;
        }
    }

    f.close();

    //crear los Bloque/Salida/Compuerta dinámicos iniciales
    for(int i = 0; i < countBloques; i++){
        //el id del Bloque es el id 0-indexed del archivo (initID[i])
        bloques[i] = new Bloque((uint8_t)initID[i], initX[i], initY[i], sd);
    }
    for(int i = 0; i < countSalidas; i++){
        salidas[i] = new Salida(i, sd->largoISalidas[i], sd);
    }
    for(int i = 0; i < countCompuertas; i++){
        compuertas[i] = new Compuerta(i, sd->colorICompuertas[i], sd);
    }

    //limpiar buffers temporales
    if(initX)  delete[] initX;
    if(initY)  delete[] initY;
    if(initID) delete[] initID;

    //asignar al estado del Parser
    this->bloques = bloques;
    this->salidas = salidas;
    this->compuertas = compuertas;

    return true;
}