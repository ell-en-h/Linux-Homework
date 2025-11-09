all: interactive-shell

interactive-shell: main.o
	g++ main.o -o interactive-shell

main.o: main.cpp
	g++ -c main.cpp -o main.o

run: interactive-shell
	./interactive-shell

clean:
	rm -f *.o interactive-shell

.PHONY: all clean run
