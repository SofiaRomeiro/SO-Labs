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

// helper function to send messages
// retries to send whatever was not sent in the begginning
void send_msg(int tx, char const *str, char *pipe_name) {
    size_t len = strlen(str);
    size_t written = 0;

    ssize_t ret;

    while (written < len) {
        ret = write(tx, str + written, len - written);
        if (ret < 0) {
            fprintf(stderr, "[ERR]: write failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        written += ret;
    }

    size_t written_pipename = 0;
    len += (strlen(pipe_name) + 1);

    printf("[ + ] Ack name in sender : %s\n", pipe_name);

    while (written < len) {
        ret = write(tx, pipe_name + written_pipename, len - written);
        if (ret < 0) {
            fprintf(stderr, "[ERR]: write failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        written += ret;
        written_pipename += ret;
    }
    
    if (ret < 0) {
        fprintf(stderr, "[ERR]: write failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int wait_ack(int rx, int msg) {

    size_t size = sizeof(ACK_MSG);

    char buffer[size + 1];
    ssize_t ret = read(rx, buffer, sizeof(buffer));

    if (ret == -1) {
        fprintf(stderr, "[ERR]: read failed : %s\n", strerror(errno));
        return -1;
    }

    buffer[size] = 0; // similar to '\0'
    printf ("Ack signal %d : %s\n", msg, buffer);

    return memcmp(ACK_MSG, buffer, sizeof(ACK_MSG));
}

int main(int argc, char **argv) {

    printf("[ + ] Starting server..\n");

    char *ack_pipe;

    if (argc > 1) {
        ack_pipe = argv[1];
    }
    else {
        ack_pipe = "pipe.name";
    }

    // remove pipe if it does not exist
    if (unlink(FIFO_PATHNAME) != 0 && errno != ENOENT) {
        fprintf(stderr, "[ERR]: unlink(%s) failed: %s\n", FIFO_PATHNAME,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    // remove ack pipe if it does not exist
    if (unlink(ack_pipe) != 0 && errno != ENOENT) {
        fprintf(stderr, "[ERR]: unlink(%s) failed: %s\n", ack_pipe,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    // create pipe
    // Permissions : -rw- r-- ---
    if (mkfifo(FIFO_PATHNAME, 0640) != 0) {
        fprintf(stderr, "[ERR]: mkfifo failed (%s): %s\n", FIFO_PATHNAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // create ack pipe
    // Permissions : -rw- rw- ---
    if (mkfifo(ack_pipe, 0660) != 0) {
        fprintf(stderr, "[ERR]: mkfifo failed (%s): %s\n", ack_pipe, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // open pipe for writing
    // this waits for someone to open it for reading - implementation of "open" 
    // function cuz it recognizes that the file is "special"
    int tx = open(FIFO_PATHNAME, O_WRONLY);
    if (tx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // The parent is likes classic rock:
    // https://www.youtube.com/watch?v=lrpXArn3hII
    send_msg(tx, "Tell me now\n", ack_pipe);

    int rx = open(ack_pipe, O_RDONLY);
    if (rx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (wait_ack(rx, 0) != 0) {
        printf("[ERR] Ack 0 failed\n");
        exit(EXIT_FAILURE);
    }    

    sleep(3);

    send_msg(tx, "Is he good to you?\n", ack_pipe);
    if (wait_ack(rx, 1) != 0) {
        printf("[ERR] Ack 1 failed\n");
        exit(EXIT_FAILURE);
    }

    sleep(3);

    send_msg(tx, "Can you make you the meals that I do?\n", ack_pipe);
    if (wait_ack(rx, 2) != 0) {
        printf("[ERR] Ack 2 failed\n");
        exit(EXIT_FAILURE);
    }

    sleep(3);

    fprintf(stderr, "[INFO]: closing pipe (%s)\n", FIFO_PATHNAME);
    close(tx);
}
