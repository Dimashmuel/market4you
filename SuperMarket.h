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
#define CREADIT_CARD 16


// Files
#define FILE_TEMP "Data\\Temp.csv"
#define FILE_ORDERS "Data\\Orders\\Order.csv"
#define FILE_TICKETS "Data\\Tickets\\Ticket.csv"
#define FILE_MANAGERS "Data\\Users\\Managers.csv"
#define FILE_CATALOGS "Data\\Catalogs\\Catalog.csv"
#define FILE_CUSTOMERS "Data\\Users\\Customers.csv"


// Folders
#define FOLDER_DATA "Data\\"
#define FOLDER_DATA_USERS "Data\\Users\\"
#define FOLDER_DATA_ORDERS "Data\\Orders\\"
#define FOLDER_DATA_TICKETS "Data\\Tickets\\"
#define FOLDER_DATA_CATALOGS "Data\\Catalogs\\"
#define FOLDER_DATA_TICKETS_TICKETSUMMARY "Data\\Tickets\\TicketSummary\\"
#define FOLDER_DATA_ORDERS_ORDERSUMMARY "Data\\Orders\\OrderSummary\\"


// Enums
typedef enum { readId, readPassword, readName, readPhone, readSupermarketPoints } FieldUser;
typedef enum { productName, productCompany, productCategory, productQuantity, productPrice } FieldProduct;
typedef enum { none, customer, manager } UserType;
typedef enum { false, true } bool;


// Structs
typedef struct Details 
{
	char* name;
	char* ID;
	char* password;
	char* phone;
	float points;
} Details;
typedef struct Product 
{
	char* name;
	char* company;
	char* category;
	int quantity;
	float price;
} Product;
typedef struct Date 
{
	int day, month, year;
} Date;
typedef struct Order 
{
	Product* products;
	Date date;
	char* customerId;
	char* orderId;
	char* totalOrder;
} Order;
typedef struct Cart 
{
	Product* products;
	int itemsCount;
} Cart;


// Global Variables
char* Identity;


// Strings
void inputString(char** _Str);
void appendString(char** _Str1, char* _Str2);
void appendStringComma(char** _Str1, char* _Str2);
void appendStringNewLine(char** _Str1, char* _Str2);
char* copyString(char* _Str);
char* strToLower(char* _Str);
float stringToFloat();
int stringToInt();


// Files
void checkFiles(); 
void checkFolder(); 
void createFolder(char* dirname); 
void writeFile(char* filename, char* content);
void writeUserType(Details* d, UserType type); 
bool doesFileExists(char* filename);
Details* readUser(char* filename, UserType type);
UserType findUserType();


// Authentication
bool verifyName(Details* d);
bool verifyPassword(Details* d);
bool verifyId(Details* d);
bool verifyAge();
bool verifyPhone(Details* d);
bool termsAndConditions();
bool verifyCreaditCard();
bool verifyCCV();
bool verifyMonth();
bool verifyYear();


//CustomerMenu
void customerMenu();
void customerShop(Cart* cart);
void viewCart(Cart* cart);
void printCart(Cart* cart);
void openTicket();
void addToCart(Cart* cart, Product product);
void removeFromCart(Cart* cart, int index);
void changeQuantity(Cart* cart, int index, int newQuantity);
void writeOrder(Cart* cart);
void finishOrder(Cart* cart);
void updatePoints(float newPoints);
Product selectByCategory();


//ManagerMenu
void managerMenu();
void managerStoreActions();
void addToCatalog();
void deleteFromCatalog(Product* p);
void updateCatalog(Product* p, int userQuantity);
void seeTickets();
void printTicket(int ticketId);
void changeTicketStatus(int id);
void printRevenue();
void showOrders();
void printOrder(int orderId);
void changeOrderStatus(int id);
int calcDateDiff(Date d2);
Date getCurrentDate();


//General
void registerUserType(UserType type);
void welcomeScreen();
void registerStage();
void loginUser();
void profile();
void printProfile();
void updateProfile();
char* getNextOrderIdStr();
char* getNextTicketIdStr();
bool doesProductExist(char* filename, char* _ProductName, char* _Company);
Product selectProduct(Cart cart);
Product readProduct(char* filename, char* _ProductName, char* _Company);
Cart retrieveProducts(bool returnAll, char* search, char* searchCategory);
