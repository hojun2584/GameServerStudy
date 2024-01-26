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

    // addrinfo 의 경우 여러개의 랜카드를 가지고 있거나 ip family 가 달라지는 경우들 도 존재하기 때문에
    // ai_next를 통해서 그런 addrinfo 를 가지고 와서 처리해야 함 그거를 위한 for 문

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        
        ConnectSocket = socket( ptr->ai_family ,ptr->ai_socktype , ptr->ai_protocol );
        // 여기서 닫아야 하는 줄 알았는데 생각해보니까 닫으면
        // ai_next 를 돌때 해당 socket 사용 못함
        // 아 그냥 socket 자체가 안열렸나? 그럴 수 도 있겠네
        if (ConnectSocket != INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }

        iResult = connect(ConnectSocket, ptr->ai_addr , ptr->ai_addrlen);


        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if ( ConnectSocket == INVALID_SOCKET ) {
        WSACleanup();
        return 1;
    }
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {

        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }


    iResult = shutdown(ConnectSocket , SD_SEND);
    if (iResult == SOCKET_ERROR) {

        closesocket(ConnectSocket);
        WSACleanup();
    }

    do {
        iResult = recv(ConnectSocket , reciveBuf, recvbufLen , 0);

        if (iResult > 0) {
            printf("Bytes Received : %d \n" , iResult);
        }
        else if (iResult == 0) {
            printf("connection close");
        }
        else {
            printf("recv Fail with error : %d \n" , WSAGetLastError() );
        }


    } while (iResult == 0);



    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

