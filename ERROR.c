#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<pthread.h>
#include<sys/wait.h>

int info=0;//Info = 0时， 线程A工作，1时 线程B工作，2时，线程C工作 反之挂起

pthread_cond_t A;
pthread_cond_t B;
pthread_cond_t C;

pthread_mutex_t lock;
pthread_rwlock_t rwlock;

char bufer_A[1024];
char bufer_B[1024];
//int fd;
int dest_fd;
FILE * fp;

void* THREAD_A(void* arg)
{
	pthread_detach(pthread_self());  
	while(1)
	{
		pthread_mutex_lock(&lock);     
		if(info!=0)
			pthread_cond_wait(&A,&lock);   
		if(fgets(bufer_A, 1024, fp) == NULL)  
			exit(0);
		info=1;
		pthread_mutex_unlock(&lock);   
		pthread_cond_signal(&B);    
	}
	pthread_exit(NULL);

}

void* THREAD_B(void* arg)
{
         pthread_detach(pthread_self());
         while(1)
         {
                pthread_mutex_lock(&lock);
                if(info!=1)
                         pthread_cond_wait(&B,&lock);
                char char1[7]="E CamX";
		char char2[13]="E CHIUSECASE";
		if(strstr(bufer_A,char1)==NULL&&strstr(bufer_A,char2)==NULL){
			memset(bufer_A,0,sizeof(bufer_A));    
		}else{
			strcpy(bufer_B,bufer_A);       
			printf("strcpy bufer_B = %s\n",bufer_B);
		}
		memset(bufer_A,0,sizeof(bufer_A));

 
                 info=2;
                 pthread_mutex_unlock(&lock);
                 pthread_cond_signal(&C);
         }
         pthread_exit(NULL);

}

void* THREAD_C(void* arg)
{
	pthread_detach(pthread_self());
        while(1)
        {
		pthread_rwlock_rdlock(&rwlock);
                if(info!=2)
			pthread_cond_wait(&C,&lock);
                write(dest_fd,bufer_B,strlen(bufer_B));
		memset(bufer_B,0,sizeof(bufer_B)); 
                info =0; 
                pthread_rwlock_unlock(&rwlock);
                pthread_cond_signal(&A);
          }
          pthread_exit(NULL);
 
}

int main(void)
{
	fp = fopen("ERROR.log","r");
	dest_fd = open("Result.log",O_RDWR|O_CREAT,0664);	

        pthread_mutex_init(&lock,NULL);
	pthread_rwlock_init(&rwlock,NULL);
        pthread_cond_init(&A,NULL);
        pthread_cond_init(&B,NULL);
        pthread_cond_init(&C,NULL);

	pthread_t tids[3];
	pthread_create(&tids[0],NULL,THREAD_A,NULL);
	pthread_create(&tids[1],NULL,THREAD_B,NULL);
	pthread_create(&tids[2],NULL,THREAD_C,NULL);
		
	while(1)
		sleep(1);	
	
	pthread_mutex_destroy(&lock);
	pthread_rwlock_destroy(&rwlock);
	pthread_cond_destroy(&A);
	pthread_cond_destroy(&B);
	pthread_cond_destroy(&C);

}
