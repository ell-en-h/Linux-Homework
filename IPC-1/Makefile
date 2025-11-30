all: signal-echo

signal-echo.o: signal-echo.cpp
	g++ -c signal-echo.cpp -o signal-echo.o -pthread

signal-echo: signal-echo.o
	g++ signal-echo.o -o signal-echo -pthread

clean:
	rm -f *.o signal-echo

.PHONY: all clean

