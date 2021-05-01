#ifndef DRAW_H
#define DRAW_H

#define SCR_WIDTH  (unsigned int)320
#define SCR_HEIGHT (unsigned int)200
#define DW_PIXEL_SCALAR 2

unsigned char far *screen = (unsigned char far*)0xa0000000L;
int draw_retrace=0;
int draw_frames;
unsigned char *scrBuf;

void draw_videoMode(int mode){
   asm{
      mov AX, mode
      int 10h
   }
}

void draw_init(){
   printf("ENJIN STAHTU!!!!!!!!!\n");
   scrBuf = (unsigned char*) malloc(160*100);
   if(scrBuf==NULL){throwFatalError("Not enough memory for double buffer");}
   draw_videoMode(0x13);
}

int draw_inRetrace(){
   unsigned char byt;
   asm{
     push dx
     mov dx,3dah
     in al,dx
     mov byt,al
     pop dx
   }
 return (byt&0x08);
}

void draw_realPixel(int x,int y,int col){
   screen[(y<<6)+(y<<8)+x]=col;
}
void draw_realRow(int x, int y, unsigned char* col){
 //int temPos=((y<<6)+(y<<8)+x);//+0xa0000000;
 /*asm{
  push ax
  mov ax,cs
  mov ds,ax
  mov si,[col]
  mov ax,0a000h
  mov es,ax
  mov di,temPos
  lodsd
  stosd
  pop ax
 }*/
 x=y;
 y=x;
 col="blaub"; //to stop warnings goddanigt
 col++;
}
void draw_paintpixel(int x, int y, int color){
 scrBuf[(100*y)+x]=color;
}

void draw_pixel(int x, int y, int color){
   long pos =((y*2)<<6)+((y*2)<<8)+x*2;
   screen[pos]=color;
   screen[pos+1]=color;
   screen[pos+320]=color;
   screen[pos+321]=color;
}

void draw_blit(){
 for(int i=0;i<=100;i++){
  for(int j=0;j<=160;j++){
   //draw_blitPixel(j,i,scrBuf[(100*i)+j]);
  }
 }
 sound(1000);
 delay(50);
 nosound();
}

void draw_tile(int x, int y, int ID){
 char* data = tileset_get(ID);
 for(int i=0;i<=15;i++){
  for(int j=0;j<=15;j++){
   if(data[(i<<4)+j]!='\xFA')draw_pixel(j+x,i+y,data[(i<<4)+j]);//FA=transparent
  }
 }
}

void draw_gridTile(int x, int y, int ID){draw_tile(x*16,y*16,ID);}

void draw_gridTileRaw(int x, int y, char* data){
 for(int i=0;i<=15;i++){
  for(int j=0; j<=15;j++){
   if(data[(i<<4)+j]!='\xFA')draw_pixel(j+x*16,i+y*16,data[(i<<4)+j]);
  }
 }
}

void draw_tileGrid(unsigned char* grid){
 for(int i=0;i<6;i++){
  for(int j=0;j<10;j++){
   draw_gridTile(j,i,grid[(10*i)+j]);
  }
 }
}

void draw_text(int x, int y, int color, char* msg, int backColor=-1){ //26x16 characters
   int size = strlen(msg);
   int currCol=color;
   int line=0;
   int charPos=0;
   for(int i=0;i<=size-1;i++){
    if(msg[i]!='^'){
       char* ch=charMapA_get(msg[i]);
       for(int j=0;j<=5;j++){
	for(int k=0;k<=5;k++){
	 if(((ch[j]<<k)&0x10)==0x10){
	  draw_pixel(x+k+6*charPos,y+j+line*6,currCol);
	 }else if(backColor>=0){
	  draw_pixel(x+k+6*charPos,y+j+line*6,backColor);
	 }
	}
       }
       charPos++;
     } else {
	i++;
	if(msg[i]=='n'){line++;charPos=0;}
	if(msg[i]=='c'){i++;currCol=msg[i];}
     }
   }
}

void draw_testScaled(){
 for(int j=0;j<=SCR_HEIGHT/DW_PIXEL_SCALAR-1;j++){
  for(int i=0;i<=SCR_WIDTH/DW_PIXEL_SCALAR-1;i++){
   draw_pixel(i,j,i%15+draw_frames);
  }
 }
}

#endif // DRAW_H
