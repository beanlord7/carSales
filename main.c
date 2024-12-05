#include <stdio.h>
#include <stdlib.h> //required for system()
#include <string.h>

//CONSTANTS

#define DISCOUNT_AGE 65
#define LICENSE_AGE 16
#define PURCHASE_AGE 18
#define MENU_BUY '1'
#define MENU_STAT_LIST '1'
#define MENU_CUSTOMER_INFO '2'
#define MENU_VIEW_STATS '3'
#define MENU_FEEDBACK '4'
#define MENU_EXIT '0'
//for creating "pseudo-booleans" as integer variables (C doesn't support boolean variables natively).
#define MAX_SALES 20
#define TRUE 1
#define FALSE 0

//

//VARIABLES

//unsigned short where no negative values expected.
unsigned short carChoice, carsSold = 0, carAmountPerSale[MAX_SALES], carsAvailable[] = {1, 23, 9, 4, 1, 10, 1}, userAge, carYearPerSale[MAX_SALES], menuChoiceValid = FALSE, carChoiceValid = FALSE, feedbackAvailable = FALSE, salesCount = 0;

char menuChoice, userName[201] = "N/A", customerNames[MAX_SALES][201], feedback[1001];

//discount is expressed via multiplier variable that changes depending on eligibility, 1 being 100% normal price with no discount. this also lets us avoid using the "pseudo-boolean" variable giveDiscount.
float totalSales = 0, discountMultiplier = 1, sales[MAX_SALES], discountPerSale[MAX_SALES];

float carModelPrices[] = {20000.00f, 15000.00f, 24300.00f, 69420.00f, 999999.99f, 29000.00f, 350.00f};
//car prices corresponding for each of the models below.
char *carModelNames[] = {
    "FolksWagon Beatle", "Toyoda CarRolla", "Khevrollet Impaler", "Tegra Model L", "Tegra CyberDuck",
    "Geep Mangler", "Hounda Civil"
}; //position important, each position represents the same model in other arrays, allowing us to save multiple parameters for the same model.
unsigned short carModelYears[] = {1969, 2010, 2014, 2024, 2026, 2020, 1994};
char carModelYearsMenu[][20] = {"1969 (classic)", "2010", "2014", "2024", "2026 (pre-order)", "2020", "1994 (used)"}; //separate array just for menu display containing additional info for customer.
char carModelPerSale[MAX_SALES][20];

void clearConsole() {
    //#ifdef, #elif, #else and #endif are used for conditional compilation, and represent "if defined", "else if" "else" and "end if" ("break"). the clearConsole() function will be resolved when compiling, and then can be used throughout the code later.
#ifdef _WIN32//means "if defined" and checks if a macro is defined when compiling. "_WIN32" will be defined on windows (both 32 and 64 bit).

    system("cls");
#elif __linux__ || __APPLE__//"__linux__" will be defined on linux, and __APPLE__ will be defined in macOS.
    system("clear");
#endif
} //clears terminal window in windows, linux, and macOS.

char inputChar(char prompt[201]) {
    printf("%s", prompt);
    //function's parameter is a prompt that we print for the user to know what sort of input is required.
    char userInput;
    scanf(" %c", &userInput);
    return userInput;
} //user input of a single char (menu selection)

void inputStr(char prompt[201], char *output) {
    printf("%s", prompt);
    char userInput[201];
    scanf(" %[^\n]", output);
} //user input of a string.

unsigned short inputUnsignedShort(char prompt[201]) {
    printf("%s", prompt);
    unsigned short userInput;
    scanf("%hd", &userInput);
    return userInput;
} //user input of a single unsigned short.

void enterToReturn(unsigned int chars) {
    printf("\n\nPress Enter to return to main menu...\n");
    do {
        getchar();
        chars--;
    }while (chars > 1);
} //Enter to return message + program pause until input. parameter determines how many getchar() functions to call - use more if program doesn't pause, less if multiple inputs are needed.

void menuWelcome() {
    clearConsole(); //just to remove any compiler warnings.
    printf(R"(
  _|_|_|    _|_|    _|_|_|        _|_|_|    _|_|    _|        _|_|_|_|    _|_|_|
_|        _|    _|  _|    _|    _|        _|    _|  _|        _|        _|
_|        _|_|_|_|  _|_|_|        _|_|    _|_|_|_|  _|        _|_|_|      _|_|
_|        _|    _|  _|    _|          _|  _|    _|  _|        _|              _|
  _|_|_|  _|    _|  _|    _|    _|_|_|    _|    _|  _|_|_|_|  _|_|_|_|  _|_|_| v0.9

Hello, beloved customer! Welcome to the Car Sales program.
Please enter your personal details before making a purchase.

Press Enter to continue...)");
    getchar();
} //welcome screen, appears only once.

void menuBuy() {
    if (strcmp("N/A", userName) == 0) {
        printf(R"(*** BUY CARS ***

Please enter your details before making a purchase.
(Main menu option [2]))");
        enterToReturn(2);
        return;
    }
    printf(R"(*** BUY CARS ***

We currently have these models in stock:
)");
    for (int i = 0; i < 7; i++) {
        printf("[%d] %s %s, %.2f GBP (stock: %hd).\n", i + 1, carModelNames[i], carModelYearsMenu[i],
               carModelPrices[i] * discountMultiplier, carsAvailable[i]);
    }
    //b/c we are starting the menu with [1] and reserving [0] for back / exit, we print i+1.
    printf("[0] Return to main menu.");
    carChoice = inputUnsignedShort("\nPlease enter your choice:\n");
    if (carChoice == 0) {
        return;
    }
    carChoice--;
    //b/c user choice will always be index+1, we reduce it here so we can point to actual index in the rest of the code.

    //checking if there are cars left.
    if (carsAvailable[carChoice] < 1) {
        printf(
            "I'm sorry, but there are no more %s units available in stock at the moment.",
            carModelNames[carChoice]);
        enterToReturn(2);
        return;
    }
    clearConsole();
    printf(R"(*** BUY CARS ***

How many of %s cars would you like to purchase?
)", carModelNames[carChoice]);
    carAmountPerSale[salesCount] = inputUnsignedShort("Please enter the amount:\n");

    //checking if there are cars of a specific model left.
    if (carAmountPerSale[salesCount] > carsAvailable[carChoice]) {
        printf(
            "\nI'm sorry, but we only have %hd %s cars remaining in stock, please try again!",
            carsAvailable[carChoice], carModelNames[carChoice]);
        enterToReturn(2);
        return;
    }

    clearConsole();
    strcpy(customerNames[salesCount], userName); //commit temp userName into a permanent customerNames array.
    strcpy(carModelPerSale[salesCount], carModelNames[carChoice]); //record which model was purchased.
    sales[salesCount] = carAmountPerSale[salesCount] * carModelPrices[carChoice] * discountMultiplier;
    //calculate total for this sale.
    carsAvailable[carChoice] -= carAmountPerSale[salesCount]; //update car stock.
    carsSold += carAmountPerSale[salesCount]; //update total car sold counter.
    discountPerSale[salesCount] = (discountMultiplier - 1) * 100;

    printf(R"(*** BUY CARS ***

You have purchased %hu %s cars. The total cost is %.2f GBP. Thank you for your business!)",
        carAmountPerSale[salesCount], carModelNames[carChoice], sales[salesCount]);
    //to output 2 decimal points of the float only, we can use ".2" after % and before f.
    totalSales += sales[salesCount]; //update total sales counter.
    salesCount++; //update the sale number for the next sale.
    feedbackAvailable = TRUE;
    enterToReturn(2);
} //car purchase menu.

void menuFeedback() {
    if (feedbackAvailable == FALSE) {
        printf(R"(*** LEAVE FEEDBACK ***

Please enter your details and make a purchase before leaving feedback.
(Main menu option [2], then [2]))");
        enterToReturn(2);
        return;
    }
    printf("*** LEAVE FEEDBACK ***\n\n\n");
    inputStr("Please enter a short message to express your feedback:\n", feedback);
}

void fillSpace(char **string, int index, char *output) {
    unsigned int numberOfStrings = 0;
    unsigned int maxLength = 0;
    while (string[numberOfStrings] != NULL) {
        if (strlen(string[numberOfStrings]) > maxLength) {
            maxLength = strlen(string[numberOfStrings]);
        }
        numberOfStrings++;
    }
    unsigned int spaceCount = maxLength - strlen(string[index]);
    for (unsigned int i = 0; i < spaceCount; i++){
        strcat(output, " ");
    }
}

void menuStatistics() {
    clearConsole();
    printf(
        R"(*** STATISTICS ***

- Cars available:)");
    for (int i = 0; i < 7; i++) {
        char tempSpace[201] = "";
        fillSpace(carModelNames, i, tempSpace);
        printf(R"(
    - %s %s %hd: %hd.)", carModelNames[i], tempSpace, carModelYears[i], carsAvailable[i]);
        strcpy(tempSpace, "");
    }
    printf(R"(
- Total cars sold:      -- %hd.
- Total car sales:      -- %.2f GBP.

Press Enter to proceed to the transaction list.
)",
           carsSold, totalSales);
    getchar();
    getchar();
    clearConsole();
    printf("*** TRANSACTION LIST ***\n\n");

    for (int i = 0; i < salesCount; i++) {
        printf(R"(#%d %s: %s (x%hu) -- %.2f GBP (%.0f%% discount)
)",
               i + 1, customerNames[i], carModelPerSale[i], carAmountPerSale[i], sales[i],
               discountPerSale[i]);
    }
enterToReturn(1);
}

void menuMain() {
    //main menu function
    clearConsole(); //our function will clear console every time the user returns to main menu.
    printf(R"(*** MAIN MENU ***

Current User: %s
Please choose from the following options:
[1] Buy cars (%.0f%% discount).
[2] Update user information.
[3] View stock and sales statistics.
[0] Exit.
)", userName, (discountMultiplier - 1) * 100);

    menuChoiceValid = FALSE; //reset the boolean before checking menu choice.

    do {
        //valid choice check:
        menuChoice = inputChar("Enter your choice: \n");

        if (menuChoice == MENU_BUY || menuChoice == MENU_EXIT || menuChoice == MENU_VIEW_STATS || menuChoice ==
            MENU_CUSTOMER_INFO) {
            menuChoiceValid = TRUE;
        } else {
            clearConsole();
            printf(R"(*** MAIN MENU ***

Current User: %s
Sorry, menu choice invalid, please try again:
[1] Buy cars (%.0f%% discount).
[2] Update user information.
[3] View stock and sales statistics.
[0] Exit.
)", userName, (discountMultiplier - 1) * 100);
        }
    } while (menuChoiceValid == FALSE);
} //main menu.

void menuCustomerInfo() {
    printf("*** CUSTOMER INFORMATION ***\n\n");
    inputStr("Please enter your full name: ", userName);
    userAge = inputUnsignedShort("\nWe are currently offering a 15%% discount for seniors.\nPlease enter your age: ");
    //TODO create user validation for age input (only a whole int value should be allowed and recorded.
    clearConsole();
    //checking for purchase legal eligibility.
    if (userAge < LICENSE_AGE) {
        printf(R"(*** CUSTOMER INFORMATION ***

I'm sorry, but you have to be 16 years old or older to legally drive a car. The sale cannot be completed.)");
        userAge = 0;
        strcpy(userName, "N/A");
        enterToReturn(2);
        feedbackAvailable = FALSE;
        return;
    }
    if (userAge >= LICENSE_AGE && userAge < PURCHASE_AGE) {
        printf(R"(*** CUSTOMER INFORMATION ***

I'm sorry, but at %d years old you cannot legally purchase a car by yourself. In order to complete the purchase, please have your parent / guardian use this program instead.)",
               userAge);
        userAge = 0;
        strcpy(userName, "N/A");
        feedbackAvailable = FALSE;
        enterToReturn(2);
        return;
    }
    if (userAge >= DISCOUNT_AGE) {
        discountMultiplier = 0.85;
        printf(R"(*** CUSTOMER INFORMATION ***

A 15%% senior discount has been applied!
User info saved, please proceed to your purchase.)");
        enterToReturn(2);
        feedbackAvailable = FALSE;
        return;
    }
    discountMultiplier = 1;
    feedbackAvailable = FALSE;
    printf(R"(*** CUSTOMER INFORMATION ***

You are not eligible for a discount.
User info saved, please proceed to your purchase.)");
    enterToReturn(2);
} //customer info recording menu.

int main() {
    menuWelcome();
    do {
        //this is the loop for the menu - it repeats as long as the menuChoice is not selected by the user as 0 (exit).
        menuMain();

        clearConsole();

        switch (menuChoice) {
            case MENU_BUY:
                menuBuy();
                break;
            case MENU_CUSTOMER_INFO:
                menuCustomerInfo();
                break;
            case MENU_VIEW_STATS:
                menuStatistics();
                break;
            // case MENU_FEEDBACK:
            //     menuFeedback();
            //     break;
            case MENU_EXIT:
                clearConsole();
                printf(R"(Thank you, bye!

)");
                break;
        }
    } while (menuChoice != MENU_EXIT);
    return 0;
}
