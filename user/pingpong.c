#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//#include "user/ulib.c"

int main(void) {

    int pipefd[2];
    char buf;

    if(pipe(pipefd) == -1){

        printf("pipe error");
        exit(-1);

    }

    int cpid = fork();
    int ppid = getpid();
    switch(cpid) {

    case -1:
        printf("there is an error");
        exit(-1);
    case 0:
        //printf("child!");
        //close(pipefd[1]); // close unused write end
        while( read(pipefd[0],&buf,5) > 0 )
            printf("%d: received ping\n",getpid());
        write(pipefd[1],"pong",5);
        close(pipefd[1]); // EoF
        exit(0);
    default:
        //printf("Child PID: %d\n",cpid);
        //printf("we are in parent\n");
        //close(pipefd[0]); // close unused read end
        write(pipefd[1],"ping",5);
        wait(0);
        close(pipefd[1]);
        while( read(pipefd[0],&buf,5) > 0 )
            printf("%d: received pong\n",ppid);

        //wait(0);
        //exit(0);

    }


    exit(0);


}
