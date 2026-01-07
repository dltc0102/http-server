#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 9002

int main()
{
    char server_message[256] = "You have reached the server!";

    // create server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define address structure
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // listen for connections
    int allowed_connections = 5;
    listen(server_socket, allowed_connections);

    // accept connections
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);

    // send message to client
    send(client_socket, server_message, sizeof(server_message), 0);

    // close socket
    close(server_socket);

    return 0;
}