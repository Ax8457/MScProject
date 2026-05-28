// HWU MSc project
/*
*	NFSv4 Data-In-Flight Encryption over Noise protocol Framework 
*
*	Axel Biegalski
*/

/*
	Libs
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include "noise_crypto.h"
#include "client.h"
#include "handshake.h"

/*
	Functions
*/
//bind server
void bind_server(struct noise_peer *peer){
	//vars to handle messages
	struct ikpsk2_msg1 *m1 = calloc(1, sizeof(struct ikpsk2_msg1));
	//pipes
	int fc2s_fd = open("/tmp/" FIFO_C2S, O_WRONLY);    
	int fs2c_fd = open("/tmp/" FIFO_S2C, O_RDONLY);
	if (fc2s_fd < 0 || fs2c_fd < 0) {
		printf("[ERROR] Value of errno: %d\n", errno);
		perror("[x] Failed to open pipes");
		return;
	}
	
	//polling mec
	struct pollfd fds[2];
	fds[0].fd = fs2c_fd;
	fds[0].events = POLLIN;
	fds[1].fd = 0;
	fds[1].events = POLLIN;

	printf("binding server.\n");

	if (peer->handshake.state == HANDSHAKE_ZEROED){
		//message1
		noise_handshake_create_initiation(m1, &peer->handshake);
		printf("handshake initiation ok.\n");	
		//send message and update state
		if (write(fc2s_fd, m1, sizeof(struct ikpsk2_msg1)) > 0) {
            printf("handshake initiation sent to server.\n");
            peer->handshake.state = HANDSHAKE_CREATED_INITIATION;       
        } else {
            perror("[x] Failed to send handshake initiation");
            free(m1);
            return;
        }		
	}
	//free struct allocated
	free(m1);

	//process handshake
	while(1) {
		//dependeing on the state => look at 2 pipes
		int nfds = (peer->handshake.state == COMM) ? 2 : 1;
        int ret = poll(fds, nfds, -1); //while nothing appends => block
        if (ret < 0) {
            printf("[ERROR] Value of errno: %d\n", errno);
            perror("[x] Poll error");
            break;
        }
		//handle different states of the handshake
		if (fds[0].revents & POLLIN) {
			switch (peer->handshake.state) {

				case HANDSHAKE_CREATED_INITIATION: {}

				case COMM: {}

				default:
					printf("[WARNING] Unexpected packet or state condition (%d)\n", peer->handshake.state);
					break;
			}
		}
	}
	close(fc2s_fd);
	close(fs2c_fd);
}



