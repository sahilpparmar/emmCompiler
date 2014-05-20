#include<stdio.h>

int factorial(int x)
{
    if(x<=1)
        return 1;
    else if (x==2)
	return factorial(x-1)*x;
    else return factorial(x-2) + factorial(x-1);
}

int main() {
    int i;
    scanf("%d", &i);

    printf("Fac: %d\n", factorial(i));
}

