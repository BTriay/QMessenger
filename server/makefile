GCC=g++-6
C14=-std=c++14
FLAGS=-Wall -Wextra
THREADING=-lpthread -ldl
SER=-DSERVER
TEST=-DTEST_COUT
DEBUG=-g

objFiles = main.o connection.o threads.o ThreadFunc.o Db.o MatrixServer.o UserServer.o User.o RoomServer.o sqlite3.o parser.o Bundle.o

all:server

server: $(objFiles)
	$(GCC) -o server $(objFiles) $(THREADING) $(TEST)

main.o: main.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) main.cpp

parser.o: ../parser.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) ../parser.cpp $(SER) $(TEST)

ThreadFunc.o: ThreadFunc.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) ThreadFunc.cpp $(THREADING) $(SER) $(TEST)

threads.o: threads.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) threads.cpp $(THREADING) $(SER) $(TEST)

Bundle.o: Bundle.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) Bundle.cpp $(SER) $(TEST)

Db.o: Db.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) Db.cpp $(TEST)

MatrixServer.o: MatrixServer.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) MatrixServer.cpp $(TEST)

UserServer.o: UserServer.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) UserServer.cpp $(TEST)

User.o: ../User.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) ../User.cpp $(TEST)

RoomServer.o: RoomServer.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) RoomServer.cpp $(TEST)

connection.o: connection.cpp
	$(GCC) -c $(DEBUG) $(C14) $(FLAGS) connection.cpp $(TEST)

sqlite3.o: ../sqlite-amalgamation/sqlite3.c
	gcc -c ../sqlite-amalgamation/sqlite3.c -DSQLITE_THREADSAFE $(TEST) #$(THREADING)

clean:
	rm *.o
