#pragma comment(lib,"ws2_32")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <memory>
using namespace std;


#define PORT	"3210"
#define BUFLEN	512

int main(int argc, char *argv[])
{

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(1, 1), &wsaData);

	// 1. 윈도우 공식 문서는 왜 변수인데 대문자 시작이지??
	// 이건 아직도 잘 모르겠음 그냥 라이브러리마다 그럴 수 있다는데 이해하기 힘드네..
	// 2. SOCKET은 왜 대문자인가 => 런타임 변하지 않음. 그렇기에 사실상 상수 취급으로 인한
	// 상수 네이밍 룰 따라간듯. 특이하네... 
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	// 윈도우 공식 문서에서는 앞에 struct를 붙인다.
	// 이유가 뭔가?(해결)
	// c언어에서는 typerdef 를 해주지 않은 struct는 무조건 struct 키워드를 붙여야 한다고 한다.
	// c++ 에서도 원래는 필요하지만 추가 규칙에 의해서 struct임을 명시 하지 않아도 된다고함
	// c++ 과 c 언어의 호환성을 위해서 struct 붙여둔 것으로 판단됨.
	struct addrinfo* result = NULL;
	struct addrinfo hints;




	if ( iResult != 0 )
		return 1;

	MessageBox(NULL, TEXT("윈속 초기화 성공"), TEXT("알림"), MB_OK);
	



	SOCKET tcp_sock = socket(AF_INET6, SOCK_STREAM, 0);

	if (tcp_sock == INVALID_SOCKET)
		return 1;

	MessageBox(NULL, TEXT("소켓 초기화 성공"), TEXT("알림"), MB_OK);
	
	
	closesocket(tcp_sock);
	WSACleanup();



	return 0;
}