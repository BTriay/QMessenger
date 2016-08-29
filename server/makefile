C11=-std=c++11
THREADING=-lpthread -ldl
SER=-DSERVER
#TEST=-DTEST_COUT

objFiles = main.o connection.o threads.o ThreadFunc.o Db.o MatrixServer.o UserServer.o User.o RoomServer.o sqlite3.o parser.o

all:server

server: $(objFiles)
	g++ -o server $(objFiles) $(THREADING) #$(TEST)

main.o: main.cpp
	g++ -c $(C11) main.cpp

parser.o: ../parser.cpp
	g++ -c $(C11) ../parser.cpp $(SER) #$(TEST)

ThreadFunc.o: ThreadFunc.cpp
	g++ -c $(C11) ThreadFunc.cpp $(THREADING) $(SER) #$(TEST)

threads.o: threads.cpp
	g++ -c $(C11) threads.cpp $(THREADING) $(SER) #$(TEST)

Db.o: Db.cpp
	g++ -c $(C11) Db.cpp #$(TEST)

MatrixServer.o: MatrixServer.cpp
	g++ -c $(C11) MatrixServer.cpp #$(TEST)

UserServer.o: UserServer.cpp
	g++ -c $(C11) UserServer.cpp #$(TEST)

User.o: ../User.cpp
	g++ -c $(C11) ../User.cpp #$(TEST)

RoomServer.o: RoomServer.cpp
	g++ -c $(C11) RoomServer.cpp #$(TEST)

connection.o: connection.cpp
	g++ -c $(C11) connection.cpp #$(TEST)

sqlite3.o: ../sqlite-amalgamation/sqlite3.c
	gcc -c ../sqlite-amalgamation/sqlite3.c -DSQLITE_THREADSAFE #$(THREADING) $(TEST)

clean:
	rm *.o
