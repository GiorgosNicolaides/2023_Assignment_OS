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

void signal_handler(int signum);
void *thread_func(void *args);
void child();
void parent();
int sum = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

main()
{
    int fd;
    //create a file 
    if((fd=open("data.txt" , O_CREAT, 0666)) == -1)
    {
        perror("open");
    }

    printf("PID:%d\n", getpid());
    { // signal handling
        int signal_ign;
        for (signal_ign = 0; signal_ign < 31; signal_ign++)
        {
            if (signal_ign == 2)
                signal(SIGINT, signal_handler); // handle kill-2
            else if (signal_ign == 9)
                signal(SIGTERM, signal_handler); // handle kill-15
            else
                signal(signal_ign, SIG_IGN); // ignore all other signals
        }
    }
    int pid, status;
    if ((pid = fork()) == -1) // check for errord uring fork
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid != 0) // parents code
    {
        printf("Parent process .....\n");
        parent();
        waitpid(pid, &status, WNOHANG);
    }
    else if (pid == 0) // child code
    {
        sleep(4);
        child();
    }
    sleep(4);
}

void child()
{
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
        for (int i = 0; i < N_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }

        printf("The file contains %d character from a-z\n", sum);
        close(fd);
    }
}

void signal_handler(int signum) // void to handle signals
{
    char response;
    printf("\nAre you sure you want to exit?\n");
    scanf("%c", &response);
    if (response == 'Y' | 'y')
    {
        exit(signum);
    }
}

void *thread_func(void *args)
{
    int *fd = args;
    char buff[500];
    read(*fd, buff, 500);
    for (int i = 0; i < 500; i++)
    {
        if (buff[i] >= 97 && buff[i] <= 122)
        {
            sum += 1;
        }
    }
}

void parent()
{
    { // open the file
        srand(time(NULL));
        int fd, bytes;
        int buf[SIZE];
        char buffer[SIZE];

        if ((fd = open("data.txt", O_WRONLY )) == -1) // check for errors during open
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        // write to the file
        for (int i = 0; i < SIZE; i++)
        {
            buf[i] = (rand() % (122 - 97 + 1)) + 97;
            buffer[i] = buf[i];
        }
        bytes = write(fd, buffer, 2000);
        printf("Bytes were written SIZE:%d\n", bytes);
        close(fd);
    }
}