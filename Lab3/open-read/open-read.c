/*
 * open-read.c
 *
 * Simple example of opening and reading to a file.
 *
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char *argv[])
{
   /*
    *
    * the attributes are:
    * - O_RDONLY: open the file for reading
    *
    */
   
   /*
   int fd = open("test.txt", O_RDONLY);
   if (fd < 0){
      printf("open error: %s\n", strerror(errno));
      return -1;
   }

   char buffer[128];
   memset(buffer,0,sizeof(buffer));
   */

  

   /* read the contents of the file */

   /*
    * -> read() (C System Call)
    *
    * ssize_t read(int fildes, void *buf, size_t nbytes);
    *
    * int fildes ->  The file descriptor of where to write the output. 
    *                Ex: file descriptor obtained from the open system call
    *                    (0)  standard input   
    *                    (1)  standard output
    *                    (2)  standard error                 
    *
   */ 

   /*
   int bytes_read = read(fd, buffer, sizeof(buffer));
   if (bytes_read < 0){
      printf("read error: %s\n", strerror(errno));
      return -1;
   }

   printf("%s",buffer);

   */

   /* close the file */
   //close(fd);

   FILE *fp = NULL;
   fp = fopen("test.txt", "r");

   char buffer[128];
   memset(buffer, 0, sizeof(buffer));

   /*
    * size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
    * 
    * ptr      − Pointer to a block of memory with a minimum size of (size*nmemb) bytes.
    * size     − Size (in bytes) of each element to be read.
    * nmemb    − Number of elements, each one with a size of "size" bytes.
    * stream   − Pointer to a FILE object that specifies an input stream.
    *
   */

   int bytes_read = fread(&buffer, sizeof(char), sizeof(buffer) /* = 128 */, fp);
   if (bytes_read < 0){
      printf("read error: %s\n", strerror(errno));
      return -1;
   }

   printf("%s\n",buffer);

   fclose(fp);

   return 0;
}
