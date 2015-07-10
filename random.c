#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int j;
	
	srand((unsigned)time(NULL));
	char random[27];
	char material[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int i;
	for( j= 0 ; j<100 ;j++){
	
			for(i=0;i<26;i++){
				random[i] = material[rand()%(sizeof(material)-1)];
			}
		random[i] = '\n';
		printf(random);
	}
	return 0;
	
}