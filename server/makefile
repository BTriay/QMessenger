objFiles = main.o connection.o threads.o Db.o MatrixServer.o UserServer.o User.o RoomServer.o Room.o sqlite3.o parser.o

all:server

server: $(objFiles)
	g++ -o server $(objFiles) -lpthread -ldl 

main.o: main.cpp
	g++ -c -std=c++11 main.cpp

threads.o: threads.cpp
	g++ -c -std=c++11 threads.cpp -lpthread -ldl

Db.o: Db.cpp
	g++ -c -std=c++11 Db.cpp

MatrixServer.o: MatrixServer.cpp
	g++ -c -std=c++11 MatrixServer.cpp

UserServer.o: UserServer.cpp
	g++ -c -std=c++11 UserServer.cpp

User.o: ../User.cpp
	g++ -c -std=c++11 ../User.cpp

RoomServer.o: RoomServer.cpp
	g++ -c -std=c++11 RoomServer.cpp

Room.o: ../Room.cpp
	g++ -c -std=c++11 ../Room.cpp

parser.o: ../parser.cpp
	g++ -c -std=c++11 ../parser.cpp

connection.o: connection.cpp
	g++ -c -std=c++11 connection.cpp

sqlite3.o: ../sqlite-amalgamation-3130000/sqlite3.c
	gcc -c ../sqlite-amalgamation-3130000/sqlite3.c -lpthread -ldl

clean:
	rm *.o
