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

#define MSG_MAX_LEN 4096
#define STR_END "end"
#define STR_FINAL "shut_down"
#define MSG_NOSIGNAL SO_NOSIGPIPE

#define perro(x) {fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, x, strerror(errno));exit(1);}



void send_cmd(int sock, int pid)
{
	char str[MSG_MAX_LEN] = {0};
	char res[MSG_MAX_LEN] = {0};
	int leave_counter = 0;
	printf("> ");
	while (fgets(str, MSG_MAX_LEN, stdin) == str)
	{
		if ((strncmp(str, STR_END, strlen(STR_END)) == 0) || (leave_counter == 1) || (strncmp(str, STR_FINAL, strlen(STR_FINAL)) == 0))
			if (leave_counter)
			{
				printf("> We will miss you (press ENTER one more time)");
				leave_counter++;
			}
			else
			{
				printf("\n> ");
				leave_counter++;
			}
		else
			printf("\n> ");
		if (strlen(str) > 1024)
			printf("Your sentence is too big. Use maximum 2014 symbols!");
		else
		{
			int j = 0;
   	 	int i;
    	for (i = 0; i<strlen(str) ; i++)
			{
	    	if ((str[i] == ' ') || (str[i]=='\t'))
				{
	      	if (j==0)
							continue;
	        if (str[i+1] == ' ')
							continue;
	        if (str[i+1] == '\t')
							continue;
	       }
	       res[j] = str[i];
	       j++;
   			}
			i = strlen(res);
    	if ((res[i-2] == ' ') || (res[i-2] == '\t'))
      	res[i-2] = '\0';
			if(send(sock, res, strlen(str)+1, 0) < 0) perro("send");
		}
	}
	kill(pid, SIGKILL);
	printf("Goodbye.\n");
}

void receive(int sock)
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
		printf("You have been disconnected from server. Tap ENTER 2 times to leave.\n");
}

int main(int argc, char **argv)
{
	if(argc != 3) perro("USE ./client [PORT] 127.0.0.1");

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		perro("Socket error.");

	struct in_addr server_addr;
	if(!inet_aton(argv[2], &server_addr))
		perro("inet_aton");

	struct sockaddr_in connection;
	connection.sin_family = AF_INET;
	memcpy(&connection.sin_addr, &server_addr, sizeof(server_addr));
	connection.sin_port = htons(atoi(argv[1]));
	if (connect(sock, (const struct sockaddr*) &connection, sizeof(connection)) != 0) perro("Connect error.");

	int pid;
	if ((pid = fork())) send_cmd(sock, pid);
	else receive(sock);

	return 0;
}
