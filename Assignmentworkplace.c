#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/file.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define N_THREADS 4
#define SIZE 2000
int sum=0;

void *thread_func(void *args);
int sum;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
main()
{
    int fd;
    // open fd
    if ((fd = open("data.txt", O_RDONLY)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("Pid of process PID:%d\n", getpid());
    pthread_t threads[N_THREADS];

    int count[N_THREADS];

    for (int i = 0; i < N_THREADS; i++)
    {
        count[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &fd);
    }
    for(int i=0;i<N_THREADS;i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("The file contains %d character from a-z\n" , sum);
}

void *thread_func(void *args)
{
    int *fd = args;
    char buff[500];
    read(*fd, buff, 500);
    for (int i = 0; i < 500; i++)
    {
        if(buff[i]>=97 && buff[i]<=122)
        {
            sum += 1;
            
        }
    }
}
