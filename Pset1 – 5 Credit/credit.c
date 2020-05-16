#include <stdio.h>
#include <cs50.h>
#include <math.h>
long get_card_number(void);
int main(void)
{
    
    // Apologise in advance for the ratched program!!! 

    long card_number;
    long dummy;
    int length;
    
    // Get the card number
    
    card_number = get_card_number();
    dummy = card_number;
    length = 0;
    
    // Define number length
    
    while (dummy > 0)
    {
        dummy = dummy / 10;
        length++;
    }
    
    dummy = card_number;
    
    // Get last two digits
    
    int loop_times = length;
    
    while (loop_times > 2)
    {
        dummy = dummy / 10;
        loop_times--;
    }
    
    int first_digits = dummy;
    
    // Get last difit for VISA
    
    int visa_detector = first_digits / 10;
    
    dummy = card_number;
    
    // Here we go – this is where it goes crazy analog:

    int d1 = dummy % 10;
    int d2 = (dummy / 10 % 10) * 2;
    int d3 = dummy / 100 % 10;
    int d4 = (dummy / 1000 % 10) * 2;
    int d5 = dummy / 10000 % 10;
    int d6 = (dummy / 100000 % 10) * 2;
    int d7 = dummy / 1000000 % 10;
    int d8 = (dummy / 10000000 % 10) * 2;
    int d9 = dummy / 100000000 % 10;
    int d10 = (dummy / 1000000000 % 10) * 2;
    int d11 = dummy / 10000000000 % 10;
    int d12 = (dummy / 100000000000 % 10) * 2;
    int d13 = dummy / 1000000000000 % 10;
    int d14 = (dummy / 10000000000000 % 10) * 2;
    int d15 = dummy / 100000000000000 % 10;
    int d16 = (dummy / 1000000000000000 % 10) * 2;
    
    // Now I break down the multiplied into two individual numbers

    int e1 = d2 % 10;
    int e2 = d2 / 10 % 10;
    int e3 = d4 % 10;
    int e4 = d4 / 10 % 10;
    int e5 = d6 % 10;
    int e6 = d6 / 10 % 10;
    int e7 = d8 % 10;
    int e8 = d8 / 10 % 10;
    int e9 = d10 % 10;
    int e10 = d10 / 10 % 10;
    int e11 = d12 % 10;
    int e12 = d12 / 10 % 10;
    int e13 = d14 % 10;
    int e14 = d14 / 10 % 10;
    int e15 = d16 % 10;
    int e16 = d16 / 10 % 10;
    
    // Add them all up
    
    int calculation = d1 + e1 + e2 + d3 + e3 + e4 + d5 + e5 + e6 + d7 + e7 + e8 + d9 + e9 + e10 + d11 + e11 + e12 + d13 + e13 + e14 + d15 + e15 + e16;
    
    // Breakdown the result

    int last_digit = calculation % 10;
    
    //  If Statements
    
    if (last_digit != 0 || (first_digits != 34 || first_digits != 37 || first_digits != 51 || first_digits != 52 || first_digits != 53 || first_digits != 54 || first_digits != 55 || visa_detector !=4) || (length < 13 || length > 16))
    {
        printf("INVALID\n");        
    }
    
    
    if (last_digit == 0 && (first_digits == 34 || first_digits == 37) && length == 15)  // Amex 
    {
        printf("AMEX\n");   
    }
    
    if (last_digit == 0 && visa_detector == 4 && (length >= 13 && length <= 16))  // Visa
    {
        printf("VISA\n");   
    }
    
    if (last_digit == 0 && (first_digits == 51 || first_digits == 52 || first_digits == 53 || first_digits == 54 || first_digits == 55) && length == 16)  // Mastercard
    {
        printf("MASTERCARD\n");   
    }
    
        
}

// Abstracted function to get the number

long get_card_number(void)
{
    long number;
    
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);
    return number;
}
