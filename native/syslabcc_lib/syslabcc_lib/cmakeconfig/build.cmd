@echo off
cmake -S . -B build -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Release "%*"  && cmake --build build