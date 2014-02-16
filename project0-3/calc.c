/*
 * [yufan xue]
 * [02/09/2014]
 *
 * [Project 0.3 - Program 1 -File Echo Program ]
 */
#include "support.h"



int main(int argc, char **argv) {
    int i;

    for( i = 1; i < argc; ++i ) {
        printf("Processing File: %s \n", argv[i]);
        process_file(argv[i]);
    }

    return 0;
}

int process_file(char *filename) {
    char buffer[LINELEN];
    char *fgets_rtn = NULL;
    double total = 0;
    // 0: invalid operator
    // 1: +
    // 2: -
    // 3: *
    // 4: /
    // by default is  +
    int operator = 1;

    // TODO: Open the file here
    FILE *fd = NULL;
    fd = fopen(filename,"r");
    if(NULL ==fd){
	fprintf(stderr,"Error: Cannot open thie file %s for reading! \n",filename);
	return -1;
    }
    

    // TODO: Change loop condition to check for the End-Of-File
    while( 0 == feof(fd) ) {
        // TODO: Instead of reading from 'stdin' you should pass the file descriptor
        /* Read input from the user */
        fgets_rtn = fgets(buffer, LINELEN, fd);
        if( NULL == fgets_rtn) {
            /* Ctrl-D */
            break;
        }

        /* Strip off the new line */
        if( '\n' == buffer[ strlen(buffer) - 1] ) {
            buffer[ strlen(buffer) - 1] = '\0';
        }

        int is_operator_res = is_operator(buffer);
        if(is_operator_res)
        {
            int operator_valid = is_valid_operator(buffer);
            switch(operator_valid)
            {
                case 0:
                    printf("Error : \"%s\" is not a valid operation \n",buffer);
                    break;

                case 1:
                    operator = 1;
                    break;
                
                case 2:
                    operator = 2;
                    break;
                
                case 3:
                    operator = 3;
                    break;
                
                case 4:
                    operator = 4;
                    break;

                case 5:
                    operator = 5;
                    break;
            }
        }else
        {
            int int_valid = is_valid_int(buffer);   
            int float_valid = is_valid_float(buffer);
            if(int_valid || float_valid)
            {
                
                double convert_num = strtod(buffer,NULL);

                switch(operator)
                {

                    case 1: // +
                        printf("    %.3f +      %.3f =      %.3f \n",total,convert_num,total+convert_num);
                        total += convert_num;
                        break;
                    
                    case 2:// -
                        printf("    %.3f -      %.3f =      %.3f \n",total,convert_num,total-convert_num);
                        total -= convert_num;
                        break;
                    
                    case 3:// *
                        printf("    %.3f *      %.3f =      %.3f \n",total,convert_num,total*convert_num);
                        total *= convert_num;
                        break;
                    
                    case 4:// /
                        printf("    %.3f /      %.3f =      %.3f \n",total,convert_num,total/convert_num);
                        total = total / convert_num;
                        break;

                    case 5:// %
                        {
                            double res = 0;
                            int temp = total/convert_num;
                            res = total - convert_num* temp;
                            printf("    %.3f %%      %.3f =      %.3f \n",total,convert_num,res);
                            total = res;
                        }
                        break;
                }
            }else
            {
                printf("Error: %s is not a valid number\n", buffer);
            }
        }
    }
    printf("Final Total: %.2f\n", total);
    // TODO: Close the file
    fclose(fd);
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

int is_valid_operator(char *str)
{
    str++;
    str++;
    str++;
    if(*str =='+')
        return 1;
    else if( *str =='-')
        return 2;
    else if( *str == '*')
        return 3;
    else if(*str =='/')
        return 4;
    else if(*str =='%')
        return 5;
    else
        return 0;

}

int is_operator(char *str)
{

    if(*str == 'o')
    {
        str++;
        if(*str =='p')
            return 1;
    }
    
    return 0;
}