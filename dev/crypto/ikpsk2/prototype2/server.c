// HWU MSc project
/*
* NFSv4 Data-In-Flight Encryption over Noise protocol Framework 
*
* Axel Biegalski
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
#include <stdlib.h>
#include "noise_crypto.h"
#include "server.h"  
#include "handshake.h"

/*
    Functions
*/

//Create two pipes <=> biderectional comm 
//1 pipe client to server (read only for server process and read + write for client process
//1 pipe server to client (same but conversely)
void create_named_pipe(const char *fifo_name)
{    
    char fifo_path[128];
    snprintf(fifo_path, sizeof(fifo_path), "/tmp/%s", fifo_name);
    if (mkfifo(fifo_path, 0666) < 0){
    	printf("[ERROR] Value of errno: %d\n", errno);
        perror("[x] Failed to create named pipe");
    }
    else{
    	//printf("[DEBUG] Value of errno: %d\n", errno);
        printf("[OK] Success,named pipe created: %s\n", fifo_path);
    }   
}

//start server 
void start_server(struct noise_peer *peer) {
    //create structs 
    struct ikpsk2_msg1 *m1 = calloc(1, sizeof(struct ikpsk2_msg1));
    struct ikpsk2_msg2 *m2 = calloc(1, sizeof(struct ikpsk2_msg2));
    if (!m1 || !m2) {
        perror("[x] Memory allocation failed");
        free(m1);
        free(m2);
        return;
    }

    //open pipes 
    int fc2s_fd = open("/tmp/" FIFO_C2S, O_RDONLY);    
    int fs2c_fd = open("/tmp/" FIFO_S2C, O_WRONLY);
    
    if (fc2s_fd < 0 || fs2c_fd < 0) {
        printf("[ERROR] Value of errno: %d\n", errno);
        perror("[x] Failed to open pipes");
        free(m1);
        free(m2);
        if (fc2s_fd >= 0) close(fc2s_fd);
        if (fs2c_fd >= 0) close(fs2c_fd);
        return;
    }

    printf("server ready.\n");

    //polling mec
    struct pollfd fds[2];
    fds[0].fd = fc2s_fd; 
    fds[0].events = POLLIN;
    fds[1].fd = 0;       
    fds[1].events = POLLIN;
    //update state 
    peer->handshake.state = HANDSHAKE_ZEROED;

    while(1) {
        int nfds = (peer->handshake.state == COMM) ? 2 : 1;
        int ret = poll(fds, nfds, -1);
        
        if (ret < 0) {
            printf("[ERROR] Value of errno: %d\n", errno);
            perror("[x] Poll error");
            break;
        }

        if (fds[0].revents & POLLIN) {
            switch (peer->handshake.state) {

                case HANDSHAKE_ZEROED: {
                    ssize_t bytes_read = read(fc2s_fd, m1, sizeof(struct ikpsk2_msg1));
                    if (bytes_read <= 0) {
                        perror("[x] Failed to read Message 1 or client disconnected");
                        goto end_loop;
                    }
                    printf("message1 received.\n");
                    //consume initiation
                    handshake_consume_initiation(m1, peer);

                    printf("message1 consumed.\n");
                    printf ("state updated.\n");

                    //create response 
                    handshake_create_response(m2, peer);
                    printf("response created.\n");
                    break;
                }

                case COMM: {
                }

                default:
                    printf("[WARNING] Unexpected state condition (%d)\n", peer->handshake.state);
                    goto end_loop;
            }
        }
    }

end_loop:
    free(m1);
    free(m2);
    close(fc2s_fd);
    close(fs2c_fd);
}