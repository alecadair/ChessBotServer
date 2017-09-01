all:stockfish_server stockfish_client

stockfish_server:stockfish_server.o
	g++ stockfish_server.o -o stockfish_server

stockfish_client:stockfish_client.o
	g++ stockfish_client.o -o stockfish_client

stockfish_server.o:stockfish_server.cpp
	g++ -c stockfish_server.cpp

stockfish_client.o:stockfish_client.cpp
	g++ -c stockfish_client.cpp

clean:
	rm -f stockfish_server
	rm -f stockfish_client
	rm -f stockfish_server.o
	rm -f stockfish_client.o
