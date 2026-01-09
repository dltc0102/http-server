#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 9002
#define BUFFER_SIZE 1024
#define NO_SOCKET -1
#define MODE_EXIT 0
#define MODE_CONTINUE 1

int checkfunc(int exp_result, char *message, int given_socket, int mode)
{
    if (exp_result < 0)
    {
        perror(message);
        if (given_socket >= 0)
        {
            close(given_socket);
        }
        if (mode == MODE_EXIT)
        {
            exit(1);
        }
        else
        {
            return -1;
        }
    }
    return exp_result;
}

int main()
{
    // create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkfunc(server_socket, "Socket creation failed", NO_SOCKET, MODE_EXIT);

    // set socket option to resue address
    int opt = 1;
    int errno = 0;
    checkfunc(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "Setsockopt failed", NO_SOCKET, MODE_EXIT);

    // define address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind socket
    checkfunc(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)), "Bind failed", server_socket, MODE_EXIT);

    // listen for connections
    int allowed_connections = 5;
    checkfunc(listen(server_socket, allowed_connections), "Listen failed", server_socket, MODE_EXIT);

    printf("HTTP server running on http://localhost:%d\n", PORT);

    // accept connections
    int client_socket;
    char request_buffer[BUFFER_SIZE];

    while (1)
    {
        client_socket = accept(server_socket, NULL, NULL);
        checkfunc(client_socket, "Accept failed", NO_SOCKET, MODE_CONTINUE);
        printf("Client connected\n");

        // read request
        int bytes_received = recv(client_socket, request_buffer, sizeof(request_buffer), 0);
        checkfunc(bytes_received, "No bytes received.", client_socket, MODE_CONTINUE);
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