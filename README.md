#Integrantes Tarea 1 Sistemas operativos IECI
Andrés Torres Castro
Tomás Méndez Andrades

# Simulador de Carrera de Autos (Multihebras)

## Descripción
Este programa simula una carrera de autos utilizando programación multihebras en C++. Cada auto avanza una cantidad aleatoria de metros en intervalos de tiempo aleatorios. La carrera termina cuando todos los autos completan la distancia total.

## Uso
Para ejecutar el programa:

1. Compilar el programa:
   
   make
   

2. Ejecutar el programa especificando la distancia total de la carrera y el número de autos:
   
   ./carrera <distancia_total> <numero_de_autos>
   

   Ejemplo:
   
   ./carrera 100 3
   

## Requisitos
- Sistema operativo Linux.
- Compilador `g++` con soporte para `C++11` y `pthread`.
