all: interactive-shell

interactive-shell: main.o complex/Complex.o sort/Sort.o
	g++ main.o complex/Complex.o sort/Sort.o -o interactive-shell

main.o: main.cpp complex/Complex.h sort/Sort.h
	g++ -c main.cpp -o main.o

complex/Complex.o: complex/Complex.cpp complex/Complex.h
	g++ -c complex/Complex.cpp -o complex/Complex.o

sort/Sort.o: sort/Sort.cpp sort/Sort.h complex/Complex.h
	g++ -c sort/Sort.cpp -o sort/Sort.o

run: interactive-shell
	./interactive-shell

clean:
	rm -f *.o complex/*.o sort/*.o interactive-shell

.PHONY: all clean run
