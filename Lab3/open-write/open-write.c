/*
 * open-write.c
 *
 * Simple example of opening and writing to a file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char *argv[]) {

   /*
    * the attributes are:
    * - O_CREAT: create the file if it does not exist
    * - O_TRUNC: erase the contents of the file
    * - O_WRONLY: open the file for writting
    *
    * the permissions are applied if the file is created:
    * - S_IRUSR: reading, user
    * - S_IWUSR: writting, user
    */

   /*
    * -> open() (C System Call)
    *
    * int open(const char *path, int oflags, mode_t mode);
    * int open(const char *path, int oflags);
    *
   */ 

  
   int fd = open("test.txt", O_CREAT | O_TRUNC | O_WRONLY , S_IRUSR | S_IWUSR); 
   if (fd < 0){
      printf("open error: %s\n", strerror(errno));
      return -1;
   }

   char buffer[] = "Hello World!\n";


   /* write a string to the file */

   /*
    * -> write() (C System Call)
    *
    * ssize_t write(int fildes, const void *buf, size_t nbytes);
    *
    * int fildes ->  The file descriptor of where to write the output. 
    *                Ex: file descriptor obtained from the open system call
    *                    (0)  standard input   
    *                    (1)  standard output
    *                    (2)  standard error                 
    *
   */ 

 
   int bytes_written = write(1, buffer, strlen(buffer));
   if (bytes_written < 0){
      printf("write error: %s\n", strerror(errno));
      return -1;
   }


   /* close the file */
   close(fd);

   /* USING STDIO FUNCTIONS */

   /*
    *
    * FILE *fopen(const char *filename, const char *mode)
    *
   */

   /*
   FILE *fp = NULL;
   fp = fopen("test.txt", "w");

   if (!fp) {
      exit(1);
   }

   char buffer[] = "Hello World!\n";
   */

   /*
    * size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
    * 
    * ptr      − Pointer to the array of elements to be written.
    * size     − Size (in bytes) of each element to be written.
    * nmemb    − Number of elements, each one with a size of "size" bytes.
    * stream   − Pointer to a FILE object that specifies an output stream.
    *
   */

   /*
   int bytes_written = fwrite(buffer, sizeof(char), strlen(buffer), fp);

   if (bytes_written < 0) {
      printf("write error: %s\n", strerror(errno));
      exit(1);
   }

   fclose(fp);
   */

   return 0;
}
