#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_PATHNAME "fifo.pipe"
#define ACK_PATHNAME "ack.pipe"

#define ACK_MSG "Received!"

#define BUFFER_SIZE (300)

int send_ack(int tx) {

    return ( write(tx, ACK_MSG, sizeof(ACK_MSG)) > 0 ) ? 0 : -1;
}

int main() {

    printf("[ + ] Starting client..\n");

    int tx = -2;

    // open pipe for reading
    // this waits for someone to open it for writing
    int rx = open(FIFO_PATHNAME, O_RDONLY);
    if (rx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    /*int tx = open(ACK_PATHNAME, O_WRONLY);
    if (tx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    */

    while (true) {
        char buffer[BUFFER_SIZE];
        memset(buffer, '\0', sizeof(buffer));
        ssize_t ret = read(rx, buffer, BUFFER_SIZE - 1);
        if (ret == 0) {
            // ret == 0 signals EOF
            fprintf(stderr, "[INFO]: pipe closed (%s)\n", FIFO_PATHNAME);
            return 0;
        } else if (ret == -1) {
            // ret == -1 signals error
            fprintf(stderr, "[ERR]: read failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        while(buffer[ret-1] != '\0') {
            ret = read(rx, buffer, BUFFER_SIZE - 1);
            if (ret == 0) {
                // ret == 0 signals EOF
                fprintf(stderr, "[INFO]: pipe closed (%s)\n", FIFO_PATHNAME);
                return 0;
            } else if (ret == -1) {
                // ret == -1 signals error
                fprintf(stderr, "[ERR]: read failed: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        fprintf(stderr, "[INFO]: received %zd B\n", ret);
        buffer[ret] = 0;
        fputs(buffer, stdout);

        int i = 0;
        for (i = 0; i < BUFFER_SIZE; i++) {
            if (buffer[i] == '\n') {
                i++;
                break;
            }
        }           

        printf("\n[ + ] Ack pipe : |%s|\n", buffer + i);  

        tx = open(buffer + i, O_WRONLY);

        if (tx == -1) {
            printf("[ERR] Failed opening ack pipe\n");
            exit(EXIT_FAILURE);
        }

        sleep(2);
        if (send_ack(tx) == -1) {
            printf("[ERR] Failed sending ack\n");
        }

    }

    close(tx);
    close(rx);
}
