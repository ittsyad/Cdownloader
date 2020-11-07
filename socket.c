#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <memory.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "socket.h"

//Sockets job
void *SockInit(void *ThreadArgs){ 
    int   t_len = 0;
    int   f_len = 99353;
    thread_args_t *at_args = (thread_args_t *)ThreadArgs;
	thread_args_t *t_args = (thread_args_t *)malloc(sizeof(thread_args_t));
    t_args->filename = at_args->filename;
	t_args->msg = at_args->msg;
	t_args->host = at_args->host;
    
    char  s_reply[buf_size]; 
    
    FILE *file = NULL;
    
    struct sockaddr_in server;
    struct hostent *host;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { fprintf(stdout, "Socket creation failed"); }	
    
    host = gethostbyname(t_args->host);  
    server.sin_addr = *((struct in_addr *)host->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
	    fprintf(stdout, "Connection failed");
    }
    if (send(sockfd, t_args->msg, strlen(t_args->msg), 0) <  0){
	    fprintf(stdout,"Send failed");
    }   
    
    remove(t_args->filename);
    file = fopen(t_args->filename, "ab");

    if (file == NULL) { fprintf(stdout, "Error opening file"); }   
    	
    while (1){
	int recieved_len = recv(sockfd, s_reply, sizeof(s_reply), 0);
	if ( recieved_len < 0 ){ fprintf(stdout, "Receving failed"); break; }
	t_len += recieved_len;
	fwrite(s_reply, recieved_len, 1, file);
	if (t_len >= f_len){ break; }	
	
	printf("\nReceived byte size = %d\nTotal lenght = %d", recieved_len, t_len);
	}
	fclose(file);
	pthread_exit(0);
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
void gGetMsg(char *tree, char *host){
	int BUF_SIZE =  150; // need to figure out what actual strlen of get is
	// host and data are corruprruptinh here
	strcpy(buf, "GET ");
	strcat(buf, tree);
	strcat(buf, "HTTP/1.1\r\nHost: ");
	strcat(buf, host);
	strcat(buf, "\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n");
	printf("%s", buf)
;

}
// http://www.axmag.com/download/pdfurl-guide.pdf
int main(int argc, char **argv){	
	net_data_t NetData;
	NetData.hostname = "http://www.axmag.com/download/pdfurl-guide.pdf";	
	NetData.port = ParseHTTP(NetData.hostname);
	ParseHOST_TREE(&NetData, NetData.hostname, NetData.port);
	gGetMsg(NetData.tree, NetData.host);
	const char *filename = "111.pdf";	

	thread_args_t ThreadArgs;
	ThreadArgs = (thread_args_t){
		.msg     = NetData.msg,
		.filename= filename,
	    .host    = NetData.host};
	pthread_t thread_1;
	pthread_create(&thread_1, NULL, SockInit, &ThreadArgs);
	pthread_join(thread_1, NULL);
	//SockInit(NetData.msg, filename, NetData.host);  
	return 0;

}
