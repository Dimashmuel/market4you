#include "header.h"

// String functions definitions
void inputString(char** _str)
{
	char str[100];

	scanf_s("%s", str, 100);
	*_str = malloc(strlen(str) * sizeof(char) + sizeof(char));
	strcpy(*_str, str);
}
void appendString(char** _str1, char* _str2)
{
	char* str = malloc((strlen(*_str1) + strlen(_str2)) * sizeof(char) + sizeof(char));
	if (str == NULL) exit(1);

	strcpy(str, *_str1);
	strcat(str, _str2);
	free(*_str1);
	*_str1 = str;
}
void appendStringComma(char** _str1, char* _str2)
{
	appendString(_str1, _str2);
	appendString(_str1, ",");
}
void appendStringNewLine(char** _str1, char* _str2)
{
	appendString(_str1, _str2);
	appendString(_str1, "\n");
}
char* copyString(char* _str)
{
	char* str = malloc(sizeof(char) * strlen(_str) + sizeof(char));
	if (str == NULL)
		exit(1);

	strcpy(str, _str);
	return str;
}
char* strToLower(char* _str) {
	char* str = copyString(_str);
	if (!str) exit(1);
	for (int i = 0; i < strlen(_str); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

// Files functions definitions
void checkFiles() //Create the defualts files.
{
	if (!doesFileExists(FILE_CUSTOMERS)) writeFile(FILE_CUSTOMERS, "");
	if (!doesFileExists(FILE_MANAGERS)) writeFile(FILE_MANAGERS, "");
	if (!doesFileExists(FILE_CATALOG)) writeFile(FILE_CATALOG, "");
	if (!doesFileExists(FILE_GLOBAL)) writeFile(FILE_GLOBAL, "0");
	if (!doesFileExists(FILE_ORDERS)) writeFile(FILE_ORDERS, "");
}
void checkFolder() //Create the defualts folders.
{
	if (!doesFileExists(FOLDER_DATA)) createFolder(FOLDER_DATA);
	if (!doesFileExists(FOLDER_DATA_USERS)) createFolder(FOLDER_DATA_USERS);
	if (!doesFileExists(FOLDER_DATA_ORDERS)) createFolder(FOLDER_DATA_ORDERS);
}
void createFolder(char* dirname)
{
	int check;
	check = mkdir(dirname);
	if (check)
	{
		printf("Unable to create folder\n");
		exit(1);
	}
}
void writeFile(char* filename, char* content)
{
	FILE* file;
	file = fopen(filename, "ab+");
	if (!file)
	{
		printf("Error.\n");
		return;
	}
	fprintf(file, "%s", content);
	fclose(file);
}
void writeUserType(Details* d, UserType type) {
	char output[200];
	type == customer ?
		sprintf(output, "%s,%s,%s,%f,%s", d->name, d->ID, d->password, d->points, d->phone) : // customer
		sprintf(output, "%s,%s,%s,%s", d->name, d->ID, d->password, d->phone); // manager

	type == customer ?
		writeFile(FILE_CUSTOMERS, output) :
		writeFile(FILE_MANAGERS, output);
}
Details readUser(char* filename, char* id, FieldUser field)
{
	Details details = { NULL, NULL, NULL, NULL, 0 };
	char nameString[100], ID[20], password[100], phoneString[20];
	float supermarketPoints = 0;

	FILE* file;
	file = fopen(filename, "r");
	if (!file)
	{
		printf("Error2.\n");
		return;
	}

	char buf[500];
	while (fscanf(file, "%s", buf) == 1)
	{
		field == customer ?
			sscanf(buf, "%[^,],%[^,],%[^,],%f,%[^,]", nameString, ID, password, &supermarketPoints, phoneString) :
			sscanf(buf, "%[^,],%[^,],%[^,],%[^,]", nameString, ID, password, phoneString);
		if (strcmp(ID, id) == 0)
		{
			Details returnDetails = { nameString, ID, password, phoneString, supermarketPoints };
			fclose(file);
			return returnDetails;
		}


	}
	fclose(file);
	return details;
}
bool doesFileExists(char* filename)
{
	struct stat buffer;
	return !stat(filename, &buffer);
}

// Authentication functions definitions
void registerUserType(UserType type)
{
	Details d;
	{
		printf("\nRegister Stage:\n");
		while (!verifyName(&d))
			printf("Invalid, Please try again!\n\n");

		while (!verifyPassword(&d))
			printf("Invalid, Please try again!\n\n");

		bool flag = false;
		while (!verifyId(&d, &flag))
		{
			if (flag)
			{
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
			else
				printf("Invalid, Please try again!\n\n");
		}

		if (type == customer)
		{
			if (!verifyAge())
				return false;
			// Credit points ??
		}

		while (!verifyPhone(&d))
			printf("Invalid, Please try again!\n\n");

		if (!termsAndConditions())
			return;

		writeUserType(&d, type);
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
	int letters = 0, numbers = 0;

	printf("Password --> ");
	inputString(&str);

	if (strlen(str) < MIN_PASSWORD_LEN)
		return false;

	for (int i = 0; i < strlen(str); i++)
	{
		if (!(str[i] >= 'a' && str[i] <= 'z'))
		{
			if (!(str[i] >= 'A' && str[i] <= 'Z'))
			{
				if (!(str[i] >= '0' && str[i] <= '9'))
					return false;
				else numbers++;
			}
			else letters++;
		}
		else letters++;
	}

	if (!numbers)
		printf("Password must contain at least one number\n");

	if (!letters)
		printf("Password must contain at least one letter\n");

	if (letters && numbers)
	{
		d->password = str;
		return true;
	}
	return false;
}
bool verifyId(Details* d, bool* flag)
{
	char* str;

	printf("ID --> ");
	inputString(&str);

	long idVal = strtol(str, NULL, 10);
	int Digit1, Digit2, Digit3, Digit4, Digit5, Digit6, Digit7, Digit8, Digit9, Sum, Res;

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
			if (findUserType(str))
			{
				*flag = true;
				return false;
			}

			d->ID = str;
			return true;
		}

		else if (Res != Digit9)
			return false;
	}

	else
		return false;
}
bool verifyAge()
{
	int age;

	printf("Age --> ");
	scanf_s("%d", &age);

	if (age < LEGAL_AGE)
	{
		printf("We are sry, The minimum age is 16\n");
		return false;
	}
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
bool termsAndConditions()
{
	char terms;

	printf("Do you agree to the terms and conditions (Y/N)\nInput --> ");
	getchar();
	scanf("%c", &terms);

	while (terms != 'Y' || terms != 'N')
	{
		if (terms == 'Y')
			return true;
		else if (terms == 'N')
			return false;
		else
		{
			printf("Invalid input, Try again\nInput --> ");
			getchar();
			scanf("%c", &terms);
		}
	}
}
UserType findUserType(char* id)
{
	Details details = readUser(FILE_CUSTOMERS, id, readId);

	if (details.ID)
		return customer;

	details = readUser(FILE_MANAGERS, id, readId);
	if (details.ID)
		return manager;

	return none;
}
UserType loginUser()
{
	char* id, * pass;

	printf("ID --> ");
	inputString(&id);

	UserType type = findUserType(id);
	if (!type)
	{
		printf("ID doesnt exist\n");
		return none;
	}

	printf("Password --> ");
	inputString(&pass);

	if (strcmp(readUser(type == customer ? FILE_CUSTOMERS : FILE_MANAGERS, id, readPassword).password, pass) == 0)
	{
		printf("You've successfully logged in as a ");
		if (type == customer)
		{
			printf("customer.\n");
			return customer;
		}

		if (type == manager)
		{
			printf("manager.\n");
			return manager;
		}
	}

	else
	{
		printf("Invalid Password\n");
		return none;
	}
}

// Catalog definitions
void catalogAddProduct() {
	char* rProductName = NULL;
	while (!rProductName)
	{
		printf("Product name --> ");
		inputString(&rProductName);

		for (int i = 0; i < strlen(rProductName); i++)
		{
			if (!(rProductName[i] >= 'a' && rProductName[i] <= 'z' || rProductName[i] >= 'A' && rProductName[i] <= 'Z'))
			{
				rProductName = NULL;
				printf("Invalid product name\n");
				break;
			}
		}
	}

	char* rProductCompany = NULL;
	while (!rProductCompany)
	{
		printf("Company product --> ");
		inputString(&rProductCompany);

		if (doesProductExist(FILE_CATALOG, rProductName, rProductCompany))
		{
			printf("Company product already in stock\n");
			return;
		}

		for (int i = 0; i < strlen(rProductCompany); i++)
		{
			if (!(rProductCompany[i] >= 'a' && rProductCompany[i] <= 'z' || rProductCompany[i] >= 'A' && rProductCompany[i] <= 'Z'))
			{
				rProductCompany = NULL;
				printf("Invlaid company product\n");
				break;
			}
		}
	}

	float rProductPrice = -1;
	while (rProductPrice < 0)
	{
		printf("Product price --> ");
		scanf("%f", &rProductPrice);
		if (rProductPrice < 0)
			printf("Price must be positive, please try again.\n");
	}

	int rProductQuantity = -1;
	while (rProductQuantity < 0)
	{
		printf("Product quantity --> ");
		scanf("%d", &rProductQuantity);
		if (rProductQuantity < 0)
			printf("Quantity must be positive, please try again.\n");
	}

	char* rProductCategory = NULL;
	while (!rProductCategory)
	{
		printf("Product category --> ");
		inputString(&rProductCategory);

		for (int i = 0; i < strlen(rProductCategory); i++)
		{
			if (!(rProductCategory[i] >= 'a' && rProductCategory[i] <= 'z' || rProductCategory[i] >= 'A' && rProductCategory[i] <= 'Z'))
			{
				rProductCategory = NULL;
				printf("Invlaid input\n");
				break;
			}
		}
	}
	char output[200];
	sprintf(output, "%s,%s,%s,%f,%d\n", rProductName, rProductCompany, rProductCategory, rProductPrice, rProductQuantity);
	writeFile(FILE_CATALOG, output);
}
bool doesProductExist(char* filename, char* _productName, char* _company) {
	Product tempProduct = { NULL, NULL, NULL, 0, 0 };
	char name[100], company[100];
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Error.");
		exit(1);
	}

	char buf[500];
	while (fscanf(file, "%s", buf) == 1) {
		sscanf(buf, "%[^,],%[^,],%*[^,],%*d,%*f", name, company);
		if (strcmp(_productName, name) == 0 && strcmp(_company, company) == 0) {
			fclose(file);
			return true;
		}
	}
	fclose(file);
	return false;
}
Cart retrieveProducts(bool returnAll, char* search, char* searchCategory) {
	int count = 0;
	FILE* file = fopen(FILE_CATALOG, "r");
	char buffer[500];
	while (fscanf(file, "%s", buffer) == 1) {
		char name[40], company[40], category[40];
		int quantity = -1;
		sscanf(buffer, "%[^,],%[^,],%[^,],%*f,%d", name, company, category, &quantity);
		if (search) {
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0) {
				if (quantity > 0 || returnAll)
					count++;
			}
		}
		else if (searchCategory) {
			if (strcmp(category, searchCategory) == 0) {
				if (quantity > 0 || returnAll)
					count++;
			}
		}
		else if (quantity > 0 || returnAll)
			count++;
	}

	char name[100], company[100], category[100];
	int quantity = -1;
	float price = -1;
	int ite = 0;
	Cart cart;
	cart.products = malloc(sizeof(Product) * count);
	cart.itemsCount = count;
	fclose(file);
	file = fopen(FILE_CATALOG, "r");
	while (fscanf(file, "%s", buffer) == 1) {
		sscanf(buffer, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);
		if (search) {
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0) {
				if (quantity > 0 || returnAll) {
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}
			}
		}
		else if (searchCategory) {
			if (strcmp(category, searchCategory) == 0) {
				if (quantity > 0 || returnAll) {
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}
			}
		}
		else if (quantity > 0 || returnAll) {
			Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
			cart.products[ite] = tempProduct;
			ite++;
		}
	}
	fclose(file);
	return cart;
}
void customerShop(Cart* cart) {
	Product product = selectProduct(retrieveProducts(false, NULL, NULL));
	if (!product.name) {
		printf("Something went wrong.\n");
		return;
	}
	int selectedQuantity = -1;
	printf("Please select the amount to add --> ");
	scanf("%d", &selectedQuantity);
	while (!(selectedQuantity > 0 && selectedQuantity <= product.quantity)) {
		printf("Incorrect amount, please select the amount to add --> ");
		scanf("%d", &selectedQuantity);
	}
	Product addedProduct = product;
	addedProduct.quantity = selectedQuantity;
	addToCart(cart, addedProduct);
	//updateQuantityProduct()...
}

Product selectProduct(Cart cart) {
	Product product = { NULL, NULL, NULL, 0, 0 };

	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "ID", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
	int i = 1;
	char buffer[500];

	for (i = 0; i < cart.itemsCount; i++) {
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n",
			i + 1,
			cart.products[i].name,
			cart.products[i].company,
			cart.products[i].category,
			cart.products[i].price,
			cart.products[i].quantity);
	}

	printf("Enter 0 to sort the products by ascending price\n");
	printf("Or select a product --> ");

	int res = 0;
	scanf("%d", &res);
sort:
	if (res == 0) {
		for (int p = 0; p < cart.itemsCount; p++) {
			for (int j = p + 1; j < cart.itemsCount; j++) {
				if (cart.products[p].price > cart.products[j].price) {
					Product tempPrd = cart.products[p];
					cart.products[p] = cart.products[j];
					cart.products[j] = tempPrd;
				}
			}
		}

		printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "ID", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
		for (int i = 0; i < cart.itemsCount; i++) {
			printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n",
				i + 1,
				cart.products[i].name,
				cart.products[i].company,
				cart.products[i].category,
				cart.products[i].price,
				cart.products[i].quantity);
		}

		printf("Select product --> ");
		scanf("%d", &res);
		while (!(res - 1 >= 0 && res - 1 < i)) {
			printf("Invalid selection, please try again.\n");
			printf("Select product --> ");
			scanf("%d", &res);
		}
		return cart.products[res - 1];
	}
	else {
		while (!(res -1 >= 0 && res - 1 < i)) {
			printf("Invalid selection, please try again.\n");
			printf("Select product --> ");
			scanf("%d", &res);
			if (res == 0) goto sort;
		}
		return cart.products[res - 1];
	}
	return product;
}
Product readProduct(char* filename, char* _productName, char* _company, FieldProduct field) {
	Product tempProduct = { NULL, NULL, NULL, 0, 0 };
	char name[100], company[100], category[100];
	int quantity = 0;
	float price = 0;

	FILE* file;
	file = fopen(filename, "r");
	if (!file) {
		printf("Error.");
		exit(1);
	}

	char buf[500];
	while (fscanf(file, "%s", buf) == 1)
	{
		sscanf(buf, "%[^,],%[^,],%[^,],%d,%f", name, company, category, quantity, price);

		if (strcmp(_productName, name) == 0 && strcmp(_company, company) == 0)
		{
			Product anotherTempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
			return anotherTempProduct;
		}
	}
	return tempProduct;
}

// Cart definitios
int getNextOrderId() {
	int next;
	FILE* file = fopen(FILE_GLOBAL, "r");
	fscanf(file, "%d", &next);
	fclose(file);
	return next;
}
char* getNextOrderIdStr() {
	char* str = malloc(sizeof(char) * 100);
	FILE* file = fopen(FILE_GLOBAL, "r");
	fscanf(file, "%s", str);
	fclose(file);
	return str;
}
void appendOrderId() {
	int next;
	FILE* file = fopen(FILE_GLOBAL, "r");
	fscanf(file, "%d", &next);
	fclose(file);
	file = fopen(FILE_GLOBAL, "w");
	fprintf(file, "%d", ++next);
}
void addToCart(Cart* cart, Product product) {
	if (cart->itemsCount == 0) {
		cart->products = malloc(sizeof(Product));
		cart->products[0] = product;
	}
	else {
		Product* newProducts = malloc(sizeof(Product) * (cart->itemsCount + 1));
		for (int i = 0; i < cart->itemsCount; i++) newProducts[i] = cart->products[i];
		newProducts[cart->itemsCount] = product;
		cart->products = newProducts;
	}
	cart->itemsCount++;
}
void printCart(Cart* cart) {
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "ID", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
	float total = 0;
	int i;
	for (i = 0; i < cart->itemsCount; i++) {
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n",
			i + 1,
			cart->products[i].name,
			cart->products[i].company,
			cart->products[i].category,
			cart->products[i].price,
			cart->products[i].quantity);
		total += cart->products[i].price * cart->products[i].quantity;
	}
	printf("Total: %.2f\n", total);
}
void viewCart(Cart* cart) {
	printCart(cart);
	int option;
	printf("1. Edit cart\n2.Back to menu\nSelect an option --> ");
	scanf("%d", &option);
	switch (option) {
	case 1:
	{
		int anotherOption;
		printf("Select the product you'd like to edit --> ");
		scanf("%d", &anotherOption);

		while (!(anotherOption - 1 < cart->itemsCount && anotherOption > 0)) {
			printf("Invalid selection, please try again.\n");
			printf("Select the product you'd like to edit --> ");
			scanf("%d", &anotherOption);
		}

		int newOption;
		printf("1. Change quantity.\n2. Remove product\nSelect an option --> ");
		scanf("%d", &newOption);

		while (!(newOption == 1 || newOption == 2)) {
			printf("Invalid selection, please try again.\n");
			printf("1. Change quantity.\n2. Remove product\n3. Return to menu\nSelect an option --> ");
			scanf("%d", &newOption);
		}

		if (newOption == 1) {
			printf("Please enter the new quantity for the product --> ");
			scanf("%d", &newOption);
			while (newOption < 0) {
				printf("Invalid qunatity, please try agian.\n");
				printf("Please enter the new quantity for the product --> ");
				scanf("%d", &newOption);
			}

			changeQuantity(cart, anotherOption - 1, newOption);
		}
		break;
	}
	case 2:
	{
		int anotherOption;
		printf("Select the product you'd like to remove --> ");
		scanf("%d", &anotherOption);

		while (!(anotherOption - 1 < cart->itemsCount && anotherOption > 0)) {
			printf("Invalid selection, please try again.\n");
			printf("Select the product you'd like to edit --> ");
			scanf("%d", &anotherOption);
		}
		removeFromCart(cart, anotherOption - 1);
		break;
	}
	case 3:
		return;
	default:
		printf("Bad option, returning to menu.\n");
		break;
	}
}
void changeQuantity(Cart* cart, int index, int newQuantity) {
	bool flag = false;
	int availableQuantity = 0;
	if (newQuantity > cart->products[index].quantity) {
		if (newQuantity - cart->products[index].quantity > availableQuantity) {
			printf("The selected quantity is exceeding the current available quantity, you may add up to %d items.", availableQuantity);
		}
	}
	//updateProduct(...);
	cart->products[index].quantity = newQuantity;
}
void removeFromCart(Cart* cart, int index) {
	Product* newProduct = malloc(sizeof(Product) * (cart->itemsCount - 1));

	for (int i = 0; i < index; i++) {
		Product tempProduct = {
			copyString(cart->products[i].name),
			copyString(cart->products[i].company),
			copyString(cart->products[i].category),
			cart->products[i].price,
			cart->products[i].quantity
		};
		newProduct[i] = tempProduct;
	}

	for (int i = index + 1; i < index; i++) {
		Product tempProduct = {
		copyString(cart->products[i].name),
		copyString(cart->products[i].company),
		copyString(cart->products[i].category),
		cart->products[i].price,
		cart->products[i].quantity
		};
		newProduct[i - 1] = tempProduct;
	}
}
bool verifyPayment(char* creditCard, char* cvv, int expirationMonth, int expirationYear) {
	if (expirationMonth <= 0 || expirationMonth > 12) {
		return false;
	}
	for (int i = 0; i < strlen(creditCard); i++) {
		if (!(creditCard[i] >= '0' && creditCard[i] <= '9')) {
			return false;
		}
	}
	for (int i = 0; i < strlen(cvv); i++) {
		if (!(cvv[i] >= '0' && cvv[i] <= '9')) {
			return false;
		}
	}
	Date date = getCurrentDate();
	if (date.year > expirationYear)
		return false;
	else if (date.year == expirationYear)
		 return date.month <= expirationMonth;
	return true;

}
void finishOrder(Cart* cart, char* id) {
	printCart(cart);
	int ans;
	printf("Please make sure the information shown above is correct.\nAre you sure you would like to finish the order?\n1. Yes\n2. No\nSelect option --> ");
	scanf("%d", &ans);
	while (ans != 1 && ans != 2) {
		printf("Incorrect answer, please try again.\n");
		printf("Are you sure you would like to finish the order? (Y/N) ");
		scanf("%d", &ans);
	}
	if (ans == 2)
		return;

	char creditCard[20], cvv[20];
	int expirationMonth, expirationYear;
	printf("Please enter your credit card number --> ");
	scanf("%s", creditCard);
	printf("Please enter the CVV --> ");
	scanf("%s", cvv);
	printf("Please enter expiration month (numerical value) --> ");
	scanf("%d", &expirationMonth);
	printf("Please enter expiration year --> ");
	scanf("%d", &expirationYear);
	while (!verifyPayment(creditCard, cvv, expirationMonth, expirationYear)) {
		printf("The entered card is invalid, please try again.\n");
		printf("Please enter your credit card number --> ");
		scanf("%s", creditCard);
		printf("Please enter the CCV --> ");
		scanf("%s", creditCard);
		printf("Please enter expiration month (numerical value) --> ");
		scanf("%d", &expirationMonth);
		printf("Please enter expiration year --> ");
		scanf("%d", &expirationYear);
	}
	writeOrder(cart, id);
}
void writeOrder(Cart* cart, char* id) {
	char source[300];
	sprintf(source, "%s%s.csv", FOLDER_DATA_ORDERS, getNextOrderIdStr());
	FILE* file = fopen(source, "w");
	Date date = getCurrentDate();
	fprintf(file, "%d,%d,%d/%d/%d\n", getNextOrderId(), cart->itemsCount, date.day, date.month, date.year);
	float total = 0;
	int i;
	for (i = 0; i < cart->itemsCount; i++) {
		fprintf(file, "%s,%s,%s,%f,%d\n",
			cart->products[i].name,
			cart->products[i].company,
			cart->products[i].category,
			cart->products[i].price,
			cart->products[i].quantity);
		total += cart->products[i].price * cart->products[i].quantity;
	}
	fprintf(file, "%.2f\n", total);
	fclose(file);
	file = fopen(FILE_ORDERS, "ab+");
	fprintf(file, "%d,%s,%.2f,%d/%d/%d,WAITING\n", getNextOrderId(), id, total, date.day, date.month, date.year);
	fclose(file);
	appendOrderId();
	Details user = readUser(FILE_CUSTOMERS, id, customer);
	int ans;
	if (user.points != 0) {
		printf("You have %.2f supermakert points available to use, would you like to reedem them in this purchase?\n1. Yes\n2. No\nSelect option --> ", user.points);
		scanf("%d", &ans);
		while (ans != 1 && ans != 2) {
			printf("Would you like to reedem them in this purchase?\n1. Yes\n2. No\nSelect option --> ");
			scanf("%d", &ans);
		}
		if (ans == 1) {
			updatePoints(id, user.points > total ? user.points - total : 0);
		}
	}
	printf("In this purchase you've earned %.2f supermarket points (3%% of your purchase amount).\n", total*0.03);
	updatePoints(id, user.points + total * 0.03);
}
void printOrder(int orderId) {
	char source[300];
	sprintf(source, "%s%d.csv", FOLDER_DATA_ORDERS, orderId);
	if (!doesFileExists(source)) {
		printf("Order doesn't exist.");
		return;
	}
	FILE* file = fopen(source, "r");
	char str[20];
	int count;
	fscanf(file, "%*d,%d,%s", &count, str);
	printf("Order ID: %d from %s\n", orderId, str);
	printf("%-15s%-15s%-15s%-15s%-15s\n", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
	char name[100], company[100], category[100];
	int quantity = -1;
	float price = -1;
	for (int i = 0; i < count; i++) {
		fscanf(file, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);
		printf("%-15s%-15s%-15s%-15.2f%-15d\n",
			name,
			company,
			category,
			price,
			quantity);
	}
	fscanf(file, "%f", &price);
	printf("Total: %.2f", price);
	fclose(file);
}
Date getCurrentDate() {
	Date date = { 0,0,0 };
	struct tm* tm;
	time_t t;
	char str_date[100];
	t = time(NULL);
	tm = localtime(&t);
	strftime(str_date, sizeof(str_date), "%d %m %Y", tm);
	sscanf(str_date, "%d %d %d", &date.day, &date.month, &date.year);
	return date;
}
void changeOrderStatus(int id) {
	if (!doesFileExists(FILE_ORDERS)) {
		printf("Orders file missing..?");
		return;
	}
	FILE* file = fopen(FILE_ORDERS, "r");
	int count = 0;
	char buffer[500];
	char* output = copyString("");
	while (fscanf(file, "%s", buffer) == 1) {
		char temp[500];
		char customerId[30], date[40], status[30];
		int orderId = -1;
		float price = -1;
		sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
		sprintf(temp, "%d,%s,%f,%s,%s\n", orderId, customerId, price, date, id == orderId ? "CONFIRMED" : status);
		appendString(&output, temp);
	}
	fclose(file);
	file = fopen(FILE_ORDERS, "w");
	fputs(output, file);
	fclose(file);
	free(output);
	printf("Order has been approved.\n");
}
void updatePoints(char* id, float newPoints) {
	if (!doesFileExists(FILE_CUSTOMERS)) {
		printf("Orders file missing..?");
		return;
	}
	FILE* file = fopen(FILE_CUSTOMERS, "r");
	int count = 0;
	char buffer[500];
	char* output = copyString("");
	while (fscanf(file, "%s", buffer) == 1) {
		char temp[500];
		char name[30], customerId[40], pass[30], phone[30];
		float points = -1;
		sscanf(buffer, "%[^,],%[^,],%[^,],%f,%[^,]", name, customerId, pass, &points, phone);
		sprintf(temp, "%s,%s,%s,%f,%s\n", name, customerId, pass, strcmp(customerId, id) == 0 ? newPoints : points, phone);
		appendString(&output, temp);
	}
	fclose(file);
	file = fopen(FILE_CUSTOMERS, "w");
	fputs(output, file);
	fclose(file);
	free(output);
	printf("Current supermaker points %f.\n", newPoints);
}
void showOrders() {
	int option;
	printf("1. All Orders\n2. Unconfimred Orders\nSelect option --> ");
	scanf("%d", &option);
	while (!(option <= 3 && option >= 0)) {
		printf("Invalid selection, please try again.\nSelect option --> ");
		scanf("%d", &option);
	}

	FILE* file = fopen(FILE_ORDERS, "r");
	char buffer[500];
	int count = 0;
	int waitingCount = 0;

	int* orders;
	int* waitingOrders;
	while (fscanf(file, "%s", buffer) == 1) {
		char status[30];
		sscanf(buffer, "%*[^,],%*[^,],%*[^,],%*[^,],%[^,]", status);
		if (strcmp(status, "WAITING") == 0) waitingCount++;
		count++;
	}
	fclose(file);
	if (option == 1) {
		//fclose(file);
		file = fopen(FILE_ORDERS, "r");
		orders = malloc(sizeof(int) * count);
		printf("%-15s%-15s%-15s%-15s%-15s\n", "Order ID", "Customer ID", "Total", "Date", "Status");
		int i = 0;
		while (fscanf(file, "%s", buffer) == 1) {
			char customerId[30], date[40], status[30], address[40];
			int orderId = -1;
			float price = -1;
			sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
			printf("%-15d%-15s%-15.2f%-15s%-15s\n", orderId, customerId, price, date, status);
			orders[i] = orderId;
			i++;
		}
		bool flag = false;
		while (!flag) {
			int ans;
			printf("Select order (ID) -- > ");
			scanf("%d", &ans);
			for (int i = 0; i < count; i++) {
				if (orders[i] == ans) {
					printOrder(ans);
					free(orders);
					fclose(file);
					return;
				}
			}
			printf("Invalid selection, please try again.\n");
		}
	}
	else if (option == 2) {
		//fclose(file);
		file = fopen(FILE_ORDERS, "r");
		waitingOrders = malloc(sizeof(int) * waitingCount);
		printf("%-15s%-15s%-15s%-15s%-15s\n", "Order ID", "Customer ID", "Total", "Date", "Status");
		int i = 0;
		while (fscanf(file, "%s", buffer) == 1) {
			char customerId[30], date[40], status[30];
			int orderId = -1;
			float price = -1;
			sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
			if (strcmp(status, "WAITING") == 0) {
				printf("%-15d%-15s%-15.2f%-15s%-15s\n", orderId, customerId, price, date, status);
				waitingOrders[i] = orderId;
			}
		}
		bool flag = false;
		
		while (!flag) {
			int ans;
			printf("Select order (ID) -- > ");
			scanf("%d", &ans);
			for (int i = 0; i < waitingCount; i++) {
				if (waitingOrders[i] == ans) {
					printOrder(ans);
					changeOrderStatus(ans);
					free(waitingOrders);
					fclose(file);
					return;
				}
			}
			printf("Invalid selection, please try again.\n");
		}
	}
}
Product selectByCategory() {
	Cart tempCart = retrieveProducts(false, NULL, NULL);
	int count = 0;
	char** categories = malloc(sizeof(char*) * tempCart.itemsCount);
	for (int i = 0; i < tempCart.itemsCount; i++) {
		bool flag = false;
		for (int j = 0; j < count; j++) {
			if (strcmp(tempCart.products[i].category, categories[j]) == 0) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			categories[count] = malloc(sizeof(char) * strlen(tempCart.products[i].category) + sizeof(char));
			strcpy(categories[count], tempCart.products[i].category);
			printf("%d. %s\n", count + 1, categories[count]);
			count++;
		}
	}
	if (count == 0) {
		printf("No categories found..?\n");
		return;
	}
	int ans;
	printf("Please select a category from the list shown above (index) --> ");
	scanf("%d", &ans);
	ans--;
	while (!(ans >= 0 && ans < count)) {
		printf("Incorrect category selected.\n");
		printf("Please select a ctaegory from the list shown above --> ");
		scanf("%d", &ans);
		ans--;
	}
	tempCart = retrieveProducts(false, NULL, categories[ans]);
	return selectProduct(tempCart);
}
int calcDateDiff(Date d2) {
	Date d1 = getCurrentDate();
	int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int count1 = d1.year * 365 + d1.day;
	int count2 = d2.year * 365 + d2.day;
	for (int i = 0; i < d1.month; i++) count1 += daysInMonth[i];
	for (int i = 0; i < d2.month; i++) count2 += daysInMonth[i];
	return count1 - count2;
}
void printRevenue() {
	int ans;
	printf("1. Last 30 days\n2. Last 7 days\n3. Current day\nSelect option -- > ");
	scanf("%d", &ans);
	while (!(ans <= 3 && ans >= 1)) {
		printf("Incorrect selectio, please try again.\nSelect option -- > ");
		scanf("%d", &ans);
	}
	Date tempDate;
	FILE* file = fopen(FILE_ORDERS, "r");
	int ordersCount = 0;
	float revenue = 0;
	char buffer[500];
	while (fscanf(file, "%s", buffer) == 1) {
		float price = 0;
		sscanf(buffer, "%*[^,],%*[^,],%f,%d/%d/%d,%*[^,]", &price, &tempDate.day, &tempDate.month, &tempDate.year);
		int dayDiff = calcDateDiff(tempDate);
		if (ans == 1) {
			if (dayDiff <= 30) {
				revenue += price;
				ordersCount++;
			}
		}
		else if (ans == 2) {
			if (dayDiff <= 7) {
				revenue += price;
				ordersCount++;
			}
		} else if (ans == 3) {
			if (dayDiff == 0) {
				revenue += price;
				ordersCount++;
			}

		}
	}
	fclose(file);
	printf("Revenue for the selected time period: %.2f from a total of %d orders.", revenue, ordersCount);
}

//Menu definitions
void welcomeScreen()
{
	int loop = 1, regLog = 1, regType = 1, managerCode;

	printf("Welcome to \"Market for you\"\n'1' Register\n'2' Log in\n'3' Exit\nInput --> ");
	scanf_s("%d", &regLog);

	while (loop)
	{
		switch (regLog)
		{
		case 1:
			printf("\nSign up as\n'1' Customer\n'2' Manager\n'3' Exit\nInput --> ");
			scanf_s("%d", &regType);

			switch (regType)
			{
			case 1:
				registerUserType(customer);
				break;

			case 2:
				printf("Input manager code --> ");
				scanf_s("%d", &managerCode);

				if (MANAGER_CODE == managerCode)
					registerUserType(manager);

				else
					printf("Wrong Code\n");

				break;

			case 3:
				break;

			default:
				printf("Error input, Try again!\n\nInput --> ");
				scanf_s("%d", &regType);
				break;

			}
			break;

		case 2:
			if (loginUser() == customer)
				customerMenu();
			else if (loginUser() == manager)
				managerMenu();

			else
				printf("Failed login\n");
			break;

		case 3:
			loop = 0;
			break;

		default:
			printf("Invalid input, Try again!\n\nInput --> ");
			scanf_s("%d", &regLog);
			break;

		}
		return;
	}
}
void customerMenu()
{
	Cart cart = { NULL, 0 };
	for (int i = 0; i < 3; i++) {
		customerShop(&cart);
	}
	finishOrder(&cart, "123456789");
}
void managerMenu()
{
	return;
}

//char* userId;
int main() {
	//Cart cart = { NULL, 0 };
	//checkFolder();
	//checkFiles();
	//welcomeScreen();
	//printOrder(4);
	customerMenu();
	//showOrders();
	//updatePoints("317450727", 123456);
	//printRevenue();
	return 0;
}