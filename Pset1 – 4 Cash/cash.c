#import <stdio.h>
#import <cs50.h>
#import <math.h>

int main(void)
{    
    // Set up variable

    float change;
    
    // Get change from user
    
    do 
    {
        change = get_float("Change owed:");        
    }
    
    // Only accept positive values And 

    while (change < 0);
    
    // Convert dollars to cents

    int round_change = round(change * 100);
    
    // Initialize my coin values

    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;
    
    // Setup my variable for the number of coins

    int quarters_used = 0; 
    int dimes_used = 0; 
    int nickels_used = 0; 
    int pennies_used = 0; 
    
    int coins;
    
    // Leftover starting point
    
    int leftover = round_change;
    
    if (leftover >= quarter)
    {
        quarters_used = leftover / quarter;
        leftover = leftover - (quarters_used * quarter);
    }
    
    if (leftover >= dime)
    {
        dimes_used = leftover / dime;
        leftover = leftover - (dimes_used * dime);
    }
    
    if (leftover >= nickel)
    {
        nickels_used = leftover / nickel;
        leftover = leftover - (nickels_used * nickel);
    }
    
    if (leftover >= penny)
    {
        pennies_used = leftover / penny;
        leftover = leftover - (pennies_used * penny);
    }
    
    coins = quarters_used + dimes_used + nickels_used + pennies_used;
    
    printf("%i\n", coins);
    

}
