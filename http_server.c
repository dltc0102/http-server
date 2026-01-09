#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 9002
#define BUFFER_SIZE 1024

int main()
{
    // create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // set socket option to resue address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Setsockopt failed");
    }

    // define address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind socket
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    // listen for connections
    int allowed_connections = 5;
    if (listen(server_socket, allowed_connections) < 0)
    {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }

    printf("HTTP server running on http://localhost:%d\n", PORT);

    // accept connections
    int client_socket;
    char request_buffer[BUFFER_SIZE];

    while (1)
    {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0)
        {
            perror("Accept failed");
            continue;
        }
        printf("Client connected\n");

        // read request
        int bytes_received = recv(client_socket, request_buffer, sizeof(request_buffer), 0);
        if (bytes_received <= 0)
        {
            close(client_socket);
            continue;
        }
        request_buffer[bytes_received] = '\0';
        puts(request_buffer);

        char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "<h1>Hello from server!</h1>";

        send(client_socket, response, strlen(response), 0);
        close(client_socket);
    }
    return 0;
}