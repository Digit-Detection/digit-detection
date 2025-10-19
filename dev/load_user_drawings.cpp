#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "neural/Data_Handling/DataSet.h"
#include "neural/Data_Handling/DataPoint.h"

int main(int argc, char** argv) {
    std::string path = "user_drawings.bin";
    if (argc > 1) path = argv[1];

    auto res = DataSet::LoadDataPoints(path);
    DataPoint** arr = res.first;
    int count = res.second;

    std::cout << "Loaded " << count << " datapoints from '" << path << "'\n";
    if (count == 0) return 0;

    std::cout << "Labels: ";
    int to_show = std::min(count, 10);
    for (int i = 0; i < to_show; ++i) {
        std::cout << arr[i]->get_label() << (i+1<to_show?", ":"\n");
    }

    // Dump first datapoint to PGM if inputs_length == 28*28
    DataPoint* first = arr[0];
    int ins = first->get_inputs_length();
    double* inputs = first->get_inputs();
    if (ins == 28*28) {
        std::ofstream out("dev/first_dump.pgm", std::ios::binary);
        if (!out) {
            std::cerr << "Failed to open dev/first_dump.pgm for writing\n";
        } else {
            out << "P5\n28 28\n255\n";
            for (int i = 0; i < ins; ++i) {
                unsigned char v = (unsigned char)(std::max(0.0, std::min(1.0, inputs[i])) * 255.0);
                out.write((char*)&v, 1);
            }
            out.close();
            std::cout << "Wrote dev/first_dump.pgm\n";
        }
    } else {
        std::cout << "First datapoint has inputs_length=" << ins << ", not 784; skipping PGM dump.\n";
    }

    // Cleanup
    for (int i = 0; i < count; ++i) delete arr[i];
    delete [] arr;
    return 0;
}
