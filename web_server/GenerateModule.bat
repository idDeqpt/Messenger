mkdir build
cd build

cmake ..
cmake --build .

cd ..
copy "C:\Projects\C++\libraries\Network\bin\lib\Debug\network.dll" ".\bin\Debug\"
xcopy /s ".\src\resources\" ".\bin\Debug\resources\" /Y /e /i
