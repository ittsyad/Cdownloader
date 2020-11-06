#ifndef SOCKET_H
#define SOCKET_H

#define h_addr h_addr_list[0]
#define buf_size 10000
typedef struct net_data{
	const char *hostname;
	const char *host;
	const char *tree;
	const char *port;
	const char *msg;
} net_data_t;
typedef struct thread_args{
	const char* msg;
	const char* filename;
	const char* host;
} thread_args_t;

struct sockaddr_in server;
struct hostent *host;

//void SockInit(thread_args_t *ThreadArgs);
void CreateSock(int *sockfd);
void SockConfigure(struct sockaddr_in *server, struct hostent *host, const char *hostname);

char *ParseHTTP(const char *hostname);
//void SockConnect(int *sockfd,int t_len,int f_len, struct sockaddr_in server, const char msg,const char filename, FILE *file, char s_reply );

#endif
