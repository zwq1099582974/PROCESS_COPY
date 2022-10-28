#include <PROCESS_COPY.h>

int PROCESS_CREATE(const char* Sfile, const char* Dfile, int prono, int blocksize)
{
	pid_t pid;
	int flag;
	for(flag=0;flag<prono;flag++){
		pid=fork();
		if(pid == 0)
		  break;
	}

	if(pid>0){
		printf("Parent process %d waiting...\n",getpid());
		PROCESS_WAIT();
	}
	else if(pid==0)
	{
		char STRPOS[100];
		char STRBLOCKSIZE[100];
		bzero(STRPOS,100);
		bzero(STRBLOCKSIZE,100);
		int POS;
		POS=flag*blocksize;
		sprintf(STRPOS,"%d",POS);
		sprintf(STRBLOCKSIZE,"%d",blocksize);
		printf("child process %d pos [%d]\n",pid,POS);

		execl("/home/0103/testLinux/PROCESS_COPY/moudle/COPY","COPY",Sfile,Dfile,STRBLOCKSIZE,NULL);
	}
	else{
		perror("fork call failed");
		exit(0);
	}
	return 0;
}
