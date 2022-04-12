#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


#define SERVER_IP_ADDRESS "127.0.0.1" //IPv4 address of server
#define PORT_NUM 5077
#define SIZE 65536 //2^16


int tcpConnection() //this function open a connention with the server
{
  int sock = 0;
  struct sockaddr_in server_a;        //is a server address
  bzero(&server_a, sizeof(server_a)); //Initializes the socket address
  server_a.sin_family = AF_INET;      // ipv4
  server_a.sin_port = htons(PORT_NUM);
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) //SOCK_STREM-TCP
  {
    printf("\n Error: in socket creation \n"); // if did not open soket you get this error
    return -1;
  }
  if (connect(sock, (struct sockaddr *)&server_a, sizeof(server_a)) < 0)
  {
    printf("Failed: connect failed, Error code : %d \n", errno); // if the connect is faild you get this error
    return -1;
  }
  printf("Connected to server\n"); // else you get that connected is work
  dup2(sock,1); //The output standard will be copied to TCP socket
  return sock;
}

void sendm(char *s, int len, int sock) //This function receives a socket number and a message and sends to the server that the same socket is connected to
{
  int bytes_s = send(sock, s, len, 0); //send the data to the server
  if (bytes_s == -1)
  {
    printf("Error: in sending file: %d", errno);
  }
  else if (bytes_s < len)
  {
    printf("Sent just %d bytes from the requisite %d\n", bytes_s, len);
  }
  else if (bytes_s == 0)
  {
    printf("the TCP has closed before the send \n");
  }
}
int main()
{
  char data[1024]; //into this array we put the command
  while (1)
  {
    memset(data, 0, strlen(data));
    printf("1. Push\n2. Pop\n3. Top\n4. Exit\n");
    printf("\nEnter your choice : ");
    fgets(data, 1024, stdin); //receive information from the user
    puts(data);
    
  }
  return 0;
}