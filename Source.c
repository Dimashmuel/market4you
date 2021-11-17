#pragma warning (disable: 6031)
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MANGER_KEY "2022" // a way to differentiate between a manager and customer.
#define KEY_SIZE 5
#define MIN_PASS_CHAR 6 // min six char password.
#define LEGAL_AGE 16 // legal age to register is 16
#define TRIES_NUM 3

struct userDetails
{
	char* name;
	long idNum;
	char* password;
	char* phoneNum;
}typedef data;
struct Customer
{
	data details;
	int age;
	int point;

}typedef cust;
struct manager
{
	data details;
}typedef mang;
void firstMenu();
void mangerRegister(mang* a);
void checkManger();
void MangerOrCustomer();
void registerCust(cust* a);
int fullName(data* a);
int nationalID(data* a);
int password(data* a);
int legalAge(cust* a);
int phoneNum(data* a);


int main()
{
	firstMenu();
	return 0;
}
void firstMenu()
{
	cust a;
	int regLog;
	for (;;)
	{
		printf("Welcome to \"Market for you\"\nChoose one of options:\n1)Register\n2)Log in\n");
		scanf_s("%d", &regLog);
		switch (regLog)
		{
		case 1:
			registerCust(&a);
			break;
		case 2:
			break;
		default:
			printf("Error, please try again!\n");
			break;
		}
	}
}
void mangerRegister(mang* a)
{
	printf("Register:\n");
	while (!fullName(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
	while (!nationalID(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
	while (!password(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
	while (!phoneNum(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
}
void checkManger()
{
	mang a;
	int i;
	char key[KEY_SIZE];
	for (i = 0; i <= TRIES_NUM; i++)
	{
		printf("Please enter manger key\n");
		gets(key);
		if (key == MANGER_KEY)
		{
			mangerRegister(&a);
			return;
		}
	}
	exit(1);
}
void MangerOrCustomer()
{
	int user;
	printf("Welcome to \"Market for you\"\nChoose one of options:\n1)Manager\n2)Customer\n");
	scanf_s("%d", &user);
	switch (user)
	{
	case 1:
		checkManger();
		break;
	case 2:
		firstMenu();
		break;
	default:
		break;
	}
}
void registerCust(cust* a)
{
	printf("Register:\n");
	while (!fullName(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
	while (!nationalID(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
	while (!password(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
	while (!legalAge(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
	while (!phoneNum(&(a->details)))
	{
		printf("Error, please try again!\n");
	}
}
int fullName(data* a)
{
	int i;
	printf("Full name:\n");
	gets((*a).name);
	for (i = 0; i < strlen((*a).name); i++)
		if (!(((*a).name[i] >= 'a' && (*a).name[i] <= 'z') || ((*a).name[i] >= 'A' && (*a).name[i] <= 'Z')) && (*a).name[i] != ' ')
			return 0;
	return 1;
}
int nationalID(data* a)
{
	printf("National ID:\n");
	scanf("%id", &(*a).idNum);
	if ((*a).idNum < 100000000 || (*a).idNum > 999999999)
		return 1;
	return 0;
}
int password(data* a)
{
	int letters = 0;
	int numbers = 0;
	int i;
	printf("Password:\n");
	gets((*a).password);
	if (strlen((*a).password) < MIN_PASS_CHAR)
		return 0;
	for (i = 0; i < strlen((*a).password); i++)
	{
		if (!((*a).password[i] >= 'a' && (*a).password[i] <= 'z')) {
			if (!((*a).password[i] >= 'A' && (*a).password[i] <= 'Z')) {
				if (!((*a).password[i] >= '0' && (*a).password[i] <= '9'))
					return 0;
				else numbers++;
			}
			else letters++;
		}
		else letters++;
	}

	return letters && numbers;
}
int legalAge(cust* a)
{
	printf("Age:\n");
	scanf("%d", &(*a).age);
	if ((*a).age <= LEGAL_AGE)
		return 0;
	return 1;
}
int phoneNum(data* a)
{
	int i;
	printf("Phone number:\n");
	gets((*a).phoneNum);
	for (i = 0; i < strlen((*a).phoneNum); i++)
	{
		if (!((*a).phoneNum[i] >= '0' && (*a).phoneNum[i] <= '9'))
		{
			return 0;
		}
	}
	return 1;
}