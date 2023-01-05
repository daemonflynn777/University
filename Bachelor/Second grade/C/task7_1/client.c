#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define PORT 6666
#define MSG_MAX_LEN 128
#define STR_END "end\n"
#define STR_FINAL "shut_down\n"

void msg_snd(int sock, int pid)
{
	char *send_text = "  Hi. ";
	char str[MSG_MAX_LEN] = {0};
	char res[MSG_MAX_LEN] = {0};
	printf("Your session started!\n> ");
	int users_left = 0;
	while (fgets(str, MSG_MAX_LEN, stdin) == str)
	{
			if ((strncmp(str, STR_END, strlen(STR_END)) == 0) ||
					(users_left == 1) ||
					(strncmp(str, STR_FINAL, strlen(STR_FINAL)) == 0))
					if (users_left)
					{
							printf("Press Enter key one more time to leave.");
							users_left++;
					}
					else
					{
							printf("\n> ");
							users_left++;
					}
			else
					printf("\n> ");
			if (strlen(str) > 1024)
					printf("Your message is too long. Use maximum 2014 symbols!");
			else
			{
					int j = 0;
   	 			int i;

					for (i = 0; i<strlen(str) ; i++)
					{
	        		if ((str[i] == ' ') || (str[i] == '\t'))
							{
	         				if (j == 0) continue;
	          			if (str[i+1] == ' ') continue;
	          			if (str[i+1] == '\t') continue;
	        		}
	        		res[j] = str[i];
	        		j++;
   				}
					i = strlen(res);
    			if ((res[i-2] == ' ') || (res[i-2] == '\t'))
        			res[i-2] = '\0';
					if(send(sock, res, strlen(str)+1, 0) < 0)
					{
							printf("Could not send!");
							exit(1);
					}
		}
	}
	kill(pid, SIGKILL);
	printf("Session ended\n");
}

void msg_rcv(int sock)
{
	char buf[MSG_MAX_LEN] = {0};
	int not_kick = 1;
	int filled = 0;
	while ((filled = recv(sock, buf, MSG_MAX_LEN-1, 0)))
	{
		buf[filled] = '\0';
		if (strncmp(buf, "[BAN]", strlen("[BAN]")) == 0)
		{
			printf("%s", buf);
			not_kick = 0;
			kill(getppid(), SIGKILL);
			break;
		}
		printf("%s", buf);
		printf("> ");
		fflush(stdout);
	}
	if (not_kick)
		printf("You have been disconnected from server. Double-press ENTER to leave.\n");
}

char message[32];
char buf[64];

int main(int argc, char *argv[])
{
    int sock;
    int pid;
    struct sockaddr_in client_addr;
    struct in_addr server_addr;

    if (argc != 3)
		{
        printf("Wrong command! Try again\n");
				exit(1);
		}

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("Error occured!\n");
        exit(1);
    }

		if(!inet_aton(argv[2], &server_addr))
		{
				printf("inet_aton error\n");
				exit(1);
		}

    client_addr.sin_family = AF_INET;
		memcpy(&client_addr.sin_addr, &server_addr, sizeof(server_addr));
    client_addr.sin_port = htons(atoi(argv[1]));

    if(connect(sock, (const struct sockaddr*) &client_addr, sizeof(client_addr)) != 0)
    {
        printf("Could not connect!\n");
        exit(2);
    }

    printf("Hello!\n");

    if ((pid = fork()))
        msg_snd(sock, pid);
  	else msg_rcv(sock);

    return 0;
}
