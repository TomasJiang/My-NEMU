#include <stdio.h>
#include <string.h>

char str1[] = "Hello";
char str[20];


int main() {
    printf("%s", strcpy(str, str1));
    printf("%s", str);
    return 0;
}
