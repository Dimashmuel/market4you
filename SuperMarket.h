#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

// Consts
#define LEGAL_AGE 16
#define MANAGER_CODE 1111
#define PHONE_NUM_LEN 10
#define MIN_PASSWORD_LEN 6

// Files (We got 2 temp files)
#define FILE_ORDER "data\\temp.csv"
#define FILE_ORDERS "data\\orders.csv"
#define FILE_GLOBAL "data\\global.csv"
#define FILE_CATALOG "data\\catalog.csv"
#define FILE_TEMP "data\\users\\temp.csv"
#define FILE_TICKET "data\\tickets\\ticket.csv"
#define FILE_MANAGERS "data\\users\\managers.csv"
#define FILE_CUSTOMERS "data\\users\\customers.csv"

// Folders
#define FOLDER_DATA "data\\"
#define FOLDER_DATA_USERS "data\\users\\"
#define FOLDER_DATA_ORDERS "data\\orders\\"
#define FOLDER_DATA_TICKETS "data\\tickets\\"

// Enums
typedef enum { readId, readPassword, readName, readPhone, readSupermarketPoints } FieldUser;
typedef enum { productName, productCompany, productCategory, productQuantity, productPrice } FieldProduct;
typedef enum { none, customer, manager } UserType;
typedef enum { false, true } bool;

// Structs
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

// Global Variables
char* Identity;

// Strings (Done)
void inputString(char** _str);
void appendString(char** _str1, char* _str2);
void appendStringComma(char** _str1, char* _str2);
void appendStringNewLine(char** _str1, char* _str2);
char* copyString(char* _str);
char* strToLower(char* _str);

// Files (Done)
void checkFiles();
void checkFolder();
void createFolder(char* dirname);
void writeFile(char* filename, char* content);
void writeUserType(Details* d, UserType type);
bool doesFileExists(char* filename);
Details* readUser(char* filename, UserType type);
UserType findUserType();

// Authentication (Done)
bool verifyName(Details* d);
bool verifyPassword(Details* d);
bool verifyId(Details* d);
bool verifyAge();
bool verifyPhone(Details* d);
bool verifyPayment(char* creditCard, char* cvv, int expirationMonth, int expirationYear);
bool termsAndConditions();

//CustomerMenu (Done)
void customerMenu();
void customerShop(Cart* cart);
void viewCart(Cart* cart);
void printCart(Cart* cart);
void openTicket();
void addToCart(Cart* cart, Product product);
void removeFromCart(Cart* cart, int index);
void changeQuantity(Cart* cart, int index, int newQuantity);
void writeOrder(Cart* cart, char* id);
void finishOrder(Cart* cart, char* id);
void updatePoints(char* id, float newPoints);
Product selectByCategory();

//ManagerMenu(Done)
void managerMenu();
void managerStoreActions();
void addToCatalog();
void deleteFromCatalog(Product* p);
void updateCatalog(Product* p, int userQuantity);
void seeTickets(); //Work (add option to delete ticket after manager mark it)
void printRevenue();
void showOrders(char* id);
void printOrder(int orderId);
void changeOrderStatus(int id);
int calcDateDiff(Date d2);
Date getCurrentDate();

//Manager + Customer (Done)
void registerUserType(UserType type);
void welcomeScreen();
void registerStage();
void loginUser();
void profile();
void printProfile();
void updateProfile();
bool doesProductExist(char* filename, char* _productName, char* _company);
Product selectProduct(Cart cart);
Product readProduct(char* filename, char* _productName, char* _company);
Cart retrieveProducts(bool returnAll, char* search, char* searchCategory);

//Place in the right subject
int getNextOrderId();
char* getNextOrderIdStr();
void appendOrderId();


// Yellow Warnings
// addToCart
// removeFromCart
// selectByCategory
// deleteFromCatalog
// updateCatalog
// printRevenue
// printOrder
// changeOrderStatus
// getCurrentDate
// updateProfile
// doesProductExist
// readProduct
// retrieveProducts
// getNextOrderId
// getNextOrderIdStr
// appendOrderId

// showOrders (Can be much shorter)