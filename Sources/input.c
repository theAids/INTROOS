#include <system.h>

char *tempch;

void parse(char *in){
	
	int i=0;
	if(in[0] != '\n'){
			
		while(in[i] != '\n' && in[i] != ' '){
			tempch[i] = in[i];
			i++;
		}
		
		tempch[i]='\0';
		
		if(strcmp(tempch, "cls")){
			cls();
			
		}else if(strcmp(tempch, "say")){
			i++;
			while(in[i] != '\n'){		
				putch(in[i]);
				i++;
			}
			putch('\n');
			
		}else{
			puts("command not found");
			putch('\n');
			
		}
	}
	
	puts("TEAM APOL>");
	
	i = 0;
	
}
