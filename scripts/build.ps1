cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON . -B target 
cp target/compile_commands.json . 
cmake --build target 
