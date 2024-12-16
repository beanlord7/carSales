#include <stdio.h>
#include <stdlib.h> //required for system()
#include <string.h>

// *** CONSTANTS ***

#define DISCOUNT_AGE 65
#define LICENSE_AGE 16
#define PURCHASE_AGE 18
#define MAX_SALES 20
#define CAR_MODEL_COUNT 7

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

//starting to use size_t instead of unsigned int because apparently it is portable between 32bit and 64bit systems and automatically adjusts, and helps avoid overflows sometimes. but also it's faster to type!
size_t carChoice, carsSold = 0, carAmountPerSale[MAX_SALES], carsAvailable[] = {1, 23, 9, 4, 1, 10, 1}, userAge, carYearPerSale[MAX_SALES], menuChoiceValid = FALSE, carChoiceValid = FALSE, feedbackAvailable = FALSE, salesCount = 0;

char menuChoice, userName[201] = "N/A", customerNames[MAX_SALES][201], feedbackComment[MAX_SALES][501], menuFeedbackOption[201] = "";;

//discount is expressed via multiplier variable that changes depending on eligibility, 1 being 100% normal price with no discount. this also lets us avoid using the "pseudo-boolean" variable giveDiscount.
float totalSales = 0, discountMultiplier = 1, salesPerSale[MAX_SALES], discountPerSale[MAX_SALES];

float carModelPrices[] = {20000.00f, 15000.00f, 24300.00f, 69420.00f, 999999.99f, 29000.00f, 350.00f}, salesPerCar[] = {0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f};
//car prices corresponding for each of the models below.
char *carModelNames[] = {
    "FolksWagon Beatle", "Toyoda CarRolla", "Khevrollet Impaler", "Tegra Model L", "Tegra CyberDuck",
    "Geep Mangler", "Hounda Civil"
}; //position important, each position represents the same model in other arrays, allowing us to save multiple parameters for the same model.
char *carModelFeedback[];
size_t carModelYears[] = {1969, 2010, 2014, 2024, 2026, 2020, 1994};
char carModelYearsMenu[][20] = {"1969 (classic)", "2010", "2014", "2024", "2026 (pre-order)", "2020", "1994 (used)"}; //separate array just for menu display containing additional info for customer.
char carModelPerSale[MAX_SALES][20];

void clearBuffer() {
    while (getchar() != '\n');
}//clear the input buffer, useful in validation loops to prevent spamming errors when user inputs multiple chars.

void enterToReturn() {
    printf("\n\nPress Enter to return to main menu...\n");
    getchar();
    //getchar();
    // do {
    //     getchar();
    //     chars--;
    // }while (chars > 1);
}//Enter to return message + program pause until input. parameter determines how many getchar() functions to call - use more if program doesn't pause, less if multiple inputs are needed.

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
    fileOpen("salesData.csv", "r");
    if (fileStatus == 'O') {
        int totalData = fscanf(file, "%f;%zu;%[^\n]", &totalSales, &carsSold);
        for (int i = 0; i < CAR_MODEL_COUNT; i++) {
            // unsigned short tempCarsSold = 0, tempCarsAvailable = 0;
            // float tempSalesPerCar;
            int perCarData = fscanf(file, "%f;%zu;%[^\n]", &salesPerCar[i], &carsAvailable[i]);
        }//data for all 7 cars in a simple loop of 7 iterations.
        while (1) {
            char line[2001];//buffer sizeable enough to hold a full line without overflow?
            size_t tempCarAmountPerSale = 0;
            float tempSalesPerSale = 0, tempDiscountPerSale = 0;
            if (fgets(line, sizeof(line), file) != NULL) {
                char tempFeedbackPerSale[501], tempCarModelPerSale[20], tempCustomerNames[201];
                sscanf(line, "%zu;%200[^;];%19[^;];%500[^;];%f;%f", &tempCarAmountPerSale, tempCustomerNames, tempCarModelPerSale, tempFeedbackPerSale, &tempSalesPerSale, &tempDiscountPerSale);
                if (strlen(line) == 0 || line[0] == '\n') {
                    continue;
                }
                carAmountPerSale[salesCount] = tempCarAmountPerSale;
                salesPerSale[salesCount] = tempSalesPerSale;
                discountPerSale[salesCount] = tempDiscountPerSale;
                strcpy(customerNames[salesCount], tempCustomerNames);
                strcpy(carModelPerSale[salesCount], tempCarModelPerSale);
                strcpy(feedbackComment[salesCount], tempFeedbackPerSale);
                // printf("%f /// %f \n", tempDiscountPerSale, discountPerSale[salesCount]); //debug
                salesCount++;
            }//parse line after line from the file until we parse it as NULL, meaning it's EOF.
            else {
                break;
            }
        }
    }
    else if (fileStatus == 'E') {
        printf("Error: There was a problem reading salesData.csv.");
        enterToReturn();
    }
    //fileClose();
}//pulls sales data from the file, used on init.

void fileWriteData() {
    fileOpen("salesData.csv", "w");
    if (fileStatus == 'O') {
        fprintf(file, "%.2f;%zu\n", totalSales, carsSold);
        for (int i = 0; i < CAR_MODEL_COUNT; i++) {
            // unsigned short tempCarsSold = 0, tempCarsAvailable = 0;
            // float tempSalesPerCar;
            fprintf(file, "%f;%zu\n", salesPerCar[i], carsAvailable[i]);
        }
        for (int i = 0; i <= salesCount; i++) {
            fprintf(file, "%zu;%s;%s;%s;%f;%f\n", carAmountPerSale[i], customerNames[i], carModelPerSale[i], feedbackComment[i], salesPerSale[i], discountPerSale[i]);//using semicolon here as a delimiter in order to avoid problems when commas appear in user input like in feedbackPerSale.
        }
    }
    else if (fileStatus == 'E') {
        printf("Error: There was a problem writing into salesData.csv.");
        enterToReturn();
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

size_t inputUnsignedInteger(char prompt[201]) {
    printf("%s", prompt);
    size_t userInput;
    scanf("%zu", &userInput);
    clearBuffer();
    return userInput;
} //user input of a single unsigned short.

size_t maxStrLen(char **string) {
    unsigned int numberOfStrings = 0;
    unsigned int maxLength = 0;
    while (string[numberOfStrings] != NULL) {
        unsigned int length = strlen(string[numberOfStrings]);
        if (length > maxLength) {
            maxLength = length;
        }
        numberOfStrings++;
    }
    return maxLength;
}//returns the max length of any string from an array of strings.

void sortStrByABC(char **string, size_t maxLength, int* positionMarker) {
    for (int i = 0; i < salesCount - 1; i++) {
        for (int j = i + 1; j < salesCount; j++) {
            if (strcmp(string[i], string[j]) < 0) {//strcmp will return <0 if string 1 is less than 2nd, 0 if equal, >0 if first string is less than 2nd.
                char temp[maxLength];//using a temp array (size has to be pre-determined in the parameters)
                strcpy(temp, string[j]);
                strcpy(string[i], string[j]);
                strcpy(string[j], string [i]);
            }
        }
    }
}//pointless alphabetical sort function due to me making it before noticing it's not being required by the assignment. :)

void sortStatistics() {
    for (int i = 0; i < CAR_MODEL_COUNT - 1; i++) {
        for (int j = i + 1; j < CAR_MODEL_COUNT; j++) {
            if (carModelYears[i] < carModelYears[j]) {
                size_t temp = carModelYears[i];
                carModelYears[i] = carModelYears[j];
                carModelYears[j] = temp;
                temp = carsAvailable[i];
                carsAvailable[i] = carsAvailable[j];
                carsAvailable[j] = temp;
                char tempStr[20]; //max length of car model name will be 19 (+1 for '/0')
                strcpy(tempStr, carModelYearsMenu[i]);
                strcpy(carModelYearsMenu[i], carModelYearsMenu[j]);
                strcpy(carModelYearsMenu[j], tempStr);
                char *tempStr2 = carModelNames[i]; //we use a pointer for this one.
                carModelNames[i] = carModelNames[j];
                carModelNames[j] = tempStr2;
                float tempFloat = carModelPrices[i];
                carModelPrices[i] = carModelPrices[j];
                carModelPrices[j] = tempFloat;
                tempFloat = salesPerCar[i];
                salesPerCar[i] = salesPerCar[j];
                salesPerCar[j] = tempFloat;
            }
        }
    }
}//sorts the statistics screen by car manufacture date in descending order.

void sortTransactionList() {
    for (int i = 0; i < salesCount - 1; i++) {
        for (int j = i + 1; j < salesCount; j++) {
                if (salesPerSale[i] < salesPerSale[j]) {
                    float tempFloat = salesPerSale[i];
                    salesPerSale[i] = salesPerSale[j];
                    salesPerSale[j] = tempFloat;
                    tempFloat = discountPerSale[i];
                    discountPerSale[i] = discountPerSale[j];
                    discountPerSale[j] = tempFloat;
                    size_t temp = carAmountPerSale[i];
                    carAmountPerSale[i] = carAmountPerSale[j];
                    carAmountPerSale[j] = temp;
                    char tempStr[201];
                    strcpy(tempStr, customerNames[i]);
                    strcpy(customerNames[i], customerNames[j]);
                    strcpy(customerNames[j], tempStr);
                    char tempStr2[20];
                    strcpy(tempStr2, carModelPerSale[i]);
                    strcpy(carModelPerSale[i], carModelPerSale[j]);
                    strcpy(carModelPerSale[j], tempStr2);
                    char tempStr3[501];
                    strcpy(tempStr3, feedbackComment[i]);
                    strcpy(feedbackComment[i], feedbackComment[j]);
                    strcpy(feedbackComment[j], tempStr3);
                }

        }
    }
}//sorts the transaction list screen by total sale amount in descending order.

void menuWelcome() {
    clearConsole(); //just to remove any compiler warnings.
    fileReadData();
    // sortTransactionList();
    // sortStatistics();
    printf(R"(
  _|_|_|    _|_|    _|_|_|        _|_|_|    _|_|    _|        _|_|_|_|    _|_|_|
_|        _|    _|  _|    _|    _|        _|    _|  _|        _|        _|
_|        _|_|_|_|  _|_|_|        _|_|    _|_|_|_|  _|        _|_|_|      _|_|
_|        _|    _|  _|    _|          _|  _|    _|  _|        _|              _|
  _|_|_|  _|    _|  _|    _|    _|_|_|    _|    _|  _|_|_|_|  _|_|_|_|  _|_|_| v1.0

Hello, beloved customer! Welcome to the Car Sales program.
Please enter your personal details before making a purchase.

Press Enter to continue...)");
    getchar();
} //welcome screen, appears only once, so contains all operations that are to be run on init.


int validationInputLength(char string[], unsigned int maxLength) {
    if (strlen(string) > maxLength) {
        return FALSE;
    }
    return TRUE;
}//validate if the user's input string is not too long to avoid overflow. returns 1 on pass, 0 on fail.

void menuFeedback() {
    clearConsole();
    if (feedbackAvailable == FALSE) {
        printf(R"(*** LEAVE FEEDBACK ***

Please enter your details and make a purchase before leaving feedback.
(Main menu option [2], then [2]))");
        enterToReturn();
        return;
    }
    salesCount--; //temporarily going back by 1, because we increased it when finalising the purchase.
    printf("*** LEAVE FEEDBACK ***\n\n\n");
    size_t rating = inputUnsignedInteger("Please rate your purchased car from 0 to 5 stars:\n\n");
    while (rating != 0 && rating != 1 && rating != 2 && rating != 3 && rating != 4 && rating != 5) {
        clearConsole();
        rating = inputUnsignedInteger("*** LEAVE FEEDBACK ***\n\n\nInvalid choice, please try again.\nRate your purchased car from 0 to 5 stars:\n\n");
    }
    clearConsole();
    inputStr("*** LEAVE FEEDBACK ***\n\n\nPlease enter a short message to express your feedback:\n\n", feedbackComment[salesCount]);
    while (validationInputLength(feedbackComment[salesCount], 500) == 0){
        clearConsole();
        printf("*** LEAVE FEEDBACK ***\n\n\n");
        inputStr("I'm sorry, but your message exceeds the 500 character limit, please enter a short message to express your feedback:\n\n", feedbackComment[salesCount]);
    }
    clearConsole();
    char stars[6];
    for (int i = 0; i < rating; i++) {
        strcat(stars, "*");
    }
    strcat(stars, " :");
    //convert the number rating into a string of stars.
    printf("*** LEAVE FEEDBACK ***\n\n\nYour feedback is appreciated! <3");
    // sprintf(carModelFeedback[carChoice], "%s;%s;%s\n", customerNames[salesCount], stars, feedbackComment[salesCount]);
    fileWriteData();
    salesCount++; //bring back the sales count to the next customer.
    feedbackAvailable = FALSE;
    enterToReturn();
}//menu for providing feedback.

void menuBuy() {
    if (salesCount == 20) {
        printf(R"(*** BUY CARS ***

Sorry, but the CarSales program has reached its maximum sale limit (20), predetermined by immutable cosmic forces. ¯\_(ツ)_/¯)");
        enterToReturn();
    }
    if (strcmp("N/A", userName) == 0) {
        printf(R"(*** BUY CARS ***

Please enter your details before making a purchase.
(Main menu option [2]))");
        enterToReturn();
        return;
    }
    printf(R"(*** BUY CARS ***

We currently have these models in stock:
)");
    for (int i = 0; i < CAR_MODEL_COUNT; i++) {
        printf("[%d] %s %s, %.2f GBP (stock: %zu).\n", i + 1, carModelNames[i], carModelYearsMenu[i],
               carModelPrices[i] * discountMultiplier, carsAvailable[i]);
    }
    //b/c we are starting the menu with [1] and reserving [0] for back / exit, we print i+1.
    printf("[0] Return to main menu.");
    carChoice = inputUnsignedInteger("\nPlease enter your choice:\n");
    if (carChoice == 0) {
        return;
    }
    carChoice--;//b/c user choice will always be index+1, we reduce it here so we can point to actual index in the rest of the code.

    //checking if there are cars left.
    if (carsAvailable[carChoice] < 1) {
        printf(
            "I'm sorry, but there are no more %s units available in stock at the moment.",
            carModelNames[carChoice]);
        enterToReturn();
        return;
    }
    clearConsole();
    printf(R"(*** BUY CARS ***

How many of %s cars would you like to purchase?
)", carModelNames[carChoice]);
    carAmountPerSale[salesCount] = inputUnsignedInteger("Please enter the amount:\n");

    //checking if there are cars of a specific model left.
    if (carAmountPerSale[salesCount] > carsAvailable[carChoice]) {
        printf(
            "\nI'm sorry, but we only have %zu %s cars remaining in stock, please try again!",
            carsAvailable[carChoice], carModelNames[carChoice]);
        enterToReturn();
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

You have purchased %zu %s cars. The total cost is %.2f GBP. Thank you for your business!
)",
        carAmountPerSale[salesCount], carModelNames[carChoice], salesPerSale[salesCount]);
    //to output 2 decimal points of the float only, we can use ".2" after % and before f.
    totalSales += salesPerSale[salesCount]; //update total sales counter.
    strcpy(feedbackComment[salesCount], "N/A");
    feedbackAvailable = TRUE;
    fileWriteData();
    salesCount++; //update the sale number for the sorting algo.
    char yesNo = inputChar("Would you like to leave feedback about your experience? [y/n] ");
    while (yesNo != 'y' && yesNo != 'n' && yesNo != 'Y' && yesNo != 'N') {
        yesNo = inputChar("\nInvalid choice. please enter a single character and press enter. [y/n] ");
    }
    if (yesNo == 'y' || yesNo == 'Y') {
        menuFeedback();
    }
    sortTransactionList();
    sortStatistics();
    salesCount--;
    fileWriteData();
    salesCount++;
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
        sprintf(tempCarsAvailable[i], "x%zu", carsAvailable[i]);
    }
    for (int i = 0; i < 7; i++) {
        //sprintf(tempSalesPerCar, "%f", salesPerCar[i]);
        char tempSpace1[201] = "\0", tempSpace2[201] = "\0";
        fillSpace(carModelNames, i, tempSpace1);
        fillSpace(tempCarsAvailable, i, tempSpace2);
        printf(R"(
    - %s %s (%zu): %s %s-- %.2f.)", carModelNames[i], tempSpace1, carModelYears[i], tempCarsAvailable[i], tempSpace2, salesPerCar[i]);//TODO: fix spacing here.
        strcpy(tempSpace1, "");
        strcpy(tempSpace2, "");
        //free(tempCarsAvailable[i]);//note to self: don't free up allocated memory until every string in the array is done - turns out when you free the memory it gets filled with junk bits which actually change the string's length!
    }
    for (int i = 0; i < 7; i++) {
        free(tempCarsAvailable[i]);//free the allocated memory after we're done, so we can allocate it again.
    }
    printf(R"(

- Total cars sold:      -- %zu.
- Total car sales:      -- %.2f GBP.

Press Enter to proceed to the transaction list.
)",
           carsSold, totalSales);
    getchar();
    clearConsole();
    printf("*** TRANSACTION LIST ***\n\n");

    char *tempCustomerNames[MAX_SALES] = {NULL}, *tempCarAmountPerSale[MAX_SALES] = {NULL}, *tempCarModelPerSale[MAX_SALES] = {NULL};
    for (int i = 0; i < salesCount; i++) {
        tempCustomerNames[i] = malloc(201);
        tempCarAmountPerSale[i] = malloc(4);
        tempCarModelPerSale[i] = malloc(20); //don't forget +1 for '\0'

        /*if (tempCustomerNames[i] == NULL || tempCarAmountPerSale[i] == NULL || tempCarModelPerSale == NULL) {
            perror("mem alloc failed. :/");
            exit(1);}//for testing purposes*/

        //converting strings into strings compatible with fillSpace.
        sprintf(tempCustomerNames[i], "%s", customerNames[i]);
        sprintf(tempCarAmountPerSale[i], "%zu", carAmountPerSale[i]);
        sprintf(tempCarModelPerSale[i], "%s", carModelPerSale[i]);
    }

    for (int i = 0; i < salesCount; i++) {
        char tempSpace1[201] = "\0", tempSpace2[201] = "\0", tempSpace3[201] = "\0";
        fillSpace(tempCustomerNames, i, tempSpace1);
        fillSpace(tempCarAmountPerSale, i, tempSpace2);
        fillSpace(tempCarModelPerSale, i, tempSpace3);

        printf(R"(#%d %s:%s x%zu%s %s%s -- %.2f GBP (%.0f%% discount)
)",
               i + 1, customerNames[i], tempSpace1, carAmountPerSale[i], tempSpace2, carModelPerSale[i], tempSpace3, salesPerSale[i],
               discountPerSale[i]);
    }

    for (int i = 0; i < salesCount; i++) {
        free(tempCustomerNames[i]);
        free(tempCarAmountPerSale[i]);
        free(tempCarModelPerSale[i]);
    }
enterToReturn();
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
    userAge = inputUnsignedInteger("\nWe are currently offering a 15%% discount for seniors.\nPlease enter your age: ");
    //TODO create user validation for age input (only a whole int value should be allowed and recorded.
    clearConsole();
    //checking for purchase legal eligibility.
    if (userAge < LICENSE_AGE) {
        printf(R"(*** CUSTOMER INFORMATION ***

I'm sorry, but you have to be 16 years old or older to legally drive a car. The sale cannot be completed.)");
        userAge = 0;
        strcpy(userName, "N/A");
        enterToReturn();
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
        enterToReturn();
        return;
    }
    if (userAge >= DISCOUNT_AGE) {
        discountMultiplier = 0.85f;
        printf(R"(*** CUSTOMER INFORMATION ***

A 15%% senior discount has been applied!
User info saved, please proceed to your purchase.)");
        feedbackAvailable = FALSE;
        enterToReturn();
        return;
    }
    discountMultiplier = 1.0f;
    feedbackAvailable = FALSE;
    printf(R"(*** CUSTOMER INFORMATION ***

You are not eligible for a discount.
User info saved, please proceed to your purchase.)");
    enterToReturn();
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
