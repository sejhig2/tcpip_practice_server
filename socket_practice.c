#include <sys/types.h>
#include  <sys/socket.h>

// 소캣을 생성하는 함수.
int socket(int domain, int type, int protocol);

//소캣에 주소를 할당하는 함수
int bind(int sockfd, struct sockaddr *myaddr, int addrlen);

// 소캣을 연결 가능 상태, 연결 대기 상태로 만드는 함수
int listen(int sockfd, int backlog);

//요청 수락하는 함수 만들들기
int accept(int sockfd, struct sockaddr *addr, int *addrlen);

