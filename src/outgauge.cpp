// (C) komunre 2025 LGPL-2.1 License

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <string.h> // for memset
#include <iostream>
#include <poll.h>
#include "outgauge.h"

// Server socket
static int g_socket;
// Outgauge protocol datagram size
static constexpr std::size_t buffer_length = sizeof(unsigned int) * 3 
        + sizeof(char) * 5 
        + sizeof(unsigned short)
        + sizeof(float) * 10
        + sizeof(int);
// Outgauge datagram storage
static char buf[buffer_length];

void open_socket()
{
    // AF_INET specifies we want IPv4 things.
    //  SOCK_DGRAM (UDP)
    //          Supports datagrams (connectionless, unreliable messages of
    //          a fixed maximum length). That means UDP.

    // In other words, this is UDP socket.
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == -1)
    {
        std::cerr << "Error creating server socket!\n";
    }

    // We bind it here
    struct sockaddr_in si_server {};
    memset((char *)&si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(OG_PORT);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr*)&si_server, sizeof(si_server)) == -1)
    {
        std::cerr << "Error binding server socket!\n";
    }

    g_socket = s;
}

Outgauge_t* receive_data()
{
    struct pollfd pfd {};
    pfd.fd = g_socket;
    // Specify which events wait for
    pfd.events = POLLIN;

    // Wait for g_socket to receive data
    // timeout after 100 ms
    // If no data received - return NULL.
    if (poll(&pfd, 1, 100) == 0)
    {
        return NULL;
    }

    // Abort if event is not an incoming data (somehow)
    if ((pfd.revents & POLLIN) == 0)
    {
        std::cout << "Some socket event, but no data to read\n";
        return NULL;
    }

    struct sockaddr_in si_other {};

    long int recv_len;
    unsigned int slen;
    if ((recv_len = recvfrom(g_socket, buf, buffer_length, 0, (struct sockaddr*)&si_other, &slen)) == -1)
    {
        std::cerr << "Error reading socket.\n";
        // Attempt to reopen server socket, assuming server socket has failed
        close(g_socket);
        open_socket();
    }

    if (recv_len != buffer_length)
    {
        std::cerr << "Received malformed data!\n";
        return NULL;
    }

    Outgauge_t* data = (Outgauge_t*)buf;
    
    return data;
}