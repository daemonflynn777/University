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
#include <time.h>
#include <pthread.h>

#define MAX_CLIENTS	64

static unsigned int client_count = 0;
static int user_id = 10;
int clients_online = 0;
int count = 0;
int admin_try = 0;
char names[200][200];
char ban_list[200];
char ban_name[32];
const char password[] = "notadmin";

typedef struct
{
	  struct sockaddr_in addr;
	  int connection;
	  int user_id;
	  char name[32];
	  char pm[32];
	  char pmget[32];
	  char history[MAX_CLIENTS][1024];
		int root;
} client_t;

client_t *clients[MAX_CLIENTS];

void add_queue(client_t *cl)
{
		int i;
		for (i = 0; i < MAX_CLIENTS; i++)
		{
		  	if (clients[i] == 0)
			{
			  		clients[i] = cl;
			      return;
		  	}
		}
}

void rmv_queue(int user_id)
{
		int i;
		for (i = 0; i < MAX_CLIENTS; i++)
  	{
				if (clients[i])
    		{
						if (clients[i]->user_id == user_id)
      			{
								clients[i] = NULL;
								return;
						}
				}
		}
}

void msg_snd(char *s, int user_id)
{
		int i;
		for (i = 0; i < MAX_CLIENTS; i++)
  	{
				if (clients[i])
				{
						if (clients[i]->user_id != user_id)
						{
				    		if (write(clients[i]->connection, s, strlen(s)) < 0)
							{
					     			printf("Could not send!\n");
					     			exit(-1);
				    		}
						}
				}
		}
}

void msg_snd_all(char *s)
{
		int i;
		for (i = 0; i < MAX_CLIENTS; i++)
		{
				if (clients[i])
				{
						//printf("\nT E S T\n");
						if (write(clients[i]->connection, s, strlen(s)) < 0)
						{
				    		printf("Could not send to all users!\n");
				    		exit(-1);
						}
				}
		}
}

void msg_snd_sender(const char *s, int connection)
{
		if (write(connection, s, strlen(s)) < 0)
		{
				printf("Could not send to sender!\n");
				exit(-1);
		}
}

void msg_snd_client(char *s, int user_id, int sender_id)
{
		int i;
		int flg = 0;
		for (i = 0; i < MAX_CLIENTS; i++)
		{
				if (clients[i])
				{
						if (clients[i]->user_id == user_id)
						{
								if (write(clients[i]->connection, s, strlen(s)) < 0)
								{
										printf("Could not send to client %d !\n", user_id);
										exit(-1);
								}
								flg = 1;
						}
				}
		}
		if (flg == 0)
		msg_snd_sender("Error. No clients with this ID\n", sender_id);
}

void msg_snd_clients_active(int connection)
{
		int i;
		char s[64];
		for (i = 0; i < MAX_CLIENTS; i++)
		{
				if (clients[i])
				{
						sprintf(s, "Client ID: %d | NAME: %s\r\n", clients[i]->user_id, clients[i]->name);
						msg_snd_sender(s, connection);
				}
		}
		sprintf(s, ">>>>>>>>>>>>>>\n");
		msg_snd_sender(s, connection);
}

void get_client_addr(struct sockaddr_in addr)
{
		printf("%d.%d.%d.%d",
						addr.sin_addr.s_addr & 0xFF,
						(addr.sin_addr.s_addr & 0xFF00)>>8,
						(addr.sin_addr.s_addr & 0xFF0000)>>16,
						(addr.sin_addr.s_addr & 0xFF000000)>>24);
}

void convert_to_endline(char *s)
{
		while(*s != '\0')
		{
				if(*s == '\r' || *s == '\n')
						*s = '\0';
				s++;
		}
}

int int_check(char *inpt)
{
int i = 0;
    while ((inpt[i] != '\n') && (inpt[i] != '\0'))
		{
        if ((inpt[i] >= '0') && (inpt[i] <= '9'))
            i++;
        else
            return 0;
    }
    return 1;
}

void *client_interaction(void *arg)
{
  char msg_out[2048];
  char msg_in[1024];
	int read_length;
	char last_word[2048];
	client_count++;
	client_t *cl = (client_t *)arg;
	admin_try = 0;

	printf("<<ACCEPT ");
	get_client_addr(cl->addr);
	printf(" REFERENCED BY %d\n", cl->user_id);
	sprintf(msg_out, "New user connected. ID: %s\r\n> Use \\help\n\n", cl->name);
	msg_snd_all(msg_out);

	while ((read_length = read(cl->connection, msg_in, sizeof(msg_in) - 1)) > 0)
	{
	     msg_in[read_length] = '\0';
	     msg_out[0] = '\0';
			 convert_to_endline(msg_in);

			 if (!strlen(msg_in))
		   		continue;

			if(msg_in[0] == '\\')
			{
					char *command;
					char *parameter;

					command = strtok(msg_in, " ");

					if (!strcmp(command, "\\quit"))
		 			{
		 					parameter = strtok(NULL, " ");
		 					while (parameter != NULL)
							{
		 							strcat(last_word, " ");
		 							strcat(last_word, parameter);
		 							parameter = strtok(NULL, " ");
		 					}
		 					strcat(last_word, "\r\n");
		 					break;
		 			}

					else if (!strcmp(command, "\\ping"))
		 					msg_snd_sender("pong\r\n\n", cl->connection);

					else if(!strcmp(command, "\\name"))
					{
							parameter = strtok(NULL, " ");
							char *parameter2 = strtok(NULL, " ");
							if(parameter)
							{
											if ((parameter2)==NULL)
											{
													if (strlen(parameter) < 32)
													{
															char *old_name = strdup(cl->name);
																if (strstr(ban_list, parameter) != NULL)
																{
																	sprintf(msg_out, "[BAN][ADMIN] YOU HAVE BEEN BANNED [BAN][ADMIN]\nComment: ");
																	strcat(msg_out, "\r\n");
																	msg_snd_sender(msg_out, cl->connection);
																}
																else
																{
																	if (count == 0)
																	{
																			strcat(names[count], parameter);
																			count++;
																			strcpy(cl->name, parameter);
																			sprintf(msg_out, "Client with name/ID %s changed his name to %s\r\n\n", old_name, cl->name);
																			free(old_name);
																			msg_snd_all(msg_out);
																	}
																	else
																	{
																			int name_flg = 0;
																			for (int zz = 0; zz <= clients_online; zz++)
																					if (strstr(names[zz],parameter) != NULL)
																					{
																							name_flg = 1;
																							break;
																					}
																			if (!name_flg)
																			{
																					strcpy(cl->name, parameter);
																					sprintf(msg_out, "Client with name/ID %s changed his name to %s\r\n\n", old_name, cl->name);
																					free(old_name);
																					msg_snd_all(msg_out);
																			}
																			else
																			{
																					sprintf(msg_out, "This name is already taken. Sorry. \r\n\n");
																					msg_snd_sender(msg_out, cl->connection);
																			}
																	}
															}
													}
													else
															msg_snd_sender("Name can't be more than 32 symbols \r\n\n", cl->connection);
											}
											else
													msg_snd_sender("Name can't contain more than 1 word \r\n\n", cl->connection);
							}
							else
									msg_snd_sender("Name can't be a NULL \r\n\n", cl->connection);
					}

					else if (!strcmp(command, "\\private"))
					{
							parameter = strtok(NULL, " ");
							if (parameter)
							{
									if (int_check(parameter))
									{
											int user_id = atoi(parameter);
											char str[32];

											sprintf(str, "%d", user_id);
											strcat(cl->pm, str);
											strcat(cl->pm, " ");
											parameter = strtok(NULL, " ");
											if (parameter)
											{
													sprintf(msg_out, "New Private Message from user %s:\n", cl->name);
													while(parameter != NULL)
													{
															strcat(msg_out, " ");
															strcat(msg_out, parameter);
															parameter = strtok(NULL, " ");
													}
													strcat(msg_out, "\r\n\n");
													msg_snd_client(msg_out, user_id, cl->connection);
											}
											else
						  						msg_snd_sender("Message can't be a NULL \r\n\n", cl->connection);
									}
									else
									{
											for (int j = 0; j < clients_online; j++)
											{
													if (!strcmp(parameter,clients[j]->name))
													{
															char str[32];

															sprintf(str, "%s", parameter);
															strcat(cl->pm, str);
															strcat(cl->pm, " ");
															parameter = strtok(NULL, " ");
															if (parameter)
															{
																	sprintf(msg_out, "New Private Message from user %s:\n", cl->name);
																	while(parameter != NULL)
																	{
																			strcat(msg_out, parameter);
																			strcat(msg_out, " ");
																			parameter = strtok(NULL, " ");
																	}
																	strcat(msg_out, "\r\n\n");
																	strcat(clients[j]->pmget, cl->name);
																	strcat(clients[j]->pmget, " ");
																	msg_snd_client(msg_out, clients[j]->user_id, cl->connection);
															}
															else
																	msg_snd_sender("Message can't be a NULL \r\n\n", cl->connection);
															break;
												}
												if (j == clients_online-1)
														msg_snd_sender("No such nickname online. \r\n\n", cl->connection);
									}
							}
						}
							else
									msg_snd_sender("Reference can't be a NULL \r\n\n", cl->connection);
					}

					else if (!strcmp(command, "\\active"))
					{
							sprintf(msg_out, "Active clients: %d\r\n", client_count);
							msg_snd_sender(msg_out, cl->connection);
							msg_snd_clients_active(cl->connection);
					}

					else if (!strcmp(command, "\\help"))
					{
							strcat(msg_out, "\\quit: Quit chatroom.\r\n");
							strcat(msg_out, "> \\ping: Server test.\r\n");
							strcat(msg_out, "> \\name <name>: Change nickname.\r\n");
							strcat(msg_out, "> \\private <ID>: <message> Send private message.\r\n");
							strcat(msg_out, "> \\active: Show active clients.\r\n");
							strcat(msg_out, "> \\help: Show help.\r\n");
							strcat(msg_out, "> \\history: Show ID's for Private Messages.\r\n");
							strcat(msg_out, "> \\admin: Become admin ()password required.\r\n");
							strcat(msg_out, "> \\kick <ID> <comment>: Kick user.\r\n");
							strcat(msg_out, "> \\ban <ID> <comment>: Ban user.\r\n");
							strcat(msg_out, "> \\shutdown: Power off server.\n");
							msg_snd_sender(msg_out, cl->connection);
					}

					else if (!strcmp(command, "\\history"))
					{
							sprintf(msg_out, "Private messages ID's: %s\r\n", cl->pm);
							msg_snd_sender(msg_out, cl->connection);
					}

					else if(!strcmp(command, "\\admin"))
					{
						sprintf(msg_out, "Password: ");
						msg_snd_sender(msg_out, cl->connection);
						admin_try = 1;
					}

					else if(!strcmp(command, "\\ban"))
					{
						if (cl->root)
						{
							parameter = strtok(NULL, " ");
							if(parameter)
							{
								if (int_check(parameter))
								{ //ID
									int user_id = atoi(parameter);
									int k = user_id;
									int flag_adm = 0;
									for(int j=0;j<clients_online;j++)
									{
										if(clients[j]->user_id == k)
										{
											if (clients[j]->root == 1)
												flag_adm = 1;
										}
									}
									if (flag_adm)
									{
										msg_snd_sender("You can't ban other admin.\r\n\n", cl->connection);
										sprintf(msg_out, "Name/ID: %s tried to ban you!\r\n", cl->name);
										msg_snd_client(msg_out, user_id, cl->connection);
									}else
									{
										for(int j=0;j<clients_online;j++)
										{
											if(clients[j]->user_id == k)
											{
												for (int l = 0; l < sizeof(ban_name); l++)
													ban_name[l]=clients[j]->name[l];
											}
										}
										strcat(ban_list, ban_name);
										strcat(ban_list, " ");
										parameter = strtok(NULL, " ");
										if(parameter)
										{
											sprintf(msg_out, "[BAN][ADMIN] YOU HAVE BEEN BANNED [BAN][ADMIN]\nComment: ");
											while(parameter != NULL)
											{
												strcat(msg_out, " ");
												strcat(msg_out, parameter);
												parameter = strtok(NULL, " ");
											}
											strcat(msg_out, "\r\n");
											msg_snd_client(msg_out, user_id, cl->connection);
										}
										else
											msg_snd_sender("Message can't be a NULL \r\n\n", cl->connection);
									}
								}
								else
								{
									for(int j=0;j<clients_online;j++)
									{
										if(!strcmp(parameter,clients[j]->name))
										{
											int flag_adm = 0;
											if (clients[j]->root == 1)
													flag_adm = 1;
											if (flag_adm)
											{
												msg_snd_sender("You can't ban other admin.\r\n\n", cl->connection);
												sprintf(msg_out, "Name/ID: %s tried to ban you! Fight him!!!\r\n", cl->name);
												msg_snd_client(msg_out, clients[j]->user_id, cl->connection);
											}
											else
											{
												for (int l = 0; l < sizeof(ban_name); l++)
												ban_name[l]=clients[j]->name[l];

												strcat(ban_list, ban_name);
												strcat(ban_list, " ");
												parameter = strtok(NULL, " ");
												if(parameter)
												{
													sprintf(msg_out, "[BAN][ADMIN] YOU HAVE BEEN BANNED [BAN][ADMIN]\nComment: ");
													while(parameter != NULL)
													{
														strcat(msg_out, " ");
														strcat(msg_out, parameter);
														parameter = strtok(NULL, " ");
													}
													strcat(msg_out, "\r\n");
													msg_snd_client(msg_out, clients[j]->user_id, cl->connection);
												}
												else
													msg_snd_sender("Message can't be a NULL \r\n\n", cl->connection);
											}
											break;
										}
										if (j==clients_online-1)
											msg_snd_sender("No such nickname online. \r\n\n", cl->connection);
									}
								}
							}
							else
								msg_snd_sender("Name can't be a NULL \r\n\n", cl->connection);
						}
						else
							msg_snd_sender("You are not admin \r\n\n", cl->connection);
					}

					else if(!strcmp(command, "\\kick"))
					{
						if (cl->root)
						{
							parameter = strtok(NULL, " ");
							if(parameter)
							{
								if (int_check(parameter))
								{ // ID
									int user_id = atoi(parameter);
									int k = user_id;
									int flag_adm = 0;
									for(int j=0;j<clients_online;j++)
									{
										if(clients[j]->user_id == k)
										{
											if (clients[j]->root == 1)
												flag_adm = 1;
										}
									}
									if (flag_adm)
									{
										msg_snd_sender("You can't kick other admin.\r\n\n", cl->connection);
										sprintf(msg_out, "Name/ID: %s tried to kick you! Be a man, call him a pussy in the chat!\r\n", cl->name);
										msg_snd_client(msg_out, user_id, cl->connection);
									}
									else
									{
										parameter = strtok(NULL, " ");
										if(parameter)
										{
											sprintf(msg_out, "[BAN][ADMIN] YOU HAVE BEEN KICKED [BAN][ADMIN]\nComment: ");
											while(parameter != NULL)
											{
												strcat(msg_out, " ");
												strcat(msg_out, parameter);
												parameter = strtok(NULL, " ");
											}
											strcat(msg_out, "\r\n");
											msg_snd_client(msg_out, user_id, cl->connection);
										}
										else
											msg_snd_sender("Message can't be a NULL \r\n\n", cl->connection);
									}
								}
								else
								{ // Nickname
									for(int j=0;j<clients_online;j++)
									{
										if(!strcmp(parameter,clients[j]->name))
										{
											int flag_adm = 0;
											if (clients[j]->root == 1)
												flag_adm = 1;
											if (flag_adm)
											{
												msg_snd_sender("You can't kick other admin.\r\n\n", cl->connection);
												sprintf(msg_out, "Name/ID: %s tried to kick you! Be a man, call him a pussy in the chat!\r\n", cl->name);
												msg_snd_client(msg_out, clients[j]->user_id, cl->connection);
											}
											else
											{
												parameter = strtok(NULL, " ");
												if(parameter)
												{
													sprintf(msg_out, "[BAN][ADMIN] YOU HAVE BEEN KICKED [BAN][ADMIN]\nComment: ");
													while(parameter != NULL)
													{
														strcat(msg_out, " ");
														strcat(msg_out, parameter);
														parameter = strtok(NULL, " ");
													}
													strcat(msg_out, "\r\n");
													msg_snd_client(msg_out, clients[j]->user_id, cl->connection);
												}
												else
													msg_snd_sender("Message can't be a NULL \r\n\n", cl->connection);
											}
											break;
										}
										if (j==clients_online-1)
											msg_snd_sender("No such nickname online. \r\n\n", cl->connection);
									}
								}
							}
							else
								msg_snd_sender("Name can't be a NULL \r\n\n", cl->connection);
						}
						else
							msg_snd_sender("You are not admin \r\n\n", cl->connection);
					}

					else if(!strcmp(command, "\\shutdown"))
					{
						if (cl->root)
						{
							sprintf(msg_out, "Server is finishing his work, because of %s\r\n\n", cl->name);
							msg_snd_all(msg_out);
							exit(-1);
						}
						else
						{
							msg_snd_sender("You are not admin.\r\n\n", cl->connection);
						}
					}

					else
					{
						msg_snd_sender("Unknown command! \r\n\n", cl->connection);
						break;
					}
		}
		else
		{
			if (!admin_try)
			{
				time_t rawtime;
  			struct tm * timeinfo;
  			time ( &rawtime );
  			timeinfo = localtime ( &rawtime );
				snprintf(msg_out, sizeof(msg_out), "[%s][TIME]\n%s %s\r\n", cl->name, asctime(timeinfo), msg_in);
				msg_snd_all(msg_out);
			}
			else
			{
				if (!strcmp(msg_in, password))
				{
					cl->root = 1;
					admin_try = 0;
					sprintf(msg_out, "Admin granted!\n\n");
					msg_snd_sender(msg_out, cl->connection);
				}
				else
				{
					admin_try = 0;
					sprintf(msg_out, "Wrong password!\n\n");
					msg_snd_sender(msg_out, cl->connection);
				}
			}
		}
	}
	sprintf(msg_out, "User %s is leaving, bye!\n His last words: %s\n", cl->name, last_word);
	msg_snd_all(msg_out);
	close(cl->connection);

	rmv_queue(cl->user_id);
	printf("<<LEAVE ");
	get_client_addr(cl->addr);
	printf(" REFERENCED BY %d\n", cl->user_id);
	free(cl);
	client_count--;
	pthread_detach(pthread_self());

	return NULL;
}



int main(int argc, char *argv[])
{
    int sock = 0;
		int client = sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    pthread_t tid;

		if (argc != 2)
		{
				printf("Error! Type ./server [port]\n");
				exit(1);
		}

		sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("Error occured!\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    signal(SIGPIPE, SIG_IGN);

    if (bind(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("Could not bind!\n");
        exit(2);
				return 1;
    }

    if (listen(sock, 10) < 0)
    {
        printf("Error!\n");
        exit(1);
    }

    printf("Server created.\n");

    while(1)
    {
			socklen_t client_length = sizeof(client_addr);
			client = accept(sock, (struct sockaddr*)&client_addr, &client_length);

			if ((client_count + 1) == MAX_CLIENTS)
			{
					printf("No more clients can be accepted!\n");
					printf("Wait or go away :<\n");
					get_client_addr(client_addr);
					printf("\n");
					close(client);
					continue;
			}

			client_t *cl = (client_t *)malloc(sizeof(client_t));
			cl->addr = client_addr;
			cl->connection = client;
			cl->user_id = user_id++;
			sprintf(cl->name, "%d", cl->user_id);
			clients_online++;

			add_queue(cl);
			pthread_create(&tid, NULL, &client_interaction, (void*)cl);

			sleep(1);
		}
}
