# Digit Detection

A C++ application for recognizing handwritten digits with a neural network using wxWidgets for GUI rendering.

## Requirements
- Debian-based system (Recommended Ubuntu 24.04 or later)
- CMake (Recommended V3.28.3)
- C++ Compiler (Recommended C++17)
  
## Install wxWidgets Locally (OPTIONAL)
Method 1: Install directly:
```bash
sudo apt install libwxgtk3.2-dev
```
Method 2: [wxWidgets Installation Guide](https://github.com/marcus-ng379/digit-detection/blob/main/docs/wxWidgetsInstallation.md)

## Installation
1. Make sure you've updated your system
```bash
sudo apt update
sudo apt upgrade
```

2. Install dependecies
```bash
sudo apt install cmake # Install CMakeLists
sudo apt install build-essential # Installs Make, G++, GCC
sudo apt install libgtk-3-dev # CMake pkg for GUIs
```

3. Clone the project using
```bash
git clone https://github.com/marcus-ng379/digit-detection.git
cd digit-detection
```

4. Build the project from one of the options:
<details>
<summary><strong>OPTION 1: wxWidgets is installed locally</strong></summary>

If you already installed wxWidgets locally, you can simply compile the program with:

    mkdir build
    cd build
    cmake -DUSE_SYSTEM_WXWIDGETS=ON ..
    cmake --build .

</details>

<details>
<summary><strong>OPTION 2: wxWidgets NOT installed locally</strong></summary>

Use the bundled submodule to build wxWidgets from source (note this may take a few minutes to fully build):

    git submodule update --init --recursive
    mkdir build
    cd build
    cmake -DUSE_SYSTEM_WXWIDGETS=OFF ..
    cmake --build .

</details>


5. Run the program inside *build* folder
```bash
./Digit_Detection
```
