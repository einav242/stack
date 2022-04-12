CXX=clang++-9 
CXXFLAGS=-std=c++2a -Werror

HEADERS=stack.hpp

all: server client
	
server: server.o stack.o
	$(CXX) -lpthread $(CXXFLAGS) $^ -o server

client: client.o stack.o 
	$(CXX) $(CXXFLAGS) $^ -o client


%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@

clean:
	rm -f *.o server client 