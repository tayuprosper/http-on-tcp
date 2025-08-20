server: server.o http.o
	g++ -o server server.o http.o


http.o: http.cpp http.h server.o
	g++ -c http.cpp

server.o: server.cpp http.h
	g++ -c server.cpp


clean:
	rm -f *.o server

run:
	./server