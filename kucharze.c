#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/sem.h>

#include <sys/wait.h>

#include <time.h>

#include <pthread.h>

#include <stdbool.h>



#define LICZBA_KUCHARZY 5

#define K 4

#define W 7

#define FORKS 5

#define WEIGHT_OF_MEAL 2





static struct sembuf buf;



void opuscSemafor(int semid, int semnum, int value){

  buf.sem_num=semnum;

  buf.sem_op=(-1)*value;

  buf.sem_flg=0;

  if(semop(semid,&buf,1)==-1){printf("zmiana wartosci semafora\n"); exit(1);}

}



void podniesSemafor(int semid, int semnum, int value){

  buf.sem_num=semnum;

  buf.sem_op=value;

  buf.sem_flg=0;

  if(semop(semid,&buf,1)==-1){printf("zmiana wartosci semafora\n"); exit(1);}

}

void pokazStanSemaforow(int semid) {

    int widelce = semctl(semid, 0, GETVAL);

    int waga    = semctl(semid, 1, GETVAL);

    int miejsca = semctl(semid, 2, GETVAL);

    int dania   = semctl(semid, 3, GETVAL);



    printf("Widelce: %d | Waga stołu: %d | Wolne miejsca: %d | Gotowe dania: %d\n", 

           widelce, waga, miejsca, dania);

}



bool myslenie(int semid){

  int time_of_thinking=rand()%10; 

  sleep(time_of_thinking);

  int dania   = semctl(semid, 3, GETVAL);

  int waga    = semctl(semid, 1, GETVAL);

  int miejsca = semctl(semid, 2, GETVAL);

  if(dania==0){

    printf("Musze gotowac");

      return 0;

  }

  else if(miejsca==0||waga<=1){

  printf("Musze jesc");

  return 1;

  }

  else{

    bool dec=rand()%2;

    if(dec){

      printf("Wymyslilem!!, bede jesc\n");

    }

    else{

      printf("Wymyslilem!!, bede gotowac\n");}

    return dec;

    }

} 



void jest_glodny(int semid){

//zabrac widelce

    pokazStanSemaforow(semid);

    opuscSemafor(semid,0,2);

    printf("cyk widelczyki\n");

    pokazStanSemaforow(semid);

}



void przy_stole(int semid, bool if_eating){

  int my_dish;

  pokazStanSemaforow(semid);

  int dania   = semctl(semid, 3, GETVAL);

  int waga    = semctl(semid, 1, GETVAL);

  int miejsca = semctl(semid, 2, GETVAL);

  if(if_eating ||(!if_eating && (dania==3||waga<=1))){

    //zmiejszamy liczbe dan i zwiekszamy liczbe miejsca oraz dopuszczalna wage dan

    opuscSemafor(semid,3,1);

    podniesSemafor(semid,2,1);

    podniesSemafor(semid,1,3);

    int time_of_eating=rand()%10; 

    sleep(time_of_eating);

    printf("Mniam!!\n");

  }

  else{

  opuscSemafor(semid,2,1);

  opuscSemafor(semid,1,3);  

  int time_of_preparing=rand()%10; 

  sleep(time_of_preparing);

  printf("Ale zem pichcil\n");

  podniesSemafor(semid,3,1);

  }

  pokazStanSemaforow(semid);

}



void koniec_posilku(int semid){

  //oddac widelce

  pokazStanSemaforow(semid);

  podniesSemafor(semid,0,2);

  printf("widelce oddane. Uciekam na drzemke\n");

  pokazStanSemaforow(semid);

}





int main(){

  

  int semid; 

  semid=semget(3,5,IPC_CREAT|0600);

  if(semid==-1){printf("tworzenie semaforow"); exit(1);}

  //widelce

  if(semctl(semid,0,SETVAL,FORKS)==-1){printf("wartosc pierwszego semafora"); exit(1);}

  //miejsca

  if(semctl(semid,1,SETVAL,W)==-1){printf("wartosc trzeciego semafora");exit(1);}

  //dopuszczalna waga

  if(semctl(semid,2,SETVAL,K)==-1){printf("wartosc drugiego semafora");exit(1);}

  //liczba posilkow na stole

  if(semctl(semid,3,SETVAL,0)==-1){printf("wartosc czwartego semafora");exit(1);}

  //dopuszczalni kucharze jedzacy

  //if(semctl(semid,4,SETVAL,3)==-1){printf("wartosc piatego semafora");exit(1);}

  //dopuszczalni kucharze gotujacy

 // if(semctl(semid,5,SETVAL,3)==-1){printf("wartosc szostego semafora");exit(1);}

  

  for(int i = 0; i < LICZBA_KUCHARZY; i++){

    

    if(fork()==0){

    while(1){

      srand(time(NULL)^getpid());

      bool decyzja=myslenie(semid); 

      jest_glodny(semid);

      przy_stole(semid,decyzja);

      koniec_posilku(semid);

    }

  }

  }

  for(int i = 0; i < LICZBA_KUCHARZY; i++){

    wait(NULL);

  }

  semctl(semid,0,IPC_RMID);

  



} 