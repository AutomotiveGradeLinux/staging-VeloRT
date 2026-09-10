//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

void
error(const char *msg)
{
	perror(msg);
	exit(1);
}

int
server(int portno)
{
	int                sockfd, newsockfd;
	socklen_t          clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int                n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port        = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <
	    0) {
		error("ERROR on binding");
	}
	listen(sockfd, 5);
	clilen    = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		error("ERROR on accept");
	}
	n = write(newsockfd, "Hello Client!", 13);
	if (n < 0)
		error("ERROR writing to socket");
	close(newsockfd);
	close(sockfd);
	return 0;
}
