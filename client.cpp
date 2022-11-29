/*
**cite client.c -- a stream socket client demo from beej's network programming.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <arpa/inet.h>

#define PORT "25001" // the port client will be connecting to 

#define MAXDATASIZE 1024 // max number of bytes we can get at once 

using namespace std;
//1. use getaddrinfo to have informaion of the server
//2. create soket
//3. bind to random port
//4. connect to server
//5. send

//plaintext -> ciphertext:


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	cout << "The client is up and running." << endl;
	int sockfd, numbytes, sk;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				0)) == -1) {
			perror("client: socket");
			continue;
		}



		if (sk = connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			//close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	//printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure


	//48 - 57 NUMBER
	//65 - 90 UPPER
	//97-122 LOWER
	int tryout = 3;
	while(1){
		string plainu;
		string plainp;
		cout << "Please enter the username: " <<endl;
		getline(cin, plainu);// get username plaintext
		string uname = plainu;
		plainu = plainu + "\n";
		cout << "Please enter the password: "<<endl;
		getline(cin, plainp);// get username plaintext
		plainp = plainp + "\n";


		
		//cout << cipher(plainu) << endl;
		//cout << cipher(plainp) << endl;

		//sent to serverM:

		//cout<< "good yet" << endl;


		const char* us = plainu.c_str();
		const char* ps = plainp.c_str();

		string m = plainu+plainp;
		const char* msg = m.c_str();

		int bytesent, bs;
		//cout << lenu << endl;


		bytesent = send(sockfd, msg, strlen(msg), 0);

		 cout << uname <<" sent an authentication request to the main server."<<endl;


			//recv


		if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		buf[numbytes] = '\0';

		string coursename;
		string cat;

		if(strcmp(buf, "Di") == 0){
			cout << uname <<" received the result of authentication using TCP over port" << ((struct sockaddr_in*)p->ai_addr)->sin_port<< ". Auhentication is sucessful."<< endl;
			while(1){
			string where;
			cout << "Please enter the course to query:";
			getline(cin, coursename);// get username plaintext

			cout << "Please enter the category (Credit / Professor / Days / CourseName):";
				if(coursename.find("EE")!=string::npos){
				where= "77,";
			}
			else if(coursename.find("CS")!=string::npos){
				where = "78,"; 
			}
			getline(cin, cat);// get username plaintext
			string ms = where+coursename+","+cat;
			const char* mg = ms.c_str();
			int b = send(sockfd, mg, strlen(mg), 0);
			cout <<  uname <<" send a request to the main server. "<<endl; 

			if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
				perror("recv");
				exit(1);
			}
			buf[numbytes] = '\0';
			cout << "The client received the response from the Main server using TCP over port "<<((struct sockaddr_in*)p->ai_addr)->sin_port<< "."<<endl;
			printf("%s\n", buf);

			cout << ("-----Start a new request------")<<endl;
		}


		}
		


	}

	close(sk);
	close(sockfd);

	//printf("client: received '%s'\n",buf);


	return 0;
}
