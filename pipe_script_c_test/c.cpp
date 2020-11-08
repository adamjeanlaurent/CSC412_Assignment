
// C program to implement one side of FIFO 
// This side reads first, then reads 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <iostream>

void ReadPipe() 
{
    int fd1; 
  
    // FIFO file path 
    char * myfifo = "/tmp/myfifo"; 
  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>,<permission>) 
    mkfifo(myfifo, 0666); 
    int i = 0;
    char s1[50];
  
    // First open in read only and read 
    fd1 = open(myfifo,O_RDONLY); 
    read(fd1, s1, 50); 

    s1[strcspn(s1, "\n")] = 0;
    // Print the read string and close 
    printf("num: %s\n", s1); 
    printf("len: %lu\n", strlen(s1));
    close(fd1); 
    
}

void WritePipe()
{
    int fd1; 
  
    char * myfifo = "/tmp/myfifo"; 
  
    mkfifo(myfifo, 0666); 
    char s1[50] = "Hello From C++ Program\n";

    fd1 = open(myfifo, O_WRONLY); 
    
    write(fd1, s1, strlen(s1) + 1);

    
    close(fd1);
}


int main() 
{
    ReadPipe();
    ReadPipe();
    ReadPipe();
    return 0;  
} 