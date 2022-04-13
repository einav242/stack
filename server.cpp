/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include"stack.hpp"
#include <mutex>

using namespace std;

#define PORT "3531" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold
#define MAXSIZE 1024
#define NUMBER_OF_USER 5
struct arg_struct {
    int arg1;
    mystack *arg2;
};

void *func(void *value)
{
	struct arg_struct *args = (struct arg_struct *)value;
	int sock=args->arg1;
	mystack *s=args->arg2;
    char data[1024];
    int check;
	memset(data,0,strlen(data));
    while (1)
    {
		if(strncmp(data, "EXIT",4)==0)
		{
			break;
		}
        if (strncmp(data, "PUSH", 4) == 0)
        {
			int i=0;
			int k=5;
			char value[strlen(data)];
			while(i<strlen(data))
			{
				value[i]=data[k];
				i++;
				k++;
			}
			push(s,value);
        }
        else if (strncmp(data, "POP", 3) == 0)
        {
			pop(s);
        }
        else if (strncmp(data, "TOP", 3) == 0)
        {
			Top(s);
        }
       memset(data,0,strlen(data));
        if ((check = recv(sock, data, 1023, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }
        if (check==0)
        {
            printf("User logged out\n");
            close(sock);
            return NULL;
        }
        
        data[strlen(data)] = '\0';
    }
    printf("Communication with this user is over\n");
    close(sock);
    return NULL;
}

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0)
		;

	errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
	mystack *stack=(mystack*)malloc(sizeof(mystack));
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
							 p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
					   sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");
	pthread_t threads[NUMBER_OF_USER];
    int i = 0;
	mystack *st=(mystack*)malloc(sizeof(mystack));
	struct arg_struct args;
	while (1)
	{ // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1)
		{
			perror("accept");
			continue;
		}
		args.arg1=new_fd;
		args.arg2=st;
		inet_ntop(their_addr.ss_family,
				  get_in_addr((struct sockaddr *)&their_addr),
				  s, sizeof s);
		printf("server: got connection from %s\n", s);
		if (pthread_create(&threads[i], NULL, &func, &args) != 0)
        {
            printf("Failed to create thread\n");
        }
		i++;
        if (i >= NUMBER_OF_USER)
        {
            for(int j=0;j<NUMBER_OF_USER;j++)
            {
                pthread_join(threads[j], NULL);
            }
            i = 0;
        }
		// pthread_t thread;
		// pthread_create(&thread, NULL, &func, &args);
		// pthread_join(thread, NULL);
		// close(new_fd); // parent doesn't need this
	}

	return 0;
}
