# Create Build Dir
mkdir -p build
cd build

# Cmake with debug and G++
cmake -DCMAKE_BUILD_TYPE=DEBUG \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    ..
make