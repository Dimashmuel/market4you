#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>






/// <summary>
/// CONSTS
/// </summary>
const char pathCustomers[] = "data/users/customers/";
const char pathManagers[] = "data/users/managers/";
const char pathUsers[] = "data/users/users/";
const char pathOrdersUnconfirmed[] = "data/users/orders/unconfirmed";
const char pathOrdersConfirmed[] = "data/users/orders/confirmed";
typedef enum { false, true } bool;

/// <summary>
/// STRING FUNCTIONS
/// </summary>
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

void overrideString(char** _str1, char* _str2) {
    char* str;
    str = malloc(sizeof(char) * strlen(_str2) + sizeof(char));
    if (str == NULL) exit(1);
    strcpy(str, _str2);
    free(*_str1);
    *_str1 = str;
}

void appendString(char** _str, char* _str2) {
    char* newStr = malloc((strlen(*_str) + strlen(_str2)) * sizeof(char) + sizeof(char));
    if (newStr == NULL) exit(1);
    strcpy(newStr, *_str);
    for (int i = 0; i < strlen(_str2); i++) {
        newStr[i + strlen(*_str)] = _str2[i];
    }
    newStr[strlen(*_str) + strlen(_str2)] = '\0';
    free(*_str);
    *_str = newStr;
}

void printStr(char* _str) {
    printf("%s", _str);
}

void printStrLine(char* _str) {
    printf("%s\n", _str);
}

/// <summary>
/// TEST FUNCTIONS
/// </summary>
void writeFile(char* filename, char* content) {

    FILE* file;
    file = fopen(filename, "ab+");
    if (!file) {
        printStrLine("File not found.");
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
}

void readFile(char* filename) {
    char name[30], pass[30], big_string[30];
    int age;

    FILE* file;
    file = fopen(filename, "r");
    if (!file) {
        printStrLine("File not found.");
        return;
    }
    fscanf(file, "%s %s %s %d", name, pass, big_string, &age);
    printf("%s %s %s %d", name, pass, big_string, &age);
}

bool doesFileExists(char* filename) {
    struct stat buffer;
    return !stat(filename, &buffer);

}

int main() {
    writeFile("data/users/test", "admin password firstname-lastname 16");
    readFile("data/users/test");
    return 0;
}