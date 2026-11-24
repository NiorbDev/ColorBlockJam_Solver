#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstring>
#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif
#include "Aestrella.h"

bool Aestrella::prepararConsola(){
#ifdef _WIN32
    //Windows 11 soporta ANSI, pero en algunas consolas hay que activarlo.
    HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    if(salida == INVALID_HANDLE_VALUE) return false;

    DWORD modo = 0;
    if(!GetConsoleMode(salida, &modo)) return false;

    modo |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if(!SetConsoleMode(salida, modo)) return false;

    return true;
#else
    //En Ubuntu/Linux las terminales habituales ya interpretan ANSI.
    return true;
#endif
}

void Aestrella::limpiarLineaActual(){
    if(prepararConsola()){
        //\r vuelve al inicio y \033[2K borra toda la linea actual.
        std::cout << "\r\033[2K";
    } else {
        //Respaldo para consolas sin ANSI: borrar visualmente con espacios.
        std::cout << "\r";
        for(int i = 0; i < 140; i++) std::cout << ' ';
        std::cout << "\r";
    }
}

void Aestrella::ocultarCursor(){
    if(prepararConsola()){
        std::cout << "\033[?25l";
    }
}

void Aestrella::mostrarCursor(){
    if(prepararConsola()){
        std::cout << "\033[?25h";
    }
}

size_t Aestrella::memoriaActualMB(){
// definicion de la funcion en windows
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))){
        return pmc.WorkingSetSize / (1024 * 1024);
    }
    return 0;
#else
// definicion de la funcion en linux
    FILE* f = fopen("/proc/self/status", "r");
    if(!f) return 0;
    char line[128];
    size_t vmrss = 0;
    while(fgets(line, sizeof(line), f)){
        if(strncmp(line, "VmRSS:", 6) == 0){
            sscanf(line + 6, "%zu", &vmrss);
            break;
        }
    }
    fclose(f);
    return vmrss / 1024;
#endif
}

// imprime el estado actual del algoritmo en la consola, sobreescribiendo la misma línea,
// lo hace con formato fijo para que no se mueva al actualizar
void Aestrella::imprimirEstado(int explorados, int generados, int bloquesRestantes, double ms){
    limpiarLineaActual();
    std::cout << "  Explorados: " << explorados
              << " | Generados: " << generados
              << " | Bloques: " << bloquesRestantes
              << " | Memoria: " << memoriaActualMB() << " MB"
              << " | " << (int)ms << "ms   "
              << std::flush;
}


Aestrella::Aestrella(Tablero* tableroInicial){
    // inicializar atributos
    this->inicial = tableroInicial;
    this->ultimoTiempo = 0;
    this->estadosExplorados = 0;
    this->estadosGenerados = 0;
}

Aestrella::~Aestrella(){
    //no liberamos 'inicial': no somos dueños
}

// retorna el tiempo de la última ejecución
int Aestrella::tiempoMs(){
    return this->ultimoTiempo;
}

//el algoritmo A* en si
Camino* Aestrella::resolver(){
    //iniciar el cronometro
    clock_t inicioTiempo = clock();//guardar el tiempo en el que inició

    //datos para comparar en el resultado y ver la eficiencia
    this->estadosExplorados = 0;
    this->estadosGenerados = 0;

    //OpenSet y ClosedSet
    MinHeap* openSet = new MinHeap(256);//arreglo de prioridad
    HashSet* closedSet = new HashSet(1021);//hash con un primo grande

    //clonamos el tablero inicial para que el OpenSet/ClosedSet tengan su propio tablero inicial
    //el tablero inicial recibido se hizo en otro lado, no soy dueño de el
    Tablero* raiz = this->inicial->clonar();
    raiz->padre = nullptr; //la raiz no tiene padre en el arbol de busqueda
    int h0 = raiz->heuristica();//calcular su heuristica para la prioridad inicial
    openSet->push(raiz, 0 + h0);//f = g + h, g=0 en la raiz

    //obtener el limite de movimientos inicial
    int stepLimit = this->inicial->staticData->limiteMovimientos;

    //si se necesitan más pasos que el limite del mapa, se da por imposible y finaliza
    int cantMinMovimientos = raiz->contarMovimientosNecesarios();
    if(cantMinMovimientos > stepLimit){
        delete raiz;
        delete openSet;
        delete closedSet;
        clock_t finTiempo = clock();
        this->ultimoTiempo = (int)((double)(finTiempo - inicioTiempo) * 1000.0 / CLOCKS_PER_SEC);
        return nullptr; //sin solucion
    }

    // inicializamos las varibles de la solucion
    Camino* resultado = nullptr;
    Tablero* tableroSolucion = nullptr;

    int bloquesRestantes = this->inicial->cantidadBloques;

    ocultarCursor(); // ocultar cursor durante el progreso
    imprimirEstado(0, 0, bloquesRestantes, 0.0);

    // flujo principal del algoritmo A*:
    while(!openSet->vacio()){
        // tomamos el tablero con menor f = g + h
        Tablero* actual = openSet->pop();

        // si ya lo exploramos, lo descartamos
        if(closedSet->contiene(actual)){
            delete actual;
            continue;
        }
        // sumamos a estados explorados (después de verificar que no es repetido)
        this->estadosExplorados++;

        bloquesRestantes = actual->cantidadBloques;

        // cada 500 estados explorados, imprimimos el estado actual del algoritmo
        // para hacer un seguimiento del progreso, incluyendo el tiempo transcurrido 
        // hasta ahora
        if(this->estadosExplorados % 500 == 0){
            double msActual = (double)(clock() - inicioTiempo) * 1000.0 / CLOCKS_PER_SEC;
            imprimirEstado(this->estadosExplorados, this->estadosGenerados, bloquesRestantes, msActual);
        }

        // si es solución, terminamos la búsqueda
        if(actual->esSolucion()){
            tableroSolucion = actual;
            break;
        }

        // si el tablero actual ya superó el límite de movimientos, lo descartamos (poda)
        if(actual->contadorMovimientos >= stepLimit){
            closedSet->insertar(actual);
            actual->liberarCuadricula();
            continue;
        }

        // generar vecinos (estados alcanzables con un movimiento válido)
        int numeroVecinos;
        Tablero** vecinos = actual->generarVecinos(numeroVecinos);
        this->estadosGenerados += numeroVecinos;

        closedSet->insertar(actual);
        actual->liberarCuadricula();

        // procesar cada vecino generado
        for(int i = 0; i < numeroVecinos; i++){
            Tablero* v = vecinos[i];
            if(closedSet->contiene(v)){
                delete v;
                continue;
            }
            // calcular la prioridad f = g + h de cada vecino 
            int g = v->contadorMovimientos;
            int h = v->heuristica();
            // lo agregamos al openset con su prioridad
            openSet->push(v, g + h);
        }
        delete[] vecinos;
    }

    // imprimir estado final y restaurar cursor
    {
        double msActual = (double)(clock() - inicioTiempo) * 1000.0 / CLOCKS_PER_SEC;
        imprimirEstado(this->estadosExplorados, this->estadosGenerados, bloquesRestantes, msActual);
    }
    mostrarCursor();
    std::cout << std::endl;

    //si se encontro solucion, reconstruir el camino
    if(tableroSolucion != nullptr){
        resultado = reconstruirCamino(tableroSolucion);
    }

    //LIMPIEZA: vaciar lo que queda en OpenSet (tableros encolados no explorados)
    while(!openSet->vacio()){
        Tablero* t = openSet->pop();
        //puede que ya este en closedSet (duplicado); si no, hay que liberarlo.
        //ya que el closed set se encargará de liberarlo después
        if(!closedSet->contiene(t)){
            delete t;
        }
    }
    
    delete openSet;

    //LIMPIEZA: ya se reconstruyó el camino así que no necesitamos los
    //estados usados, borramos la solucion (si es que hay)
    //y borramos todos los tableros de closedSet
    if(tableroSolucion != nullptr){
        delete tableroSolucion;
    }
    delete closedSet;


    clock_t finTiempo = clock();//guardar el tiempo en donde termina
    double ms = (double)(finTiempo - inicioTiempo) * 1000.0 / CLOCKS_PER_SEC;//calcular diferencia de tiempo
    this->ultimoTiempo = (int)ms;//guardar el resultado

    return resultado;//retornar el camino
}

//Reconstruye el camino solucion siguiendo padres desde 'solucion' hasta el inicial
//Los movimientos se van guardando en orden inverso, y al final se dan vuelta
//para que queden del primero al ultimo
Camino* Aestrella::reconstruirCamino(Tablero* solucion){
    //contar cuantos movimientos hay en la cadena
    int n = 0;
    Tablero* tablero = solucion;
    // recorremos hacia atras hasta que no hayan estados padres, 
    // contando cuantos movimientos hay
    while(tablero->padre != nullptr){
        n++;
        tablero = tablero->padre;
    }

    //crear camino con capacidad suficiente
    Camino* camino;
    if(n == 0){
        //caso borde: solucion ya es el inicial, no hay movimientos
        camino = new Camino(1);
    } else {
        camino = new Camino(n);//camino con n movimientos
    }

    //recolectar movimientos en orden inverso
    Movimiento* inverso = new Movimiento[n];
    int idx = 0;
    tablero = solucion;  //reiniciar el tablero para este otro while
    while(tablero->padre != nullptr){
        inverso[idx] = tablero->movimientoOrigen;//guardar el movimiento
        idx++;
        tablero = tablero->padre;//retroceder al tablero anterior
    }

    //pasar al camino en orden directo
    for(int i = n - 1; i >= 0; i--){
        camino->agregar(inverso[i]);
    }
    delete[] inverso;//borrar el buffer temporal
    return camino;
}
