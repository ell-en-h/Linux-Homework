all: read-file

read-file: read-file.o
	g++ read-file.o -o read-file

read-file.o: read-file.cpp
	g++ -c read-file.cpp -o read-file.o

run: read-file
	./read-file input.txt

clean:
	rm -f *.o read-file

.PHONY: all clean run
