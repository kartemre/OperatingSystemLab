#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>


void dummyFunc();
void testAddress(char *);
void handler(int, siginfo_t *, void *);

extern int etext, edata, __executable_start;
int globlVar = 1;
jmp_buf temp;

int main()
{
	int localVar = 6;
	int *localVar2 = (int *)malloc(sizeof(int));
	*localVar2 = localVar;
	signal(SIGSEGV, (void *)handler);

	printf("\n\npageSize = %u\n", getpagesize());

	printf("globlVar = %d\n", globlVar);
	printf("&globlVar = %p\n", &globlVar);

	printf("localVar = %d\n", localVar);
	printf("&localVar = %p\n", &localVar);

	printf("*localVar2 = localVar = %d\n", *localVar2);
	printf("localVar2 = %p\n", localVar2);
	printf("&localVar2 = %p\n", &localVar2);

	printf("\nPrototypes: \n");
	printf("void dummyFunc()	//Does nothing\n");
	printf("void testAddress(char* addr)	//Test address\n");
	printf("void handler(int, siginfo_t*, void*)	//To jump the specific location that we defined before in main()\n");

	printf("\nAddresses\n");
	printf("&dummyFunc() = %p\n", &dummyFunc);
	printf("&testAddress() = %p\n", &testAddress);
	printf("&handler() = %p\n", &handler);
	printf("&main() = %p\n", &main);

	printf("\ntext-seg 1st page  = %p\n", &__executable_start);
	printf("text-seg end = %p\n", &etext);
	printf("data-seg end = %p\n", &edata);
	printf("heap-seg end = %p\n", sbrk(0));

	testAddress((void *)&globlVar);
}


void dummyFunc() {}

void testAddress(char *addr)
{
	char c;
	printf("\nReading %p: \n", addr);
	c = *addr;

	printf("Value is %d\n", c);
	printf("Writing %d back to %p: \n", c, addr);
	fflush(stdout);
	*addr = c;

	printf("ok\n\n\n\n");
}


void handler(int a, siginfo_t *b, void *c)
{
	printf("Segmentation Fault handled.\n");
	longjmp(temp, SIGSEGV);
}