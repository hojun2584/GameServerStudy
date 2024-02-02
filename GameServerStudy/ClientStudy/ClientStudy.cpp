#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define BUFLEN 512
#define DEFAULT_PORT "3210"

int main(int argc, char** argv)
{
    WSADATA wsaData;

    SOCKET connectSocket = INVALID_SOCKET;

    addrinfo* ptr = NULL;
    addrinfo* result = NULL;
    addrinfo hints;

    const char* sendbuf = "this is test";

    char reciveBuf[BUFLEN];
    char message[BUFLEN];


    int iResult;
    int recvbufLen = BUFLEN;

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
        
        connectSocket = socket( ptr->ai_family ,ptr->ai_socktype , ptr->ai_protocol );
        
        // 여기서 닫아야 하는 줄 알았는데 생각해보니까 닫으면
        // ai_next 를 돌때 해당 socket 사용 못함
        // 아 그냥 socket 자체가 안열렸나? 그럴 수 도 있겠네

        if (connectSocket == INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }

        iResult = connect(connectSocket, ptr->ai_addr , ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if ( connectSocket == INVALID_SOCKET ) {
        WSACleanup();
        return 1;
    }

    if (iResult == SOCKET_ERROR) {

        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }


    
    string recvBuf;
    char writeBuf[BUFLEN];
    int strLen;



    while (true) {

        fputs("Input message(q to quit) : " , stdout);
        fgets( message , BUFLEN , stdin);

        if (!strcmp(message , "Q") || !strcmp(message, "q")) {
            cout << "end" << endl;
            break;
        }

        send(connectSocket , message , BUFLEN , 0);
         
        // 이거 -1 은 오류비트 체크인가? 음~ 그건 아닌거 같은데..
        //strLen = recv(connectSocket , message , BUFLEN-1 , 0);
        
        while (true) {
            recv(connectSocket, message, strlen(message), 0);
            if (((string)message).find("\n") != string::npos)
                break;
        }
        //strLen = recv(connectSocket, message, strlen(message), 0);
        cout << message << endl;
        
        ZeroMemory(writeBuf , BUFLEN);

        //printf("message from server : %s" , message);

    }




        

    //do {
    //    

    //    
    //    if (iResult > 0) {
    //        printf("Bytes Received : %d \n" , iResult);
    //        
    //        recvBuf = recv(ConnectSocket, buffer, DEFAULT_BUFLEN - 1, 0);
    //        cout << "String receoved:\n" << recvBuf.c_str() << endl;

    //    }
    //    else if (iResult == 0) {
    //        printf("connection close");
    //    }
    //    else {
    //        printf("recv Fail with error : %d \n" , WSAGetLastError() );
    //    }

    //} while (iResult == 0);

    iResult = shutdown(connectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {

        closesocket(connectSocket);
        WSACleanup();
    }



    closesocket(connectSocket);
    WSACleanup();

    return 0;
}

