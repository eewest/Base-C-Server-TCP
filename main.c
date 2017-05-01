/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: eewest
 *
 * Created on April 28, 2017, 6:18 PM
 */
#include "server.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main() {
    struct sockaddr_in *serverAddress = 0;
    char *message = 0;
    int socket;

    int result = prepareServer(serverAddress, &socket, 8881);
    if(result != 0){
            printf("Address initialization failed!\n");
    }else{
            printf("Address initialization succeeded!\n");
            runServer(socket);
    }
}

