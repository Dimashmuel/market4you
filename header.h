// CONSTS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

// CONSTS
#define LEGAL_AGE 16
#define MANAGER_CODE 1111
#define PHONE_NUM_LEN 10
#define MIN_PASSWORD_LEN 6

// FILES
#define FILE_CUSTOMERS "data\\users\\customers.csv"
#define FILE_MANAGERS "data\\users\\managers.csv"
#define FILE_TEMP "data\\users\\temp.csv"
#define FILE_TICKET "data\\tickets\\ticket.csv"
#define FILE_CATALOG "data\\catalog.csv"
#define FILE_GLOBAL "data\\global.csv"
#define FILE_ORDERS "data\\orders.csv"
#define FILE_ORDER "data\\temp.csv"


// FOLDERS
#define FOLDER_DATA "data\\"
#define FOLDER_DATA_USERS "data\\users\\"
#define FOLDER_DATA_ORDERS "data\\orders\\"
#define FOLDER_DATA_TICKETS "data\\tickets"

typedef enum { readId, readPassword, readName, readPhone, readSupermarketPoints } FieldUser;
typedef enum { productName, productCompany, productCategory, productQuantity, productPrice } FieldProduct;
typedef enum { none, customer, manager } UserType;
typedef enum { false, true } bool;

// Global variables
char* Identity;

/// <summary>
/// STRUCTS
/// </summary>
typedef struct Details {
	char* name;
	char* ID;
	char* password;
	char* phone;
	float points;
} Details;

typedef struct Product {
	char* name;
	char* company;
	char* category;
	int quantity;
	float price;
} Product;

typedef struct Date {
	int day, month, year;
} Date;

typedef struct Order {
	Product* products;
	Date date;
	char* customerId;
	char* orderId;
	char* totalOrder;
} Order;

typedef struct Cart {
	Product* products;
	int itemsCount;
} Cart;

Product selectByCategory();
// String functions definitions 
void inputString(char** _str);
void appendString(char** _str1, char* _str2);
void appendStringComma(char** _str1, char* _str2);
void appendStringNewLine(char** _str1, char* _str2);
char* copyString(char* _str);

// Files functions definitions
void checkFiles();
void checkFolder();
void createFolder(char* dirname);
void writeFile(char* filename, char* content);
void writeUserType(Details* d, UserType type);
Details readUser(char* filename, char* id);
bool doesFileExists(char* filename);
FILE* openFile(char* filename, char* access, char* skipFormat);

// Authentication functions definitions
void registerUserType(UserType type);
bool verifyName(Details* d);
bool verifyPassword(Details* d);
bool verifyAge();
bool verifyPhone(Details* d);
bool termsAndConditions();
UserType findUserType(char* id);
void loginUser();

// Catalog & cart functions definitions
void catalogAddProduct();
void changeQuantity(Cart* cart, int index, int newQuantity);
void printProduct(Product* product);
bool doesProductExist(char* filename, char* _productName, char* _company);
void changeQuantity(Cart* cart, int index, int newQuantity);
void removeFromCart(Cart* cart, int index);
bool verifyPayment(char* creditCard, char* ccv, int expirationMonth, int expirationYear);
void finishOrder(Cart* cart);
Product selectProduct();
Date getCurrentDate();
void writeOrder(Cart* cart, char* id);
void printRevenue();
void updatePoints(char* id, float newPoints);

// Menu functions definitions
void welcomeScreen();
void customerMenu();
void managerMenu();
void addToCart(Cart* cart, Product product);

void registerStage();
void profile();
void updateProfile();
void printProfile();

void managerStoreActions();
bool checkPassword(char** _Password);
bool checkPhone(char** _Phone);
void updateCatalog(Product* p, int updateQuantity);
void deleteFromCatalog(Product* p);
// Global
/*
char* userId;
UserType user;
*/