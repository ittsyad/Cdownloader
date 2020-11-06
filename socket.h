#ifndef SOCKET_H
#define SOCKET_H

#define h_addr h_addr_list[0]
#define buf_size 10000
#define IS_HOST true
#define IS_TREE false

typedef struct net_data net_data_t;
struct sockaddr_in server;
struct hostent *host;

void SockInit(const char *msg, const char *filename, const char *hostname);
void CreateSock(int *sockfd);
void SockConfigure(struct sockaddr_in *server, struct hostent *host, const char *hostname);

char *ParseHTTP(const char *hostname);
//void SockConnect(int *sockfd,int t_len,int f_len, struct sockaddr_in server, const char msg,const char filename, FILE *file, char s_reply );

#endif
