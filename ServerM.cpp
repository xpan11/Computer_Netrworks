/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <iostream>
#include <unistd.h>


#include <arpa/inet.h>

using namespace std;

#define PORT "25001"  // the port users will be connecting to
#define UDP "21003"
#define MYUDP "24004"

#define MAXBUFLEN 1000
#define BACKLOG 20	 // how many pending connections queue will hold

//plaintext -> ciphertext:

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
// plain to cipher
string cipher(string plain){
	//cout << plain.length() << endl;
	//cout << "in cipher " << plain << endl;
	string ans = plain;
	for(int i = 0; i< plain.length(); i++){
		if(isdigit(plain[i])){
			if((plain[i]+4-48)>=10){
				ans[i] = plain[i]+4-10;
			}
			else{
				ans[i] = plain[i] + 4;
			}
			//cout << "p:" << plain[i] << "a: "<< ans[i] << endl;
		}

		else if(islower(plain[i])){
			if((plain[i]+4-97)>=26){
				ans[i] = plain[i]+4-26;
			} 
			else{
				ans[i] =plain[i] + 4;
			}
				//		cout << "p:" << plain[i] << "a: "<< ans[i] << endl;

		}
		else if(isupper(plain[i])){
			if((plain[i]+4-65)>=26){
				ans[i] = plain[i]+4-26;
			} 
			else{
				ans[i] = plain[i]+4;
			}
			//cout << "p:" << plain[i] << "a: "<< ans[i] << endl;
		}
		
	}
	return ans;
}


int main(void)
{	cout << "The main server is up and running. "<< endl;
	int sockfd, new_fd, nb, ut;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, h,*servinfo,*si, *p, *t;
	struct sockaddr_storage their_addr, their_addrr; // connector's address information
	socklen_t sin_size, addr_len;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN], ss[INET6_ADDRSTRLEN];
	int rv, dt;
	socklen_t al;
	int tryout = 3;
	unsigned int sleep = 1000;

	char buf[MAXBUFLEN];
	int numbytes;
	char c[MAXBUFLEN];

	memset(&hints, 0, sizeof hints);

	//tcp
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	//udp listener
	

	//use for sendto

	struct sockaddr_in server;
	memset((char*) &server, 0, sizeof server);


	server.sin_family = AF_INET;
	server.sin_port =htons(21003);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	//use for cs
	struct sockaddr_in cs;
	memset((char*) &cs, 0, sizeof cs);


	cs.sin_family = AF_INET;
	cs.sin_port =htons(22001);
	cs.sin_addr.s_addr = inet_addr("127.0.0.1");
	//use for ee
	struct sockaddr_in ee;
	memset((char*) &ee, 0, sizeof ee);


	ee.sin_family = AF_INET;
	ee.sin_port =htons(23001);
	ee.sin_addr.s_addr = inet_addr("127.0.0.1");





	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}


	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	memset(&h, 0, sizeof h);

	h.ai_family = AF_UNSPEC;
	h.ai_socktype = SOCK_DGRAM;
	h.ai_flags = AI_PASSIVE;
	char buff[MAXBUFLEN];
	if ((dt = getaddrinfo(NULL, "24004", &h, &si)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(dt));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(t = si; t != NULL; t = t->ai_next) {
		if (( ut = socket(t->ai_family, t->ai_socktype,
				t->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(ut, t->ai_addr, t->ai_addrlen) == -1) {
			close(ut);
			perror("listener: bind");
			continue;
		}

		break;
	}

	

	if (t == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}


		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);

		while(1){

		if ((numbytes = recv(new_fd, buf, MAXBUFLEN-1, 0)) == -1){
				perror("recv");
				exit(1);
			}
		buf[numbytes] = '\0';
		//printf("client: received '%s'\n",buf);

		string cc = cipher(buf);
		//cout << cc << endl;
		strcpy(c, cc.c_str());
		//printf("client: received '%s'\n",buf);

		//udp talker

		char *pch, *un;
		pch = strtok(buf, "\n");
		un = pch;
		cout << "The main server received the authentication for " << un << " using TCP over port 25001." << endl;
 		if((nb = sendto(ut, c, strlen(c), 0, (struct sockaddr*) &server, sizeof(server))==-1)){
			perror("talker: sendto");
			exit(1);
		}
		//printf("client: received '%s'\n",c);
;

		//printf("talker: sent %d bytes to %s\n", nb, "serverC");

		//udp lisenter

		

		addr_len = sizeof their_addr;
		socklen_t s = sizeof server;


		if ((numbytes = recvfrom(ut, buff, MAXBUFLEN-1 , 0,(struct sockaddr*) &their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		buff[numbytes] = '\0';
		printf("client: received '%s'\n",buff);
;



		//send authentication message

		if(strcmp(buff, "1") == 0){
			tryout = tryout - 1;
			string t = to_string(tryout);
			char const *p = t.c_str();
			if (!fork()) { // this is the child process
				close(sockfd); // child doesn't need the listener

				send(new_fd, un, strlen(un), 0);
				send(new_fd, "received the result of authentication using TCP over port ", strlen("received the result of authentication using TCP over port "), 0);
				send(new_fd, PORT, strlen(PORT), 0);
				send(new_fd, "Authentication failed: Username Does not exist \n Attempts remaining:", strlen("Authentication failed: Username Does not exist \n Attempts remaining:"), 0);
				send(new_fd, p,strlen(p),0);

				close(new_fd);

				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);				
				exit(0);
			}
			//close(new_fd);

		}
		else if(strcmp(buff , "2") == 0){
			tryout = tryout - 1;
			string t = to_string(tryout);
			char const *p = t.c_str();
			if (!fork()) { // this is the child process
				close(sockfd); // child doesn't need the listener

				send(new_fd, un, strlen(un), 0);
				send(new_fd, "received the result of authentication using TCP over port ", strlen("received the result of authentication using TCP over port "), 0);
				send(new_fd, PORT, strlen(PORT), 0);
				send(new_fd, "Authentication failed: Password Does not exist \n Attempts remaining:", strlen("Authentication failed: Password Does not exist \n Attempts remaining:"), 0);
				send(new_fd, p,strlen(p),0);

				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);
				exit(0);
			}
		}
		else if(strcmp(buff , "3") == 0){
			tryout = tryout - 1;
			string t = to_string(tryout);
			char const *p = t.c_str();
			if (!fork()) { // this is the child process
				close(sockfd); // child doesn't need the listener

				send(new_fd, un, strlen(un), 0);
				send(new_fd, "received the result of authentication using TCP over port ", strlen("received the result of authentication using TCP over port "), 0);
				send(new_fd, PORT, strlen(PORT), 0);
				send(new_fd, "Authentication failed: Both Username and Password Does not exist \n Attempts remaining:", strlen("Authentication failed: Both Username and Password Does not exist \n Attempts remaining:"), 0);
				send(new_fd, p,strlen(p),0);

				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);
				exit(0);
			}
		}

		else if(strcmp(buff,"0") == 0){
			while(1){
			if(!fork()){
				close(sockfd);

				send(new_fd,"Di", strlen("Di"), 0);

				close(new_fd);

				exit(0);

			}
			if ((numbytes = recv(new_fd, buf, MAXBUFLEN-1, 0)) == -1){
				perror("recv");
				exit(1);
			}
			buf[numbytes] = '\0';
		    int psd = 0;
			char *pch;
			char * id, * cn, *ctg;
			//cout << buf << endl;
			pch = strtok(buf, ",");
			id= pch;
			pch = strtok(NULL, ",");
			cn= pch;
			pch = strtok(NULL,",");
			ctg = pch;
			char* msg = new char[strlen(cn)+strlen(ctg)+1];
			strcpy(msg, cn);
			strcat(msg, ",");
			strcat(msg, ctg);
			if(strcmp(id, "77") == 0){
				if((nb = sendto(ut, msg, strlen(msg), 0, (struct sockaddr*) &ee, sizeof(ee))==-1)){
					perror("talker: sendto");
					exit(1);
				}
				cout << "The main Server sent a request to serverEE. "<<endl;

			}
			if(strcmp(id, "78") == 0){
				if((nb = sendto(ut, msg, strlen(msg), 0, (struct sockaddr*) &cs, sizeof(cs))==-1)){
					perror("talker: sendto");
					exit(1);
				}
				cout << "The main Server sent a request to serverCS. "<<endl;
			}
			if ((numbytes = recvfrom(ut, buff, MAXBUFLEN-1 , 0,(struct sockaddr*) &their_addr, &addr_len)) == -1) {
				perror("recvfrom");
				exit(1);
			}
			if(strcmp(id, "77") == 0){
				cout << "The main server recived the response from serverEE using UDP over port 24004" << endl;}
			if(strcmp(id, "78") == 0){
				cout << "The main server recived the response from serverCS using UDP over port 24004" << endl;}	
			buff[numbytes] = '\0';
			if (strcmp(buff, "none") == 0){
				if(!fork()){
					close(sockfd);
					send(new_fd, "Didn't find the course: ", strlen( "Didn't find the course: "), 0);
					send(new_fd, cn, strlen(cn) ,0);
					close(new_fd);
					close(new_fd);
					exit(0);}

			}
			else{
			if(!fork()){
				close(sockfd);
				send(new_fd, "The ", strlen("The "), 0);
				send(new_fd, ctg, strlen(ctg), 0);
				send(new_fd, " of ", strlen(" of "), 0);
				send(new_fd, cn, strlen(cn), 0);
				send(new_fd, " is ", strlen(" is "), 0);
				send(new_fd, buff, strlen(buff), 0);
				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);
				close(new_fd);

				exit(0);
			}

			
			cout << "The main server sent the query information to the client." << endl;
		}



		if(tryout == 0){
			break;
		}
		}
		}
		}		

		//printf("talker: sent %d bytes to %s\n", nb, "serverC");
		//freeaddrinfo(servinfo);
		//close(udp);
		//close(new_fd);  // parent doesn't need this
		//freeaddrinfo(sinfo);
		

		
	
	freeaddrinfo(si);
	close(ut);
}



	//udp







	
	close(new_fd);
	close(sockfd);
	return 0;
}
