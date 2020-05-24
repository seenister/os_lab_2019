#include <stdio.h>
#include <stdlib.h>

#include "revert_string.h"

void RevertString(char **str)
{
  int len = strlen(*str);
	char *buf_str = malloc(sizeof(char) * (len + 1));
	strcpy(buf_str, *str);

	int i = 0;
	while (i < len)
	{
		*(*str+i) = buf_str[len-i-1];
		i++;
	}

	free(buf_str);
}
