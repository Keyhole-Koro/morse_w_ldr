#DEFINE RISE (1)
#DEFINE FLAT (0)
#DEFINE FALL (-1)

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <chrono>

class Morse {
private:
    std::ifstream file;
    std::vector<int> data;
    std::vector<std::vector<int>> l_input_data;
    int count;
    std::array<int, 20> input_data;
    int index;

public:
    Morse(std::ifstream& inputFile) : file(std::move(inputFile)), count(0), index(0) {
        int value;
        while (file >> value) {
            data.push_back(value);
        }
    }

    void main() {
        auto start_time = std::chrono::steady_clock::now();
        int function1Interval = 20;
        int function2Interval = 1000;
        int function3Interval = 2000;

        auto function1PreviousMillis = std::chrono::steady_clock::now();
        auto function2PreviousMillis = std::chrono::steady_clock::now();
        auto function3PreviousMillis = std::chrono::steady_clock::now();

        int previous_value = 0;

        while (true) {
            // in milliseconds
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();

            if (elapsed_time - function1PreviousMillis.count() >= function1Interval) {
                function1PreviousMillis = std::chrono::steady_clock::now();

                int new_input_value = std::stoi(data[index]);

                if (previous_value == 0) {
                    previous_value = new_input_value;
                }

                int inclination = (new_input_value + previous_value) / 2;

                previous_value = new_input_value;

                if (index == 20) {
                    for (int i = 0; i < 20; i++) {
                        int start = std::max(0, i - 5);
                        std::vector<int> part_data(input_data.begin() + start, input_data.begin() + i);
                        int mean_value = std::accumulate(part_data.begin(), part_data.end(), 0) / part_data.size();
                        input_data[i] = mean_value;
                    }

                    l_input_data.push_back(std::vector<int>(input_data.begin(), input_data.end()));
                    index = 0;
                }

                input_data[index] = inclination;

                index++;
            }

            if (elapsed_time - function2PreviousMillis.count() >= function2Interval) {
                function2PreviousMillis = std::chrono::steady_clock::now();

                // Add your function2 logic here
            }

            if (elapsed_time - function3PreviousMillis.count() >= function3Interval) {
                function3PreviousMillis = std::chrono::steady_clock::now();

                // Add your function3 logic here
            }
        }
    }
};

int main() {
    std::ifstream inputFile("C:\\Users\\kiho\\OneDrive\\デスクトップ\\morse\\KihoData.txt");
    if (inputFile) {
        Morse morse(inputFile);
        morse.main();
    }
    else {
        std::cout << "Error opening file!" << std::endl;
    }
    return 0;
}

