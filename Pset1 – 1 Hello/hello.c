#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Asks user for name and stores it into a variable
    string name = get_string("What's your name?");
    
    // Returns hello + variable
    printf("hello, %s\n", name);
}
