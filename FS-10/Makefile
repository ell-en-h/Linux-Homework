all: copy

copy: copy.o
	g++ copy.o -o copy

copy.o: copy.cpp
	g++ -c copy.cpp -o copy.o

run: copy
	./copy source-file destination-file

clean:
	rm -f *.o copy

.PHONY: all clean run
