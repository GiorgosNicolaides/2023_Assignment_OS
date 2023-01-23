#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signum);

main()
{
    printf("PID:%d", getpid());
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