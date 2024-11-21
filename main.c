#include <stdio.h>
#include <stdlib.h> //required for system()
#include <string.h>

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
    //CONSTANTS

#define CAR_PRICE 15000.00f //defining car price, should use f at the end of the value if floating.
#define DISCOUNT_AGE 65
#define LICENSE_AGE 16
#define PURCHASE_AGE 18
    //#define NUMBER_OF_MODELS 8
    //the reason we define these is to improve readability of our code - instead of using 1, 2, etc. in our code, each option used is a definition with immediately clear meaning.
    //defining these as chars instead of integers so the program is able to check any keyboard input against expected ones.
#define MENU_BUY '1'
#define MENU_STAT_LIST '1'
#define MENU_APPLY_DISCOUNT '2'
#define MENU_VIEW_STATS '3'
#define MENU_EXIT '0'
    //for creating "pseudo-booleans" as integer variables (C doesn't support boolean variables natively).
#define MAX_SALES 20
#define TRUE 1
#define FALSE 0

    //

    //VARIABLES

    //unsigned short where no negative values expected.
    short carsAvailable[] = {14, 23, 9, 4, 1, 10, 1};
    unsigned short carChoice, carsSold = 0, carAmountPerSale[MAX_SALES];

    int menuChoiceValid = FALSE, carChoiceValid = FALSE, userAge, salesCount = 0;

    char menuChoice, customerNames[MAX_SALES][201];

    //discount is expressed via multiplier variable that changes depending on eligibility, 1 being 100% normal price with no discount. this also lets us avoid using the "pseudo-boolean" variable giveDiscount.
    float totalSales = 0, discountMultiplier = 1, sales[MAX_SALES], discountPerSale[MAX_SALES];

    float carModelPrices[] = {20000.00f, 15000.00f, 24300.00f, 69420.00f, 999999.99f, 29000.00f, 350.00f};
    //car prices corresponding for each of the models below.
    char carModelNames[][20] = {
        "FolksWagon Beatle", "Toyoda CarRolla", "Khevrollet Impaler", "Tegra Model L", "Tegra CyberDuck",
        "Geep Mangler", "Hounda Civil 1994"
    }; //position important, each position represents the same model in other arrays, allowing us to save multiple parameters for the same model.
    char carModelPerSale[MAX_SALES][20];
    //

    printf("Hello, beloved customer! Welcome to the car sales program.\n");

    //this is the loop for the menu - it repeats as long as the menuChoice is not selected by the user as 0 (exit).
    do {
        //main menu loop

        clearConsole(); //our function will clear console every time the user returns to main menu.

        printf(R"(*** MAIN MENU ***

Please choose from the following options:
[1] Buy cars (%.0f%% discount).
[2] Apply discount.
[3] View stock and sales statistics.
[0] Exit.
)", (discountMultiplier-1)*100);

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
[1] Buy cars (%.0f%% discount).
[2] Apply discount.
[3] View stock and sales statistics.
[0] Exit.
)", (discountMultiplier-1)*100);
                }
        } while (menuChoiceValid == FALSE);

        clearConsole();

        switch (menuChoice) {
            case MENU_BUY:
                printf(R"(*** BUY CARS ***

We currently have these models in stock:
)");
            for (int i = 0; i < 7; i++) {
                printf("[%d] %s, %.2f GBP (stock: %hd).\n", i + 1, carModelNames[i], carModelPrices[i], carsAvailable[i]);
            }
            //b/c we are starting the menu with [1] and reserving [0] for back / exit, we print i+1.
            printf("[0] Return to main menu.\nPlease enter your choice:\n");
            scanf("%hd", &carChoice);
            if(carChoice == 0) {
                break;
            }
            carChoice--;
            //b/c user choice will always be index+1, we reduce it here so we can point to actual index in the rest of the code.

            //checking if there are cars left.
            if (carsAvailable[carChoice] < 1) {
                printf(
                    "I'm sorry, but there are no more %s units available in stock at the moment.\n\nPress any key to return to main menu...\n",
                    carModelNames[carChoice]);
                getchar();
                getchar();
                break;
            }
            clearConsole();
            printf(R"(*** BUY CARS ***

How many of %s cars would you like to purchase?
Please enter the amount:
)", carModelNames[carChoice]);
            scanf("%hu", &carAmountPerSale[salesCount]);

            //checking if there are cars of a specific model left.
            if (carAmountPerSale[salesCount] > carsAvailable[carChoice]) {
                printf(
                    "\nI'm sorry, but we only have %hd %s cars remaining in stock, please try again!\n\nPress any key to return to main menu...\n",
                    carsAvailable[carChoice], carModelNames[carChoice]);
                getchar();
                getchar();
                break;
            }

            clearConsole();
            printf(R"(*** BUY CARS ***

Please enter your full name:
)");

            //adding a " " before "%" avoids potential newline or space character that might be in the console's input buffer.
            //%[^\n] will read every char from input until \n.
            scanf(" %[^\n]", customerNames[salesCount]); //this will start at salesCount = 0.

            sales[salesCount] = carAmountPerSale[salesCount] * carModelPrices[carChoice] * discountMultiplier;//calculate total for this sale.
            carsAvailable[carChoice] -= carAmountPerSale[salesCount];//update car stock.
            strcpy(carModelPerSale[salesCount], carModelNames[carChoice]);//record which model was purchased.
            carsSold += carAmountPerSale[salesCount];//update total car sold counter.
            discountPerSale[salesCount] = (discountMultiplier - 1) * 100;

            printf(
                "You have purchased %hu %s cars. The total cost is %.2f GBP. Thank you for your business!\n\nPress any key to return to main menu...\n",
                carAmountPerSale[salesCount], carModelNames[carChoice], sales[salesCount]);
            //to output 2 decimal points of the float only, we can use ".2" after % and before f.
            totalSales += sales[salesCount];//update total sales counter.
            discountMultiplier = 1; //reset discount status.
            salesCount++; //update the sale number for the next sale.
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
                    "A 15%% senior discount has been applied to your NEXT purchase!\n\nPress any key to return to main menu...\n");
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

- Cars available:
)");
            for (int i = 0; i < 7; i++) {
                printf(R"(
    - %s: %hd.)", carModelNames[i], carsAvailable[i]);
            }
            printf(R"(

- Total cars sold:      -- %hd.
- Total car sales:      -- %.2f GBP.

*** TRANSACTION LIST ***

)",
                carsSold, totalSales);
            for (int i = 0; i < salesCount; i++) {
                printf(R"(#%d %s: %s (x%hu) -- %.2f GBP (%.0f%% discount)
)",
                       i+1, customerNames[i], carModelPerSale[i], carAmountPerSale[i], sales[i], discountPerSale[i]);
            }
            printf("\n\nPress any key to return to main menu.\n");
            getchar();
            getchar();
            break;

            case '0':
                clearConsole();
            printf(R"(Thank you, bye!

)");
            return MENU_EXIT;
        }
    }while(menuChoice != MENU_EXIT);

    return 0;
}
