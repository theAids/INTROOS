/* TEAM APOL
*  kernel.c
*/

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

unsigned int k = 2;
unsigned int j = 0;
unsigned int i = 0;
unsigned int xcur = 0;
unsigned int ycur = 0;
char *vidptr = (char*)0xb8000; 	//video mem begins here.
unsigned int attrib = 0x0F;
unsigned int lastline = 3840;

//for cursor
void outb(u16int port, u8int value)
{
  asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

//for input
u8int inb(u16int port)
{
   u8int ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

int newline(int i){	
  if(i<=lastline)
  while(i < 80*k){
    i++;
  }
  k =  k+2;
  return i;
}

static void move_cursor(unsigned int ycur, unsigned int xcur)
{
   // The screen is 80 characters wide...
  u16int cursorLocation = ycur * 80 + xcur;
  outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
  outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
  outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
  outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

void print(char *str){
  while(str[j] != '\0') {
    if(str[j] == '\n'){
      i = newline(i);
      j += 1;
      ycur++;
      xcur = 0;
    }else{
      vidptr[i] = ' ';
      switch(str[j]){
        case '%': vidptr[i+1] = 0x4f;break;
        case '$': vidptr[i+1] = 0xff;break;
        case '*': vidptr[i+1] = 0x2f;break;
        case '#': vidptr[i+1] = 0xef;break;
        default: vidptr[i] = str[j];vidptr[i+1] = attrib;break;
       }
       ++j;
       i = i + 2;
       ++xcur;
    }
  }	
  j = 0;
  move_cursor(ycur, xcur);
}

void clear(void)
{
  i = 0;
  while(i < 80 * 25 * 2) {		
    vidptr[i] = ' '; //blank character	
    vidptr[i+1] = attrib; //attribute-byte: for color of cursor and background, should be same as one above.
    i = i + 2;
  }
  i=0;
  xcur = 0;
  ycur = 0;
  move_cursor(ycur, xcur);
}

void clearlast(void){ //clears the last line
	char *tempvid;
	tempvid = vidptr;
	i=lastline;
	while(i<4000){
		tempvid[i] = ' ';
		tempvid[i+1]= attrib;
		
		i += 2;
	}
	i=lastline;
	ycur=24;
	xcur=0;
	
	move_cursor(ycur,xcur);
}

void scrollup(void){
		char *tempvid;
		tempvid = vidptr;
		
		i=0;
		unsigned int nextline = 160;
		while(i<4000){
			vidptr[i] = tempvid[nextline];
			vidptr[i+1] = tempvid[nextline+1];
			i += 2;
			nextline += 2;
			
		}	
				
		clearlast();	
}


void printchar(char str){
	if(str == '\n'){
		ycur++;
		if(ycur<24){
			i = newline(i);
			j += 1;
			xcur = 0;
		}else{
			scrollup();
			ycur=24;
			xcur=0;
		}
	}else{
		if(i<4000){
			vidptr[i] = str;
			vidptr[i+1] = attrib;
			++j;
			i = i + 2;
			xcur++;
		}else{
			scrollup();
			ycur=24;
			xcur=0;
			
			vidptr[i] = str;
			vidptr[i+1] = attrib;
			++j;
			i = i + 2;
			xcur++;
		}
				
	}
	
	move_cursor(ycur, xcur);
		
}
		


void delay(void){
 int a, b;
  for(a=0; a<2000; a++){
    for(b=0; b<2000; b++){
    }
  }
}

char toASCII(int v)
{
  char val;
  if (v == 16){ val='q';}
      else if (v == 17) {val='w';}
      else if (v == 18) {val='e';}
      else if (v == 19) {val='r';}
      else if (v == 20) {val='t';}
      else if (v == 21) {val='y';}
      else if (v == 22) {val='u';}
      else if (v == 23) {val='i';}
      else if (v == 24) {val='o';}
      else if (v == 25) {val='p';}
      else if (v == 30) {val='a';}
      else if (v == 31) {val='s';}
      else if (v == 32) {val='d';}
      else if (v == 33) {val='f';}
      else if (v == 34) {val='g';}
      else if (v == 35) {val='h';}
      else if (v == 36) {val='j';}
      else if (v == 37) {val='k';}
      else if (v == 38) {val='l';}
      else if (v == 44) {val='z';}
      else if (v == 45) {val='x';}
      else if (v == 46) {val='c';}
      else if (v == 47) {val='v';}
      else if (v == 48) {val='b';}
      else if (v == 49) {val='n';}
      else if (v == 50) {val='m';}
      else if (v == 28) {val='\n';}
      else if (v == 57) {val='\t';}
      else if (v == 51) {val=',';}
      else if (v == 78) {val='+';}
      else if (v == 55) {val='*';}
      else if (v == 98) {val='/';}
      else if (v == 13) {val='=';}
      else if (v == 39) {val=';';}
      else if ((v == 11)||(v == 82)) {val='0';}
      else if ((v == 2)||(v == 79)) {val='1';}
      else if ((v == 3)||(v == 80)) {val='2';}
      else if ((v == 4)||(v == 81)) {val='3';}
      else if ((v == 5)||(v == 75)) {val='4';}
      else if ((v == 6)||(v == 76)) {val='5';}
      else if ((v == 7)||(v == 77)) {val='6';}
      else if ((v == 8)||(v == 71)) {val='7';}
      else if ((v == 9)||(v == 72)) {val='8';}
      else if ((v == 10)||(v == 73)) {val='9';}
      else if ((v == 12)||(v == 74)) {val='-';}
      else if ((v == 83)||(v== 52)) {val='.';}
      else if (v == 38) {val=' ';}
  return val;
}

void logo(void)
{
  int temp1 = 1;
  int maxRow = 1;
  	
  while(maxRow<25){
    switch(temp1){      
      case 1: print("                           *\n"); break;
      case 2: print("                         ***\n"); break;
      case 3: print("                       *****\n"); break;
      case 4: print("                      *****\n"); break;
      case 5: print("                      *****          ########\n"); break;	
      case 6: print("                       ***          ######$#$#\n"); break;
      case 7: print("              %%%%%%%%%%**%%%%%%%% #####$#####\n"); break;
      case 8: print("          %%%%%%%%%%%%%%%%%%%%%%%%#######$$$$  Hey Hey APOL!\n"); break;
      case 9: print("        %%%%%%%%%%%%%%%%%%%%%%%%%###########\n"); break;
      case 10:print("       %%%%%%%%$$%%%%%%%%%%%%%%%######%%\n"); break;
      case 11:print("      %%%%%%%$$$%%%%%%%%%%%%%%%######%%%%\n"); break;
      case 12:print("     %%%%%%$$$$%%%%%%%%%%%%%%%%%####%%%%%%%\n"); break;
      case 13:print("     %%%%%$$$$%%%%%%%%%%%%%%%%%%%##%%%%%%%%\n"); break;
      case 14:print("     %%%%%$$$%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 15:print("     %%%%%%$$%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 16:print("     %%%%%%%$%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 17:print("     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 18:print("   #####%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 19:print("  ########%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 20:print(" ##########%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 21:print("#   ########%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 22:print("       #########%%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 23:print("          ######%%%%%%%%%%%%%%%%%%%%%\n"); break;
      case 24:print("             %%%%%%%%%%%%%%%%%%%%%%%"); break;
    }		
    delay();
    temp1++;
    maxRow++;	
  }
}

void keyboard(void)
{
  char code = 0; //0 means no key pressed
  do
  {
    if(inb(0x60)!=code) //PORT FROM WHICH WE READ
    {
      code = inb(0x60);
      if(code>0){
        printchar(toASCII(code)); //print on screen
      }
    }
  }while(code!=1); //1 is ESC
}

/*
void scroll(void)
{
  if(ycur > 25){
    clear();
    j= 1;
    keyboard();
  }
} */

/*MAIN*/
void kmain(void)
{
  clear();
  logo();

  keyboard();
  
  
  //scroll();
  return;
}
