#include "../include/rawmidi.h"
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

static bool isLoop = true;


void turn(snd_rawmidi_t *midi,int ledId, bool on){
  unsigned char buff[3];
  buff[0] = 176;//0x90;
  buff[1] = ledId;
  buff[2] = on?127:0;

  rawmidi_hw_write(midi, buff, 3);

  usleep(100000);
}


void *midiOutThread(void *void_ptr){
  /* increment x to 100 */
  snd_rawmidi_t *midiOut = (snd_rawmidi_t *)void_ptr;

  printf("Write starts\n");
  int index = 32;
  bool on = true;
  while(isLoop){    
      turn(midiOut,index,on);
      
      on = !on;
      if(on)
        index=(index+1)%72;
  }

  printf("Write ends\n");

  /* the function must return something - NULL will do */
  return NULL;
}

void *midiInThread(void *void_ptr){
  /* increment x to 100 */
  snd_rawmidi_t *midiIn = (snd_rawmidi_t *)void_ptr;

  unsigned char readBuff[3];
  printf("Read starts\n");
  while(isLoop){
    ssize_t ssize = rawmidi_hw_read(midiIn, readBuff, 3);
    if(ssize>0){
      for(int j=0;j<ssize;j++)
        printf("%u\t",readBuff[j]);
      printf(".\n");
    }
      
  }
  printf("Read ends\n");

  /* the function must return something - NULL will do */
  return NULL;
}

int main(){
  int err;

  snd_rawmidi_t *midiIn;
  snd_rawmidi_t *midiOut;

  if ( err = rawmidi_hw_open(&midiIn, &midiOut, "/dev/midi1", "UM-1", O_RDWR) < 0 ) {
    printf("can't open midi pmidi for writing\n");
    return 1;
  }

  
  /* this variable is our reference to the second thread */
  pthread_t inputThread;
  pthread_t outputThread;

  if(pthread_create(&inputThread,NULL, midiInThread, midiIn)) {  
    fprintf(stderr, "Error creating input thread\n");
    return 1;
  }
  if(pthread_create(&outputThread,NULL, midiOutThread, midiOut)) {  
    fprintf(stderr, "Error creating out thread\n");
    return 1;
  }
  printf("Press Any Key to stop\n");  
  getchar(); 
  isLoop = false;

  /* wait for the second thread to finish */
  if(pthread_join(inputThread, NULL)) {
    fprintf(stderr, "Error joining input thread\n");
    return 2;
  }
  if(pthread_join(outputThread, NULL)) {
    fprintf(stderr, "Error joining output thread\n");
    return 2;
  }


  printf("Bye\n");

return 0;

}