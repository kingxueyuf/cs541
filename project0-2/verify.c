/*
 * [yufan xue]
 * [2/6/2014]
 *
 * [Project 0.2 - Program 1 -Echo Program]
 */

#include "verify.h"

#define LINELEN 256

int main(int argc, char **argv){
	char buffer[LINELEN];
	char *fgets_rtn = NULL;
	int start =0;
	float total = 0;
	
	fgets_rtn = fgets(buffer,LINELEN,stdin);

	while(fgets_rtn != NULL)
	{
		if( '\n' == buffer[strlen(buffer)-1]){
			buffer[strlen(buffer)-1] = '\0';
		}
		int int_valid = is_valid_int(buffer);	
		int float_valid = is_valid_float(buffer);
		//printf("%d \n",float_valid);
		if(int_valid)
		{
			int convert_int = strtol(buffer,NULL,10);
			if(!start)
			{
				start =1;
				total = convert_int;
			}else
			{
				total += convert_int;
			}
			printf("Running Total: %.2f \n",total);
		}else if(float_valid)
		{
			float convert_float = strtod(buffer,NULL);
			if(!start)
			{
				start =1;
				total =convert_float;
			}else{
				total += convert_float;
			}
			printf("Running Total: %.2f \n",total);
		}else
		{
			printf("\"%s\" is not a valid number\n", buffer);
		}

		// if(int_valid)
		// {
		// 	printf("\"%s\" is a valid integer \n",buffer);	
		// }else if(float_valid)
		// {
		// 	printf("\"%s\" is a valid double \n", buffer); 
		// }else
		// {
		// 	printf("\"%s\" is not a valid number\n", buffer);
		// }

		fgets_rtn = fgets(buffer,LINELEN,stdin);
	}
	printf("\n");
	printf("Final Total:%.2f \n",total);
	return 0;
}

int is_valid_int(char* str){
	//TODO
	int start = 0;
		
	while( *str != '\0')
	{
		if(start)
		{
			if(!isdigit(*str)){
				return 0;
			}
		}else if(*str == '+' || *str== '-'|| isdigit(*str)){
			//printf("here");
			start =1;
		}else if(!isspace(*str))
		{
			return 0;
		}
		str++;			
	}
	return 1;
}

int is_valid_float(char* str){
	//
	int start = 0;	
	int have_dot = 0;
	while( *str != '\0')
	{
		if(start)
		{
			if( *str =='.')
			{
				if(!have_dot)
				{
					have_dot = 1;
				}else
				{
					return 0;
				}
			}else if(!isdigit(*str))
				return 0;
		}else if(*str == '+' || *str== '-'|| isdigit(*str)){
			start =1;
		}else if(*str =='.')
		{
			start =1;
			have_dot=1;
		}
		else if(!isspace(*str))
		{
			return 0;
		}
		str++;			
	}
	return 1;
}
