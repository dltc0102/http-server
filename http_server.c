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

    // get html file size
    fseek(html_data, 0, SEEK_END);
    long file_size = ftell(html_data);
    fseek(html_data, 0, SEEK_SET);

    // allocate memory for html file size
    char *file_content = malloc(file_size + 1);
    if (file_content == NULL)
    {
        perror("Memory allocation failed");
        fclose(html_data);
        exit(1);
    }

    // Read entire file
    size_t bytes_read = fread(file_content, 1, file_size, html_data);
    // file_content[bytes_read] = '\0';
    fclose(html_data);

    // create http response with headers
    char http_header[BUFFER_SIZE];
    snprintf(http_header, sizeof(http_header),
             "HTTP/1.1 200 OK\r\n"
             "Server: Simple C Server\r\n"
             "Content-Type: text/html\r\n"
             "Connection: close\r\n"
             "Content-Length: %ld\r\n\r\n",
             bytes_read);

    // get http response size
    size_t header_len = strlen(http_header);
    size_t total_len = header_len + bytes_read;
    char *response = malloc(total_len + 1);
    if (response == NULL)
    {
        perror("Response allocation failed");
        free(file_content);
        exit(1);
    }

    // Combine header and content
    strcpy(response, http_header);
    strcat(response, file_content);

    // create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Socket creation failed");
        free(file_content);
        free(response);
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
        free(file_content);
        free(response);
        exit(1);
    }

    // listen for connections
    int allowed_connections = 5;
    if (listen(server_socket, allowed_connections) < 0)
    {
        perror("Listen failed");
        close(server_socket);
        free(file_content);
        free(response);
        exit(1);
    }

    printf("HTTP server running on http://localhost:%d\n", PORT);

    // accept connections
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0)
        {
            perror("Accept failed");
            continue;
        }
        printf("Client connected\n");

        send(client_socket, response, total_len, 0);
        close(client_socket);
    }
    return 0;
}