all: hacker-rm

hacker-rm: hacker-rm.o
	g++ hacker-rm.o -o hacker-rm

hacker-rm.o: hacker-rm.cpp
	g++ -c hacker-rm.cpp -o hacker-rm.o

run: hacker-rm
	./hacker-rm file-to-erase

clean:
	rm -f *.o hacker-rm

.PHONY: all clean run
