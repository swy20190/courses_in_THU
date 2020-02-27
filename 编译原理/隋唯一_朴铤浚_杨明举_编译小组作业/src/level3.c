#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double Operate(double a, double b, char op)
{
	if (op == '+') {
		return a + b;
	}
	if (op == '-') {
		return a - b;
	}
	if (op == '*') {
		return a * b;
	}
	if (op == '/') {
		return a / b;
	}
	return 0;
}

double Calculate(char str[])
{
	double stack_operand[300];
	char stack_op[300];
	int p1 = -1;
	int p2 = 0;
	int n = strlen(str) + 1;
	double a;
	double b;
	double temp;
	char op;

	str[n - 1] = '#';
	stack_op[p2] = '#';

	for (int i = 0; i < n; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			temp = str[i] - '0';
			while (str[i + 1] >= '0' && str[i + 1] <= '9') {
				i++;
				temp = temp * 10 + str[i] - '0';
			}
			stack_operand[++p1] = temp;
		}
		else {
			if (str[i] == '(')
			{
				stack_op[++p2] = str[i];
			}
			else
				{
				if (str[i] == ')') {
					while (stack_op[p2] != '(') {
						b = stack_operand[p1--];
						a = stack_operand[p1--];
						op = stack_op[p2--];
						stack_operand[++p1] = Operate(a, b, op);
					}
					p2--;
				} else {
					while (str[i] == '#' || (stack_op[p2] != '#' && stack_op[p2] != '(' &&
											 (stack_op[p2] == '*' || stack_op[p2] == '/' || str[i] == '+' ||
											  str[i] == '-'))) {
						if (str[i] == '#' && stack_op[p2] == '#') {
							return stack_operand[p1];
						}
						b = stack_operand[p1--];
						a = stack_operand[p1--];
						op = stack_op[p2--];
						stack_operand[++p1] = Operate(a, b, op);
					}
					stack_op[++p2] = str[i];
				}

			}

		}
	}
	return stack_operand[p1];
}

int main()
{
	char str[100] = "1+(5-2)*4/(2+5)";
	printf("%f\n", Calculate(str));
	return 0;

}


