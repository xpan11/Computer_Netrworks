#compliler
CC =  g++

#compiler flags
CFLAGS = -std=c++11


all: client ServerM ServerC ServerEE ServerCS

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp

ServerM: ServerM.cpp
	$(CC) $(CFLAGS) -o ServerM ServerM.cpp

ServerC: ServerC.cpp
	$(CC) $(CFLAGS) -o ServerC ServerC.cpp
ServerEE: ServerEE.cpp
	$(CC) $(CFLAGS) -o ServerEE ServerEE.cpp
ServerCS: ServerCS.cpp
	$(CC) $(CFLAGS) -o ServerCS ServerCS.cpp
