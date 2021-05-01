#ifndef DIALOG_H
#define DIALOG_H
#include <CHARTILE.H>

int inDialog=0;
int Dialog_exit=0;
int D_pitch=5; //default values
int D_speed=5;
int D_color=0;
char* dialogResult="";
char D_line1[30];
char D_line2[30];
char D_line3[30]; //bottom one.
int D_running=0;
int D_parsePos=0;
char* D_currentLine="";
int D_speedDelay=0;
int D_currentDest=1;
int D_waiting=0;
int D_beeping=0;
int D_BeepSeq=0;
int D_longWait=1;
int D_WFE=0; //wait for exit
int D_wfi=0; //wait for input
char D_title[27];
int D_charID;

void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

void Dialog_drawChar(int ID){ //Draw the character frame of 3x3 tiles.
if(ID!=0){
	/*int k=1;
	for(int i=1; i<=3;i++){
		for(int j=1; j<=3;j++){
			draw_gridTileRaw(j,i,charTileset_get((ID*10)+k));
			k++;
		}
	}*/
	draw_gridTileRaw(1,1,charTileset_get((ID*10)+1));
	draw_gridTileRaw(2,1,charTileset_get((ID*10)+2));
	draw_gridTileRaw(3,1,charTileset_get((ID*10)+3));
	draw_gridTileRaw(1,2,charTileset_get((ID*10)+4));
	draw_gridTileRaw(2,2,charTileset_get((ID*10)+5));
	draw_gridTileRaw(3,2,charTileset_get((ID*10)+6));
	draw_gridTileRaw(1,3,charTileset_get((ID*10)+7));
	draw_gridTileRaw(2,3,charTileset_get((ID*10)+8));
	draw_gridTileRaw(3,3,charTileset_get((ID*10)+9));
}
}


int Dialog_PLUT(char p){ // LUT for chatter pitch
   switch(p){
      case 0: return 75;
	  case 1: return 150;
	  case 2: return 250;
	  case 3: return 310;
	  case 4: return 450;
	  case 5: return 500;
	  case 6: return 550;
	  case 7: return 710;
	  case 8: return 1500;
	  case 9: return 2100;
	  case 'A': return 1;
   }
   return 1;
}

char* Dialog_start(char* text){
   D_line1[0]='\0';
   D_line2[0]='\0';
   D_line3[0]='\0';
   D_title[0]='\0';
   D_pitch=5; //default values
   D_speed=1; //how much frames to wait for the next letter
   D_color=0;
   dialogResult="";
   inDialog=1;
   D_running=1;
   D_currentLine=text;
   D_currentDest=1;
   D_beeping=0;
   D_wfi=0;
   D_BeepSeq=0;
   D_longWait=1;
   D_parsePos=0;
   D_charID=0;
   
   return "1";	
}

void Dialog_end(){
	
}

void Dialog_paint(){
	for(int j=4;j<=5;j++){
	   for(int i=0;i<=9;i++){
	    	draw_gridTile(i,j,254);
	    }
	}
	if(D_beeping&&D_speedDelay==1){nosound(); D_beeping=0;} //reset chatter beep if beeping
	if(!D_running&&D_wfi){
		if(keyReg[0x39]||keyReg[0x1C]){ //if space or enter pressed and halting, release.
			D_wfi=0;D_running=1;
			if(D_WFE){inDialog=0; Dialog_end(); return;}
		}
	} 
	if(D_BeepSeq>0&&D_BeepSeq<10){ //update halt-sound
		switch(D_BeepSeq){
			case 1: sound(330);break;
			case 2: sound(500);break;
			case 3: sound(660);break;
			case 4: nosound();D_BeepSeq=-1;
		}
		D_BeepSeq++;
	}
	if(D_running){ //if free-running and it needs to parse a new character
		if( D_speedDelay==D_speed*D_longWait){
			D_speedDelay=0;
			D_longWait=1;
			
			if(D_currentLine[D_parsePos]=='/'){
				D_parsePos++;
				switch(D_currentLine[D_parsePos]){
					case 'n': if(D_currentDest!=3){D_currentDest++;}else{strcpy(D_line1,D_line2);strcpy(D_line2,D_line3);D_line3[0]='\0';}break;
					case 'h': D_wfi=1;D_running=0;D_BeepSeq=1;break;
					case 'c': D_parsePos++;
					          switch(D_currentDest){
								 case 1: append(D_line1,'^');append(D_line1,'c');append(D_line1,D_currentLine[D_parsePos]);break;
							 	 case 2: append(D_line2,'^');append(D_line2,'c');append(D_line2,D_currentLine[D_parsePos]);break;
								 case 3: append(D_line3,'^');append(D_line3,'c');append(D_line3,D_currentLine[D_parsePos]);break;
							  } break;
					case 'w': D_parsePos++;D_longWait=atoi((const char *)D_currentLine[D_parsePos]);break;
					case 'r': D_BeepSeq=1;D_WFE=1;D_wfi=1;D_running=0;break;
					case 'T': D_parsePos++;
					          for(int i=0;D_currentLine[D_parsePos]!='_';i++){
								 D_title[i]=D_currentLine[D_parsePos];
								 D_parsePos++;
							  } break;
					case 'a': char D_charIDRaw[4];
							  for(int j=0;j<=3;j++){
								D_parsePos++;
								D_charIDRaw[j]=D_currentLine[D_parsePos];
							  }
							  D_charID=atoi(D_charIDRaw); break;
				}
			} else {
				if(D_currentDest==3){
					append(D_line3,D_currentLine[D_parsePos]);
					if(strlen(D_line3)>=26){
						strcpy(D_line1,D_line2);
						strcpy(D_line2,D_line3);
						D_line3[0]='\0';
					}
				}
				if(D_currentDest==2){
					append(D_line2,D_currentLine[D_parsePos]);
					if(strlen(D_line2)>=26){D_currentDest++;}
				}
				if(D_currentDest==1){
					append(D_line1,D_currentLine[D_parsePos]);
					if(strlen(D_line1)>=26){D_currentDest++;}
				}
				if(D_currentLine[D_parsePos]!=' '){
					D_beeping=1;
					sound(Dialog_PLUT(D_pitch));
				}
			}
		D_parsePos++;
		} else {
			D_speedDelay++;
		}
	}
	Dialog_drawChar(D_charID);
	draw_text(3,67,15,D_title);
	draw_text(1,76,15,D_line1);
	draw_text(1,82,15,D_line2);
	draw_text(1,88,15,D_line3);
}

// 0=lime C=sandYellow E=yellow ^=almostWhite X=almostWhite \x0=dark blue \x4 = dark red
// q=brown (=red *=orange ,=Yellow /=green 4=cyan 5=lightBlue 6=Sky ' '=deepBlue !=deepPurple #=pink \xf=white
char* Dialog_getString(int index){
switch(index){
case 0: return "/a0001/TPOOPOO_THIS IS A TEST./h/nI AM GOING TO SPEAK IN/n/c BLUE/c\xf NOW./h/nWAIT A SECOND/w5./w5./w5./w5 OH FUCK!/nTHIS IS A NEW LINE./nTHIS IS ANOTHER./h/nARE WE SCROLLING OK?/h/nGOOD!/r";
}	
return "";
}

#endif // DIALOG_H
