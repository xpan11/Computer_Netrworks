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
#include <unistd.h>


//#define MYPORT "21003"	// the port users will be connecting to
#define PORT "23001"

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
{
	cout<< "The ServerEE is up and running using UDP on port 23001." << endl;
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
	if ((rv = getaddrinfo(NULL, "23001", &hints, &servinfo)) != 0) {
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

	



//	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	usleep(3000000);
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}


	buf[numbytes] = '\0';

	//freeaddrinfo(servinfo);

	//close(sockfd);
	//check cred.txt
	//char t[5] = "\n"; // split by this
	//buf[strstr(buf, t)-buf] = ',';
	//cout << buf;

	FILE *fp;

	char ss[MAXBUFLEN];

	int linecount = 0;

	char *pch;
	char * cs, *ctg;
	pch = strtok(buf, ",");
	cs = pch;
	pch = strtok(NULL,",");
	ctg = pch;
	cout << "The ServerEE received a request from the Main Server about the "<< ctg << " of "<< cs<< "."<<endl;
	int cateid;
	if(strcmp("Professor", ctg) == 0){
		cateid = 2;
	}
	if(strcmp("Credit", ctg) == 0){
		cateid = 1;
	}
	if(strcmp("Days", ctg) == 0){
		cateid = 3;
	}
	if(strcmp("CourseName", ctg) == 0){
		cateid = 4;
	}



	//cout << pw << endl;
	


	fp = fopen("./ee.txt", "r");

	char *ppch;
	char * css , *ctgs = "none";


	while(fgets(ss, sizeof(ss), fp) != NULL){

		ppch = strtok(ss, ",");
		css = ppch;
		if(strcmp(css, cs) == 0){
			while(cateid != 0){
				ppch = strtok(NULL, ",");
				cateid = cateid - 1;

			}
			ctgs = ppch;
			break;
		}



	}
	if(strcmp("none",ctgs) == 0){
		cout << "Didn't find the course" << cs << "."<<endl;
	}
	fclose(fp);

	int nb;
		if((nb = sendto(sockfd, ctgs, strlen(ctgs), 0, (struct sockaddr*) &server, sizeof(server)) == -1)){
				perror("talker:sendto");
				exit(1);
			}
			//freeaddrinfo(servinfo);
			cout << "The ServerEE finished sending the response to the Main Server." << endl;




	

		
	}
	freeaddrinfo(servinfo);

	close(sockfd);


	return 0;
}