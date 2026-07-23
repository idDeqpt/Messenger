echo "mkdir build"
mkdir build

echo "cmake . -B build -DCMAKE_BUILD_TYPE=Release"
cmake . -B build -DCMAKE_BUILD_TYPE=Release
echo "cmake --build build"
cmake --build build