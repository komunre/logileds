// (C) komunre 2025 LGPL-2.1 License

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <string.h> // for memset
#include <iostream>
#include "outgauge.h"

static int g_socket;
static constexpr std::size_t buffer_length = sizeof(unsigned int) * 3 
        + sizeof(char) * 5 
        + sizeof(unsigned short)
        + sizeof(float) * 10
        + sizeof(int);
static char buf[buffer_length];

void open_socket()
{
    // AF_INET specified we want IPv4 things.
    //  SOCK_DGRAM (UDP)
    //          Supports datagrams (connectionless, unreliable messages of
    //          a fixed maximum length). That means UDP.

    // In other words, this is UDP socket.
    // We create it.
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == -1)
    {
        std::cout << "Error creating server socket!\n";
    }

    // We bind it here
    struct sockaddr_in si_server {};
    memset((char *)&si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(OG_PORT);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr*)&si_server, sizeof(si_server)) == -1)
    {
        std::cout << "Error binding server socket!\n";
    }

    g_socket = s;
}

Outgauge_t* receive_data()
{
    struct sockaddr_in si_other {};

    long int recv_len;
    unsigned int slen;
    if ((recv_len = recvfrom(g_socket, buf, buffer_length, 0, (struct sockaddr*)&si_other, &slen)) == -1)
    {
        std::cout << "Error reading socket.\n";
        open_socket();
    }

    if (recv_len != buffer_length)
    {
        std::cout << "Received malformed data!\n";
        std::cout << "Returning previous entry\n";
        return (Outgauge_t*)buf;
    }

    Outgauge_t* data = (Outgauge_t*)buf;
    
    return data;
}