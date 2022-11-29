/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

//#define MYPORT "21003"	// the port users will be connecting to
#define PORT "24004"

#define MAXBUFLEN 1000
using namespace std;

bool check(char* c, string s){
	bool ans = true;
	int size = s.length();
	for(int i = 0; i< size; i++){
		if(c[i] != s[i]){
			ans = false;
			break;
		}
	}
	return ans;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{	cout << "The ServerC is up and running using UDP on port 21003."<< endl;
	int tryout = 0;

	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	//info of server
	struct sockaddr_in server;
	memset((char*) &server, 0, sizeof server);


	server.sin_family = AF_INET;
	server.sin_port =htons(24004);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");



	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;

	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo(NULL, "21003", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	while(1){
	
	//freeaddrinfo(servinfo);

	



	//printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	buf[numbytes] = '\0';
	//printf("%s\n", buf);
	cout << "The ServerC received an authetication request from the Main Server." << endl;



	//freeaddrinfo(servinfo);

	//close(sockfd);
	//check cred.txt
	//char t[5] = "\n"; // split by this
	//buf[strstr(buf, t)-buf] = ',';
	//cout << buf;

	FILE *fp;

	char ss[MAXBUFLEN];

	int linecount = 0;

	int usn = 0;
	int psd = 0;
	char *pch;
	char * un, *pw;


	pch = strtok(buf, "\n");
	un = pch;
	pch = strtok(NULL, "\n");
	pw = pch;

	//cout << pw << endl;


	fp = fopen("./cred.txt", "r");


	while(fgets(ss, sizeof(ss), fp) != NULL){

		char *ppch;
		char * uun, *ppw;
		ppch = strtok(ss, ",");
		uun = ppch;
		ppch = strtok(NULL, ",");
		ppw = ppch;


		if(strncmp(un,uun, std::min(strlen(uun), strlen(un))) == 0){
			usn = 1;


		}
		if(strncmp(pw,ppw, std::min(strlen(pw), strlen(ppw))) == 0){
			psd = 1;

		}


	}
	fclose(fp);
	int nb;

	/*
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}*/

	
		if(usn == 0 and psd == 0){
			if((nb = sendto(sockfd, "3", strlen("3"), 0, (struct sockaddr*) &server, sizeof(server)) == -1)){
				perror("talker:sendto");
				exit(1);
			}
			//freeaddrinfo(servinfo);
			printf("talker: sent %d bytes to %s\n", nb, "M in 00");

		}
		else if(usn == 0 and psd ==1){
			if((nb = sendto(sockfd, "1", strlen("1"), 0, (struct sockaddr*) &server, sizeof(server)) == -1)){
				perror("talker:sendto");
				exit(1);
			}
			//freeaddrinfo(servinfo);
			printf("talker: sent %d bytes to %s\n", nb, "M in 01");

		
			
		}
		else if(psd == 0 and usn == 1){
			if((nb = sendto(sockfd, "2", strlen("2"), 0,(struct sockaddr*) &server, sizeof(server)) == -1)){
				perror("talker:sendto");
				exit(1);
			}
			//freeaddrinfo(servinfo);
			printf("talker: sent %d bytes to %s\n", nb, "M in 10");

		}
		else if(usn == 1 and psd ==1 ){
			if((nb = sendto(sockfd, "0", strlen("0"), 0, (struct sockaddr*) &server, sizeof(server)) == -1)){
				perror("talker:sendto");
				exit(1);
			}
			printf("talker: sent %d bytes to %s\n", nb, "M in 11");
			

		}
		cout << "The ServerC finished sending the response to the Main Server."<< endl;
		
	}
	freeaddrinfo(servinfo);

	close(sockfd);


	return 0;
}