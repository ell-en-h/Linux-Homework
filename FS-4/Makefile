all: redirect-cin

redirect-cin: redirect-cin.o
	g++ redirect-cin.o -o redirect-cin

redirect-cin.o: redirect-cin.cpp
	g++ -c redirect-cin.cpp -o redirect-cin.o

run: redirect-cin
	./redirect-cin input-file

clean:
	rm -f *.o redirect-cin

.PHONY: all clean run
