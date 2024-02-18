#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "server.h"

#define SERVERPORT "0"

int server(void)
{
	char buf[4096];
	int sfd, cfd;

	sfd = get_server_socket();

	cfd = accept(sfd, NULL, NULL);
	recv(cfd, buf, sizeof(buf), 0);

	printf("Message received:\n%s\n", buf);

	close(cfd);
	close(sfd);
	return 0;
}

int get_server_socket()
{
	struct addrinfo hints, *res, *iter;
	int sfd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, SERVERPORT, &hints, &res)) {
		perror("getaddrinfo error");
		exit(1);
	}

	// Loop through *res list till an available/working address is found
	for (iter = res; iter != NULL; iter = iter->ai_next) {
		sfd = socket(iter->ai_family, iter->ai_socktype,
							iter->ai_protocol);
		if (sfd == -1) {
			continue;
		}

		if(bind(sfd, iter->ai_addr, iter->ai_addrlen)) {
			close(sfd);
			perror("bind error");
			continue;
		}

		break;
	}

	// exit if iter is NULL bc that means it went through all of *res
	if (iter == NULL) {
		perror("Failed to create or bind socket");
		exit(1);
	}


	// print port of sfd
	struct sockaddr_in test;
	socklen_t tmp = sizeof(test);
	getsockname(sfd, (struct sockaddr *)&test, &tmp);
	printf("port: %d\n", ntohs(test.sin_port));

	// make port listen
	if (listen(sfd, 20)) {
		perror("listen error");
		exit(1);
	}

	return sfd;
}
