#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
	int sleep_sec;
	if (argc < 2){
		printf("Usage: sleep seconds\n");
		exit(-1);
	}

        sleep_sec = atoi(argv[1]);

        printf("nighty nighty!\n");
        sleep(sleep_sec);
        exit(0);

}
