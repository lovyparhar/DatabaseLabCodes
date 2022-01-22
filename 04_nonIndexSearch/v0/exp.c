#include<stdio.h>


void f1(int x) {
	printf("I am f1 %d\n", x*x);
}

void f2(int y) {
	printf("I am f2 %d\n", y+y);
}


void somefun(void (*thisfunction)(int), int secparam) {
	
	(*thisfunction)(secparam); // or just thisfunction()
}

int main() {

	somefun(f1, 5);
	somefun(f2, 10);

	return 0;
}


