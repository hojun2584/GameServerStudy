#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "3210"

int main(int argc, char** argv)
{
    WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;

    addrinfo* ptr = NULL;
    addrinfo* result = NULL;
    addrinfo hints;

    const char* sendbuf = "this is test";

    char reciveBuf[DEFAULT_BUFLEN];

    int iResult;
    int recvbufLen = DEFAULT_BUFLEN;

    iResult = WSAStartup(MAKEWORD(2,2),&wsaData);

    if (iResult != 0) {
        return 1;
    }

    ZeroMemory(&hints , sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    iResult = getaddrinfo( argv[1], DEFAULT_PORT,&hints , &result);

    if (iResult != 0) {
        WSACleanup();
        return 1;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    }

}

