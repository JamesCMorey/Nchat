#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include "server.h"

int client(char *port)
{
	struct addrinfo hints, *res, *iter;
	int sfd;
	char buf[4096] = "Hello, me.";

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(NULL, port, &hints, &res)) {
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

	send(sfd, buf, sizeof(buf), 0);

	freeaddrinfo(res);
	close(sfd);
	return 0;
}
