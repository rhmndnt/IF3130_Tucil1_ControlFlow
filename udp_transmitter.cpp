/*
 * File : udp_transmitter.h
 * Usage: transmitter [receiver IP] [receiver port] [message.txt]
 */

#include "udp_dcomm.h"
#include <sys/socket.h>
#include <unistd.h> // close socket
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> // inet_pton

/* file descriptor for connected socket */
int socketfd;

/* address structure */
struct sockaddr_in transmitterAddr; 
struct sockaddr_in receiverAddr;

/*
	Connect to the service running on host:port.
	This function return 0 on failure, non-zero on success. 
*/
int binding(char *host, int port) {

	printf("Membuat socket untuk koneksi ke %s:%d...\n", host, port);

	/* Construct the receiver sockaddr_ structure */
  memset(&transmitterAddr, 0, sizeof(transmitterAddr));
  transmitterAddr.sin_family = AF_INET; // AF_INET it family code for IP4
  transmitterAddr.sin_addr.s_addr = htonl(INADDR_ANY); // let OS choose interface for this socket by give INADDR_ANY 
  transmitterAddr.sin_port = htons(0); // let the OS pick any available port number by specifying port 0

  /* Create the socket */
	if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	  perror("Failed to create socket");
	  return 0;
	}

	if(bind(socketfd, (struct sockaddr *) &transmitterAddr, sizeof(transmitterAddr)) < 0) {
	  perror("Failed to bind socket");
	  return 0;
	}

	/* Construct the transmitter sockaddr_ structure */
  memset(&receiverAddr, 0, sizeof(receiverAddr));
  receiverAddr.sin_family = AF_INET; // AF_INET it family code for IP4
  inet_pton(AF_INET, host, &receiverAddr.sin_addr.s_addr);
  receiverAddr.sin_port = htons(port);

 	return 1;
}

int main(int argc, char *argv[]) {

	if (argc > 3) {
		int bind_success = binding(argv[1], atoi(argv[2]));	
		if (!bind_success)
			return 0;

		char *msg = "Hallo";
		if (!sendto(socketfd, msg, strlen(msg), 0, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr)))
			perror("sendto failed");

		close(socketfd); // destroy socket
	}
	else {
		printf("Usage : transmitter [receiver IP] [receiver port] [message.txt]\n");
	}

	return 0;
}
