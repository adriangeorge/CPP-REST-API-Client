CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS =  -Wall -g

SRC = ClientClass.cpp  client.cpp
OBJ = $(SRC:.cc=.o)
EXEC = client

all: $(EXEC)

IP = 34.118.48.238
PORT = 8080

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

run:
	./$(EXEC) $(IP) $(PORT)

clean:
	rm -rf $(EXEC)