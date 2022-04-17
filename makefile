CXX=clang++-9 
CXXFLAGS=-std=c++2a -Werror

HEADERS=stack.hpp 

all: server client
	
server: server.o stack.o malloc.o
	$(CXX) -lpthread $(CXXFLAGS) $^ -o server


client: client.o stack.o malloc.o
	$(CXX) $(CXXFLAGS) $^ -o client


stack.o: stack.cpp stack.hpp
	$(CXX) $(CXXFLAGS) --compile $< -o $@


malloc.o: malloc.cpp malloc.hpp
	$(CXX) $(CXXFLAGS) --compile $< -o $@


clean:
	rm -f *.o server client 