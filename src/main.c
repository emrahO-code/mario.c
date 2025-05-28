#include <_stdio.h>
#include <operations/basic.h>


int main()
{
    const int a = 1;
    const int b = 1;
    const int c = add(a, b);
    printf("Hello, world!\n");
    printf("a + b = %d\n", c);
    return 0;
}