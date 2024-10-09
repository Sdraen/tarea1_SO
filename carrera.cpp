
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <random>
#include <iomanip>

std::mutex mtx;  // Mutex para manejar la sincronización en la terminal
bool race_over = false;
std::vector<std::pair<int, int>> results;  // Para almacenar el lugar de llegada y el ID del auto

void autos_coloreados(int car_id, int distance, bool finished) {
    if (finished) {
        std::cout << "\033[1;32mAuto" << car_id << " avanza " << distance 
                  << " metros y termina la carrera en el lugar " << results.size() << "!" << "\033[0m" << std::endl;
    } else {
        std::cout << "Auto" << car_id << " avanza " << distance 
                  << " metros (total: " << results[car_id].second << " metros)" << std::endl;
    }
}

void carrera(int car_id, int total_distance) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distance_dist(1, 10);  // Avanza entre 1 y 10 metros
    std::uniform_int_distribution<> time_dist(100, 500);   // Espera entre 100 y 500 ms

    int current_distance = 0;

    while (current_distance < total_distance) {
        int distance = distance_dist(gen);
        int wait_time = time_dist(gen);

        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));

        current_distance += distance;

        std::lock_guard<std::mutex> lock(mtx);
        if (current_distance >= total_distance) {
            current_distance = total_distance;
            if (!race_over) {
                race_over = true;
            }
            results.push_back({(int)results.size() + 1, car_id});
            autos_coloreados(car_id, distance, true);
        } else {
            std::cout << "Auto" << car_id << " avanza " << distance 
                      << " metros (total: " << current_distance << " metros)" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia total> <numero de autos>" << std::endl;
        return 1;
    }

    int total_distance = std::stoi(argv[1]);
    int num_cars = std::stoi(argv[2]);

    std::vector<std::thread> cars;
    for (int i = 0; i < num_cars; ++i) {
        cars.push_back(std::thread(carrera, i, total_distance));
    }

    for (auto& car : cars) {
        car.join();
    }

    std::cout << "\nResultados finales:\n";
    std::cout << "Lugar\tAuto\n";
    for (const auto& result : results) {
        std::cout << result.first << "\tAuto" << result.second << std::endl;
    }

    std::cout << "Carrera finalizada!" << std::endl;
    return 0;
}