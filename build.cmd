gcc -fdiagnostics-color=always -g -o knight.exe main.c -s -static -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm && knight.exe
