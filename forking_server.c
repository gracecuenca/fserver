#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  int from_client;
  int sub;

  while(1){
    from_client = server_setup();
    printf("sending you to a subserver process\n");
    sub = fork();
    //if you are the child, send client to subserver
    if(!sub){
      subserver(from_client);
    }
  }
}

void subserver(int from_client) {
  char buff[BUFFER_SIZE];
  //establishing connection
  int to_client = server_connect(from_client);

  //put this in a while loop so it keeps looping until no more is to be read
  while(read(from_client, buff, sizeof(buff))){
    printf("subserver %d receieved the following: %s\n", getpid(), buff);
    process(buff);
    printf("subserver %d sending the following: %s\n", getpid(), buff);
    write(to_client, buff, sizeof(buff));
  }
  exit(0);
}

void process(char * s) {
  //turning all c's to b's
  int counter = 0;
  while(s[counter]){
    if(s[counter] == 'c'){
      s[counter] = 'b';
    }
    counter++;
  }
}
