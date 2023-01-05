#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

#define NMAX 10

int shmid;
int *shm;
int msgid;
key_t key;

void sig_cntrl(int sig)
{
    printf("\nFinal tower height is %d\n", *shm);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    msgctl(msgid,  IPC_RMID,  0);
}

int main (int argc, char *argv[])
{
    struct Tower
    {
        long type;
	      int height;
    }tower;

    key = ftok("1", 128);
    shmid = shmget(key, NMAX, 0666 | IPC_CREAT);
    shm = shmat(shmid, NULL, 0);
    *shm = 0;
    msgid = msgget(IPC_PRIVATE, 0666|IPC_CREAT);
    if (fork()==0)
    {
		//Player 1
                tower.type = 1;
                tower.height = 0;
                printf("Start! Let the battle begin!\n\n");
		            msgsnd(msgid, &tower, sizeof(tower.height),0);
                sleep(1);

		            while (1)
                {
			               //tower.type = 2;
			               msgrcv(msgid, &tower,sizeof(tower.height), 2,0);
		                 tower.type = 1;
                     tower.height++;
                     sleep(1);
                     *shm += 1;
			               printf("Now it's your turn!\nTower height is %d cubes.\n\n", *shm);
		       	         msgsnd (msgid, &tower, sizeof(tower.height),0);
                     sleep(1);
		            }
	   }
     if (fork()==0)
     {
		 //Player 2
		            while (1)
                {
			                   //tower.type = 1;
			                   msgrcv(msgid, &tower,sizeof(tower.height),1,0);
			                   tower.type = 2;
                         tower.height++;
                         sleep(1);
                         *shm += 1;
			                   printf("Your move!\nTower height is %d cubes.\n\n", *shm);
		                     msgsnd (msgid, &tower, sizeof(tower.height),0);
                         sleep(1);
		            }
	   }
     signal(SIGINT,sig_cntrl);
     pause();
     signal(SIGINT,SIG_IGN);
     wait(NULL);
     wait(NULL);
     return 0;
}
