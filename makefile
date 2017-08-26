all:stockfish_server

stockfish_server:stockfish_server.o
	g++ stockfish_server.o -o stockfish_server

stockfish_server.o:stockfish_server.cpp
	g++ -c stockfish_server.cpp
clean:
	rm *o stockfish_server
	
