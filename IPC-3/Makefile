all: first second

shared_array.o: shared_array.cpp
	g++ -c shared_array.cpp -o shared_array.o -pthread

first.o: first.cpp
	g++ -c first.cpp -o first.o -pthread

second.o: second.cpp
	g++ -c second.cpp -o second.o -pthread

first: first.o shared_array.o
	g++ first.o shared_array.o -o first -pthread

second: second.o shared_array.o
	g++ second.o shared_array.o -o second -pthread

clean:
	rm -f *.o first second

.PHONY: all clean

