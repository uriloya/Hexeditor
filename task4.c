#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>



int digCount(char* x)
{
	while (x[0] == '0'){ x++; }
    int ans = 0;
    while (x[0] != '\0'){
        x++;
        ans++;
    }
    return ans;
}

int main(int argc, char** argv){
	char str[100];
	printf("%d\n", digCount(argv[1]));
}