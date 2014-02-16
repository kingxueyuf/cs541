/*
 * @author yufan xue
 * @data   2/2/2014
 * @description program 2
 *
 */
#include <stdio.h>

int main(int argc , char** argv){
	int i;
	for( i = 0 ; i <argc; i++)
	{
		printf("Arg %d = (%s)\n",i,argv[i]);
	}
	return 0;
}
