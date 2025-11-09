all: manual-append

manual-append: manual-append.o
	g++ manual-append.o -o manual-append

manual-append.o: manual-append.cpp
	g++ -c manual-append.cpp -o manual-append.o

run: manual-append
	./manual-append result.txt

clean:
	rm -f *.o manual-append result.txt

.PHONY: all clean run
