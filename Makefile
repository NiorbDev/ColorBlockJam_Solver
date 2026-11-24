LDFLAGS = $(shell uname -s 2>/dev/null | grep -q MINGW && echo -lpsapi || echo)

all: testBloque testSalida testCompuerta testTablero testMovimiento testParser testMinHeap testHashSet testCamino testAestrella testStaticData testGestorJuego main

StaticData.o: StaticData.h StaticData.cpp
	g++ -c -g StaticData.cpp

Bloque.o: Bloque.h Bloque.cpp StaticData.o
	g++ -c -g Bloque.cpp
testBloque: testBloque.cpp Bloque.o
	g++ Bloque.o testBloque.cpp -o testBloque

Salida.o: Salida.h Salida.cpp StaticData.o
	g++ -c -g Salida.cpp
testSalida: testSalida.cpp Salida.o StaticData.o
	g++ StaticData.o Salida.o testSalida.cpp -o testSalida

Compuerta.o: Compuerta.h Compuerta.cpp StaticData.o
	g++ -c -g Compuerta.cpp
testCompuerta: testCompuerta.cpp Compuerta.o
	g++ Compuerta.o testCompuerta.cpp -o testCompuerta

Movimiento.o: Movimiento.h Movimiento.cpp
	g++ -c -g Movimiento.cpp
testMovimiento: testMovimiento.cpp Movimiento.o
	g++ Movimiento.o testMovimiento.cpp -o testMovimiento

Tablero.o: Tablero.h Tablero.cpp StaticData.o Salida.o Compuerta.o Bloque.o Movimiento.o
	g++ -c -g Tablero.cpp
testTablero: testTablero.cpp Tablero.o
	g++ Bloque.o Salida.o Compuerta.o StaticData.o Tablero.o Movimiento.o testTablero.cpp -o testTablero

Parser.o: Parser.h Parser.cpp StaticData.o Salida.o Compuerta.o Bloque.o Movimiento.o Tablero.o
	g++ -c -g Parser.cpp
testParser: testParser.cpp Parser.o StaticData.o Salida.o Compuerta.o Bloque.o Movimiento.o Tablero.o
	g++ Bloque.o Salida.o Compuerta.o StaticData.o Parser.o Movimiento.o Tablero.o testParser.cpp -o testParser

MinHeap.o: MinHeap.h MinHeap.cpp Tablero.o
	g++ -c -g MinHeap.cpp
testMinHeap: testMinHeap.cpp MinHeap.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ MinHeap.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testMinHeap.cpp -o testMinHeap

HashSet.o: HashSet.h HashSet.cpp Tablero.o
	g++ -c -g HashSet.cpp
testHashSet: testHashSet.cpp HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testHashSet.cpp -o testHashSet

Camino.o: Camino.h Camino.cpp Tablero.o Movimiento.o
	g++ -c -g Camino.cpp
testCamino: testCamino.cpp Camino.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ Camino.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testCamino.cpp -o testCamino

Aestrella.o: Aestrella.h Aestrella.cpp Tablero.o Camino.o MinHeap.o HashSet.o
	g++ -c -g Aestrella.cpp
testAestrella: testAestrella.cpp Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testAestrella.cpp -o testAestrella $(LDFLAGS)

testStaticData: testStaticData.cpp StaticData.o
	g++ StaticData.o testStaticData.cpp -o testStaticData

GestorJuego.o: GestorJuego.h GestorJuego.cpp Aestrella.o Parser.o Tablero.o Camino.o StaticData.o
	g++ -c -g GestorJuego.cpp

testGestorJuego: testGestorJuego.cpp GestorJuego.o Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ GestorJuego.o Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testGestorJuego.cpp -o testGestorJuego $(LDFLAGS)

main: main.cpp GestorJuego.o Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ GestorJuego.o Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o main.cpp -g -o main $(LDFLAGS)

clean:
	rm -f *.o main testBloque testSalida testCompuerta testTablero testMovimiento testParser testMinHeap testHashSet testCamino testAestrella testStaticData testGestorJuego testSalida_san
