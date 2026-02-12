mkdir build
cd build

cmake ..
cmake --build .

cd ..
copy "deps\libs\SQLite3\sqlite3.dll" "bin\Debug\"

pause