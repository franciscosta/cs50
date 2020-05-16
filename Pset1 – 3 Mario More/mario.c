#include <stdio.h>
#include <cs50.h>

int get_height(void);

int main(void)
{
    // Getting the height
    int height = get_height(); 
    
    // The spaces equal the height minus one hash
    int spaces = height - 1;   
    
    // The hashes start at 1
    int hashes = 1;            
    
    while (height > 0)
    {
        // Abstracting the spaces onto a variable so I can print them
        for (int s = spaces; s > 0; s--)  
        {
            printf(" "); 
        }
        
        // Abstracting the hashes onto a variable so I can print them
        for (int h = hashes; h > 0; h--) 
        {
            printf("#"); 
        }
        
        printf("  ");
        
        // Abstracting the hashes onto a variable so I can print them
        for (int h = hashes; h > 0; h--) 
        {
            printf("#"); 
        }
        
        // Breaking the line
        printf("\n"); 
        
        // Actually reducing the spaces by 1
        spaces--;
        
        // Actually reducing the hashes by 1
        hashes++;
        
        // Actually reducing the height by 1 so I can affect the global while
        height--;
    }
        
}    
    
int get_height(void)
{
    int n;

    do 
    {
        n = get_int("Height:");
    }
    while (n < 1 || n > 8);
    return n;
}
