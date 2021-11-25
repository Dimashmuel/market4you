#pragma warning(disable: 6031)
#pragma warning(disable: 6054)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// CONSTS
#define LEGAL_AGE 16
#define MANAGER_CODE 1111
#define PHONE_NUM_LEN 10
#define MIN_PASSWORD_LEN 6

// FILES
#define PATH_CUSTOMERS "data\\users\\customers.csv"
#define PATH_MANAGERS "data\\users\\managers.csv"
#define PATH_CATALOG "data\\catalog.csv"
#define PATH_BASKET "data\\basket.csv"
#define PATH_CATEGORIES "data\\categories.csv"
#define PATH_ORDERS_CONFIMRED "data\\orders\\unconfirmed\\"
#define PATH_ORDERS_UNCONFIRMED "data\\orders\\confirmed\\"

// FOLDERS
#define FOLDER_DATA "data"
#define FOLDER_DATA_USERS "data\\users"
#define FOLDER_DATA_ORDERS "data\\orders"
#define FOLDER_DATA_ORDERS_CONFIRMED "data\\orders\\confirmed"
#define FOLDER_DATA_ORDERS_UNCONFIRMED "data\\orders\\unconfirmed"

// Definitions
typedef enum { false, true } bool;
typedef enum { readId, readPassword, readName, readPhone, readSupermarketPoints, readOrders, readAge } FieldUser;
typedef enum { productName, productCompany, productPrice, productQuantity } FieldProduct;
typedef enum { none, customer, manager } UserType;
UserType user = none;

// Structs
typedef struct Details {
	char* name;
	char* id;
	char* password;
	char* phone;
} Details;
typedef struct Customer {
	Details details;
	int age;
	float points;
} Customer;
typedef struct Manager {
	Details details;
} Manager;
typedef struct Product {
	char* name;
	char* company;
	char* categroy;
	int quantity;
	float price;
} Product;
typedef struct Order {
	Product* products;
	char* customerId;
	char* orderId;
	char* totalOrder;
} Order;
typedef struct Date {
	int day, month, year;
} Date;

/// <summary>
/// FUNCTION DECLARATIONS
/// </summary>

// String functions declarations
void overrideString(char** _str1, char* _str2);
void appendString(char** _str1, char* _str2);
void appendStringComma(char** _str1, char* _str2);
void appendStringNewLine(char** _str1, char* _str2);
void inputString(char* _str);
void printStr(char* _str);
void printStrLine(char* _str);
char* newString(int _size);
char* copyString(char* _str);


// File functions declarations
void writeFile(FILE* file, char* content);
void readFile(char* filename);
void writeCustomer(Customer* c);
void writeManager(Manager* c);
void create_folder(char* dirname);
void checkFolder();
void checkFiles();
char* readUser(char* filename, FieldUser field);
bool doesFileExists(char* filename);
FILE* openFile(char* filename, char* skipFormat);


// Authentication functions declarations
void registerCustomer(Customer* c);
void registerManager(Manager* m);
bool loginUser();
bool verifyName(Details* d);
bool verifyPassword(Details* d);
bool verifyId(Details* d, bool* flag, int regType);
bool verifyAge(Customer* c);
bool verifyPhone(Details* d);
bool TermsAndConditions();
UserType findUserType(char* id);
void printProfile(char* id);

////change details in personal profile
void updateProfilUser(Customer* _customer)
{
	int temp = true;
	int option;
	//fields
	char str[100];
	char _name[100];
	char _id[100];
	char _password[100];
	char _age[100];
	char _points[100];
	char _phone[100];

	int check; //check if password or phone are correct
	char _char;
	FILE* f_temp = fopen("f_temp.csv", "w");
	if (f_temp == NULL)
	{
		printf("can't open your file\n");
		exit(1);
	}
	FILE* customers = fopen("data\\users\\customers.csv", "r");
	if (customers == NULL)
	{
		printf("can't open your file\n");
		exit(1);
	}
	fscanf(customers, "%s\n", &str);
	fprintf(f_temp, "%s\n", str);

	while (temp == true)
	{
		printf("Please choice an option:\n1. Change Password\n2. Change Phone Number\n");
		scanf_s("%d", &option);
		if (option != 1 && option != 2 && option != 3)
		{
			printf("bad choice, try again\n");
		}
		else
		{
			temp = false;
		}
	}
	while (!feof(customers)) //until the end of file
	{
		fscanf(customers, "%s\n", &str);
		sscanf(str, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", _name, _id, _password, _age, _points, _phone);

		if ((strcmp(_customer->details.id, _id) == 0))  //found the customer
		{
			if (option == 1)
			{
				fprintf(f_temp, "%s,%s,", _name, _id);
				check = verifyPassword(&_customer->details);
				while (check == false)
				{
					printf("try again\n");
					check = verifyPassword(&_customer->details);
				}
				fprintf(f_temp, "%s,%s,%s,%s\n", _customer->details.password, _age, _points, _phone);

			}//end if

			else if (option == 2) //change phone number
			{
				fprintf(f_temp, "%s,%s,%s,%s,%s,", _name, _id, _password, _age, _points);
				check = verifyPhone(&_customer->details);
				while (check == false)
				{
					printf("try again\n");
					check = verifyPhone(&_customer->details);
				}
				fprintf(f_temp, "%s\n", _customer->details.phone);
			}//end else if

			_char = fgetc(customers);
			while (!feof(customers))  //copies the rest of the file
			{
				fputc(_char, f_temp);
				_char = fgetc(customers);
			}
			fclose(customers);
			fclose(f_temp);

			customers = fopen("data\\users\\customers.csv", "w");
			f_temp = fopen("f_temp.csv", "r");
			_char = fgetc(f_temp);
			while (!feof(f_temp))
			{
				fputc(_char, customers);
				_char = fgetc(f_temp);
			}
			fclose(customers);
			fclose(f_temp);
			return;
		}//end if 

		else //if it's another customer
		{
			fprintf(f_temp, "%s,%s,%s,%s,%s,%s\n", _name, _id, _password, _age, _points, _phone);
		}

	}//end while
}


// Catalog
void addProduct();
char* readProduct(char* filename, char* productName, char* _brand, FieldProduct field);

//orders
FILE* buildShoppingCart(Product* _product); //Create a shopping cart
FILE* updateShoppingCart(Product* product, FILE* basket); //add product to shopping cart

// Welcome screen function declarations
void welcomeScreen();

/// <summary>
/// FUNCTION DEFINITIONS
/// </summary>

// String functions definitions
void overrideString(char** _str1, char* _str2) {
	char* str;
	str = malloc(sizeof(char) * strlen(_str2) + sizeof(char));
	if (str == NULL) exit(1);
	strcpy(str, _str2);
	free(*_str1);
	*_str1 = str;
}
void appendString(char** _str1, char* _str2) {
	char* str = malloc((strlen(*_str1) + strlen(_str2)) * sizeof(char) + sizeof(char));
	if (str == NULL) exit(1);
	strcpy(str, *_str1);
	strcat(str, _str2);
	free(*_str1);
	*_str1 = str;
}
void appendStringComma(char** _str1, char* _str2) {
	appendString(_str1, _str2);
	appendString(_str1, ",");
}
void appendStringNewLine(char** _str1, char* _str2) {
	appendString(_str1, _str2);
	appendString(_str1, "\n");
}
void inputString(char** _str) {
	char str[100];
	scanf("%s", str);
	*_str = malloc(strlen(str) * sizeof(char) + sizeof(char));
	strcpy(*_str, str);
}
void printStr(char* _str) {
	printf("%s", _str);
}
void printStrLine(char* _str) {
	printf("%s\n", _str);
}
char* newString(int _size) {
	char* str;
	str = malloc(sizeof(char) * _size + sizeof(char));
	return str;
}
char* copyString(char* _str) {
	char* str;
	str = malloc(sizeof(char) * strlen(_str) + sizeof(char));
	if (str == NULL) exit(1);
	strcpy(str, _str);
	return str;
}
char* mergeStrings(char* _str1, char* _str2) {
	char* str;
	str = malloc(sizeof(char) * (strlen(_str1) + strlen(_str2)) + sizeof(char));
	if (str == NULL) exit(1);
	strcpy(str, _str1);
	strcat(str, _str2);
	return str;
}

// Files functions definitions
void writeFile(char* filename, char* content) {
	FILE* file;
	file = openFile(filename, "ab+", NULL);
	if (!file) {
		printStrLine("Error.");
		return;
	}
	fprintf(file, "%s", content);
	fclose(file);
}
void writeCustomer(Customer* c) {
	char* output = copyString("");
	appendStringComma(&output, c->details.name);
	appendStringComma(&output, c->details.id);
	appendStringComma(&output, c->details.password);
	appendStringComma(&output, c->age);
	appendStringNewLine(&output, c->details.phone);
	//appendStringNewLine(&output, c->points);
	writeFile(PATH_CUSTOMERS, output);
	free(output);
}
void writeManager(Manager* m) {
	char* output = copyString("");
	appendStringComma(&output, m->details.name);
	appendStringComma(&output, m->details.id);
	appendStringComma(&output, m->details.password);
	appendStringNewLine(&output, m->details.phone);
	writeFile(PATH_MANAGERS, output);
	free(output);
}
void create_folder(char* dirname) {
	int check;
	check = mkdir(dirname);
	if (check)
	{
		printf("Unable to create folder\n");
		exit(1);
	}
}
void checkFolder() //Create the defualts folders.
{
	if (!doesFileExists(FOLDER_DATA))
		create_folder(FOLDER_DATA);
	if (!doesFileExists(FOLDER_DATA_USERS))
		create_folder(FOLDER_DATA_USERS);
	if (!doesFileExists(FOLDER_DATA_ORDERS))
		create_folder(FOLDER_DATA_ORDERS);
	if (!doesFileExists(FOLDER_DATA_ORDERS_CONFIRMED))
		create_folder(FOLDER_DATA_ORDERS_CONFIRMED);
	if (!doesFileExists(FOLDER_DATA_ORDERS_UNCONFIRMED))
		create_folder(FOLDER_DATA_ORDERS_UNCONFIRMED);
}
void checkFiles() //Create the defualts files.
{
	if (!doesFileExists(PATH_CUSTOMERS))
		writeFile(PATH_CUSTOMERS, "Username,Identity,Password,Age,Points,Phone\n");
	if (!doesFileExists(PATH_MANAGERS))
		writeFile(PATH_MANAGERS, "Username,Identity,Password,Phone");
	if (!doesFileExists(PATH_CATALOG))
		writeFile(PATH_CATALOG, "Name,Company,Category,Quantity,Price");
	if (!doesFileExists(PATH_BASKET))
		writeFile(PATH_BASKET, "Name,Company,Category,Quantity,Price");
	if (!doesFileExists(PATH_CATEGORIES))
		writeFile(PATH_CATEGORIES, "Category Name");
}
char* readUser(char* filename, char* id, FieldUser field) {
	char nameString[100], ID[20], password[100], phoneString[20];
	FILE* file;
	file = fopen(filename, "r");
	if (!file) {
		printStrLine("Error.");
		return;
	}

	char buf[500];
	while (fscanf(file, "%s", buf) == 1) {
		sscanf(buf, "%[^,],%[^,],%[^,],%[^,]", nameString, ID, password, phoneString);
		if (strcmp(ID, id) == 0) {
			if (field == readName) return nameString;
			if (field == readId) return ID;
			if (field == readPassword) return password;
			if (field == readPhone) return phoneString;
		}
	}
	return NULL;
}
bool doesFileExists(char* filename) {
	struct stat buffer;
	return !stat(filename, &buffer);
}
FILE* openFile(char* filename, char* access, char* skipFormat) { // skips the first line? not yet implemented correctly
	FILE* file;
	file = fopen(filename, access);
	if (!file) {
		printStrLine("Error.");
		return;
	}
	if (skipFormat)
		fprintf(file, skipFormat);
	return file;
}

// Authentication functions definitions
void registerCustomer(Customer* c)
{
	printf("\nRegister Stage:\n");
	while (!verifyName(&(c->details)))
	{
		printf("Invalid, Please try again!\n\n");
	}

	while (!verifyPassword(&(c->details)))
	{
		printf("Invalid, Please try again!\n\n");
	}

	bool flag = false;
	while (!verifyId(&(c->details), &flag, customer))
	{
		if (flag) {
			printf("ID already exist in the system.\n");
			int choice = 1;
			printf("\nAvailable actions:\n'1' Wrong ID try again\n'2' Try to login\n'3' Reported for Identity steal\n\nYour choice --> ");
			scanf_s("%d", &choice);
			switch (choice)
			{
			case 1:
				break;
			case 2:
				loginUser();
				return;
			case 3:
				printf("We are sry to hear, Your report will be treated\n");
				return;
			default:
				printf("Invalid input, Try again\n");
				break;
			}
		}
		printf("Invalid, Please try again!\n\n");
	}

	if (!verifyAge(c))
	{
		return false;
	}

	while (!verifyPhone(&(c->details)))
	{
		printf("Invalid, Please try again!\n\n");
	}

	if (!TermsAndConditions())
		exit(1);
	writeCustomer(c);
}
void registerManager(Manager* m)
{
	printf("\nRegister Stage:\n");
	while (!verifyName(&(m->details)))
	{
		printf("Invalid, Please try again!\n\n");
	}

	while (!verifyPassword(&(m->details)))
	{
		printf("Invalid, Please try again!\n\n");
	}

	bool flag = false;
	while (!verifyId(&(m->details), &flag, manager))
	{
		if (flag) {
			printf("ID already exist in the system.\n");
			int choice = 1;
			printf("\nAvailable actions:\n'1' Wrong ID try again\n'2' Try to login\n'3' Reported for Identity steal\n\nYour choice --> ");
			scanf_s("%d", &choice);
			switch (choice)
			{
			case 1:
				break;
			case 2:
				loginUser();
				return;
			case 3:
				printf("We are sry to hear, Your report will be treated\n");
				return;
			default:
				printf("Invalid input, Try again\n");
				break;
			}
		}
		printf("Invalid, Please try again!\n\n");
	}

	while (!verifyPhone(&(m->details)))
	{
		printf("Invalid, Please try again!\n\n");
	}

	if (!TermsAndConditions())
		return;

	writeManager(m);
}
bool loginUser() {
	char* id;
	printf("ID --> ");
	inputString(&id);
	UserType type = findUserType(id);
	if (!type) {
		printStrLine("The entered ID wasn't found...");
		return false;
	}

	char* pass;
	printf("Password --> ");
	inputString(&pass);
	if (strcmp(readUser(type == customer ? PATH_CUSTOMERS : PATH_MANAGERS, id, readPassword), pass) == 0) {
		printStr("You've successfully logged in as a ");
		printStr(type == customer ? "customer.\n" : "manager.\n");
		user = type;
		return true;
	}
	else {
		printStrLine("The entered password is incorrect...\n");
		return false;
	}
}
bool verifyName(Details* d)
{
	char string[100];
	printf("Name --> ");
	scanf(" %[^\n]s", string);

	for (int i = 0; i < strlen(string); i++)
		if (!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z') || (string[i] == ' ')))
		{
			printf("Must contain only English characters!\n");
			return false;
		}

	char* str = copyString(string);
	d->name = str;

	return true;
}
bool verifyPassword(Details* d)
{
	char* str;
	printf("Password --> ");
	inputString(&str);
	int letters = 0;
	int numbers = 0;
	if (strlen(str) < MIN_PASSWORD_LEN)
		return false;
	for (int i = 0; i < strlen(str); i++)
	{
		if (!(str[i] >= 'a' && str[i] <= 'z')) {
			if (!(str[i] >= 'A' && str[i] <= 'Z')) {
				if (!(str[i] >= '0' && str[i] <= '9'))
					return false;
				else numbers++;
			}
			else letters++;
		}
		else letters++;
	}

	if (!numbers)
		printf("Password must contain at least one number");
	if (!letters)
		printf("Password must contain at least one letter");
	if (letters && numbers) {
		d->password = str;
		return true;
	}
	return false;
}
bool verifyId(Details* d, bool* flag, int regType)
{
	char* str;
	printf("ID --> ");
	inputString(&str);

	long idVal = strtol(str, NULL, 10);
	int Digit1, Digit2, Digit3, Digit4, Digit5, Digit6, Digit7, Digit8, Digit9, Sum, Res; // Our Variables //

	if ((idVal <= 999999999) && (idVal >= 100000000)) // Checking if the user have entered a legal ID (8/9 digits) //
	{
		Digit1 = (idVal % 1000000000) / 100000000;   //  Most Left  //
		Digit2 = (idVal % 100000000) / 10000000;	 //             //
		Digit3 = (idVal % 10000000) / 1000000;       //   (Turning  //
		Digit4 = (idVal % 1000000) / 100000;         //   every     //
		Digit5 = (idVal % 100000) / 10000;           //   digit to  //
		Digit6 = (idVal % 10000) / 1000;             //   1 unit)   //
		Digit7 = (idVal % 1000) / 100;               //             //
		Digit8 = (idVal % 100) / 10;                 // Most Right  //
		Digit9 = (idVal % 10);                       // Digit check //

		Digit1 = Digit1 * 1;					//    Imply   //
		Digit2 = Digit2 * 2;					//            //
		Digit3 = Digit3 * 1;					//            //
		Digit4 = Digit4 * 2;					//     the    //
		Digit5 = Digit5 * 1;					//            //
		Digit6 = Digit6 * 2;					//            //
		Digit7 = Digit7 * 1;					//  Algoritm  //
		Digit8 = Digit8 * 2;					//            //

		Digit2 = ((Digit2 / 10) + (Digit2 % 10));	//  Getting a     //
		Digit4 = ((Digit4 / 10) + (Digit4 % 10));	//  tens number   //
		Digit6 = ((Digit6 / 10) + (Digit6 % 10));	//  into a one    //
		Digit8 = ((Digit8 / 10) + (Digit8 % 10));	//  singular unit //

		Sum = Digit1 + Digit2 + Digit3 + Digit4 + Digit5 + Digit6 + Digit7 + Digit8;  // The Sum of the ID according to the algoritm //

		Res = 10 - (Sum % 10); // Getting the right last digit according to the algoritm //

		if (Res == Digit9) {// Incase the last digit is correct //
			if (findUserType(str))  // in case the user already exists
			{
				*flag = true;
				return false;
			}

			d->id = str;
			return true; // True//
		}

		else if (Res != Digit9) // Incase the last digit is incorrect //
		{
			return false; // False //
		}
	}

	else // Incase the user did not have entered a legal ID formula //
	{
		return false; // False //
	}
}
bool verifyAge(Customer* c)
{
	int age;
	char string[20];

	printf("Age --> ");
	scanf_s("%d", &age);

	if (age < LEGAL_AGE)
	{
		printf("We are sry, The minimum age is 16\n");
		return false;
	}

	_itoa(age, string, 10);
	char* str = copyString(string);
	c->age = str;
	return true;
}
bool verifyPhone(Details* d)
{
	char* str;
	printf("Phone --> ");
	inputString(&str);
	if (str == NULL)
		exit(1);

	for (int i = 0; i < strlen(str); i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			printf("Phone number contain only Digits!\n");
			return false;
		}
	}

	if (strlen(str) != PHONE_NUM_LEN)
	{
		printf("Phone number must contain ten Digits!\n");
		return false;
	}

	d->phone = str;
	return true;
}
bool TermsAndConditions()
{
	char terms;
	printf("Do you agree to the terms and conditions (Y/N)\nInput --> ");
	getchar();
	scanf("%c", &terms);
	while (terms != 'Y' && terms != 'N')
	{
		printf("Invalid input, Try again\nInput --> ");
		getchar();
		scanf("%c", &terms);
	}
	if (terms == 'Y')
		return true;
	else if (terms == 'N')
		return false;
}
UserType findUserType(char* id) {
	char* ID = readUser(PATH_CUSTOMERS, id, readId);
	if (ID)
		return customer;
	ID = readUser(PATH_MANAGERS, id, readId);
	if (ID)
		return manager;
	return none;
}
void printProfile(char* id) {

	printf("Name: %s\n ", readUser(user == customer ? PATH_CUSTOMERS : PATH_MANAGERS, id, readName));
	printf("Phone number: %s\n ", readUser(user == customer ? PATH_CUSTOMERS : PATH_MANAGERS, id, readPhone));
	if (user == customer)
	{
		printf("\"Supermarket points\": %s\n ", readUser(PATH_CUSTOMERS, id, readSupermarketPoints));
		printf("Orders: %s\n ", readUser(PATH_CUSTOMERS, id, readOrders));
	}
}
void changeProfile(char* id) {
	Details a = { readUser(user == customer ? PATH_CUSTOMERS : PATH_MANAGERS, id, readName),
				readUser(user == customer ? PATH_CUSTOMERS : PATH_MANAGERS, id, readId) };
	verifyPassword(*a.password);
	verifyPhone(*a.phone);
}
// Catalog
void addProduct() {
	bool nameFlag = false;
	char* rProductName = NULL;
	while (!rProductName) {
		printStrLine("Please enter the name of the product ");
		inputString(&rProductName);
		if (readProduct(PATH_CATALOG, rProductName, NULL, productName))
			nameFlag = true;
		int length = strlen(rProductName);
		for (int i = 0; i < length; i++) {
			if (!rProductName) continue;
			if (!(rProductName[i] >= 'a' && rProductName[i] <= 'z' || rProductName[i] >= 'A' && rProductName[i] <= 'Z')) {
				rProductName = NULL;
				printStrLine("The entered product name is invalid...");
			}
		}
	}
	char* rProductBrand = NULL;
	while (!rProductBrand) {
		printStrLine("Please enter the brand of the product ");
		inputString(&rProductBrand);
		if (nameFlag) {
			if (strcmp(readProduct(PATH_CATALOG, rProductName, NULL, productCompany), rProductBrand) == 0) {
				printStrLine("The entered product from the same brand is already in the system, aborting...");
				return;
			}
		}
		int length = strlen(rProductBrand);
		for (int i = 0; i < length; i++) {
			if (!rProductBrand) continue;
			if (!(rProductBrand[i] >= 'a' && rProductBrand[i] <= 'z' || rProductBrand[i] >= 'A' && rProductBrand[i] <= 'Z')) {
				rProductBrand = NULL;
				printStrLine("The entered product brand is invalid...");
			}
		}
	}
	char* productPrice = NULL;
	while (!productPrice) {
		printStrLine("Please enter the price of the product ");
		inputString(&productPrice);
		bool flag = false;
		int length = strlen(productPrice);
		for (int i = 0; i < length; i++) {
			if (!productPrice) continue;
			if (productPrice[i] == '.' && (i == 0 || i == length - 1)) {
				if ((i == 0 || i == length - 1) || flag) {
					productPrice = NULL;
					printStrLine("The entered price of the product is invalid...");
					break;
				}
				else flag = true;
			}
			if (!(productPrice[i] >= '0' && productPrice[i] <= '9') && productPrice[i] != '.') {
				productPrice = NULL;
				printStrLine("The entered price of the product is invalid...");
			}
		}
	}

	char* productQuantity = NULL;
	while (!productQuantity) {
		printStrLine("Please enter the quantity of the product ");
		inputString(&productQuantity);
		int length = strlen(productQuantity);
		for (int i = 0; i < length; i++) {
			if (!productQuantity) continue;
			if (!(productQuantity[i] >= '0' && productQuantity[i] <= '9')) {
				rProductBrand = NULL;
				printStrLine("The entered quantity is invalid...");
			}
		}
	}

	char* output = copyString("");
	appendStringComma(&output, productName);
	appendStringComma(&output, rProductBrand);
	appendStringComma(&output, productPrice);
	appendStringNewLine(&output, productQuantity);
	writeFile(PATH_CATALOG, output);
	free(output);
}
char* readProduct(char* filename, char* productName, char* _brand, FieldProduct field) {
	char name[100], brand[100], quantity[100], price[20];
	FILE* file;
	file = fopen(filename, "r");
	if (!file) {
		printStrLine("Error.");
		return;
	}

	char buf[500];
	while (fscanf(file, "%s", buf) == 1) {
		sscanf(buf, "%[^,],%[^,],%[^,],%[^,]", name, brand, quantity, price);
		if (strcmp(productName, name) == 0 && brand ? strcmp(_brand, brand) == 0 : 1) {
			if (field == productName) return name;
			if (field == productCompany) return brand;
			if (field == productQuantity) return quantity;
			if (field == productPrice) return price;
		}
	}
	return NULL;
}

/*
FILE* buildShoppingCart(Product* _product)
{
	FILE* basket = fopen(PATH_BASKET, "w");
	if (basket == NULL)
	{
		printf("can't open your file\n");
		exit(1);
	}
	fprintf(basket, "%s,%s,%s\n", "name product", "quantity", "price");
	fclose(basket);
	return updateShoppingCart(_product, basket);
}
*/
//FILE* updateShoppingCart(Product* product) //add product to basket
//{
//	FILE* basket = fopen(PATH_BASKET, "w");
//	basket = fopen(PATH_BASKET, "ab+");
//	if (basket == NULL)
//	{
//		printf("can't open your file\n");
//		exit(1);
//	}
//	char* output = copyString("");
//	fputs(product->name, basket);
//	fputs(",", basket);
//	fputs(product->quantity, basket);
//	fputs(",", basket);
//	fputs(product->price, basket);
//	fputs("\n", basket);
//	fclose(basket);
//
//	return basket;
//}

// Welcome screen functions definitions
void welcomeScreen() {
	Customer c;
	Manager m;

	int regLog = 1, regType = 1, managerCode;

	printf("Welcome to \"Market for you\"\n'1' Register\n'2' Log in\n'3' Exit\nInput --> ");
	scanf_s("%d", &regLog);

	switch (regLog)
	{
	case 1:
		printf("\nSign up as\n'1' Customer\n'2' Manager\n'3' Exit\nInput --> ");
		scanf_s("%d", &regType);

		switch (regType)
		{
		case 1:
			registerCustomer(&c);
			break;
		case 2:
			printf("Input manager code --> ");
			scanf_s("%d", &managerCode);
			if (MANAGER_CODE == managerCode)
				registerManager(&m);
			else
				printf("Wrong Code\n");
			break;
		case 3:
			break;
		default:
			printf("Error input, Try again!\n");
			break;
		}
		break;
	case 2:
		loginUser();
		break;
	case 3:
		break;
	default:
		printf("Error input, Try again!\n");
		break;
	}
}
Product selectProduct() {
	Product product = { NULL, NULL, NULL, NULL };
	char name[100], brand[100], quantity[100], price[100];

	FILE* file;
	file = fopen(PATH_CATALOG, "r");
	if (!file) {
		printStrLine("Error.");
		return;
	}

	char buf[500];
	int i = 1;
	printf("\tNAME\t\tBRAND\t\tQUANTITY\t\tPRICE\n");
	while (fscanf(file, "%s", buf) == 1) {
		sscanf(buf, "%[^,],%[^,],%[^,],%[^,]", name, brand, quantity, price);
		printf("%d.\t%s\t\t%s\t\t%s\t\t\t%s\n", i, name, brand, quantity, price);
		i++;
	}
	printStr("Please select a product --> ");

	int res;
	scanf("%d", &res);

	// retrieve the selected product
	int j = 1;
	file = fopen(PATH_CATALOG, "r");
	while (fscanf(file, "%s", buf) == 1 && j <= i) {
		if (j != res) {
			sscanf(buf, "%*s,*s,%*s,%*s");
			j++;
		}
		else {
			sscanf(buf, "%[^,],%[^,],%[^,],%[^,]", name, brand, quantity, price);
			product.name = copyString(name);
			product.company = copyString(brand);
			product.quantity = atoi(copyString(quantity));
			product.price = atoi(copyString(price));
			return product;
		}
	}
	return product;
}
void printProduct(Product* product) {
	printf("Name %s\tBrand %s\tPrice %.2f\tQuantity %d", product->name, product->company, product->price, product->quantity);
}

int main() {
	checkFolder();
	checkFiles();
	welcomeScreen();

	///////filling the customer file for checking ///////////////////////////////////
	/*Customer tehila;
	tehila.details.name = "tehila";
	tehila.details.id = "322257213";
	tehila.details.password = "tttt12";
	tehila.age = 21;
	tehila.details.phone = "0556894552";
	tehila.points = 0.0;
	Customer* p = &tehila;
	FILE* ff = fopen("data\\users\\customers.csv", "a");*/
	/*///1///
	fputs("shani,", ff);
	fputs("123456789,", ff);
	fputs("ssss12,", ff);
	fprintf(ff, "%d,", 20);
	fprintf(ff, "%.2f,", 0.0);
	fputs("0556785432\n", ff);
	///2///
	fputs("hodaya,", ff);
	fputs("456746785,", ff);
	fputs("hhh555,", ff);
	fprintf(ff, "%d,", 20);
	fprintf(ff, "%.2f,", 0.0);
	fputs("0556783245\n", ff);
	///3///
	fputs("tehila,", ff);
	fputs("322257213,", ff);
	fputs("tttt11,", ff);
	fprintf(ff, "%d,", 23);
	fprintf(ff, "%.2f,", 0.0);
	fputs("0556894552\n", ff);
	///4///
	fputs("shir,", ff);
	fputs("345623498,", ff);
	fputs("rrrr213,", ff);
	fprintf(ff, "%d,", 18);
	fprintf(ff, "%.2f,", 0.0);
	fputs("0503453002\n", ff);*/

	//updateProfilUser(p);
	//////////////////////////////////////////


	return 0;
}