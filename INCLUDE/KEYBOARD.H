#ifndef KEYBOARD_H
#define KEYBOARD_H

unsigned char KB_lastKey=0;
int unprocKey=0;
int keyReg[93]; //keycode register, can be queried for pressed keys.

unsigned char read_scancode() {
    unsigned char res;
    _asm {
        in al, 60h
        mov res, al
        in al, 61h
        or al, 128
        out 61h, al
        xor al, 128
        out 61h, al
    }
    return res;
}

void updateKeyReg(){
	unsigned char KB_newKey=0;
	KB_newKey=read_scancode();
	if(KB_newKey!=KB_lastKey){ //if new key pressed
		unprocKey=1;           //set unprocessed keypress flag
		if((KB_newKey>>7)&1U){     //key break
			keyReg[KB_newKey&0x7F]=0;
		} else {                   //key make
			keyReg[KB_newKey&0x7F]=1;
		}
		KB_lastKey=KB_newKey;
	}
}

#endif // KEYBOARD_H
