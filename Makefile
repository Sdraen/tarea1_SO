# Compilador y banderas de compilación
COMPILADOR = g++
OPCIONES = -std=c++11 -pthread

# Objetivo principal
todo: carrera

# Enlaza el ejecutable
carrera: carrera.o
	$(COMPILADOR) $(OPCIONES) -o carrera carrera.o

# Compilación del archivo de objetos
carrera.o: carrera.cpp
	$(COMPILADOR) $(OPCIONES) -c carrera.cpp

# Limpia los archivos generados
limpiar:
	rm -f carrera.o carrera