gcc -g -Wall -std=c99 -Iinclude -c -o main.o main.c
gcc -g -Wall -std=c99 -Iinclude -c -o utils.o utils.c
gcc main.o utils.o -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin\TicTacToe