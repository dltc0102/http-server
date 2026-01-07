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
    // open an html file to serve
    FILE *html_data;
    html_data = fopen("./index.html", "r");
    if (html_data == NULL)
    {
        perror("Failed to open index.html");
        exit(1);
    }

    // read html file data
    long file_size = ftell(html_data);
    char *response_data = malloc(file_size + 1);
    fgets(response_data, BUFFER_SIZE, html_data);

    char http_header[BUFFER_SIZE * 2] = "HTTP/1.1 200 OK\r\n\n";
    strcat(http_header, response_data);

    // create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
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
    while (1)
    {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0)
        {
            perror("Accept failed");
            continue;
        }
        printf("Client connected\n");

        send(client_socket, http_header, sizeof(http_header), 0);
        close(client_socket);
    }
    return 0;
}