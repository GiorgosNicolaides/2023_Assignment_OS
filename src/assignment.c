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
int array[26];
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

main()
{
    int fd;
    // create a file
    if ((fd = open("bin/data.txt", O_CREAT, 0666)) == -1)
    {
        perror("open");
    }

    printf("PID:%d\n", getpid());
    {                                    // signal handling
        signal(SIGTERM, signal_handler); // handle kill-15
        signal(SIGINT, signal_handler);  // handle kill-2
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
        printf("Child running....\nPID:%d\n", getpid());

        child();
    }
    sleep(5);
}

void child()
{
    {
        int fd;
        // open fd
        if ((fd = open("bin/data.txt", O_RDONLY)) == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
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
        for (int i = 0; i < 26; i++)
        {
            printf("%c appears %d times\n", (97 + i), array[i]);
        }
        for (int i = 0; i < 26; i++)
        {
            sum += array[i];
        }

        printf("The file contains %d character from a-z\n", sum);

        close(fd);
    }
}

void signal_handler(int signum) // void to handle signals
{
    sigset_t mask;

    char response;
    printf("Are you sure you want to exit?\n");
    scanf("%c", &response);
    if (response != 'Y' | 'y')
    {
        exit(signum);
    }
    else
        sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);
}

void *thread_func(void *args)
{
    int *fd = args;
    char buff[500];
    read(*fd, buff, 500);

    int b;
    char c;
    for (int i = 0; i < 500; i++)
    {
        if (buff[i] >= 97 && buff[i] <= 122)
        {
            pthread_mutex_lock(&mymutex);
            c = buff[i];
            b = (int)c % 97;
            array[b]++;
            pthread_mutex_unlock(&mymutex);
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

        if ((fd = open("bin/data.txt", O_WRONLY)) == -1) // check for errors during open
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
        bytes = write(fd, buffer, sizeof(buffer));
        printf("Bytes were written SIZE:%d\n", bytes);
        close(fd);
    }
}