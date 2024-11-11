#include <stdio.h>

int main() {

#define CAR_PRICE 15000 //defining car price, should use f at the end of the value if floating.

    unsigned short carsAvailable = 15, carsNeeded;

    printf("Hello, beloved customer!\nWe have %hd cars in stock today, available for %hd GBP each.\nHow many cars would you like to purchase?\nPlease enter the amount: ", carsAvailable, CAR_PRICE);
    scanf("%hd", &carsNeeded); //scanf requires & at the start of variable when it's used for input.

    if (carsNeeded > carsAvailable) {
        printf("\nI'm sorry, but there we only have %hd cars in stock, please try again.", carsAvailable);
        return 0;
    } else {
        unsigned int totalPrice = CAR_PRICE * carsNeeded;
        carsAvailable -= carsNeeded;

        printf("\nThank you! :^)\nYou have purchased %hd cars.\nThe total cost is %d GBP.\n\nThere are %hd cars remaining in stock.\nHave a good day!", carsNeeded, totalPrice, carsAvailable);
        return 0;
    }
}