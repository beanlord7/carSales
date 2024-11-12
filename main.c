#include <stdio.h>

int main() {
#define CAR_PRICE 15000.00f //defining car price, should use f at the end of the value if floating.
#define DISCOUNT_AGE 65
#define LICENSE_AGE 16
#define PURCHASE_AGE 18
//#define DISCOUNT 0.15f //I decided not to use the discount percentage as a definition, and instead make the discount checking branch to change the percentage of total price.

    //we can use unsigned because no negative values expected.
    unsigned short carsAvailable = 15, carsNeeded, userAge;

    float discountMultiplier = 1; //discount is expressed via multiplier variable that changes depending on eligibility, 1 being 100% normal price with no discount.
    //this also lets us avoid using the "pseudoboolean" variable giveDiscount.

    //checking if there are no cars left. in this version of car sales this condition will never be fulfilled because we declare carsAvailable = 15 right before this check.
    if (carsAvailable < 1) {
        printf("I'm sorry, but there are no cars available in stock at the moment, please try again later.");
        return 0; //terminates the program early if there's no cars.
    }

    printf(
        "Hello, beloved customer!\nWe have %hd cars in stock today, available for %.2f GBP each.\nHow many cars would you like to purchase?\nPlease enter the amount: ",
        carsAvailable, CAR_PRICE);
    scanf("%hd", &carsNeeded);

    if (carsNeeded > carsAvailable) {
        printf("\nI'm sorry, but there we only have %hd cars in stock, please try again.", carsAvailable);
        return 0;
    } //no 'else' required.

    //age check for the discount. "%%" used for % sign.
    printf("\nCurrently we are offering a 15%% discount for seniors. Please enter your age: ");
    scanf("%hd", &userAge);

    //checking for purchase legal eligibility.
    if (userAge < LICENSE_AGE) {
        printf("I'm sorry, but you have to be 16 years old or older to legally drive a car. The sale cannot be completed.");
        return 0;
    }
    //no 'else' required because previous branch returns 0 if true.
    if (userAge >= LICENSE_AGE && userAge < PURCHASE_AGE) {
        printf("I'm sorry, but at %hd years old you cannot legally purchase a car by yourself. In order to complete the purchase, please have your parent / guardian use this program instead.", userAge);
        return 0;
    }
    if (userAge >= DISCOUNT_AGE) {
        discountMultiplier = 0.85;
        printf("As a senior you are eligible for a 15%% discount.");
    }
    else {
        printf("You are not eligible for a discount.");
    }

    float totalPrice = carsNeeded * CAR_PRICE * discountMultiplier;
    carsAvailable -= carsNeeded;
    printf(
        "\nThank you! :^)\nYou have purchased %hd cars.\nThe total cost is %.2f GBP.\n\nThere are %hd cars remaining in stock.\nHave a good day!",
        carsNeeded, totalPrice, carsAvailable); //to output 2 decimal points of the float only, we can use ".2" after % and before f.
    return 0;
}