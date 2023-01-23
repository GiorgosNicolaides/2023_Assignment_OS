#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signum);

main()
{
    printf("PID:%d\n", getpid());
    {//signal handling
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
    int pid;
    if((pid=fork()) == -1)//check for errorduring fork
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid != 0)//parents code
    {
        printf("I'm parent %d\n",getpid());
        wait(0);
        printf("My child is %d\n" , pid);
    }
    else if(pid == 0)//child code 
    {
        printf("I'm child %d\n",getpid());
        exit(0);
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