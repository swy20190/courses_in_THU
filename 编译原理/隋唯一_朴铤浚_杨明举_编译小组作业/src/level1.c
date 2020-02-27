#include <stdio.h>
#include <string.h>

int main() 
{
    char str[] = "12345678987654321";
    int length = strlen(str) - 1;
    int flag = 1;

    for (int i = 0; i < length; i++)
    {
        if (str[i] != str[length - i])
        {
            flag = 0;
            break;
        }
    }

    if (flag == 1)
    {
        printf("True\n");
    }
    else
    {
        printf("False\n");
    }

    return flag;
}
