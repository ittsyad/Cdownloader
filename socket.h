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

void *SockInit(void *ThreadArgs);
void  ParseHOST_TREE(net_data_t *NetData, const char *hostname, const char *port);
void gGetMsg(net_data_t *NetData, char *tree, char *host);

char *ParseHTTP(const char *hostname);

#endif
