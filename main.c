#include <stdio.h>
#include <stdlib.h> //required for system()
#include <string.h>

// *** CONSTANTS ***

#define DISCOUNT_AGE 65
#define LICENSE_AGE 16
#define PURCHASE_AGE 18
#define MAX_SALES 20

#define MENU_BUY '1'
#define MENU_STAT_LIST '1'
#define MENU_CUSTOMER_INFO '2'
#define MENU_VIEW_STATS '3'
#define MENU_FEEDBACK '4'
#define MENU_EXIT '0'

//for creating "pseudo-booleans" as integer variables (C doesn't support boolean variables natively).
#define TRUE 1
#define FALSE 0


// *** VARIABLES ***

//unsigned short where no negative values expected.
unsigned short carChoice, carsSold = 0, carAmountPerSale[MAX_SALES], carsAvailable[] = {1, 23, 9, 4, 1, 10, 1}, userAge, carYearPerSale[MAX_SALES], menuChoiceValid = FALSE, carChoiceValid = FALSE, feedbackAvailable = FALSE, salesCount = 0;

char menuChoice, userName[201] = "N/A", customerNames[MAX_SALES][201], feedbackPerSale[MAX_SALES][501], menuFeedbackOption[201] = "";;

//discount is expressed via multiplier variable that changes depending on eligibility, 1 being 100% normal price with no discount. this also lets us avoid using the "pseudo-boolean" variable giveDiscount.
float totalSales = 0, discountMultiplier = 1, salesPerSale[MAX_SALES], discountPerSale[MAX_SALES];

float carModelPrices[] = {20000.00f, 15000.00f, 24300.00f, 69420.00f, 999999.99f, 29000.00f, 350.00f}, salesPerCar[] = {0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f};
//car prices corresponding for each of the models below.
char *carModelNames[] = {
    "FolksWagon Beatle", "Toyoda CarRolla", "Khevrollet Impaler", "Tegra Model L", "Tegra CyberDuck",
    "Geep Mangler", "Hounda Civil"
}; //position important, each position represents the same model in other arrays, allowing us to save multiple parameters for the same model.
unsigned short carModelYears[] = {1969, 2010, 2014, 2024, 2026, 2020, 1994};
char carModelYearsMenu[][20] = {"1969 (classic)", "2010", "2014", "2024", "2026 (pre-order)", "2020", "1994 (used)"}; //separate array just for menu display containing additional info for customer.
char carModelPerSale[MAX_SALES][20];

void clearBuffer() {
    while (getchar() != '\n');
}//clear the input buffer, useful in validation loops to prevent spamming errors when user inputs multiple chars.

void enterToReturn(unsigned int chars) {//TODO: remove unneeded parameter and clean up the rest of the code.
    printf("\n\nPress Enter to return to main menu...\n");
    getchar();
    //getchar();
    // do {
    //     getchar();
    //     chars--;
    // }while (chars > 1);
}//Enter to return message + program pause until input. parameter determines how many getchar() functions to call - use more if program doesn't pause, less if multiple inputs are needed.

#define CSV "salesData.csv"
FILE* file;
char fileStatus = 'C'; //'C' = closed, 'O' = open, 'E' = error.

FILE *fileCreate(char* fileName) {
    //"w" - write; "r" - read; "a" - append.
    file = fopen(fileName, "w");
    if (file != NULL) {
        fclose(file);
    }
    return file;
}

void fileOpen(char* fileName, char* mode) {
    file = fopen(fileName, mode);
    if (file == NULL) {//if file doesn't exist, create one.
        if (fileCreate(fileName) == NULL) {
            fileStatus = 'E';
        }
        else {
            fileOpen(fileName, mode);
        }
    }
    else {
        fileStatus = 'O';
    }
}//open file and perform an operation ("w" - write; "r" - read; "a" - append).

void fileClose() {
    if (fileStatus == 'O') {
        fclose(file);
        fileStatus = 'C';
    }
}//close file after use.

void fileReadData() {
    fileOpen(CSV, "r");
    if (fileStatus == 'O') {
        int totalData = fscanf(file, "%f;%hd;%[^\n]", &totalSales, &carsSold);
        for (int i = 0; i < 7; i++) {
            // unsigned short tempCarsSold = 0, tempCarsAvailable = 0;
            // float tempSalesPerCar;
            int perCarData = fscanf(file, "%f;%hd;%[^\n]", &salesPerCar[i], &carsAvailable[i]);
        }//data for all 7 cars in a simple loop of 7 iterations.
        while (1) {
            char line[2001];//buffer sizeable enough to hold a full line without overflow?
            unsigned short tempCarAmountPerSale = 0;
            float tempSalesPerSale = 0, tempDiscountPerSale = 0;
            if (fgets(line, sizeof(line), file) != NULL) {
                char tempFeedbackPerSale[501], tempCarModelPerSale[20], tempCustomerNames[201];
                sscanf(line, "%hd;%200[^;];%19[^;];%500[^;];%f;%f", &tempCarAmountPerSale, tempCustomerNames, tempCarModelPerSale, tempFeedbackPerSale, &tempSalesPerSale, &tempDiscountPerSale);
                if (strlen(line) == 0 || line[0] == '\n') {
                    continue;
                }
                carAmountPerSale[salesCount] = tempCarAmountPerSale;
                salesPerSale[salesCount] = tempSalesPerSale;
                discountPerSale[salesCount] = tempDiscountPerSale;
                strcpy(customerNames[salesCount], tempCustomerNames);
                strcpy(carModelPerSale[salesCount], tempCarModelPerSale);
                strcpy(feedbackPerSale[salesCount], tempFeedbackPerSale);
                // printf("%f /// %f \n", tempDiscountPerSale, discountPerSale[salesCount]); //debug
                salesCount++;
            }//parse line after line from the file until we parse it as NULL, meaning it's EOF.
            else {
                break;
            }
        }
    }
    else if (fileStatus == 'E') {
        printf("Error: There was a problem reading %s.", CSV);
        enterToReturn(2);
    }
    //fileClose();
}//pulls sales data from the file, used on init.

void fileWriteData() {
    fileOpen(CSV, "w");
    if (fileStatus == 'O') {
        fprintf(file, "%.2f;%hd\n", totalSales, carsSold);
        for (int i = 0; i < 7; i++) {
            // unsigned short tempCarsSold = 0, tempCarsAvailable = 0;
            // float tempSalesPerCar;
            fprintf(file, "%f;%hd\n", salesPerCar[i], carsAvailable[i]);
        }
        for (int i = 0; i <= salesCount; i++) {
            fprintf(file, "%hd;%s;%s;%s;%f;%f\n", carAmountPerSale[i], customerNames[i], carModelPerSale[i], feedbackPerSale[i], salesPerSale[i], discountPerSale[i]);//using semicolon here as a delimiter in order to avoid problems when commas appear in user input like in feedbackPerSale.
        }
    }
    else if (fileStatus == 'E') {
        printf("Error: There was a problem writing into %s.", CSV);
        enterToReturn(2);
    }
    fileClose();
}//updates the file with finalised sales data.

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
    clearBuffer();
    return userInput;
} //user input of a single char (menu selection)

void inputStr(char prompt[201], char *output) {
    printf("%s", prompt);
    char userInput[201];
    scanf(" %[^\n]", output);
    clearBuffer();
} //user input of a string.

unsigned short inputUnsignedShort(char prompt[201]) {
    printf("%s", prompt);
    unsigned short userInput;
    scanf("%hd", &userInput);
    clearBuffer();
    return userInput;
} //user input of a single unsigned short.

void menuWelcome() {
    clearConsole(); //just to remove any compiler warnings.
    fileReadData();
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

int validationInputLength(char string[], unsigned int maxLength) {
    if (strlen(string) > maxLength) {
        return 0;
    }
    return 1;
}//validate if the user's input string is not too long to avoid overflow. returns 1 on pass, 2 on fail.

void menuFeedback() {
    clearConsole();
    if (feedbackAvailable == FALSE) {
        printf(R"(*** LEAVE FEEDBACK ***

Please enter your details and make a purchase before leaving feedback.
(Main menu option [2], then [2]))");
        enterToReturn(2);
        return;
    }
    salesCount--; //temporarily going back by 1, because we increased it when finalising the purchase.
    printf("*** LEAVE FEEDBACK ***\n\n\n");
    inputStr("Please enter a short message to express your feedback:\n\n", feedbackPerSale[salesCount]);
    while (validationInputLength(feedbackPerSale[salesCount], 500) == 0){
        clearConsole();
        printf("*** LEAVE FEEDBACK ***\n\n\n");
        inputStr("I'm sorry, but your message exceeds the 1000 character limit, please enter a short message to express your feedback:\n\n", feedbackPerSale[salesCount]);
    }
    clearConsole();
    printf("*** LEAVE FEEDBACK ***\n\n\nYour feedback is appreciated! <3");
    fileWriteData();
    salesCount++; //bring back the sales count to the next customer.
    feedbackAvailable = FALSE;
    enterToReturn(3);
}//menu for providing feedback.

void menuBuy() {
    if (strcmp("N/A", userName) == 0) {
        printf(R"(*** BUY CARS ***

Please enter your details before making a purchase.
(Main menu option [2]))");
        enterToReturn(3);
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
    carChoice--;//b/c user choice will always be index+1, we reduce it here so we can point to actual index in the rest of the code.

    //checking if there are cars left.
    if (carsAvailable[carChoice] < 1) {
        printf(
            "I'm sorry, but there are no more %s units available in stock at the moment.",
            carModelNames[carChoice]);
        enterToReturn(3);
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
        enterToReturn(3);
        return;
    }

    clearConsole();
    strcpy(customerNames[salesCount], userName); //commit temp userName into a permanent customerNames array.
    strcpy(carModelPerSale[salesCount], carModelNames[carChoice]); //record which model was purchased.
    salesPerSale[salesCount] = carAmountPerSale[salesCount] * carModelPrices[carChoice] * discountMultiplier;
    //calculate total for this sale.
    carsAvailable[carChoice] -= carAmountPerSale[salesCount]; //update car stock.
    carsSold += carAmountPerSale[salesCount]; //update total car sold counter.
    salesPerCar[carChoice] += salesPerSale[salesCount];
    discountPerSale[salesCount] = (discountMultiplier - 1.f) * 100.f;

    printf(R"(*** BUY CARS ***

You have purchased %hu %s cars. The total cost is %.2f GBP. Thank you for your business!
)",
        carAmountPerSale[salesCount], carModelNames[carChoice], salesPerSale[salesCount]);
    //to output 2 decimal points of the float only, we can use ".2" after % and before f.
    totalSales += salesPerSale[salesCount]; //update total sales counter.

    strcpy(feedbackPerSale[salesCount], "N/A");
    feedbackAvailable = TRUE;
    fileWriteData();
    salesCount++; //update the sale number for the next sale.
    char yesNo = inputChar("Would you like to leave feedback about your experience? [y/n] ");
    while (yesNo != 'y' && yesNo != 'n' && yesNo != 'Y' && yesNo != 'N') {
        yesNo = inputChar("\nInvalid choice. please enter a single character and press enter. [y/n] ");
    }
    if (yesNo == 'y' || yesNo == 'Y') {
        menuFeedback();
    }
} //car purchase menu.

void fillSpace(char **string, int index, char *output) {
    if (string == NULL || string[index] == NULL || output == NULL) {
        printf("Null pointer error in fillSpace\n");
        return;
    }
    unsigned int numberOfStrings = 0;
    unsigned int maxLength = 0;
    while (string[numberOfStrings] != NULL) {
        unsigned int length = strlen(string[numberOfStrings]);
        if (length > maxLength) {
            maxLength = length;
        }
        numberOfStrings++;
    }
    unsigned int spaceCount = maxLength - strlen(string[index]);
    output[0] = '\0';
    for (unsigned int i = 0; i < spaceCount; i++){
        strcat(output, " ");
    }
}//outputs a string of space chars, the amount equals to the max length of any string from STRING array minus the INDEX string's length. use it after a string to print spaces to make equal length columns.

void menuStatistics() {
    clearConsole();
    printf(
        R"(*** STATISTICS ***

- Cars available:
)");
    char *tempCarsAvailable[8] = {NULL};
    for (int i = 0; i < 7; i++) {
        tempCarsAvailable[i] = malloc(10);//allocate some memory for the pointer to avoid those pesky segmentation errors.
        if (tempCarsAvailable[i] == NULL) {
            perror("mem alloc failed. :/");
            exit(1);
        }//for testing purposes

        //converting integers into strings
        sprintf(tempCarsAvailable[i], "x%hd", carsAvailable[i]);
    }
    for (int i = 0; i < 7; i++) {
        //sprintf(tempSalesPerCar, "%f", salesPerCar[i]);
        char tempSpace1[201] = "\0", tempSpace2[201] = "\0";
        fillSpace(carModelNames, i, tempSpace1);
        fillSpace(tempCarsAvailable, i, tempSpace2);
        printf(R"(
    - %s %s (%hd): %s %s-- %.2f.)", carModelNames[i], tempSpace1, carModelYears[i], tempCarsAvailable[i], tempSpace2, salesPerCar[i]);//TODO: fix spacing here.
        strcpy(tempSpace1, "");
        strcpy(tempSpace2, "");
        //free(tempCarsAvailable[i]);//note to self: don't free up allocated memory until every string in the array is done - turns out when you free the memory it gets filled with junk bits which actually change the string's length!
    }
    for (int i = 0; i < 7; i++) {
        free(tempCarsAvailable[i]);//free the allocated memory after we're done, so we can allocate it again.
    }
    printf(R"(

- Total cars sold:      -- %hd.
- Total car sales:      -- %.2f GBP.

Press Enter to proceed to the transaction list.
)",
           carsSold, totalSales);
    getchar();
    clearConsole();
    printf("*** TRANSACTION LIST ***\n\n");

    for (int i = 0; i < salesCount; i++) {
        char tempSpace1[201] = "\0", tempSpace2[201] = "\0", tempSpace3[201] = "\0";
        printf(R"(#%d %s:%s x%hu%s %s%s -- %.2f GBP (%.0f%% discount)
)",
               i + 1, customerNames[i], tempSpace1, carAmountPerSale[i], tempSpace2, carModelPerSale[i], tempSpace3, salesPerSale[i],
               discountPerSale[i]);
    }
enterToReturn(1);
}//remaining stock + sale statistics menu.

void menuMain() {
    if (feedbackAvailable == TRUE) {
        strcpy(menuFeedbackOption, "\n[4] Provide feedback on your purchase!\n");
    }
    else {
        strcpy(menuFeedbackOption, "");
    }//hide the feedback option in the main menu if it's unavailable.

    clearConsole(); //our function will clear console every time the user returns to main menu.
    printf(R"(*** MAIN MENU ***

Current User: %s
Please choose from the following options:
[1] Buy cars (%.0f%% discount).
[2] Update user information.
[3] View stock and sales statistics.%s
[0] Exit.
)", userName, (discountMultiplier - 1) * 100, menuFeedbackOption);

    menuChoiceValid = FALSE; //reset the boolean before checking menu choice.

    do {
        //valid choice check:
        menuChoice = inputChar("Enter your choice: \n");

        if (menuChoice == MENU_BUY || menuChoice == MENU_EXIT || menuChoice == MENU_VIEW_STATS || menuChoice ==
            MENU_CUSTOMER_INFO || menuChoice == MENU_FEEDBACK) {
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
        enterToReturn(3);
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
        enterToReturn(3);
        return;
    }
    if (userAge >= DISCOUNT_AGE) {
        discountMultiplier = 0.85;
        printf(R"(*** CUSTOMER INFORMATION ***

A 15%% senior discount has been applied!
User info saved, please proceed to your purchase.)");
        feedbackAvailable = FALSE;
        enterToReturn(3);
        return;
    }
    discountMultiplier = 1;
    feedbackAvailable = FALSE;
    printf(R"(*** CUSTOMER INFORMATION ***

You are not eligible for a discount.
User info saved, please proceed to your purchase.)");
    enterToReturn(3);
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
            case MENU_FEEDBACK:
                menuFeedback();
                break;
            case MENU_EXIT:
                clearConsole();
                printf(R"(Thank you, bye!

)");
                break;
        }
    } while (menuChoice != MENU_EXIT);
    return 0;
}
