#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*

Under the same folder, file input.txt.
Each line contains a positive integer number and we guarantee the number is smaller than 1000.
You need to read the number in each line, add it by one, and write them line by line to another file called output.txt

NOTE: Please don't hardcode the results in your program because we change the content in input.txt when grading.

TIP: You can use sscanf to convert char array to int and sprintf to convert int to char array. 

*/
int main(){
    int in = open("input.txt",O_RDONLY);
    int out = open("output.txt",O_CREAT| O_WRONLY,S_IRUSR | O_TRUNC | S_IWUSR);
    if( in<0 ||out <0)
        exit(-1);

    char num[1000];
    int change[15];
    
    int offset1,offset2;
    read(in,num,100);

    char *val1=num;
    for(int i=0;i<10;i++){
        sscanf(val1, " %d%n", &change[i],&offset1);
        val1+=offset1;
        }

    sprintf(num, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", change[0]+1,change[1]+1,change[2]+1,change[3]+1,change[4]+1,change[5]+1,change[6]+1,change[7]+1,change[8]+1,change[9]+1);

    write(out,num,strlen(num));
    free(val1);
    close(in);
    close(out);
    return 0;

}