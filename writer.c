#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
struct ChatMessage
{
  char *last;
  char *previous;
};

void* readMsg(void *argMsg)
{
  struct ChatMessage *msg = argMsg;
  while(1)
  {
    if(strcmp(msg->previous, msg->last))
    {
      printf("chat updated: %s", msg->last);
      strcpy(msg->previous, msg->last);
    }
  }
  return NULL;
}

int main()
{
  struct ChatMessage msg;
  key_t key = ftok("shmfile",65);
  int shmid = shmget(key,1024,0666|IPC_CREAT);
  msg.last = (char*) shmat(shmid,(void*)0,0);
  memset(msg.last, 0, sizeof(msg.last));
  msg.previous = (char *) malloc(50*sizeof(char));

  pthread_t reader;
  pthread_create(&reader, NULL, readMsg, (void *)&msg);

  do
  {
    fgets(msg.last, 50, stdin);
  }while(strncmp(msg.last, "/quit", 5));

  shmdt(msg.last);
  return 0;
}
