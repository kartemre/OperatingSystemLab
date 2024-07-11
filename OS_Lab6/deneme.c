#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

void dummyFunc();
void testAddress(char *);
void handler(int, siginfo_t *, void *);

jmp_buf temp;

int globlVar = 1;

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
    printf("void dummyFunc()    //Does nothing\n");
    printf("void testAddress(char* addr)    //Test address\n");
    printf("void handler(int, siginfo_t*, void*)    //To jump the specific location that we defined before in main()\n");

    printf("\nAddresses\n");
    printf("&dummyFunc() = %p\n", &dummyFunc);
    printf("&testAddress() = %p\n", &testAddress);
    printf("&handler() = %p\n", &handler);
    printf("&main() = %p\n", &main);

    printf("\nAddresses\n");
    printf("Executable Start: %p\n", &main);
    printf("Executable End: %p\n", &handler);
    printf("Data End: %p\n", &temp);

    char addressString[20];
    printf("Enter a memory address (in hexadecimal): ");
    scanf("%s", addressString);

    unsigned long address;
    sscanf(addressString, "%lx", &address);

    testAddress((char *)address);

    return 0;
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
    static int errorCount = 0;
    printf("Segmentation Fault handled.\n");

    if (errorCount == 0)
    {
        printf("Error occurred. The program will continue, but further errors will cause the program to exit.\n");
        errorCount++;
    }
    else
    {
        printf("Multiple errors occurred. Exiting program.\n");
        exit(1);
    }

    longjmp(temp, SIGSEGV);
}
