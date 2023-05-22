#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main() {
	int sockfd = socket(PF_INET,SOCK_STREAM, 0);
    // Create a socket and perform the necessary operations (e.g., connect)

    // Connect the socket	
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("10.12.255.255");
	serverAddress.sin_port = 6666;
    if (connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("connect");
        return -1;
    }

    struct sockaddr_in localAddress;
    socklen_t addressLength = sizeof(localAddress);
    getsockname(sockfd, (struct sockaddr *)&localAddress, &addressLength);

    // Retrieve and print the local port
    printf("Local Port: %d\n", ntohs(localAddress.sin_port));

    // Continue with other operations or close the socket
    return 0;
}
