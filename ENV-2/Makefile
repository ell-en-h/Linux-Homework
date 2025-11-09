all: env2

env2: main.o ComplexNumber.o Sort.o
	g++ main.o ComplexNumber.o Sort.o -o env2

main.o: src/main.cpp include/ComplexNumber.h include/Sort.h
	g++ -c src/main.cpp -Iinclude -o main.o

ComplexNumber.o: src/ComplexNumber.cpp include/ComplexNumber.h
	g++ -c src/ComplexNumber.cpp -Iinclude -o ComplexNumber.o

Sort.o: src/Sort.cpp include/Sort.h include/ComplexNumber.h
	g++ -c src/Sort.cpp -Iinclude -o Sort.o

run: env2
	./env2

clean:
	rm -f *.o env2

.PHONY: all clean run
