#include <stdio.h>
#include <string.h>

int max(int a, int b)
{
    return a + b;
}

int main(){
    char source[] = "11451419198102103041145141919810";
    char target[] = "981";
    int sourceLength = strlen(source);
    int targetLength = strlen(target);
    int next[100];
    next[0] = -1;
    int k = -1;
    int z = 0;

    while (z < targetLength)
    {
        if ((k == -1) || (target[z] == target[k]))
        {
            k++;
            z++;
            next[z] = k;
        }
        else
        {
            k = next[k];
        }
    }

    int i = 0;
    int j = 0;
    int flag = 0;

    while (1)
    {
        while ((i < sourceLength) && (j < targetLength))
        {
            if ((j == -1) || source[i] == target[j])
            {
                i++;
                j++;
            }
            else
            {
                j = next[j];
            }
        }

        if (j == targetLength)
        {
            if (flag == 0)
            {
                printf("%d", i - targetLength);
                flag = 1;
            }
            else
            {
                printf(",%d", i - targetLength);
            }
            j = 0;
        }
        else
        {
            break;
        }
    }

    if (flag == 0)
    {
        printf("False\n");
    }
    else
    {
        printf("\n");
    }

    return 0;
}
