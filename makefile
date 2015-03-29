SOURCES=test.cpp Matrix.cpp
EXECUTABLE=test
CC=g++
CFLAGS=-Wall -std=c++11
LDFLAGS=
OBJECTS=$(SOURCES:.cpp=.o)

all:$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o core
	rm -rf *~
