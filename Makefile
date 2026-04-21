tetris: src/draw.o src/game.o src/piece.o main.o
	gcc $^ -o $@ -lncurses

src/%.o: src/%.c include/%.h
	gcc -c -fPIC $< -o $@ -I./include

main.o: main.c
	gcc -c -fPIC $< -o $@ -I./include

clean: 
	rm tetris *.o 
