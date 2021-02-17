#include "revert_string.h"
#include <string.h>

void RevertString(char *str)
{
	char temp;
	int n = strlen(str);
	for(int i=0; i < n/2; i++){
		temp = *(str+i);
		*(str+i) = *(str+n-1-i);
		*(str+n-1-i) = temp;
	}
}

