all: server client

server:
	g++ server.cpp -o server -pthread

client:
	g++ client.cpp -o client -pthread

clean:
	rm -f server client

.PHONY: all clean

