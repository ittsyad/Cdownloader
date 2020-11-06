#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include "socket.h"


typedef struct net_data{
	const char *hostname;
	const char *host;
	const char *tree;
	const char *port;
	const char *msg;
} net_data_t;

//Sockets job
void SockInit(const char *msg, const char *filename, const char *hostname){ 
    int   t_len = 0;
    int   f_len = 99353;
    char  s_reply[buf_size]; 
    FILE *file = NULL;
    
    struct sockaddr_in server;
    struct hostent *host;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { fprintf(stdout, "Socket creation failed"); }	
    
    host = gethostbyname(hostname);   
    server.sin_addr = *((struct in_addr *)host->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){ fprintf(stdout, "Connection failed");}
    if (send(sockfd, msg, strlen(msg), 0) <  0)				{fprintf(stdout,"Send failed");}   
    //Working w files, should be removed ASAP
    remove(filename);
    file = fopen(filename, "ab");
    if (file == NULL) { fprintf(stdout, "Error opening file"); }   
    	
    while (true){
	int recieved_len = recv(sockfd, s_reply, sizeof(s_reply), 0);
	if ( recieved_len < 0 ){
		fprintf(stdout, "Receving failed");
		break;
	}
	t_len += recieved_len;
	fwrite(s_reply, recieved_len, 1, file);
	if (t_len >= f_len){ break; }	
	
	printf("\nReceived byte size = %d\nTotal lenght = %d", recieved_len, t_len);
	}
	fclose(file);
}
char *ParseHTTP(const char *hostname){
	char s[9];
	sscanf(hostname, "%99[^:]", s);	
	if (strcmp(s , "http") == 0) return "80";        
	return "443";		
}
void ParseHOST_TREE(net_data_t *NetData, const char *hostname, const char *port){
	char page[100];
	char tmp[100];
	char host[1000];
	sscanf(hostname,"%*[^:]%*[:/]%[^/]%[^\n]",host, page); 
	NetData->host = host;
	NetData->tree = page;
}
void gGetMsg(const char* TREE, const char *HOST, net_data_t *NetData){
	char buf[100];
	snprintf(buf, sizeof(buf), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n", TREE, HOST);
	NetData->msg = buf;	
}
// http://www.axmag.com/download/pdfurl-guide.pdf
// hostname host tree port
int main(int argc, char **argv){	
	net_data_t  NetData;
	NetData.hostname = "http://www.axmag.com/download/pdfurl-guide.pdf";	
	NetData.port = ParseHTTP(NetData.hostname);
	ParseHOST_TREE(&NetData, NetData.hostname, NetData.port);
	gGetMsg(NetData.tree, NetData.host, &NetData);
	const char *message = "GET /download/pdfurl-guide.pdf HTTP/1.1\r\nHost: www.axmag.com\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n";
	const char *filename = "111.pdf";	
	SockInit(NetData.msg, filename, NetData.host);  
	return 0;

}
