#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <random>

// Control de acceso a la salida por consola
std::mutex consola_mutex;

// Lista para almacenar la posición de llegada de los autos
std::vector<std::pair<int, int>> resultados_carrera;

/**
 * @brief Muestra el progreso de un auto y si ha completado la carrera.
 * 
 * @param id El identificador del auto.
 * @param avance Cuántos metros avanzó en este turno.
 * @param total_avanzado Cuántos metros ha recorrido en total.
 * @param finalizado Indica si el auto terminó la carrera.
 */
void actualizar_progreso(int id, int avance, int total_avanzado, bool finalizado) {
    std::lock_guard<std::mutex> bloqueo(consola_mutex);  // Sincronización para evitar que los hilos interfieran
    if (finalizado) {
        std::cout << "\033[1;32mAuto" << id << " avanzó " << avance 
                  << " metros y llegó en la posición " << resultados_carrera.size() << "!\033[0m" << std::endl;
    } else {
        std::cout << "Auto" << id << " avanzó " << avance 
                  << " metros (acumulado: " << total_avanzado << " metros)" << std::endl;
    }
}

/**
 * @brief Función que simula el comportamiento de un auto en la carrera.
 * 
 * @param id_auto Identificador del auto.
 * @param meta La distancia total que debe recorrer.
 */
void simular_auto(int id_auto, int meta) {
    std::random_device rd;
    std::mt19937 generador(rd());
    std::uniform_int_distribution<> avance_dist(1, 10);  // Avanza entre 1 y 10 metros
    std::uniform_int_distribution<> tiempo_dist(100, 500);  // Tiempo de espera entre avances (100 a 500 ms)

    int acumulado = 0;

    // Mientras no haya llegado a la meta
    while (acumulado < meta) {
        int metros_avanzados = avance_dist(generador);
        int tiempo_espera = tiempo_dist(generador);

        std::this_thread::sleep_for(std::chrono::milliseconds(tiempo_espera));  // Simulación de tiempo de avance

        acumulado += metros_avanzados;

        // Si alcanza o supera la meta
        if (acumulado >= meta) {
            acumulado = meta;  // Ajuste por si pasa de la meta
            {
                std::lock_guard<std::mutex> bloqueo(consola_mutex);
                resultados_carrera.push_back({(int)resultados_carrera.size() + 1, id_auto});
            }
            actualizar_progreso(id_auto, metros_avanzados, acumulado, true);
        } else {
            actualizar_progreso(id_auto, metros_avanzados, acumulado, false);
        }
    }
}

/**
 * @brief Función principal para organizar y ejecutar la carrera entre varios autos.
 * 
 * @param argc Número de argumentos proporcionados.
 * @param argv Argumentos pasados desde la línea de comandos.
 * @return int Estado de finalización del programa.
 */
int main(int argc, char* argv[]) {
    // Validación de argumentos
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia total> <cantidad de autos>" << std::endl;
        return 1;
    }

    // Configuración de la carrera
    int distancia_total = std::stoi(argv[1]);
    int cantidad_autos = std::stoi(argv[2]);

    std::cout << "Carrera de " << distancia_total << " metros" << std::endl;
    std::cout << "-------------------------------" << std::endl;

    // Creación de hilos para cada auto
    std::vector<std::thread> hilos;
    for (int i = 0; i < cantidad_autos; ++i) {
        hilos.push_back(std::thread(simular_auto, i, distancia_total));
    }

    // Espera a que todos los autos terminen
    for (auto& hilo : hilos) {
        hilo.join();
    }

    // Imprimir tabla final de posiciones
    std::cout << "\nPosición\tAuto" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    for (const auto& resultado : resultados_carrera) {
        std::cout << resultado.first << "\t\tAuto" << resultado.second << std::endl;
    }

    return 0;
}