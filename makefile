main: main.o stack.o
	gcc -o main main.o stack.o

server: server.o stack.o
	gcc  -pthread -o server server.o stack.o

client: client.o stack.o
	gcc -o client client.o stack.o


main.o: main.c stack.h
	gcc -c main.c

server.o: server.c stack.h
	gcc -c server.c

client.o: client.c stack.h
	gcc -c client.c

stack.o: stack.c stack.h
	gcc -c stack.c

.PHONY: clean
	
clean:
	rm -f *.o  main server 