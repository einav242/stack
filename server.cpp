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
#include <iostream>
#include"stack.hpp"
#include <mutex>
#include"malloc.hpp"

using namespace std;

#define PORT "3540" 

#define BACKLOG 10 
#define MAXSIZE 1024
#define NUMBER_OF_USER 10
pthread_mutex_t Mutex_stack;
pthread_mutex_t Mutex_malloc;

mystack *stack;

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
			pthread_mutex_lock(&Mutex_stack);
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
			pthread_mutex_unlock(&Mutex_stack);
        }
        else if (strncmp(data, "POP", 3) == 0)
        {
			pthread_mutex_lock(&Mutex_stack);
			pop(s);
			pthread_mutex_unlock(&Mutex_stack);
        }
        else if (strncmp(data, "TOP", 3) == 0)
        {
			pthread_mutex_lock(&Mutex_stack);
			char *d=Top(s);
			if(d==NULL)
			{
				 char buffer[1024]="Empty stack";
				if (send(sock, buffer, 1024, 0) == -1)
				{
					perror("send");
					exit(0);
				}
			}
			else
			{
				char buffer[1024];
				memset(buffer, 0, strlen(buffer));
				for (int i = 0; i < strlen(d); i++)
				{
					buffer[i] = d[i];
				}
				buffer[strlen(d)]='\0';
				if (send(sock, buffer, 1024, 0) == -1)
				{
					perror("send");
					exit(0);
				}
			}
			pthread_mutex_unlock(&Mutex_stack);
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

void sig_handler(int sig)
{
	pthread_mutex_lock(&Mutex_malloc);
    new_free(stack);
    pthread_mutex_unlock(&Mutex_malloc);
	pthread_mutex_destroy(&Mutex_malloc);
	pthread_mutex_destroy(&Mutex_stack);
	destroy();
	printf("\nbye bye\n");
	exit(0);
}

void sigchld_handler(int s)
{
	(void)s; 


	int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0)
		;

	errno = saved_errno;
}

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
	printf("server:to exit press ctrl+c or ctrl+z\n");
	int sockfd, new_fd; 
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
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

	freeaddrinfo(servinfo);

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

	sa.sa_handler = sigchld_handler; 
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
	pthread_mutex_lock(&Mutex_malloc);
	stack=(mystack*)new_malloc(sizeof(mystack));
	pthread_mutex_unlock(&Mutex_malloc);
	struct arg_struct args;
	signal(SIGINT, sig_handler);
	signal(SIGTSTP,sig_handler);
	while (1)
	{ 
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1)
		{
			perror("accept");
			continue;
		}
		args.arg1=new_fd;
		args.arg2=stack;
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
	}

	return 0;
}