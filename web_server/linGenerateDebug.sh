echo "mkdir build"
mkdir build

echo "cmake . -B build -DCMAKE_BUILD_TYPE=Debug"
cmake . -B build -DCMAKE_BUILD_TYPE=Debug
echo "cmake --build build"
cmake --build build