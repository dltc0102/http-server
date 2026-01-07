/**
 * Stream Sockets (SOCK_STREAM): send it '1, 2' and it'll send '1, 2' back
 * Datagram Sockets (SOCK_DGRAM):
 * - sockets that use IP for routing, not TCP.
 * - they are unreliable because of possibly arriving in the wrong order or not at all.
 * - they also use UDP.
 * - they are connectionless sockets because you don’t have to maintain an open connection as you do with stream sockets
 *
 * HTTP: Hypertext Transfer Protocol
 * IP: Internet Protocol: deals with Internet routing and not so much data integrity
 * TCP: Transmission Control Protocol: Makes sure your data arrives sequencially and error-free
 */
// make a socket
// bind an address
// listen
// connect
// accept
// write
// read
// close socket
// GET Method
// POST Method
// PUT Method
// PATCH Method
// DELETE Method
// HEAD Method
// OPTIONS Method
// TRACE Method
// CONNECT Method

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 9002

int main()
{
    // create a socket
    int network_socket;
    // AF_INET is IPV4
    // SOCK_STREAM: TCP Connection
    // 0 is the protocol
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // connect to an address
    int connection_response = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_response != 1)
    {
        printf("Connection response is not 1.\n\n");
    }

    // receive data from the server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);

    // print out server's response
    printf("The server sent back the data: %s\n", server_response);

    // close socket
    close(network_socket);
    return 0;
}
