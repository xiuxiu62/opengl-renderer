# cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release . -B target 
cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON . -B target 
cp target/compile_commands.json . 
