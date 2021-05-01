     ///////////////////////////////////////////////////////////////////
	//                                                               //
   //            ~~~ DOSjinx - WoV Dungeon tech-DEMO ~~~            //
  //  Written by Ashley 'Katie' Verburg, 04-12-2020 to xx-xx-xxxx  //
 //                                                               //
///////////////////////////////////////////////////////////////////

#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <CORE.H>
#include <KEYBOARD.H>
#include <TILESET.H>
#include <CHARMAPA.H>
#include <DRAW.H>
#include <DIALOG.H>


int debug = 2;
long CTD; //Cycles required To Draw
double FRAMERATE = 50.0;
int lastFrame, seed;
int pl_X, pl_Y;
int moving=1;
unsigned char *room_tileMap;
unsigned char *room_tileMapOver;
int happenedThisFrame=0;

time_t t_begin,t_end;
clock_t t_lastDraw,t_beginDraw;
/// ------------------------------------------------------------ Init / Term

void init(){ //initialize game
   printf("DOSjinx - WoV Dungeon tech-demo...");
   room_tileMap= (unsigned char*)malloc(10*6);
   if(room_tileMap==NULL){throwFatalError("Not enough memory for tilemap");}
   room_tileMapOver=(unsigned char*)malloc(10*6);
   if(room_tileMapOver==NULL){throwFatalError("Not enough memory for tilemap overlay");}
   if(debug==2){room_tileMap="\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01";room_tileMapOver="\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01\x00";}
   draw_init();
}
/// --------------------------------------------------------------- Graphics

void paintScreen(){
   t_beginDraw=clock();
   if(debug==2){
      char temp[25];
	draw_tileGrid(room_tileMap);//draw floor

	draw_tile(pl_X,pl_Y,2);//draw all sprites

	draw_tileGrid(room_tileMapOver);// draw overlay parts (like trees)

	//draw menu items
	draw_realRow(0,0,"\x3\x5\x3\x5");
	sprintf(temp,"F:%04d FPS:%d%d %d,%d",draw_frames,CTD,pl_X,pl_Y);
	draw_text(0,94,15,temp,0);
	sprintf(temp,"LAST KEY:%X",KB_lastKey);
	draw_text(0,88,15,temp,0);
   } else {




   }
   t_lastDraw=clock();
   CTD = t_lastDraw-t_beginDraw;
   draw_frames++;
   happenedThisFrame=0;
}

/// ------------------------------------------------------ Control functions


/// ------------------------------------------------------- System functions
void processInputs(){
   updateKeyReg();
   if(keyReg[0x01]){running=0;} //esc
   if(moving&&!inDialog){
      //if((happenedThisFrame>>0)&1U==0){
      if(happenedThisFrame==0){
	 if(keyReg[0x4B]){pl_X--;}
	 if(keyReg[0x48]){pl_Y--;}
	 if(keyReg[0x50]){pl_Y++;}
	 if(keyReg[0x4D]){pl_X++;}
	 //happenedThisFrame^=1UL<<0; //set "moved this frame" flag
	 happenedThisFrame=1;
      }
   }
   if(keyReg[0x30]&&!inDialog){Dialog_start(Dialog_getString(0));}
}
/// -------------------------------------------------------------- Game Loop
void loop(){
   while(running){
      //if((double)(clock()-t_lastDraw)/CLOCKS_PER_SEC>=(double)1/FRAMERATE){paintScreen();}
      if(draw_inRetrace()>0){draw_retrace++;}
      if(draw_retrace==120){if(!inDialog){paintScreen();}else{Dialog_paint();}draw_retrace=-1;}

      processInputs();
   }
}

int main(){
   init();
   loop();
   termGame();
   printf("See you later!");
   return 0;
}


