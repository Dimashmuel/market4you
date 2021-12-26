//Headers
#include "SuperMarket.h"


// Strings
void inputString(char** _Str) 
{
	char string[100] = { NULL };

	scanf_s("%s", string, 100);
	*_Str = malloc(strlen(string) * sizeof(char) + sizeof(char));
	strcpy(*_Str, string);
}
void appendString(char** _Str1, char* _Str2) 
{
	char* pString = NULL;
	
	pString = malloc((strlen(*_Str1) + strlen(_Str2)) * sizeof(char) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, *_Str1);
	strcat(pString, _Str2);
	free(*_Str1);
	*_Str1 = pString;
}
void appendStringComma(char** _Str1, char* _Str2) 
{
	appendString(_Str1, _Str2);
	appendString(_Str1, ",");
}
void appendStringNewLine(char** _Str1, char* _Str2) 
{
	appendString(_Str1, _Str2);
	appendString(_Str1, "\n");
}
char* copyString(char* _Str) 
{
	char* pString = NULL;

	pString = malloc(sizeof(char) * strlen(_Str) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, _Str);
	return pString;
}
char* strToLower(char* _Str) 
{
	char* pString = NULL;

	pString = copyString(_Str);
	if (!pString) exit(1);

	for (int i = 0; i < strlen(_Str); i++)
		pString[i] = tolower(pString[i]);

	return pString;
}
float stringToFloat()
{
	char string[500] = { NULL };

	scanf_s("%s", string, 500);

	return atof(string);
}
int stringToInt()
{
	char string[500] = { NULL };

	scanf_s("%s", string, 500);

	return atoi(string);
}


// Files
void checkFiles() 
{
	if (!doesFileExists(FILE_TEMP)) writeFile(FILE_TEMP, "");
	if (!doesFileExists(FILE_TICKETS)) writeFile(FILE_TICKETS, "");
	if (!doesFileExists(FILE_ORDERS)) writeFile(FILE_ORDERS, "");
	if (!doesFileExists(FILE_CATALOGS)) writeFile(FILE_CATALOGS, "");
	if (!doesFileExists(FILE_MANAGERS)) writeFile(FILE_MANAGERS, "");
	if (!doesFileExists(FILE_CUSTOMERS)) writeFile(FILE_CUSTOMERS, "");
}
void checkFolder() 
{
	if (!doesFileExists(FOLDER_DATA)) createFolder(FOLDER_DATA);
	if (!doesFileExists(FOLDER_DATA_USERS)) createFolder(FOLDER_DATA_USERS);
	if (!doesFileExists(FOLDER_DATA_ORDERS)) createFolder(FOLDER_DATA_ORDERS);
	if (!doesFileExists(FOLDER_DATA_TICKETS)) createFolder(FOLDER_DATA_TICKETS);
	if (!doesFileExists(FOLDER_DATA_CATALOGS)) createFolder(FOLDER_DATA_CATALOGS);
	if (!doesFileExists(FOLDER_DATA_ORDERS_ORDERSUMMARY)) createFolder (FOLDER_DATA_ORDERS_ORDERSUMMARY);
	if (!doesFileExists(FOLDER_DATA_TICKETS_TICKETSUMMARY)) createFolder(FOLDER_DATA_TICKETS_TICKETSUMMARY);
}
void createFolder(char* dirname) 
{
	int check = 0;
	check = mkdir(dirname);
	if (check) exit(1);
}
void writeFile(char* filename, char* content) 
{
	FILE* file = fopen(filename, "ab+");
	if (!file) exit(1);

	fprintf(file, "%s", content);
	fclose(file);
}
void writeUserType(Details* d, UserType type) 
{
	char output[200] = { NULL };

	if (type == customer) 
	{
		sprintf(output, "%s,%s,%s,%.2f,%s\n", d->name, d->ID, d->password, 0.0, d->phone);
		writeFile(FILE_CUSTOMERS, output);
	}

	if (type == manager) 
	{
		sprintf(output, "%s,%s,%s,%s\n", d->name, d->ID, d->password, d->phone);
		writeFile(FILE_MANAGERS, output);
	}
}
bool doesFileExists(char* filename) 
{
	struct stat buffer;
	return !stat(filename, &buffer);
}
Details* readUser(char* filename, UserType type) 
{
	Details details = { NULL, NULL, NULL, NULL, 0 };
	char nameString[100] = { NULL }, IdString[100] = { NULL }, passwordString[100] = { NULL }, buffer[500] = { NULL }, phoneString[100] = { NULL };
	float supermarketPoints = 0.0;

	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	while (fscanf(file, "%s", buffer) == 1)
	{
		if (type == customer) sscanf(buffer, "%[^,],%[^,],%[^,],%f,%[^,]", nameString, IdString, passwordString, &supermarketPoints, phoneString);
		if (type == manager)  sscanf(buffer, "%[^,],%[^,],%[^,],%[^,]", nameString, IdString, passwordString, phoneString);

		if (strcmp(IdString, Identity) == 0) 
		{
			Details returnDetails = { copyString(nameString), copyString(IdString), copyString(passwordString), copyString(phoneString), supermarketPoints };
			fclose(file);
			return &returnDetails;
		}
	}

	fclose(file);
	return &details;
}
UserType findUserType() 
{
	Details* details = readUser(FILE_CUSTOMERS, customer);
	if (details->ID) return customer;

	details = readUser(FILE_MANAGERS, manager);
	if (details->ID) return manager;

	return none;
}


// Authentication
bool verifyName(Details* d) 
{
	char string[100] = { NULL };

	printf("Name --> ");
	scanf_s(" %[^\n]", string, 100);

	for (int i = 0; i < strlen(string); i++) 
	{
		if (!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z'))) 
		{
			printf("Name Can Contain Only English Alphabet\n\n");
			d->name = NULL;
			return false;
		}
	}

	char* pString = NULL;
	pString = copyString(string);

	d->name = pString;

	return true;
}
bool verifyPassword(Details* d) 
{
	char* pString = NULL;

	printf("Password --> ");
	inputString(&pString);

	if (strlen(pString) < MIN_PASSWORD_LEN) 
	{
		printf("Password Must Contain At Least Six Characters\n\n");
		return false;
	}

	int letters = 0, numbers = 0;

	for (int i = 0; i < strlen(pString); i++)
	{
		if (pString[i] >= 'a' && pString[i] <= 'z')
			letters++;

		else if (pString[i] >= 'A' && pString[i] <= 'Z')
			letters++;

		else if (pString[i] >= '0' && pString[i] <= '9')
			numbers++;

		else
		{
			printf("Password Contain Only English Alphabet\n\n");
			return false;
		}
	}

	if (!numbers)
		printf("Password Must Contain At Least One Number\n\n");

	if (!letters)
		printf("Password Must Contain At Least One Letter\n\n");

	if (letters && numbers) 
	{
		d->password = pString;
		return true;
	}

	return false;
}
bool verifyId(Details* d) 
{
	char* pString = NULL;

	printf("ID --> ");
	inputString(&pString);
	
	Identity = pString;
	
	long idVal = strtol(pString, NULL, 10);
	int Digit1 = 0, Digit2 = 0, Digit3 = 0, Digit4 = 0, Digit5 = 0, Digit6 = 0, Digit7 = 0, Digit8 = 0, Digit9 = 0, Sum = 0, Res = 0;

	if ((idVal <= 999999999) && (idVal >= 100000000)) 
	{
		Digit1 = (idVal % 1000000000) / 100000000;
		Digit2 = (idVal % 100000000) / 10000000;
		Digit3 = (idVal % 10000000) / 1000000;
		Digit4 = (idVal % 1000000) / 100000;
		Digit5 = (idVal % 100000) / 10000;
		Digit6 = (idVal % 10000) / 1000;
		Digit7 = (idVal % 1000) / 100;
		Digit8 = (idVal % 100) / 10;
		Digit9 = (idVal % 10);

		Digit1 = Digit1 * 1;
		Digit2 = Digit2 * 2;
		Digit3 = Digit3 * 1;
		Digit4 = Digit4 * 2;
		Digit5 = Digit5 * 1;
		Digit6 = Digit6 * 2;
		Digit7 = Digit7 * 1;
		Digit8 = Digit8 * 2;

		Digit2 = ((Digit2 / 10) + (Digit2 % 10));
		Digit4 = ((Digit4 / 10) + (Digit4 % 10));
		Digit6 = ((Digit6 / 10) + (Digit6 % 10));
		Digit8 = ((Digit8 / 10) + (Digit8 % 10));

		Sum = Digit1 + Digit2 + Digit3 + Digit4 + Digit5 + Digit6 + Digit7 + Digit8;

		Res = 10 - (Sum % 10);

		if (Res == Digit9) 
		{
			if (findUserType(pString)) 
			{
				printf("ID Already Exist In The System\n");

				int selection = 0;
				bool loop = true;

				while (loop)
				{
					printf("\nAvailable Actions\n'1' Wrong ID     '2' Report For Identity Steal\nInput --> ");
					selection = stringToInt();

					switch (selection) 
					{
					case 1:
						printf("You've Chose To Re Enter Other ID\n\n");
						loop = false;
						break;

					case 2:
						openTicket();
						printf("We Are Sry To Hear, Your Report Will Be Treated\nGoodbye\n");
						exit(1);

					default:
						printf("Invalid Input, Try Between [1 To 2]\n\n");
						break;
					}
				}

				return false;
			}
			d->ID = pString;
			return true;
		}

		else if (Res != Digit9)
		{
			printf("Incorrect ID\n\n");
			return false;
		}
	}

	else
	{
		printf("ID Must Contain Nine Didits\n\n");
		return false;
	}
}
bool verifyAge() 
{
	int age = 0;

	printf("Age --> ");
	age = stringToInt();

	if (age < LEGAL_AGE) 
	{
		printf("We Are Sry, The Minimum Age Is Sixteen\n\n");
		return false;
	}

	if (age > 120)
	{
		printf("Sry If You Are Trully  %d Years Old You Probably Death. Good Day\n\n", age);
		return false;
	}

	return true;
}
bool verifyPhone(Details* d) 
{
	char* pString = NULL;

	printf("Phone --> ");
	inputString(&pString);

	if (!pString) exit(1);

	if (strlen(pString) != PHONE_NUM_LEN) 
	{
		printf("Phone Number Must Contain Ten Digits\n\n");
		return false;
	}

	for (int i = 0; i < strlen(pString); i++) 
	{
		if (!(pString[i] >= '0' && pString[i] <= '9')) 
		{
			printf("Phone Number Contain Only Digits\n\n");
			return false;
		}
	}

	d->phone = pString;
	return true;
}
bool termsAndConditions() 
{
	printf("Terms and Conditions\n\n'1' The Site, Including Any Content And/Or Service Available Through It, Is Provided To You 'As It Is'.\nAlthough The Company Takes All Efforts To Present The Site Or Through It As Accurate And Reliable Information As Possibl\nThe Company Is Not And Will Not Be Responsible, Directly Or Indirectly, For The Availability, Veracity, Reliability\nAnd/Or Accuracy Of The Content Appearing On The Site, And Reliance On Any Content Displayed On Or Through The Site Is\nYour Full Responsibility.\n\n'2' You May Use The Site And The Content Available Through It For Private And Personal Purposes Only.\nHe Content Of The Site Does Not Grant You Any Rights Other Than Those Set Forth In These Terms\nWhich Constitutes An Agreement For All Intents And Purposes Between You And The Company.\n\n'3' The Content Of The Website May Not Be Used As A Basis For The Purpose Of Making Financial, Legal, Personal\nAnd / Or Other Decisions On Your Part. Any Action Taken By You Based On This Content Is Done At Your Sole Discretion\nAnd At Your Own Responsibility.\nProducts, Descriptions, Design, Colors Or The Appearance Of Products And Services Described Or Displayed On The Site\nAre For Illustration Purposes Only, In Order To Simulate The User's Desired Product In The Closest And Best Way.\nIf You Have Identified A Significant Gap Between The Visibility Of The Actual Requested Product And The Product\nAppearing On The Site, Please Contact Us To Correct It, At The Company's Sole Discretion.\nIn any case, the Company Shall Not Be Liable In Connection With Any Product And / or Recipe And / Or Recommendations\nDetailed Or Displayed On The Website And Through It.\n\n");
																																																																																																																																																																																																																																																																																																																																																																																																					
	int selection = 0;			
	while (!(selection >= 1 && selection <= 2))
	{
		printf("Do You Agree To The Terms And Conditions\n'1' Yes     '2' No\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 2))
			printf("Invalid Input, Try Between [1 To 2]\n\n");
	}
	
	printf("\n");
	if (selection == 1) return true;

	else if (selection == 2)
	{
		printf("We Are Sry To Hear, Have A Good Day\n\n");
		return false;
	}
}
bool verifyCreaditCard()
{
	char* pCreaditCard = NULL;

	printf("\nCreadit Card (Without Spaces)\nInput --> ");
	inputString(&pCreaditCard);

	if (strlen(pCreaditCard) != CREADIT_CARD)
	{
		printf("Creadit Card Must Contain Sixteen Digits\n\n");
		return false;
	}

	int numbers = 0;

	for (int i = 0; i < strlen(pCreaditCard); i++)
	{
		if (!(pCreaditCard[i] >= '0' && pCreaditCard[i] <= '9'))
		{
			printf("Creadit Card Contain Only Digits\n\n");
			return false;
		}
	}

	return true;
}
bool verifyCCV()
{
	int CCV = 0;
	while (!(CCV >= 100 && CCV <= 999))
	{
		printf("\nCCV\nInput --> ");
		CCV = stringToInt();

		if (!(CCV >= 100 && CCV <= 999))
			printf("Invalid Input, Try Between [100 To 999]\n\n");
	}
	
	return true;
}
bool verifyMonth()
{
	int Month = 0;
	while (!(Month >= 1 && Month <= 12))
	{
		printf("\nExpiration Month\nInput --> ");
		Month = stringToInt();

		if (!(Month >= 1 && Month <= 12))
			printf("Invalid Input, Try Between [1 To 12]\n\n");
	}
	
	return true;
}
bool verifyYear()
{
	int Year = 0;
	while (!(Year >= 2022 && Year <= 2035))
	{
		printf("\nExpiration Year\nInput --> ");
		Year = stringToInt();

		if (!(Year >= 2022 && Year <= 2035))
			printf("Invalid Input, Try Between [2022 To 2035]\n\n");
	}
	
	return true;
}


//CustomerMenu
void customerMenu() 
{
	Cart c = { NULL , 0 };
	int selection = 0;
	bool loop = true;

	while (loop) 
	{
		printf("\nCustomer Actions\n'1' Store     '2' Profile     '3' View Cart     '4' Finish Order     '5' Submit Ticket     '6' Log Out\nInput --> ");
		selection = stringToInt();

		switch (selection) 
		{
		case 1:
			customerShop(&c);
			break;

		case 2:
			profile();
			break;

		case 3:
			viewCart(&c);
			break;

		case 4:
			finishOrder(&c, Identity);
			break;

		case 5:
			openTicket();
			break;

		case 6:
			printf("You've Successfully Logged Out\n\n");
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 6]\n\n");
			break;
		}
	}
}
void customerShop(Cart* cart) 
{
	int selection = 0;
	bool loop = true;
	Product product = { NULL, NULL, NULL, 0, 0.0 };
	char* search = NULL;
	char* category = NULL;

	while (loop)
	{
		printf("\nSearch Product By\n'1' Name/Company     '2' Category     '3' Show All Products\nInput --> ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			printf("\nName/Company\nInput --> ");
			inputString(&search);
			product = selectProduct(retrieveProducts(false, search, NULL));
			if (product.quantity == 0)
				return;

			loop = false;
			break;

		case 2:
			product = selectByCategory();
			loop = false;
			break;

		case 3:
			product = selectProduct(retrieveProducts(false, NULL, NULL));
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 3]\n\n");
			break;
		}
	}

	int selectedQuantity = 0;
	while (!(selectedQuantity >= 1 && selectedQuantity <= product.quantity)) 
	{
		printf("\nProduct Quantity\nInput --> ");
		selectedQuantity = stringToInt();

		if (!(selectedQuantity >= 1 && selectedQuantity <= product.quantity)) 
			printf("Invalid Input, Try Between [1 To %d]\n\n", product.quantity);
	}
	
	Product addedProduct = product;
	addedProduct.quantity = selectedQuantity;
	addToCart(cart, addedProduct);
	updateCatalog(&product, product.quantity - selectedQuantity);
}
void viewCart(Cart* cart) 
{
	int selection = 0, optionA = 0, optionB = -1;
	bool loop = true;
	
	while (loop)
	{
		printCart(cart);
		printf("\nCart Actions\n'1' Edit Cart     '2' Back To Menu\nInput --> ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			optionA = 0;
			while (!(optionA >= 1 && optionA <= cart->itemsCount))
			{
				printf("\nSelect Product\nInput --> ");
				optionA = stringToInt();

				if (!(optionA >= 1 && optionA <= cart->itemsCount))
					printf("Invalid Input, Try Between [1 To %d]\n\n", cart->itemsCount);
			}
			
			optionB = 0;
			while (!(optionB >= 1 && optionB <= 3)) 
			{
				printf("\nProduct Actions\n'1' Change Quantity     '2' Remove Product     '3' Return\nInput --> ");
				optionB = stringToInt();

				if (!(optionB >= 1 && optionB <= 3)) 
					printf("Invalid Input, Try Between [1 To 3]\n\n");
			}

			if (optionB == 1) 
			{
				optionB = 0;
				while (optionB <= 0) 
				{
					printf("\nNew Quantity\nInput --> ");
					optionB = stringToInt();

					if (optionB <= 0)
						printf("Invalid Input, Try Between [1 Or Greater]\n\n");
				}
				changeQuantity(cart, optionA - 1, optionB);
			}

			else if (optionB == 2) 
				removeFromCart(cart, optionA - 1);

			loop = false;
			break;
		
		case 2:
			loop = false;
			return;

		default:
			printf("Invalid Input, Try Between [1 To 2]\n\n");
			break;
		}
	}
}
void printCart(Cart* cart) 
{
	printf("\n%-15s%-15s%-15s%-15s%-15s%-15s\n", "No.", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");

	float total = 0;

	for (int i = 0; i < cart->itemsCount; i++) 
	{
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart->products[i].name, cart->products[i].company, cart->products[i].category, cart->products[i].price, cart->products[i].quantity);
		total += cart->products[i].price * cart->products[i].quantity;
	}
	printf("Total --> %.2f\n\n", total);
}
void openTicket() 
{
	char source[300] = { NULL }, ticketNum[100] = { NULL }, _Report[500] = { NULL };
	strcpy(ticketNum, getNextTicketIdStr());

	sprintf(source, "%s%s.csv", FOLDER_DATA_TICKETS_TICKETSUMMARY, ticketNum);

	FILE* file = fopen(source, "w");
	if (!file) exit(1);

	Date date = getCurrentDate();
	fprintf(file, "%s,%d/%d/%d\n", ticketNum, date.day, date.month, date.year);

	printf("Input Report --> ");
	scanf_s(" %[^\n]", _Report, 500);
		
	fprintf(file, "%s\n", _Report);
	fclose(file);

	file = fopen(FILE_TICKETS, "ab+");
	if (!file) exit(1);

	fprintf(file, "%s,%s,%d/%d/%d,WAITING\n", ticketNum, Identity, date.day, date.month, date.year);
	fclose(file);

	printf("Ticket Been Sent, Have A Good Day\n\n");
}
void addToCart(Cart* cart, Product product) 
{	
	if (cart->itemsCount == 0) 
	{
		cart->products = malloc(sizeof(Product));
		cart->products[0] = product;
	}

	else 
	{
		Product* newProducts = malloc(sizeof(Product) * (cart->itemsCount + 1));
		for (int i = 0; i < cart->itemsCount; i++) 
			newProducts[i] = cart->products[i];
		
		newProducts[cart->itemsCount] = product;
		cart->products = newProducts;
	}
	cart->itemsCount++;
}
void removeFromCart(Cart* cart, int index) 
{
	Product* newProduct = malloc(sizeof(Product) * (cart->itemsCount - 1));
	Product p = readProduct(FILE_CATALOGS, cart->products[index].name, cart->products[index].company);

	updateCatalog(&p, p.quantity + cart->products[index].quantity);
	
	for (int i = 0; i < index; i++) 
	{
		Product tempProduct = {copyString(cart->products[i].name), copyString(cart->products[i].company), copyString(cart->products[i].category), cart->products[i].price, cart->products[i].quantity};
		newProduct[i] = tempProduct;
	}

	for (int i = index + 1; i < index; i++) 
	{
		Product tempProduct = {copyString(cart->products[i].name), copyString(cart->products[i].company), copyString(cart->products[i].category), cart->products[i].price, cart->products[i].quantity};
		newProduct[i - 1] = tempProduct;
	}
	cart->itemsCount--;
}
void changeQuantity(Cart* cart, int index, int newQuantity) 
{
	bool flag = false;
	Product p = readProduct(FILE_CATALOGS, cart->products[index].name, cart->products[index].company);
	int availableQuantity = p.quantity;

	if (newQuantity > cart->products[index].quantity) 
	{
		if (newQuantity - cart->products[index].quantity > availableQuantity)
			printf("Available Stock --> %d\n", availableQuantity);

		else 
		{
			updateCatalog(&p, availableQuantity - (newQuantity - cart->products[index].quantity));
			cart->products[index].quantity = newQuantity;
		}
	}

	else 
	{
		updateCatalog(&p, availableQuantity + (cart->products[index].quantity - newQuantity));
		cart->products[index].quantity = newQuantity;
	}
}
void writeOrder(Cart* cart) 
{
	char source[300] = { NULL }, orderNum[100] = { NULL };
	strcpy(orderNum, getNextOrderIdStr());

	sprintf(source, "%s%s.csv", FOLDER_DATA_ORDERS_ORDERSUMMARY, orderNum);
	
	FILE* file = fopen(source, "w");
	if (!file) exit(1);

	Date date = getCurrentDate();
	fprintf(file, "%s,%d,%d/%d/%d\n", orderNum, cart->itemsCount, date.day, date.month, date.year);
	
	float total = 0;
	
	for (int i = 0; i < cart->itemsCount; i++) 
	{
		fprintf(file, "%s,%s,%s,%f,%d\n", cart->products[i].name, cart->products[i].company, cart->products[i].category, cart->products[i].price, cart->products[i].quantity);
		total += cart->products[i].price * cart->products[i].quantity;
	}

	fprintf(file, "%.2f\n", total);
	fclose(file);
	
	file = fopen(FILE_ORDERS, "ab+");
	if (!file) exit(1);

	fprintf(file, "%s,%s,%.2f,%d/%d/%d,WAITING\n", orderNum, Identity, total, date.day, date.month, date.year);
	fclose(file);
	
	Details* user = readUser(FILE_CUSTOMERS, customer);
	
	int selection = 0;
	if (user->points > 0)
	{
		while (!(selection > 0 && selection <= 2))
		{
			printf("\nAvailable Market Points --> %.2f\nWould You Like To Reedem Them?\n'1' Yes     '2' No\nInput --> ", user->points);
			selection = stringToInt();

			if (!(selection > 0 && selection <= 2))
				printf("Invalid Input, Try Again\n\n");

			if (selection == 1)
				updatePoints(user->points > total ? user->points - total : 0);
		}
	}
	printf("In This Purchase You've Earned %.2f Market Points\n", total * 0.03);
	updatePoints(user->points + total * 0.03);
	printf("Your Purchase Was Successful\n\n");
}
void finishOrder(Cart* cart) 
{
	printCart(cart);
	
	int selection = 0;
	
	while (!(selection >= 1 && selection <= 2))
	{
		printf("Are You Sure You Would Like To Finish The Order?\n'1' Yes     '2' No\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 2))
			printf("Invalid Input, Try Between [1 To 2]\n\n");

	}
	
	if (selection == 2)
		return;
	
	printf("\nPayment Process");

	while (!(verifyCreaditCard()))
		continue;

	while (!(verifyCCV()))
		continue;

	while (!verifyMonth())
		continue;

	while (!(verifyYear()))
		continue;
	
	writeOrder(cart);
	cart->itemsCount = 0;
	free(cart->products);
}
void updatePoints(float newPoints) 
{
	FILE* file = fopen(FILE_CUSTOMERS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL }, temp[500] = { NULL }, name[30] = { NULL }, customerId[40] = { NULL }, pass[30] = { NULL }, phone[30] = { NULL }, * output = copyString("");
	float points = -1;

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f,%[^,]", name, customerId, pass, &points, phone);
		sprintf(temp, "%s,%s,%s,%f,%s\n", name, customerId, pass, strcmp(customerId, Identity) == 0 ? newPoints : points, phone);
		appendString(&output, temp);
	}
	
	fclose(file);
	file = fopen(FILE_CUSTOMERS, "w");
	if (!file) exit(1);

	fputs(output, file);
	fclose(file);
	free(output);
	printf("\nCurrent Market Points %.2f\n", newPoints);
}
Product selectByCategory() 
{
	printf("\n");

	Cart tempCart = retrieveProducts(false, NULL, NULL);
	int count = 0;
	char** categories = malloc(sizeof(char*) * tempCart.itemsCount);

	for (int i = 0; i < tempCart.itemsCount; i++) 
	{
		bool flag = false;
		for (int j = 0; j < count; j++) 
		{
			if (strcmp(tempCart.products[i].category, categories[j]) == 0) 
			{
				flag = true;
				break;
			}
		}

		if (!flag) 
		{
			categories[count] = malloc(sizeof(char) * strlen(tempCart.products[i].category) + sizeof(char));
			strcpy(categories[count], tempCart.products[i].category);
			printf("%d. %s\n", count + 1, categories[count]);
			count++;
		}
	}

	int selection = 0;
	while (!(selection >= 1 && selection <= count)) 
	{
		printf("Input No. --> ");
		selection = stringToInt();
		
		if (!(selection >= 1 && selection <= count))
			printf("Invalid Input, Try Between [1 To %d]\n\n", count);
	}

	tempCart = retrieveProducts(false, NULL, categories[selection - 1]);
	return selectProduct(tempCart);
}


//ManagerMenu
void managerMenu() 
{
	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("\nManager Actions\n'1' Store     '2' Profile     '3' View Orders     '4' Store Profits     '5' View Tickets     '6' Log Out\nInput --> ");
		selection = stringToInt();

		switch (selection) 
		{
		case 1:
			managerStoreActions();
			break;

		case 2:
			profile();
			break;

		case 3:
			showOrders();
			break;

		case 4:
			printRevenue();
			break;

		case 5:
			seeTickets();
			break;

		case 6:
			printf("You've Successfully Logged Out\n\n");
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 6]\n\n");
			break;
		}
	}
}
void managerStoreActions() 
{
	int selection = 0;
	bool loop = true;
	Product p = { NULL, NULL, NULL, 0, 0.0 };
	
	while (loop)
	{
		printf("\nStore Actions\n'1' Add Product     '2' Delete Product     '3' Update Product     '4' Return\nInput --> ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			addToCatalog();
			break;

		case 2:
			p = selectProduct(retrieveProducts(true, NULL, NULL));
			deleteFromCatalog(&p);
			break;

		case 3:
			p = selectProduct(retrieveProducts(true, NULL, NULL));
			updateCatalog(&p, -1);
			break;

		case 4:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 4]\n\n");
			break;
		}
	}
}
void addToCatalog() 
{
	printf("\nAdding Product\n");

	char* rProductName = NULL;
	while (!rProductName) 
	{
		printf("Name --> ");
		inputString(&rProductName);

		for (int i = 0; i < strlen(rProductName); i++)
		{
			if (!(rProductName[i] >= 'a' && rProductName[i] <= 'z' || rProductName[i] >= 'A' && rProductName[i] <= 'Z'))
			{
				rProductName = NULL;
				printf("Name Contain Only English Alphabet\n\n");
				break;
			}
		}
	}

	char* rProductCompany = NULL;
	while (!rProductCompany) 
	{
		printf("Company --> ");
		inputString(&rProductCompany);

		if (doesProductExist(FILE_CATALOGS, rProductName, rProductCompany)) 
		{
			printf("Company Product Already In Stock\n");
			return;
		}

		for (int i = 0; i < strlen(rProductCompany); i++) 
		{
			if (!(rProductCompany[i] >= 'a' && rProductCompany[i] <= 'z' || rProductCompany[i] >= 'A' && rProductCompany[i] <= 'Z')) 
			{
				rProductCompany = NULL;
				printf("Company Contain Only English Alphabet\n\n");
				break;
			}
		}
	}
	
	char* rProductCategory = NULL;
	while (!rProductCategory) 
	{
		printf("Category --> ");
		inputString(&rProductCategory);

		for (int i = 0; i < strlen(rProductCategory); i++) 
		{
			if (!(rProductCategory[i] >= 'a' && rProductCategory[i] <= 'z' || rProductCategory[i] >= 'A' && rProductCategory[i] <= 'Z')) 
			{
				rProductCategory = NULL;
				printf("Category Contain Only English Alphabet\n\n");
				break;
			}
		}
	}

	float rProductPrice = 0;
	while (rProductPrice <= 0) 
	{
		printf("Price --> ");
		rProductPrice = stringToFloat();

		if (rProductPrice <= 0)
			printf("Invalid Input, Try Between [1 Or Greater]\n\n");
	}

	int rProductQuantity = 0;
	while (rProductQuantity <= 0)
	{
		printf("Quantity --> ");
		rProductQuantity = stringToInt();

		if (rProductQuantity <= 0)
			printf("Invalid Input, Try Between [1 Or Greater]\n\n");
	}

	char output[200] = { NULL };
	sprintf(output, "%s,%s,%s,%.2f,%d\n", rProductName, rProductCompany, rProductCategory, rProductPrice, rProductQuantity);
	writeFile(FILE_CATALOGS, output);
}
void deleteFromCatalog(Product* p) 
{
	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* CataLog = fopen(FILE_CATALOGS, "r");
	if (!CataLog) exit(1);

	char _Name[100] = { NULL }, _Company[100] = { NULL }, _Category[100] = { NULL }, _Price[100] = { NULL }, _Quantity[100] = { NULL }, buffer[500] = { NULL };
	while (fscanf(CataLog, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);

		if (!(strcmp(p->name, _Name) == 0 && strcmp(p->company, _Company) == 0))
			fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);

	CataLog = fopen(FILE_CATALOGS, "w");
	if (!CataLog) exit(1);

	Temp = fopen(FILE_TEMP, "r");
	if (!Temp) exit(1);

	while (fscanf(Temp, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);
		fprintf(CataLog, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);
}
void updateCatalog(Product* p, int userQuantity) 
{
	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* CataLog = fopen(FILE_CATALOGS, "r");
	if (!CataLog) exit(1);

	char _Name[100] = { NULL }, _Company[100] = { NULL }, _Category[100] = { NULL }, _Price[100] = { NULL };
	char buffer[500] = { NULL }, newPrice[100] = { NULL }, newQuantity[100] = { NULL }, _Quantity[100] = { NULL };
	int selection = 0;

	if (userQuantity == -1) 
	{
		while (!(selection >= 1 && selection <= 2)) 
		{
			printf("\nProduct Actions\n");
			printf("'1' Update Price     '2' Update Quantity\nInput --> ");
			selection = stringToInt();

			if (!(selection >= 1 && selection <= 2))
				printf("Invalid Input, Try Between [1 To 2]\n\n");
		}
	}

	else
		selection = 2;
	
	int updatedQuantity = -1;
	float updatedPrice = -1;

	while (fscanf(CataLog, "%s", buffer) == 1)
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);

		if (strcmp(p->name, _Name) == 0 && strcmp(p->company, _Company) == 0) 
		{
			if (selection == 1) 
			{
				while (updatedPrice <= 0) 
				{
					printf("\nUpdate Price\nInput --> ");
					updatedPrice = stringToFloat();

					if (updatedPrice <= 0)
						printf("Invalid Input, Try Between [1 Or Greater]\n\n");
				}

				sprintf(newPrice, "%.2f", updatedPrice);
				fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, newPrice, _Quantity);
			}

			if (selection == 2) 
			{
				if (userQuantity == -1) 
				{
					while (updatedQuantity <= 0) 
					{
						printf("\nUpdate Quantity\nInput --> ");
						updatedQuantity = stringToInt();

						if (updatedQuantity <= 0)
							printf("Invalid Input, Try Between [1 Or Greater]\n\n");
					}
				}

				sprintf(newQuantity, "%d", userQuantity == -1 ? updatedQuantity : userQuantity);
				fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, newQuantity);
			}
		}

		else
			fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);

	CataLog = fopen(FILE_CATALOGS, "w");
	if (!CataLog) exit(1);

	Temp = fopen(FILE_TEMP, "r");
	if (!Temp) exit(1);

	while (fscanf(Temp, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);
		fprintf(CataLog, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);
}
void seeTickets() 
{
	int selection = 0;
	while (!(selection >= 1 && selection <= 3))
	{
		printf("\nTicket Actions\n'1' Print All Tickets     '2' Confirm/Unconfirmed Tickets     '3' Return\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 3))
			printf("Invalid Input, Try Between [1 To 3]\n\n");
	}

	FILE* file = fopen(FILE_TICKETS, "r");
	if (!file) exit(1);

	int count = 0, waitingCount = 0;
	char buffer[500] = { NULL }, status[30] = { NULL };

	while (fscanf(file, "%s", buffer) == 1)
	{
		sscanf(buffer, "%*[^,],%*[^,],%*[^,],%[^,]", status);

		if (strcmp(status, "WAITING") == 0)
			waitingCount++;
		count++;
	}

	fclose(file);

	int* tickets = NULL, * waitingTickets = NULL, ticketId = 0;
	char customerId[30] = { NULL }, date[40] = { NULL };

	if (selection == 1)
	{
		file = fopen(FILE_TICKETS, "r");
		if (!file) exit(1);

		tickets = malloc(sizeof(int) * count);

		printf("\n%-15s%-15s%-15s%-15s\n", "Ticket No.", "Customer ID", "Date", "Status");

		int i = 0;
		while (fscanf(file, "%s", buffer) == 1)
		{
			sscanf(buffer, "%d,%[^,],%[^,],%[^,]", &ticketId, customerId, date, status);
			printf("%-15d%-15s%-15s%-15s\n", ticketId, customerId, date, status);
			tickets[i] = ticketId;
			i++;
		}

		bool flag = false;
		while (!flag)
		{
			printf("Select Ticket (Ticket No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < count; i++)
			{
				if (tickets[i] == selection)
				{
					printTicket(selection);
					free(tickets);
					fclose(file);
					return;
				}
			}
			printf("Invalid Input, Try Between [0 To %d]\n\n", i - 1);
		}
	}

	else if (selection == 2)
	{
		file = fopen(FILE_TICKETS, "r");
		if (!file) exit(1);

		waitingTickets = malloc(sizeof(int) * waitingCount);

		printf("\n%-15s%-15s%-15s%-15s\n", "Ticket No.", "Customer ID", "Date", "Status");

		int i = 0;

		while (fscanf(file, "%s", buffer) == 1)
		{
			sscanf(buffer, "%d,%[^,],%[^,],%[^,]", &ticketId, customerId, date, status);

			if (strcmp(status, "WAITING") == 0)
			{
				printf("%-15d%-15s%-15s%-15s\n", ticketId, customerId, date, status);
				waitingTickets[i] = ticketId;
				i++;
			}
		}
		bool flag = false;

		while (!flag)
		{
			printf("Select Ticket (Ticket No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < waitingCount; i++)
			{
				if (waitingTickets[i] == selection)
				{
					printTicket(selection);
					changeTicketStatus(selection);
					free(waitingTickets);
					fclose(file);
					return;
				}
			}
			printf("Invalid Input, Try Between [The Ticket No. Column]\n\n");
		}
	}

	else if (selection == 3)
		return;
}
void printTicket(int ticketId)
{
	char source[300] = { NULL };
	sprintf(source, "%s%d.csv", FOLDER_DATA_TICKETS_TICKETSUMMARY, ticketId);

	FILE* file = fopen(source, "r");
	if (!file) exit(1);

	char str[100] = { NULL };
	fscanf(file, "%*d,%s", str);

	printf("\nTicket No. --> %d From %s", ticketId, str);

	char buffer[500] = { NULL }, _Index[50] = { NULL }, _Report[400] = { NULL };
	while (fgets(buffer, 500, file))
		printf("%s", buffer);

	fclose(file);
}
void changeTicketStatus(int id)
{
	int count = 0;
	char* output = copyString("");

	FILE* file = fopen(FILE_TICKETS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL }, temp[500] = { NULL }, customerId[30] = { NULL }, date[40] = { NULL }, status[30] = { NULL };
	int ticketId = 0;

	while (fscanf(file, "%s", buffer) == 1)
	{
		sscanf(buffer, "%d,%[^,],%[^,],%[^,]", &ticketId, customerId, date, status);
		sprintf(temp, "%d,%s,%s,%s\n", ticketId, customerId, date, id == ticketId ? "CONFIRMED" : status);
		appendString(&output, temp);
	}
	
	fclose(file);

	file = fopen(FILE_TICKETS, "w");
	if (!file) exit(1);

	fputs(output, file);
	fclose(file);
	free(output);
	printf("Ticket Has Been Approved\n\n");
}
void printRevenue() 
{
	int selection = 0;

	while (!(selection >= 1 && selection <= 4)) 
	{
		printf("\nRevenue Actions\n'1' Last 30 Days     '2' Last 7 Days     '3' Current Day     '4' Return\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 4)) 
			printf("Invalid Input, Try Between [1 To 4]\n\n");
	}
	
	if (selection == 4)
		return;

	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	Date tempDate = { 0,0,0 };
	char buffer[500] = { NULL };
	float revenue = 0.0, price = 0.0;
	int ordersCount = 0;

	while (fscanf(file, "%s", buffer) == 1) 
	{	
		sscanf(buffer, "%*[^,],%*[^,],%f,%d/%d/%d,%*[^,]", &price, &tempDate.day, &tempDate.month, &tempDate.year);
		
		int dayDiff = calcDateDiff(tempDate);
		
		if (selection == 1) 
		{
			if (dayDiff <= 30) 
			{
				revenue += price;
				ordersCount++;
			}
		}
		
		else if (selection == 2) 
		{
			if (dayDiff <= 7) 
			{
				revenue += price;
				ordersCount++;
			}
		}
		
		else if (selection == 3) 
		{
			if (dayDiff == 0) 
			{
				revenue += price;
				ordersCount++;
			}
		}
	}

	fclose(file);
	printf("Asked Revenue --> %.2f In %d Orders\n", revenue, ordersCount);
}
void showOrders() 
{	
	int selection = 0;
	while (!(selection >= 1 && selection <= 3)) 
	{
		printf("\nOrder Actions\n'1' Print All Orders     '2' Confirm/Unconfirmed Orders     '3' Return\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 3))
			printf("Invalid Input, Try Between [1 To 3]\n\n");
	}
	
	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	int count = 0, waitingCount = 0;
	char buffer[500] = { NULL }, status[30] = { NULL };
	
	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%*[^,],%*[^,],%*[^,],%*[^,],%[^,]", status);
		
		if (strcmp(status, "WAITING") == 0) 
			waitingCount++;
		count++;
	}
	
	fclose(file);
	
	float price = 0.0;
	int* orders = NULL, * waitingOrders = NULL, orderId = 0;
	char customerId[30] = { NULL }, date[40] = { NULL };
	
	if (selection == 1)
	{
		file = fopen(FILE_ORDERS, "r");
		if (!file) exit(1);

		orders = malloc(sizeof(int) * count);

		printf("\n%-15s%-15s%-15s%-15s%-15s\n", "Order No. ", "Customer ID", "Total", "Date", "Status");

		int i = 0;
		while (fscanf(file, "%s", buffer) == 1)
		{
			sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
			printf("%-15d%-15s%-15.2f%-15s%-15s\n", orderId, customerId, price, date, status);
			orders[i] = orderId;
			i++;
		}

		bool flag = false;
		while (!flag)
		{
			printf("Select Order (Order No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < count; i++)
			{
				if (orders[i] == selection)
				{
					printOrder(selection);
					free(orders);
					fclose(file);
					return;
				}
			}
			printf("Invalid Input, Try Between [0 To %d]\n\n", i - 1);
		}
	}

	else if (selection == 2)
	{
		file = fopen(FILE_ORDERS, "r");
		if (!file) exit(1);

		waitingOrders = malloc(sizeof(int) * waitingCount);

		printf("\n%-15s%-15s%-15s%-15s%-15s\n", "Order No. ", "Customer ID", "Total", "Date", "Status");

		int i = 0;

		while (fscanf(file, "%s", buffer) == 1)
		{
			sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);

			if (strcmp(status, "WAITING") == 0)
			{
				printf("%-15d%-15s%-15.2f%-15s%-15s\n", orderId, customerId, price, date, status);
				waitingOrders[i] = orderId;
				i++;
			}
		}
		bool flag = false;

		while (!flag)
		{
			int selection = 0;

			printf("Select Order (Order No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < waitingCount; i++)
			{
				if (waitingOrders[i] == selection)
				{
					printOrder(selection);
					changeOrderStatus(selection);
					free(waitingOrders);
					fclose(file);
					return;
				}
			}
			printf("Invalid Input, Try Between [The Order No. Column]\n\n");
		}
	}

	else if (selection == 3)
		return;
}
void printOrder(int orderId) 
{
	char source[300] = { NULL };
	sprintf(source, "%s%d.csv", FOLDER_DATA_ORDERS_ORDERSUMMARY, orderId);

	FILE* file = fopen(source, "r");
	if (!file) exit(1);

	int count = 0;
	char str[100] = { NULL };
	fscanf(file, "%*d,%d,%s", &count, str);

	printf("\nOrder No. --> %d From %s\n", orderId, str);
	printf("%-14s%-15s%-15s%-15s%s", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");

	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL };
	int quantity = -1;
	float price = -1;

	for (int i = 0; i < count; i++) 
	{
		fscanf(file, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);
		printf("%-15s%-15s%-15s%-15.2f%-15d", name, company, category, price, quantity);
	}

	fscanf(file, "%f", &price);
	printf("\nTotal --> %.2f\n", price);
	fclose(file);
}
void changeOrderStatus(int id) 
{
	int count = 0;
	char* output = copyString("");

	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL }, temp[500] = { NULL }, customerId[30] = { NULL }, date[40] = { NULL }, status[30] = { NULL };
	int orderId = 0;
	float price = 0;
	
	while (fscanf(file, "%s", buffer) == 1) 
	{	
		sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
		sprintf(temp, "%d,%s,%f,%s,%s\n", orderId, customerId, price, date, id == orderId ? "CONFIRMED" : status);
		appendString(&output, temp);
	}

	fclose(file);
	
	file = fopen(FILE_ORDERS, "w");
	if (!file) exit(1);

	fputs(output, file);
	fclose(file);
	free(output);
	printf("Order Has Been Approved\n\n");
}
int calcDateDiff(Date d2) 
{
	Date d1 = getCurrentDate();
	int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int count1 = d1.year * 365 + d1.day;
	int count2 = d2.year * 365 + d2.day;
	
	for (int i = 0; i < d1.month; i++) 
		count1 += daysInMonth[i];
	
	for (int i = 0; i < d2.month; i++) 
		count2 += daysInMonth[i];
	
	return count1 - count2;
}
Date getCurrentDate() 
{
	Date date = { 0,0,0 };
	struct tm* tm;
	time_t t;
	char str_date[100] = { NULL };
	t = time(NULL);
	tm = localtime(&t);
	strftime(str_date, sizeof(str_date), "%d %m %Y", tm);
	sscanf(str_date, "%d %d %d", &date.day, &date.month, &date.year);
	return date;
}


//General
void welcomeScreen()
{
	int selection = 0;
	bool loop = true;

	while(loop)
	{
		printf("Welcome To Online SuperMarket HomePage\n");
		printf("'1' Register     '2' Login     '3' Exit\nInput --> ");
		selection = stringToInt();
		
		switch (selection) 
		{
		case 1:
			registerStage();
			break;

		case 2:
			loginUser();
			break;

		case 3:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 3]\n\n");
			break;
		}
	}
}
void registerStage() 
{
	int selection = 0, managerCode = 0;
	bool loop = true;

	while (loop)
	{
		printf("\nRegister As\n");
		printf("'1' Customer     '2' Manager     '3' Return\nInput --> ");
		selection = stringToInt();

		switch (selection) 
		{
		case 1:
			registerUserType(customer);
			loop = false;
			break;

		case 2:
			printf("Input Manager Code --> ");
			managerCode = stringToInt();

			if (MANAGER_CODE == managerCode)
				registerUserType(manager);

			else
				printf("Wrong Code, GoodBye\n\n");

			loop = false;
			break;

		case 3:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 3]\n\n");
			break;
		}
	}
}
void registerUserType(UserType type)
{
	printf("\nRegister Stage\n");

	Details d;

	while (!verifyName(&d))
		continue;

	while (!verifyPassword(&d))
		continue;

	while (!verifyId(&d))
		continue;

	if (type == customer)
		if (!verifyAge())
			return;

	while (!verifyPhone(&d))
		continue;

	if (!termsAndConditions())
		return;
	
	printf("You've Successfully Registered\n\n\n");
	writeUserType(&d, type);
}
void loginUser() 
{
	printf("\nLogin Stage\n");

	char* userId = NULL, * userPassword = NULL;

	printf("ID --> ");
	inputString(&userId);

	Identity = userId;

	UserType type = findUserType();

	if (type == none) 
	{
		printf("ID Does Not Exist\n\n");
		return;
	}

	printf("Password --> ");
	inputString(&userPassword);

	Details* user = readUser(type == customer ? FILE_CUSTOMERS : FILE_MANAGERS, type);

	if (strcmp(readUser(type == customer ? FILE_CUSTOMERS : FILE_MANAGERS, readPassword)->password, userPassword) == 0) 
	{
		printf("You've Logged As A ");

		if (type == customer) 
		{
			printf("Customer\n");
			customerMenu();
		}

		if (type == manager) 
		{
			printf("Manager\n");
			managerMenu();
		}
	}

	else
		printf("Invalid Password\n\n");
}
void profile() 
{
	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("\nProfile Actions\n");
		printf("'1' Print Profile     '2' Update Profile     '3' Return\nInput --> ");
		selection = stringToInt();

		switch (selection) 
		{
		case 1:
			printProfile();
			loop = false;
			break;

		case 2:
			updateProfile();
			loop = false;
			break;

		case 3:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 3]\n\n");
			break;
		}
	}
}
void printProfile() 
{
	printf("\nUser Information\n");

	Details* details = readUser(findUserType(Identity) == customer ? FILE_CUSTOMERS : FILE_MANAGERS, findUserType(Identity));
	char _Name[100] = { NULL }, _ID[100] = { NULL }, _Password[100] = { NULL }, _Phone[100] = { NULL };
	
	strcpy(_Name, details->name);
	strcpy(_ID, details->ID);
	strcpy(_Password, details->password);
	strcpy(_Phone, details->phone);

	printf("Name --> %s\n", _Name);
	printf("ID --> %s\n", _ID);
	printf("Password --> %s\n", _Password);
	printf("Phone --> %s\n", _Phone);

	if (findUserType(Identity) == customer)
	{
		float _Points = details->points;
		printf("Supermarket Points --> %.2f\n", _Points);
	}
	printf("\n");
}
void updateProfile() 
{
	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* User = fopen(findUserType(Identity) == customer ? FILE_CUSTOMERS : FILE_MANAGERS, "r");
	if (!User) exit(1);

	int selection = 0;
	bool loop = true;
	char _Name[100] = { NULL }, _Id[100] = { NULL }, _Password[100] = { NULL }, _Phone[100] = { NULL }, _Points[100] = { NULL }, buffer[500] = { NULL };
	Details d;

	if (findUserType(Identity) == customer) 
	{
		while (fscanf(User, "%s", buffer) == 1) 
		{
			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Points, _Phone);

			if ((strcmp(Identity, _Id)) == 0) 
			{
				while (loop)
				{
					printf("\nUpdatable Actions\n");
					printf("'1' Name     '2' ID     '3' Password     '4' Phone     '5' Return\nInput --> ");
					selection = stringToInt();

					switch (selection)
					{
					case 1:
						while (true)
							if (verifyName(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s,%s\n", d.name, _Id, _Password, _Points, _Phone);
						loop = false;
						printf("You've Successfully Updated Your Name\n\n");
						break;

					case 2:
						while (true)
							if (verifyId(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, d.ID, _Password, _Points, _Phone);
						loop = false;
						printf("You've Successfully Updated Your ID\n\n");
						break;

					case 3:
						while (true)
							if (verifyPassword(&d))
								break;
						
						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, d.password, _Points, _Phone);
						loop = false;
						printf("You've Successfully Updated Your Password\n\n");
						break;

					case 4:
						while (true)
							if (verifyPhone(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, d.phone);
						loop = false;
						printf("You've Successfully Updated Your Phone\n\n");
						break;

					case 5:
						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, _Phone);
						loop = false;
						break;

					default:
						printf("Invalid Input, Try Between [1 To 4]\n\n");
						break;
					}
				}
			}

			else
				fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, _Phone);
		}

		fclose(Temp);
		fclose(User);

		User = fopen(FILE_CUSTOMERS, "w");
		if (!User) exit(1);

		Temp = fopen(FILE_TEMP, "r");
		if (!Temp) exit(1);

		while (fscanf(Temp, "%s", buffer) == 1) 
		{
			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Points, _Phone);
			fprintf(User, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, _Phone);
		}
		fclose(Temp);
		fclose(User);
	}

	else if (findUserType(Identity) == manager) 
	{
		while (fscanf(User, "%s", buffer) == 1) 
		{
			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Phone);

			if ((strcmp(Identity, _Id)) == 0) 
			{
				while (loop)
				{
					printf("\nUpdatable Actions\n");
					printf("'1' Name     '2' ID     '3' Password     '4' Phone     '5' Return\nInput --> ");
					selection = stringToInt();

					switch (selection)
					{
					case 1:
						while (true)
							if (verifyName(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", d.name, _Id, _Password, _Phone);
						loop = false;
						printf("You've Successfully Updated Your Name\n\n");
						break;

					case 2:
						while (true)
							if (verifyId(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, d.ID, _Password, _Phone);
						loop = false;
						printf("You've Successfully Updated Your ID\n\n");
						break;

					case 3:
						while (true)
							if (verifyPassword(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, d.password, _Phone);
						loop = false;
						printf("You've Successfully Updated Your Password\n\n");
						break;

					case 4:
						while (true)
							if (verifyPhone(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, _Password, d.phone);
						loop = false;
						printf("You've Successfully Updated Your Phone\n\n");
						break;

					case 5:
						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, _Password, _Phone);
						loop = false;
						break;

					default:
						printf("Invalid Input, Try Between [1 To 4]\n\n");
						break;
					}
				}
			}

			else
				fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, _Password, _Phone);
		}

		fclose(Temp);
		fclose(User);

		User = fopen(FILE_MANAGERS, "w");
		if (!User) exit(1);

		Temp = fopen(FILE_TEMP, "r");
		if (!Temp) exit(1);

		while (fscanf(Temp, "%s", buffer) == 1) 
		{
			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Phone);
			fprintf(User, "%s,%s,%s,%s\n", _Name, _Id, _Password, _Phone);
		}
		fclose(Temp);
		fclose(User);
	}
}
char* getNextOrderIdStr() 
{
	char _Index[50] = { NULL };
	int num = 0;

	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL };
	while (fgets(buffer, 500, file))
		num++;

	sprintf(_Index, "%d", num);
	fclose(file);

	char* pString = NULL;
	pString = (char*)malloc(strlen(_Index) * sizeof(char) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, _Index);
	return pString;
}
char* getNextTicketIdStr()
{
	char _Index[50] = { NULL };
	int num = 0;

	FILE* file = fopen(FILE_TICKETS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL };
	while (fgets(buffer, 500, file))
		num++;

	sprintf(_Index, "%d", num);
	fclose(file);

	char* pString = NULL;
	pString = (char*)malloc(strlen(_Index) * sizeof(char) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, _Index);
	return pString;
}
bool doesProductExist(char* filename, char* _ProductName, char* _Company) 
{
	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	Product tempProduct = { NULL, NULL, NULL, 0, 0.0 };
	char name[100] = { NULL }, company[100] = { NULL }, buffer[500] = { NULL };

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%*[^,],%*d,%*f", name, company);

		if (strcmp(_ProductName, name) == 0 && strcmp(_Company, company) == 0) 
		{
			fclose(file);
			return true;
		}
	}

	fclose(file);
	return false;
}
Product selectProduct(Cart cart) 
{
	Product product = { NULL, NULL, NULL, 0, 0.0 };

	if (cart.itemsCount == 0)
		return product;

	printf("\n%-15s%-15s%-15s%-15s%-15s%-15s\n", "No.", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
	
	int i = 1, res = -1;
	char buffer[500] = { NULL };

	for (i = 0; i < cart.itemsCount; i++) 
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart.products[i].name, cart.products[i].company, cart.products[i].category, cart.products[i].price, cart.products[i].quantity);

	while (!(res >= 0 && res <= cart.itemsCount))
	{
		printf("\nAvailable Actions\n");
		printf("'0' Sort By Ascending Price     '1 To %d' Select Product\nInput --> ", cart.itemsCount);
		res = stringToInt();

		if (!(res >= 0 && res <= cart.itemsCount))
			printf("Invalid Input, Try Between [0 To %d]\n\n", cart.itemsCount);
	}

sort:
	if (res == 0) 
	{
		for (int p = 0; p < cart.itemsCount; p++) 
		{
			for (int j = p + 1; j < cart.itemsCount; j++) 
			{
				if (cart.products[p].price > cart.products[j].price) 
				{
					Product tempPrd = cart.products[p];
					cart.products[p] = cart.products[j];
					cart.products[j] = tempPrd;
				}
			}
		}

		printf("\n%-15s%-15s%-15s%-15s%-15s%-15s\n", "No. ", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
		
		for (int i = 0; i < cart.itemsCount; i++) 
			printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart.products[i].name, cart.products[i].company, cart.products[i].category, cart.products[i].price, cart.products[i].quantity);

		while (!(res - 1 >= 0 && res - 1 < i)) 
		{
			printf("\nSelect Product\nInput --> ");
			res = stringToInt();

			if (!(res - 1 >= 0 && res - 1 < i))
				printf("Invalid Input, Try Between [1 To %d]\n\n", i);
		}
		return cart.products[res - 1];
	}

	else 
		return cart.products[res - 1];

	return product;
}
Product readProduct(char* filename, char* _ProductName, char* _Company) 
{
	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	Product tempProduct = { NULL, NULL, NULL, 0, 0.0 };
	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL }, buffer[500] = { NULL };
	int quantity = 0;
	float price = 0.0;

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f, %d", name, company, category, &price, &quantity);

		if (strcmp(_ProductName, name) == 0 && strcmp(_Company, company) == 0) 
		{
			Product anotherTempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
			return anotherTempProduct;
		}
	}
	return tempProduct;
}
Cart retrieveProducts(bool returnAll, char* search, char* searchCategory)
{
	FILE* file = fopen(FILE_CATALOGS, "r");
	if (!file) exit(1);

	int count = 0, quantity = -1;
	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL }, buffer[500] = { NULL };

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%*f,%d", name, company, category, &quantity);
		if (search) 
		{
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0) 
			{
				if (quantity > 0 || returnAll)
					count++;
			}
		}

		else if (searchCategory) 
		{
			if (strcmp(category, searchCategory) == 0) 
			{
				if (quantity > 0 || returnAll)
					count++;
			}
		}

		else if (quantity > 0 || returnAll)
			count++;
	}

	Cart cart = { NULL, 0 };

	if (count == 0)
	{
		printf("Product/Company Does Not Exist In Our Catalog\n\n");
		return cart;
	}

	float price = -1;
	int ite = 0;

	cart.products = malloc(sizeof(Product) * count);
	cart.itemsCount = count;
	fclose(file);

	file = fopen(FILE_CATALOGS, "r");
	if (!file) exit(1);

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);
		
		if (search) 
		{
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0) 
			{
				if (quantity > 0 || returnAll) 
				{
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}
			}
		}

		else if (searchCategory) 
		{
			if (strcmp(category, searchCategory) == 0)
			{
				if (quantity > 0 || returnAll) 
				{
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}
			}
		}

		else if (quantity > 0 || returnAll) 
		{
			Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
			cart.products[ite] = tempProduct;
			ite++;
		}
	}

	fclose(file);
	return cart;
}


int main() 
{
	checkFolder();
	checkFiles();
	welcomeScreen();
	
	return 0;
}
