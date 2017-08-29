/**
 * Uses Luhn's algorithm to determine if a credit card number
 * is a valid American Express, MasterCard, or Visa credit card.
 */

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // ensure correct credit card number input
    long long credit;
    do
    {
        printf("Number: ");
        credit = get_long_long();
    }
    while (credit < 0);

    int credit_sum = 0;
    int digit = 1;
    int temp;
    long long copy = credit;

    // parse through each digit
    while (copy > 0)
    {
        if (digit % 2 == 0)
        {
            // multiply every other digit by 2 starting from 2nd to the last digit
            temp = 2 * (copy % 10);

            // add each digit of the resulting product
            while (temp > 0)
            {
                credit_sum += temp % 10;
                temp /= 10;
            }
        }
        // add every other digit starting from last digit
        else if (digit % 2 == 1)
        {
            credit_sum += copy % 10;
        }
        copy /= 10;
        digit++;
    }

    // determine final number of digits in credit card number
    digit--;

    int startingDigits;
    bool valid = false;

    // determine credit card validity from total sum and format
    if ((credit_sum) % 10 == 0)
    {

        // for cards with 16 digits
        if (digit == 16)
        {
            // get first two digits of card number
            startingDigits = credit / 100000000000000;

            // if 51, 52, 53, 54, or 55, classify as MasterCard
            if (startingDigits >= 51 && startingDigits <= 55)
            {
                printf("MASTERCARD\n");
                valid = true;
            }
            else
            {
                // get first digit of card number
                startingDigits /= 10;

                // if 4, classify as Visa
                if (startingDigits == 4)
                {
                    printf("VISA\n");
                    valid = true;
                }
            }
        }

        // for cards with 15 digits
        else if (digit == 15)
        {
            // get first two digits of card number
            startingDigits = credit / 10000000000000;

            // if 34 or 37, classify as Amex
            if (startingDigits == 34 || startingDigits == 37)
            {
                printf("AMEX\n");
                valid = true;
            }
        }

        // for cards with 13 digits
        else if (digit == 13)
        {
            // get first digit of card number
            startingDigits = credit / 1000000000000;

            // if first digit is 4, classify as Visa
            if (startingDigits == 4)
            {
                printf("VISA\n");
                valid = true;
            }
        }
    }

    if (valid == false)
    {
        printf("INVALID\n");
    }

    // success
    return 0;
}