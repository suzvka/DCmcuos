# Synthesizer Runtime

## Requirements

+ [qmsetup](https://github.com/stdware/qmsetup)
+ [syscmdline](https://github.com/SineStriker/syscmdline)
+ [etl](https://github.com/ETLCPP/etl)

## Setup Environment

### VCPKG Packages

#### Windows
```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat

vcpkg install --x-manifest-root=../scripts/vcpkg-manifest --x-install-root=./installed
```

#### Unix
```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

./vcpkg install \
    --x-manifest-root=../scripts/vcpkg-manifest \
    --x-install-root=./installed

### Build & Install

The buildsystem is able to deploy the shared libraries to build directory and install directory automatically.