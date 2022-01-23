#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE (128)
#define ACK_BYTE "Yes"

// reads messages ( requests if the communication is server-client like ) from the parent
int child_main(int rx, int tx) {
    while (true) {
        char buffer[BUFFER_SIZE];
        ssize_t ret = read(rx, buffer, BUFFER_SIZE - 1); // rx is a open file handler

        if (ret == 0) {
            // ret == 0 signals EOF
            fprintf(stderr, "[INFO]: parent closed the pipe\n");
            break;
        } else if (ret == -1) {
            // ret == -1 signals error
            fprintf(stderr, "[ERR]: read failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "[INFO]: parent sent %zd B\n", ret);
        buffer[ret] = 0; // similar to '\0'
        fputs(buffer, stdout); // writes the buffer on the stdout

        if (write(tx, ACK_BYTE, sizeof(ACK_BYTE)) == -1) {
            fprintf(stderr, "[ERR]: write ack failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

    }

    close(rx);
    close(tx);
    return 0;
}

// helper function to send messages
// retries to send whatever was not sent in the begginning
void send_msg(int tx, char const *str) {
    size_t len = strlen(str);
    size_t written = 0;

    while (written < len) {
        ssize_t ret = write(tx, str + written, len - written);  //tx is the file handler to the parent process write the answers 
        if (ret < 0) {
            fprintf(stderr, "[ERR]: write failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        written += ret;
    }
}

int wait_ack(int rx, int msg) {

    size_t size = sizeof(ACK_BYTE);

    char buffer[size + 1];
    ssize_t ret = read(rx, buffer, sizeof(buffer));

    if (ret == -1) {
        fprintf(stderr, "[ERR]: read failed : %s\n", strerror(errno));
        return -1;
    }

    buffer[size] = 0; // similar to '\0'
    printf ("Ack signal %d : %s\n", msg, buffer);

    return memcmp(ACK_BYTE, buffer, sizeof(ACK_BYTE));

}

int parent_main(int tx, int rx) {
    // The parent is likes classic rock:
    // https://www.youtube.com/watch?v=lrpXArn3hII
    send_msg(tx, "Hey little girl\n");
    if (wait_ack(rx, 0) != 0)
        return -1;
    sleep(1);

    send_msg(tx, "Is your daddy home?\n");
    if (wait_ack(rx, 1) != 0)
        return -1;
    sleep(1);

    send_msg(tx, "Did he go and leave you all alone?\n");
    if (wait_ack(rx, 2) != 0)
        return -1;
    sleep(1);

    fprintf(stderr, "[INFO]: closing pipe\n");
    close(tx);

    close(rx);

    // parent waits for the child
    wait(NULL);
    return 0;
}

int main() {
    int filedes[2];
    int filedes2[2];

    if (pipe(filedes) != 0) {
        fprintf(stderr, "[ERR]: pipe() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pipe(filedes2) != 0) {
        fprintf(stderr, "[ERR]: pipe() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) { // this is the child

        // we need to close the ends we are not using
        // otherwise, the child will be perpetually waiting for a message that
        // will never come

        close(filedes[1]);
        close(filedes2[0]);
        return child_main(filedes[0], filedes2[1]);
    } 
    else {
        close(filedes[0]);
        close(filedes2[1]);
        return parent_main(filedes[1], filedes2[0]);
    }
}
