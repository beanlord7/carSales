#include <stdio.h>
#include <stdlib.h> //required for system()

//defining a separate function to clear console for ease of use if needed multiple times. that is because in order to clear console on windows, linux and macOS, we'll need several lines of code every time.
void clearConsole() {
    //#ifdef, #elif, #else and #endif are used for conditional compilation, and represent "if defined", "else if" "else" and "end if" ("break"). the clearConsole() function will be resolved when compiling, and then can be used throughout the code later.
#ifdef _WIN32//means "if defined" and checks if a macro is defined when compiling. "_WIN32" will be defined on windows (both 32 and 64 bit).


    system("cls");
#elif __linux__ || __APPLE__//"__linux__" will be defined on linux, and __APPLE__ will be defined in macOS.
    system("clear");
#endif
}

int main() {
#define CAR_PRICE 15000.00f //defining car price, should use f at the end of the value if floating.
#define DISCOUNT_AGE 65
#define LICENSE_AGE 16
#define PURCHASE_AGE 18
    //#define DISCOUNT 0.15f //I decided not to use the discount percentage as a definition, and instead make the discount checking branch to change the percentage of total price.
    //the reason we define these is to improve readability of our code - instead of using 1, 2, etc. in our code, each option used is a definition with immediately clear meaning.
    //defining these as chars instead of integers so the program is able to check any keyboard input against expected ones.
#define MENU_BUY '1'
#define MENU_APPLY_DISCOUNT '2'
#define MENU_VIEW_STATS '3'
#define MENU_EXIT '0'
    //for creating "pseudo-booleans" as integer variables (C doesn't support boolean variables natively).
#define TRUE 1
#define FALSE 0

    //we can use unsigned because no negative values expected.
    unsigned short carsAvailable = 15, carsNeeded, carsSold = 0;
    int menuChoiceValid = FALSE, userAge;
    char menuChoice;
    float totalSales = 0, discountMultiplier = 1;
    //discount is expressed via multiplier variable that changes depending on eligibility, 1 being 100% normal price with no discount.
    //this also lets us avoid using the "pseudo-boolean" variable giveDiscount.

    printf("Hello, beloved customer! Welcome to the car sales program.\n");

    //this is the loop for the menu - it repeats as long as the menuChoice is not selected by the user as 0 (exit).
    do {
        //main menu loop

        clearConsole(); //our function will clear console every time the user returns to main menu.

        printf(R"(*** MAIN MENU ***

Please choose from the following options:
[1] Buy cars.
[2] Apply discount.
[3] View stock and sales statistics.
[0] Exit.
)");

        menuChoiceValid = FALSE; //reset the boolean before checking menu choice.

        do {
            //valid choice check:
            printf("Enter your choice: \n");
            scanf(" %c", &menuChoice);

            if (menuChoice == MENU_BUY || menuChoice == MENU_EXIT || menuChoice == MENU_VIEW_STATS || menuChoice ==
                MENU_APPLY_DISCOUNT) {
                menuChoiceValid = TRUE;
            } else {
                clearConsole();
                printf(R"(*** MAIN MENU ***

Sorry, menu choice invalid, please try again:
[1] Buy cars.
[2] Apply discount.
[3] View stock and sales statistics.
[0] Exit.
)");
            }
        } while (menuChoiceValid == FALSE);

        clearConsole();

        switch (menuChoice) {
            case MENU_BUY:
                //checking if there are no cars left. in this version of car sales this condition will never be fulfilled because we declare carsAvailable = 15 right before this check.
                if (carsAvailable < 1) {
                    printf(
                        "I'm sorry, but there are no cars available in stock at the moment, please try again later.");
                    return 0; //terminates the program early if there's no cars.
                }
                printf(R"(*** BUY CARS ***

How many cars would you like to purchase?
Please enter the amount:
)");
                scanf("%hd", &carsNeeded);

                if (carsNeeded > carsAvailable) {
                    printf(
                        "\nI'm sorry, but we only have %hd cars remaining in stock, please try again!\n\nPress any key to return to main menu...\n",
                        carsAvailable);
                    getchar();
                    getchar();
                    break;
                } //no 'else' required.

                float totalPrice = carsNeeded * CAR_PRICE * discountMultiplier;
                totalSales += totalPrice;
                carsSold += carsNeeded; //update totalSales and carsSold after sale.
                carsAvailable -= carsNeeded;
                printf(
                    "You have purchased %hd cars. The total cost is %.2f GBP. Thank you for your business!\n\nPress any key to return to main menu...\n",
                    carsNeeded, totalPrice);
            //to output 2 decimal points of the float only, we can use ".2" after % and before f.
                getchar();
                getchar();
                break;

            case MENU_APPLY_DISCOUNT:
                //age check for the discount. "%%" used for % sign.
                printf(R"(*** APPLY DISCOUNT ***

We are currently offering a 15%% discount for seniors.
Please enter your age:
)");
                scanf("%d", &userAge);

            //checking for purchase legal eligibility.
                if (userAge < LICENSE_AGE) {
                    printf(
                        "\nI'm sorry, but you have to be 16 years old or older to legally drive a car. The sale cannot be completed.\n\nPress any key to return to main menu...\n");
                    getchar();
                    getchar();
                    break;
                }
                if (userAge >= LICENSE_AGE && userAge < PURCHASE_AGE) {
                    printf(
                        "\nI'm sorry, but at %d years old you cannot legally purchase a car by yourself. In order to complete the purchase, please have your parent / guardian use this program instead.\n\nPress any key to return to main menu...\n",
                        userAge);
                    getchar();
                    getchar();
                    break;;
                }
                if (userAge >= DISCOUNT_AGE) {
                    discountMultiplier = 0.85;
                    printf(
                        "A 15%% senior discount has been applied to your next purchase!\n\nPress any key to return to main menu...\n");
                    getchar();
                    getchar();
                    break;
                } else {
                    discountMultiplier = 1;
                    printf(
                        "I'm sorry, but you are not eligible for a discount.\n\nPress any key to return to main menu.\n");
                    getchar();
                    getchar();
                    break;
                }


            case MENU_VIEW_STATS:
                clearConsole();
                printf(
                    R"(*** STATISTICS ***

- Cars available:       -- %hd.
- Cars sold:            -- %hd.
- Car Price:            -- %.2f GBP.
- Total car sales:      -- %.2f GBP.

Press any key to return to main menu...
)",
                    carsAvailable, carsSold, CAR_PRICE, totalSales);
                getchar();
                getchar();
                break;

            case '0':
                clearConsole();
                printf(R"(Thank you, bye!

)");
                return MENU_EXIT;
        }
    } while (menuChoice != MENU_EXIT);

    return 0;
}