all: do-command

do-command: do-command.o
	g++ do-command.o -o do-command

do-command.o: do-command.cpp
	g++ -c do-command.cpp -o do-command.o

run: do-command
	./do-command ls -al

clean:
	rm -f *.o do-command

.PHONY: all clean run
