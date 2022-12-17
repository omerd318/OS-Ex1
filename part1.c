
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#define GODEL 10 


int main(int argc , char * argv[])
{
    if(argc != 3)
    {
        
        exit(-1);
    }

    char* filename1 = argv[1];
    char* filename2 = argv[2];
    char* addr1;
    char* addr2;
    struct stat stat_p1;
    struct stat stat_p2;
    int fileSize1=0,fileSize2=0;
    char buf1[GODEL+1];
    char buf2[GODEL+1];
    int samefile ;

    if(stat(filename1,&stat_p1) == -1)
    {
        
        exit(-2);
    }

    if(stat(filename2,&stat_p2) == -1)
    {
        
        exit(-2);
    }

    if((stat_p1.st_size) != (stat_p2.st_size)) //checks if the size is different than its not the same file.
    {

        return 1;
    }
    else
    {
        int fd1,fd2;
       
        if((fd1 = open(filename1 ,O_RDONLY)) < 0)
        {
            
            exit(-3);
        }
        if((fd2 = open(filename2 ,O_RDONLY)) < 0)
        {
            
            exit(-3);
        }

        int one = read(fd1,buf1,GODEL); 
        int two = read(fd2,buf2,GODEL);
        if((stat_p1.st_size)<GODEL){
int samefile = memcmp(buf1,buf2,GODEL);
if (samefile != 0)
{
    
    return(1);
}

}

   while ( (one == GODEL) && (two == GODEL)){
int samefile = memcmp(buf1,buf2,GODEL);
if (samefile != 0)
{
    
    return(1);
}else{

one = read(fd1,buf1,GODEL);
two = read(fd2,buf2,GODEL);

}


}
 
       
        close(fd1);
        close(fd2);
    }
     
    return 2;

}
