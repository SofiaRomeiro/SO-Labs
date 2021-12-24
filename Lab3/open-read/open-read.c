/*
 * open-read.c
 *
 * Simple example of opening and reading to a file.
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

   /*
   FILE *fp = NULL;
   fp = fopen("test.txt", "r");

   char buffer[128];
   memset(buffer, 0, sizeof(buffer));
   */

   /*
    * size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
    * 
    * ptr      − Pointer to a block of memory with a minimum size of (size*nmemb) bytes.
    * size     − Size (in bytes) of each element to be read.
    * nmemb    − Number of elements, each one with a size of "size" bytes.
    * stream   − Pointer to a FILE object that specifies an input stream.
    *
   */

   /*
   int bytes_read = fread(&buffer, sizeof(char), sizeof(buffer), fp);
   if (bytes_read < 0){
      printf("read error: %s\n", strerror(errno));
      return -1;
   }

   printf("%s\n",buffer);

   fclose(fp);

   */

	char* buffer = NULL;
	size_t result;
	long lSize = 0;
	FILE *fp = NULL;
	FILE *wfp = NULL;

	// open the file
	fp = fopen("test.txt","r");

	// check if open was successfull
	if (fp == NULL) {
		printf("File error\n");
		exit (1);
	}

	/*
    * int fseek(FILE *stream, long int offset, int whence)
    *
    * https://www.geeksforgeeks.org/fseek-in-c-with-example/
	*
	* Sets the file position (fp) of the stream to the given offset.
    * 
    * stream   − Pointer to a FILE object that identifies the stream.
    * offset   − Number of bytes to offset from whence.
    * whence   − Position from where offset is added. It is specified by one of the following constants:
    *				--> SEEK_SET : Beginning of file
    *				--> SEEK_CUR : Current position of the file pointer
    *				--> SEEK_END : End of file
	*
   	*/

	fseek(fp, 0, SEEK_END); //starting at the beginning

	/*
    * long int ftell(FILE *stream)
	*
	* Returns the current file position (fp) of the given stream.
    * 
    * stream   − Pointer to a FILE object that identifies the stream.
	*
   	*/

	lSize = ftell (fp);

	printf("lSize: %ld\n", lSize);

	/*
    * void rewind(FILE *stream)
	*
	* Sets the file position to the beginning of the file of the given stream.
    * 
    * stream   − Pointer to a FILE object that identifies the stream.
	*
   	*/

	rewind(fp);

	buffer = (char *) malloc(lSize);

	result = fread(buffer, sizeof(char), lSize, fp);

	if (result != lSize) {
		printf ("Reading error 2"); 
		return 1;
	}

	wfp = fopen("test-out.txt", "w");

	if (wfp == NULL) {
		printf("Error opening wfp\n");
		return 1;
	}

	size_t write_size = fwrite(buffer, sizeof(char), strlen(buffer), wfp);

	printf("[+] write_size = %ld\n strlen(buffer = %ld\n", write_size, strlen(buffer));

	if (write_size < 0) {
		printf("Error writing the file\n");
		return 1;
	}

	fclose(wfp);
	fclose (fp);

	return 0;
}
