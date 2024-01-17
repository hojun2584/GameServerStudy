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

	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	char recvBuf[BUFLEN];
	int recbuflen = BUFLEN;



	// 왜 결과값이 인트야? 진짜 이해가 안가...
	// 아무튼 0아니면 초기화에 문제 있는 것
	if (iResult != 0 )
		return 1;

	// getaddrinfo 하기전에 소켓 초기화,
	// 초기화 해야할 어떤 것에 첫번째 주소(pointer) , size
	ZeroMemory(&hints, sizeof(hints));

	// 4세대 ip
	hints.ai_family = AF_INET;
	// 스트림 소켓 사용
	hints.ai_socktype = SOCK_STREAM;
	// tcp 방식 사용 명시
	hints.ai_protocol = IPPROTO_TCP;
	// 열어놓고 대기 하라는 flag
	hints.ai_flags = AI_PASSIVE;



	// 각각 DNS 이름(NAVER , GOOGLE), port번호 , reuslt에 대한 희망 반환형에 대한 힌트?
	// iResult = getaddrinfo가 정상 작동 했는가 판단 하기 위함
	// result = getaadrinfo가 정상 작동 해서 DNS를 IP로 변환해서 가지고 오는 것.
	// 이 경우는 내꺼 찾는 거라 DNS 없어서 NULL 이고 port는 globalKnown port만 피해서 쓰고
	// hint 주고 결과 result , 그런데 나 포트 연적 없는데.. 흠.. 신기 하네
	iResult = getaddrinfo(NULL, PORT, &hints, &result);

	// gethostname -> 책이 오래되서 역시 오래된 함수 쓰는 듯
	// Thread Safe 보장 하지 않고 ipv4 만 지원함
	// 공식 문서를 가까이 하고 책은 기초만 배우도록 하자...
	// 최신 버전 winsoc는 getaddrinfo 를 권장하고 있음



	// bind -> listen -> accept  로 아는데
	// 왜 공식에서 listen -> bind 하는 거지? 이해가 안가네

	iResult = bind( ListenSocket,result->ai_addr, (int)result->ai_addrlen );
	if (iResult == INVALID_SOCKET)
		return 1;

	// clinet의 접속을 듣기 위한 소켓 생성
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
		return 1;

	// 반복자 패턴에 근거한 메모리 해제인가?
	freeaddrinfo(result);

	// 들어라! 접속을
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 클라이언트 접속 수락 이 것을 통해서 서로의 클라이언트 소켓들(서버도 클라 가지고 있음) 끼리
	// 통신을 시작함 
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	// 여기서 소켓을 닫는 이유는 아마 클라이언트가 접속 하면 더 이상 
	// 프로그램이 돌아갈 이유가 없어서 이지 않을까 예측 중임
	// 서버의 이용자가 다중이어야 할 경우 listen 소켓은 계속 열려 있고
	// accept가 주는 socket을 Thread에서 돌릴 경우 많은 이용자가 사용하는 서버 구축이 가능하지 않을까 추측해봄
	closesocket(ListenSocket);

	// TODO_LIST 연결은 했는데 서로 데이터 통신이나 이런거 하지 않음 send나 recive 이런거
	// 내일 좀 더 열심히 해볼 것


	MessageBox(NULL, TEXT("소켓 초기화 성공"), TEXT("알림"), MB_OK);
	
	
	closesocket(ListenSocket);
	WSACleanup();



	return 0;
}