#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include "client.h"

#define SERVER "irc.rizon.net"
#define PORT "6667"

int client(char *hostname, char *port)
{
	char rawbuf[512];
	pthread_t listener;
	int sfd;

	sfd = get_server_conn(hostname, port);
	pthread_create(&listener, NULL, &get_server_msgs, &sfd);

	// take user input and send it to the server
	while(1) {
		fgets(rawbuf, sizeof(rawbuf), stdin);
		send(sfd, rawbuf, strlen(rawbuf), 0);
	}


	// prevent function from returning when hardcoded examples run
	pthread_join(listener, NULL);
	return 0;
}

void *get_server_msgs(void *sfd)
{
	char buf[512];
	int size;

	//memset(buf, 0, sizeof(buf));
	while (1) {
		if ((size = recv(*((int *)sfd), buf, sizeof(buf) - 1, 0))) {
			buf[size] = '\0';
			printf("%s", buf);
			//memset(buf, 0, sizeof(buf));
		}
	}
	return 0;
}

int get_server_conn(char *server, char *port)
{
	struct addrinfo hints, *res, *iter;
	int sfd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(server, port, &hints, &res)) {
		perror("getaddrinfo error");
		exit(1);
	}


	struct sockaddr_in info;
	socklen_t info_size = sizeof(info);

	// Loop through *res list till an available/working address is found
	for (iter = res; iter != NULL; iter = iter->ai_next) {
		sfd = socket(iter->ai_family, iter->ai_socktype,
							iter->ai_protocol);
		if (sfd == -1) {
			perror("socket");
			exit(1);
		}

		if (connect(sfd, iter->ai_addr, iter->ai_addrlen)) {
			perror("connect");
			continue;
		}

		// print client port number
		getsockname(sfd, (struct sockaddr *)&info, &info_size);
		printf("port: %d\n", htons(info.sin_port));

		break;
	}

	if (iter == NULL) {
		perror("Could not bind");
		exit(1);
	}


	freeaddrinfo(res);
	return sfd;
}
