#include <iostream>
#include "neural/Data_Handling/MnistLoader.h"

int main(int argc, char** argv) {
    std::string img = "data/train-images.idx3-ubyte";
    std::string lab = "data/train-labels.idx1-ubyte";
    if (argc > 2) { img = argv[1]; lab = argv[2]; }
    auto res = MnistLoader::LoadFromGzFiles(img, lab, 10);
    std::cout << "Loaded " << res.second << " MNIST datapoints\n";
    if (res.second > 0) std::cout << "First label: " << res.first[0]->get_label() << "\n";
    for (int i = 0; i < res.second; ++i) delete res.first[i];
    delete [] res.first;
    return 0;
}
