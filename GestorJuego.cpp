//iostream se usa para leer desde teclado y escribir mensajes en consola.
#include <iostream>

//cstring se usa para strlen y strncpy, ya que el proyecto trabaja con char[]
//en vez de std::string para mantenerse simple y evitar contenedores de STL.
#include <cstring>

//GestorJuego.h declara la clase que se implementa en este archivo.
#include "GestorJuego.h"

//Parser permite cargar un archivo de nivel y construir los objetos iniciales.
#include "Parser.h"

//Aestrella permite resolver el tablero cargado con el algoritmo A*.
#include "Aestrella.h"

GestorJuego::GestorJuego(){
    //Un char[] vacio se representa poniendo '\0' en la primera posicion.
    //Esto permite preguntar despues si hay archivo cargado revisando
    //nombreArchivo[0] == '\0'.
    this->nombreArchivo[0] = '\0';

    //staticData parte en nullptr porque al iniciar el programa todavia
    //no existe ningun archivo de nivel cargado.
    this->staticData = nullptr;

    //tableroInicial parte en nullptr por la misma razon: no se puede crear
    //un Tablero hasta que Parser lea un archivo valido.
    this->tableroInicial = nullptr;

    //ultimaSolucion parte en nullptr porque A* aun no ha sido ejecutado.
    //Este puntero solo apunta a un Camino despues de resolver con exito.
    this->ultimaSolucion = nullptr;

    //El tiempo de resolucion parte en 0 hasta que se ejecute A*.
    this->ultimoTiempoMs = 0;

    //salir controla el ciclo principal. Parte en false para que el menu
    //se muestre al menos una vez.
    this->salir = false;
}

GestorJuego::~GestorJuego(){
    //Al destruir el gestor, se libera cualquier recurso que siga cargado.
    //Esto evita fugas de memoria si el usuario sale despues de cargar
    //un nivel o despues de resolver una solucion.
    this->limpiarNivelActual();
}

void GestorJuego::ejecutar(){
    //Este while mantiene vivo el programa hasta que el usuario seleccione
    //la opcion 5. La variable salir cambia a true en procesarOpcion().
    while(!this->salir){
        //Primero se muestra el menu completo para que el usuario vea
        //las acciones disponibles.
        this->mostrarMenu();

        //Luego se lee la opcion y se ejecuta la accion correspondiente.
        this->procesarOpcion();
    }

    //Este mensaje se imprime una vez, cuando el ciclo termina.
    std::cout << "Saliendo..." << std::endl;
}

void GestorJuego::mostrarMenu(){
    //Linea en blanco para separar el menu de la salida anterior.
    std::cout << "" << std::endl;

    //Titulo principal del programa.
    std::cout << "  COLOR BLOCK JAM" << std::endl;

    //Opcion 1: carga el archivo de configuracion del tablero.
    std::cout << "1) Cargar archivo de configuracion" << std::endl;

    //Opcion 2: ejecuta A* usando el tablero cargado.
    std::cout << "2) Resolver el juego con A*" << std::endl;

    //Opcion 3: reproduce la ultima solucion calculada.
    std::cout << "3) Mostrar resultado (animar solucion paso a paso)" << std::endl;

    //Opcion 4: deja probar una secuencia escrita manualmente.
    std::cout << "4) Aplicar secuencia manual de movimientos" << std::endl;

    //Opcion 5: termina el ciclo principal.
    std::cout << "5) Salir" << std::endl;

    //Otra linea en blanco para ordenar visualmente el menu.
    std::cout << "" << std::endl;

    //Prompt final. No se usa endl para que el cursor quede en la misma linea.
    std::cout << "Opcion: ";
}

void GestorJuego::procesarOpcion(){
    //Variable donde se guarda el numero ingresado por el usuario.
    int opcion;

    //operator>> intenta leer un entero. Si el usuario escribe texto,
    //la lectura falla y cin queda en estado de error.
    if(!(std::cin >> opcion)){
        //clear() quita el estado de error para permitir nuevas lecturas.
        std::cin.clear();

        //Se descarta el resto de la linea incorrecta. Si no se hiciera,
        //el mismo texto invalido quedaria en el buffer y fallaria otra vez.
        char basura[256];
        std::cin.getline(basura, sizeof(basura));

        //Se informa el problema y se vuelve al menu sin cerrar el programa.
        std::cout << "Entrada invalida." << std::endl;
        return;
    }

    //Despues de leer con >> queda pendiente el salto de linea '\n'.
    //getline lo consume para que las lecturas posteriores de rutas o
    //secuencias no reciban una cadena vacia accidentalmente.
    char basura[256];
    std::cin.getline(basura, sizeof(basura));

    //El switch separa la logica de cada opcion del menu. Cada case llama
    //a un metodo especifico para mantener este metodo corto y claro.
    switch(opcion){
        case 1:
            //Carga un archivo nuevo y reemplaza cualquier nivel anterior.
            this->cargarArchivo();
            break;
        case 2:
            //Ejecuta A* sobre el tablero inicial cargado.
            this->resolverJuego();
            break;
        case 3:
            //Anima la ultima solucion guardada, si existe.
            this->mostrarResultado();
            break;
        case 4:
            //Lee una secuencia manual y la aplica sobre un tablero fresco.
            this->aplicarSecuenciaManual();
            break;
        case 5:
            //Marca la bandera de salida. El while de ejecutar() terminara.
            this->salir = true;
            break;
        default:
            //Cualquier numero fuera del rango permitido se rechaza.
            std::cout << "Opcion invalida, solo se permiten numeros del 1 al 5" << std::endl;
            break;
    }
}

void GestorJuego::cargarArchivo(){
    //Se pide la ruta o nombre del archivo de configuracion.
    //Puede ser algo como Ejemplo1.txt si esta en la misma carpeta.
    std::cout << "Ingrese el nombre del archivo: ";

    //Buffer local para guardar lo que escribe el usuario.
    char ruta[256];

    //getline permite leer rutas completas hasta el salto de linea.
    std::cin.getline(ruta, sizeof(ruta));

    //Si la ruta viene vacia, no hay nada que cargar.
    if(std::strlen(ruta) == 0){
        std::cout << "Archivo no encontrado" << std::endl;
        return;
    }

    //Se crea un Parser dinamicamente porque sus punteros internos seran
    //transferidos al Tablero y a GestorJuego si la carga resulta correcta.
    Parser* p = new Parser();

    //cargar() lee el archivo y crea StaticData, Bloque**, Salida** y
    //Compuerta**. Si falla, no se cambia el nivel actual.
    if(!p->cargar(ruta)){
        std::cout << "No se pudo cargar el archivo." << std::endl;

        //Como este Parser no se va a usar, se libera antes de salir.
        delete p;
        return;
    }

    //Si ya habia un nivel cargado, se libera antes de guardar el nuevo.
    //Esto evita mezclar estados de archivos distintos y evita fugas.
    this->limpiarNivelActual();

    //El StaticData creado por Parser pasa a ser administrado por GestorJuego.
    //Por eso se guarda el puntero en this->staticData.
    this->staticData = p->staticData;

    //Se crea el tablero inicial usando los datos parseados.
    //El Tablero recibe los arreglos de bloques, salidas y compuertas
    //que describen el estado inicial del juego.
    this->tableroInicial = new Tablero(
        p->staticData,
        p->bloques,
        p->salidas,
        p->compuertas
    );

    //Se copia la ruta a nombreArchivo para poder recargar el mismo nivel
    //mas tarde, por ejemplo al animar una solucion desde cero.
    std::strncpy(this->nombreArchivo, ruta, sizeof(this->nombreArchivo) - 1);

    //strncpy no siempre agrega '\0' si la cadena se corta, por eso se fuerza
    //el ultimo caracter a terminador para dejar el char[] seguro.
    this->nombreArchivo[sizeof(this->nombreArchivo) - 1] = '\0';

    //El Parser ya no es necesario. Su destructor no libera los punteros
    //transferidos, porque ahora los usa el tablero y el gestor.
    delete p;

    //Desde aqui solo se imprime un resumen del nivel cargado.
    std::cout << "\nArchivo cargado: " << this->nombreArchivo << std::endl;

    //Se muestran dimensiones WIDTH x HEIGHT almacenadas en StaticData.
    std::cout << "Dimensiones: " << this->staticData->anchoTablero
              << "x" << this->staticData->altoTablero << std::endl;

    //Se muestran cantidades principales para verificar que el parser
    //leyo correctamente bloques, salidas y compuertas.
    std::cout << "Bloques: " << this->staticData->cantidadBloques
              << ", Salidas: " << this->staticData->cantidadSalidas
              << ", Compuertas: " << this->staticData->cantidadCompuertas << std::endl;

    //El limite de movimientos es importante porque A* poda estados que
    //superan este valor.
    std::cout << "Limite de movimientos: "
              << this->staticData->limiteMovimientos << std::endl;

    //Se imprime el tablero para que el usuario vea el estado inicial.
    std::cout << "\nEstado inicial:" << std::endl;
    this->tableroInicial->imprimir();
}

void GestorJuego::resolverJuego(){
    //No se puede resolver si aun no existe tablero inicial.
    //Esto ocurre cuando el usuario intenta usar opcion 2 antes de opcion 1.
    if(this->tableroInicial == nullptr){
        std::cout << "Primero carga un archivo desde la opcion 1" << std::endl;
        return;
    }

    //Cada ejecucion de A* invalida la solucion anterior.
    //Se elimina para que ultimaSolucion apunte solo al resultado mas reciente.
    this->limpiarUltimaSolucion();

    //Mensaje visible antes de iniciar una busqueda que podria tardar.
    std::cout << "\nResolviendo..." << std::endl;

    //Se crea el objeto A* usando el tablero inicial.
    //Aestrella no es dueño de tableroInicial: solo lo usa como base para clonar.
    Aestrella* astar = new Aestrella(this->tableroInicial);

    //resolver() ejecuta la busqueda. Devuelve un Camino si encuentra solucion
    //o nullptr si no encuentra una solucion dentro de las reglas del nivel.
    Camino* sol = astar->resolver();

    //Se guarda el tiempo informado por A* para mostrarlo en el resumen.
    this->ultimoTiempoMs = astar->tiempoMs();

    //Se copian las estadisticas antes de borrar astar, porque pertenecen
    //al objeto Aestrella que se destruira a continuacion.
    int explorados = astar->estadosExplorados;
    int generados = astar->estadosGenerados;

    //A* ya termino, asi que se libera su objeto auxiliar.
    //El Camino retornado no se borra aqui; se maneja aparte.
    delete astar;

    //Si sol es nullptr, la busqueda fallo o determino que no hay solucion.
    if(sol == nullptr){
        //Se imprime tiempo aun cuando no hay solucion, como pide el enunciado.
        std::cout << "Tiempo resolucion: " << this->ultimoTiempoMs
                  << " [mseg]" << std::endl;

        //Estados que efectivamente salieron del OpenSet y fueron procesados.
        std::cout << "Estados explorados: " << explorados << std::endl;

        //Estados vecinos que se alcanzaron a generar durante la busqueda.
        std::cout << "Estados generados: " << generados << std::endl;

        //Mensaje simple para el usuario.
        std::cout << "No tiene solucion" << std::endl;
        return;
    }

    //Si existe solucion, se guarda como ultimaSolucion para poder usar
    //la opcion 3 y reproducirla luego.
    this->ultimaSolucion = sol;

    //Resumen final de una busqueda exitosa.
    std::cout << "\nResumen" << std::endl;

    //Tiempo total medido por A*.
    std::cout << "Tiempo resolucion: " << this->ultimoTiempoMs
              << " [mseg]" << std::endl;

    //Cantidad de estados explorados durante A*.
    std::cout << "Estados explorados: " << explorados << std::endl;

    //Cantidad de estados generados durante A*.
    std::cout << "Estados generados: " << generados << std::endl;

    //sol->cantidad es la cantidad de movimientos compactos en el camino.
    std::cout << "Solucion encontrada en " << sol->cantidad << " pasos." << std::endl;

    //Camino::imprimir() muestra la secuencia en formato R1,2D3,1...
    std::cout << "Pasos: ";
    sol->imprimir();

    //Se recuerda al usuario que la animacion se ejecuta aparte para no
    //mezclar busqueda y visualizacion.
    std::cout << "Usa la opcion 3 para ver la animacion." << std::endl;
}

void GestorJuego::mostrarResultado(){
    //No hay nada que animar si A* aun no ha guardado una solucion.
    if(this->ultimaSolucion == nullptr){
        std::cout << "No hay solucion guardada. Usa la opcion 2 primero." << std::endl;
        return;
    }

    //Tambien se necesita saber desde que archivo recargar el tablero.
    if(this->nombreArchivo[0] == '\0'){
        std::cout << "No hay archivo cargado." << std::endl;
        return;
    }

    //sdFresco recibira el StaticData del tablero recargado.
    //Se mantiene separado de this->staticData porque es una copia nueva
    //usada solo para la animacion.
    StaticData* sdFresco = nullptr;

    //Se recarga el tablero desde disco para partir desde el estado inicial.
    //Esto es necesario porque aplicar movimientos modifica el tablero.
    Tablero* fresco = GestorJuego::recargarTableroDesdeArchivo(
        this->nombreArchivo,
        sdFresco
    );

    //Si la recarga falla, no se puede animar sin arriesgar usar punteros nulos.
    if(fresco == nullptr){
        std::cout << "Error: no se pudo recargar el archivo para animar." << std::endl;
        return;
    }

    //Se informa el inicio de la reproduccion.
    std::cout << "\nReproduciendo solucion paso a paso" << std::endl;

    //aplicar() recorre los movimientos del Camino y los ejecuta sobre fresco.
    //El segundo parametro en true indica que debe mostrar el estado inicial.
    this->ultimaSolucion->aplicar(fresco, true);

    //El tablero temporal ya no se necesita despues de animar.
    delete fresco;

    //El StaticData temporal de la recarga tambien se libera.
    GestorJuego::liberarStaticData(sdFresco);
}

void GestorJuego::aplicarSecuenciaManual(){
    //La secuencia manual necesita un nivel cargado para saber que tablero usar.
    if(this->tableroInicial == nullptr){
        std::cout << "Primero carga un archivo desde la opcion 1" << std::endl;
        return;
    }

    //Se pide la secuencia en el formato solicitado por el enunciado.
    std::cout << "Ingresar Secuencia: (ej: R2,2D2,1R1,5): ";

    //Buffer amplio para leer toda la secuencia del usuario.
    char linea[1024];

    //getline lee la secuencia completa hasta Enter.
    std::cin.getline(linea, sizeof(linea));

    //Si la linea esta vacia, no se crea Camino ni se toca el tablero.
    if(std::strlen(linea) == 0){
        std::cout << "Secuencia vacia, no se hace nada" << std::endl;
        return;
    }

    //Se crea un Camino vacio que luego intentara parsear la cadena.
    Camino* manual = new Camino();

    //parsearDesdeString valida el formato y llena el arreglo de movimientos.
    //Si falla, se libera manual y se vuelve al menu.
    if(!manual->parsearDesdeString(linea)){
        std::cout << "Formato invalido" << std::endl;
        delete manual;
        return;
    }

    //Igual que en mostrarResultado(), se usa un tablero fresco para no
    //modificar tableroInicial.
    StaticData* sdFresco = nullptr;

    //Se reconstruye el nivel desde el archivo original.
    Tablero* fresco = GestorJuego::recargarTableroDesdeArchivo(
        this->nombreArchivo,
        sdFresco
    );

    //Si no se pudo recargar, se libera el Camino manual antes de salir.
    if(fresco == nullptr){
        std::cout << "Error: no se pudo recargar el archivo." << std::endl;
        delete manual;
        return;
    }

    //Se informa que comienza la aplicacion de la secuencia.
    std::cout << "\nAplicando secuencia" << std::endl;

    //aplicar() ejecuta cada movimiento sobre el tablero fresco y lo muestra.
    manual->aplicar(fresco, true);

    //Se libera el Camino creado solo para esta operacion manual.
    delete manual;

    //Se libera el Tablero temporal usado para la animacion.
    delete fresco;

    //Se libera el StaticData temporal asociado al Tablero recargado.
    GestorJuego::liberarStaticData(sdFresco);
}

void GestorJuego::limpiarUltimaSolucion(){
    //Si no hay solucion guardada, no hay nada que borrar.
    if(this->ultimaSolucion != nullptr){
        //ultimaSolucion fue creada con new dentro de Aestrella::resolver(),
        //por eso debe destruirse con delete.
        delete this->ultimaSolucion;

        //Se deja en nullptr para evitar punteros colgantes y dobles delete.
        this->ultimaSolucion = nullptr;
    }
}

void GestorJuego::limpiarNivelActual(){
    //Primero se borra la solucion porque depende del nivel cargado.
    //Si se carga otro nivel, esa solucion ya no corresponde.
    this->limpiarUltimaSolucion();

    //Luego se borra el tablero inicial si existe.
    if(this->tableroInicial != nullptr){
        delete this->tableroInicial;
        this->tableroInicial = nullptr;
    }

    //Finalmente se borra StaticData, que contiene arreglos dinamicos propios.
    if(this->staticData != nullptr){
        GestorJuego::liberarStaticData(this->staticData);
        this->staticData = nullptr;
    }

    //Se borra el nombre del archivo para marcar que ya no hay nivel cargado.
    this->nombreArchivo[0] = '\0';

    //Se reinicia el tiempo porque ya no corresponde a ningun nivel activo.
    this->ultimoTiempoMs = 0;
}

void GestorJuego::liberarStaticData(StaticData* sd){
    //Si el puntero es nullptr, no se debe hacer nada.
    //Esto permite llamar esta funcion sin revisar antes en todos lados.
    if(sd == nullptr) return;

    //paredesTablero es una matriz dinamica char**.
    //Primero se libera cada fila y despues el arreglo de punteros.
    if(sd->paredesTablero != nullptr){
        //El bucle recorre todas las filas del tablero.
        //i va desde 0 hasta altoTablero - 1 porque hay una fila por cada y.
        for(int i = 0; i < sd->altoTablero; i++){
            //Cada fila fue creada con new[], por eso se libera con delete[].
            delete[] sd->paredesTablero[i];
        }

        //Despues de liberar todas las filas, se libera el arreglo de filas.
        delete[] sd->paredesTablero;
    }

    //Arreglos asociados a propiedades de bloques indexadas por id interno.
    delete[] sd->coloresBloques;
    delete[] sd->anchoBloques;
    delete[] sd->altoBloques;

    //geometriaBloques tambien es una estructura de dos niveles:
    //un arreglo de punteros, donde cada puntero apunta a la geometria
    //plana de un bloque.
    if(sd->geometriaBloques != nullptr){
        //El bucle recorre todos los espacios posibles del arreglo de bloques.
        //Se usa tamArrayBloques y no cantidadBloques porque los IDs del archivo
        //pueden no ser consecutivos, entonces el arreglo puede tener huecos.
        for(int i = 0; i < sd->tamArrayBloques; i++){
            //Cada geometria individual fue creada con new[].
            //delete[] sobre nullptr es seguro, asi que no hace falta otro if.
            delete[] sd->geometriaBloques[i];
        }

        //Luego se libera el arreglo que contenia los punteros a geometria.
        delete[] sd->geometriaBloques;
    }

    //Arreglos asociados a las salidas del tablero.
    //Cada posicion i describe una salida: coordenada, color, orientacion,
    //largo inicial, largo final y pasos de cambio.
    delete[] sd->xSalidas;
    delete[] sd->ySalidas;
    delete[] sd->coloresSalidas;
    delete[] sd->orientacionSalidas;
    delete[] sd->largoISalidas;
    delete[] sd->largoFSalidas;
    delete[] sd->pasosSalidas;

    //Arreglos asociados a las compuertas del tablero.
    //Cada posicion i describe una compuerta: coordenada, largo, orientacion,
    //color inicial, color final y pasos de cambio.
    delete[] sd->xCompuertas;
    delete[] sd->yCompuertas;
    delete[] sd->largoCompuertas;
    delete[] sd->orientacionCompuertas;
    delete[] sd->colorICompuertas;
    delete[] sd->colorFCompuertas;
    delete[] sd->pasosCompuertas;

    //Despues de liberar todos sus arreglos internos, se destruye el objeto.
    delete sd;
}

Tablero* GestorJuego::recargarTableroDesdeArchivo(
    const char* ruta,
    StaticData*& sdSalida
){
    //Se crea un Parser nuevo porque se quiere reconstruir el nivel desde cero.
    //Esto entrega un tablero sin movimientos aplicados.
    Parser* p = new Parser();

    //Si el archivo no carga, se libera el Parser y se informa el error
    //devolviendo nullptr.
    if(!p->cargar(ruta)){
        delete p;

        //sdSalida se deja en nullptr para que el llamador sepa que no hay
        //StaticData temporal que liberar.
        sdSalida = nullptr;
        return nullptr;
    }

    //Si la carga fue correcta, se construye un Tablero con los datos nuevos.
    //Este Tablero es independiente del tableroInicial guardado en GestorJuego.
    Tablero* t = new Tablero(
        p->staticData,
        p->bloques,
        p->salidas,
        p->compuertas
    );

    //El StaticData del Parser se entrega al llamador mediante referencia.
    //El llamador debera liberarlo con liberarStaticData().
    sdSalida = p->staticData;

    //El Parser ya no se necesita. No destruye los datos transferidos.
    delete p;

    //Se devuelve el tablero fresco para animaciones o secuencias manuales.
    return t;
}
