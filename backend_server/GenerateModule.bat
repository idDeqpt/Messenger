mkdir build
cd build

cmake ..
cmake --build .

cd ..
copy "C:\Projects\C++\libraries\Network\bin\lib\Debug\network.dll" ".\bin\Debug\"
copy "C:\Projects\C++\libraries\JSTypes\bin\lib\Debug\jstypes.dll" ".\bin\Debug\"
copy "C:\Projects\C++\libraries\SQLite3\sqlite3.dll" ".\bin\Debug\"
xcopy /s ".\src\resources\" ".\bin\Debug\resources\" /Y /e /i
