#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/file.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define SIZE 2000
void signal_handler(int signum);
// declare mutex
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
main()
{
    // declare srand
    srand(time(NULL));

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

        { // open the file
            int fd, bytes;
            char buffer[SIZE];
            int buf[SIZE];
            if ((fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC), 6666) == -1) // check for errors during open
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

        waitpid(pid, &status, WNOHANG);
    }
    else if (pid == 0) // child code
    {
        sleep(1);
        printf("I'm child %d\n", getpid());
        exit(8);
    }
    sleep(1);
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