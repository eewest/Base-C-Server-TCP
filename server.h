/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   server.h
 * Author: eewest
 *
 * Created on April 28, 2017, 6:18 PM
 */

#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DEF_BUFFER_LENGTH 5000

void error(const char *msg, int errVal){
	perror(msg);
	exit(errVal);
}

int prepareServer(struct sockaddr_in *sAddr, int *socketDesc, int port){
	sAddr = (struct sockaddr_in*)calloc(1, sizeof(struct sockaddr_in));
	bzero(sAddr, sizeof(struct sockaddr_in));
	
	sAddr->sin_family = AF_INET;
	sAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	sAddr->sin_port = htons(port);
	
	*socketDesc = socket(AF_INET, SOCK_STREAM, 0);
	if(socketDesc < 0){
		printf("Error creating socket\n");
		return -2;
	}
	
	if( bind(*socketDesc, (struct sockaddr *)sAddr, sizeof(struct sockaddr_in)) < 0 ){
		error("Error: binding socket and addresss failed.", -1);
		return -1;
	}
	
	printf("Socket has been bound!\n");
	//printf("Ready to start listening on port %d\n", sAddr->sin_port);
	
	return 0;
}

int runServer(int serverSock){
	struct sockaddr_in *clientAddr = 0;
	socklen_t clientLen = sizeof(struct sockaddr_in);
	char *buffer = (char *) calloc(500, sizeof(char));
        listen(serverSock, 2);
        while(1){
            int client = accept(serverSock, (struct sockaddr *)clientAddr, &clientLen);
            if(client < 0){
                    error("Could not accept incoming connection", client);
            }
            int childPID = fork();
            if(childPID >= 0){
                if(childPID == 0){
                    printf("Connection established.\n");
                    while(1){

                        int res = recv(client, buffer, DEF_BUFFER_LENGTH, 0);
                        if(res > 0){
                            printf("Recieved: %s\n", buffer);
                            res = send(client, buffer, res, 0);
                            if(res < 0){
                                    printf("Send failed!\n");
                            }else{
                                    printf("Echoing back: %s", buffer);
                            }
                        }else{
                            printf("closing socket.\n");
                            close(serverSock);
                            return 0;
                        }
                    }
                }else{
                    printf("Connection sent to fork() with pid: %d", childPID);
                }
            }
        }
	return 0;
}


#endif

