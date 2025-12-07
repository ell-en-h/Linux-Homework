all: prime-calculator

prime-calculator.o: prime-calculator.cpp
	 g++ -c prime-calculator.cpp -o prime-calculator.o -pthread

prime-calculator: prime-calculator.o
	 g++ prime-calculator.o -o prime-calculator -pthread

clean:
	 rm -f *.o prime-calculator

.PHONY: all clean
