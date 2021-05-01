#ifndef CORE_H
#define CORE_H

int running=1;

void draw_videoMode(int mode);

void termGame(){ //collect garbage and display goodbye message
   draw_videoMode(0x3);//default text mode
   running=0;
}

void throwFatalError(char* msg){
   termGame();
   sound(1000);
   printf("FATAL ERROR: %s\nPress any key to terminate...",msg);
   delay(200);
   nosound();
   getch();
   exit(2);
}


#endif // CORE_H