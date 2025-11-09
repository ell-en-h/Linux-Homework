all: array-summary

array-summary: main.o
	g++ main.o -o array-summary -pthread

main.o: main.cpp
	g++ -c main.cpp -o main.o -pthread

run: array-summary
	./array-summary 1000000 4

clean:
	rm -f *.o array-summary

.PHONY: all clean run

