#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int socket(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, n;
    struct sockaddr_in serv_addr; 
	
    char sendBuff[1025];
    char recvBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(2014); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
	
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
	 
    while(1)
    {
       

        ticks = time(NULL);
	puts("ticks");
        snprintf(sendBuff, sizeof(sendBuff), "Greetings from server, the time is: %.24s\r\n", ctime(&ticks));
	puts("snprintf");
        write(connfd, sendBuff, strlen(sendBuff)); 
	puts("write");
	
	memset(recvBuff, '\0', strlen(recvBuff));
	
	if ((n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0){
		printf("Message size of %d from server: %.*s\n", (int)strlen(recvBuff), (int)strlen(recvBuff), recvBuff);

		if(strcmp(recvBuff,"Quit")==0 ||strcmp(recvBuff,"quit")==0){
			printf("Quit request by client");
			break;
		}
	}	

	printf("Msg length: %d\n", n);
	
        sleep(1);

     }

	close(connfd);
	
	return(1);

}
