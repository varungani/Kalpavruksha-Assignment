#include <stdio.h>
#include <ctype.h>
#include<string.h>

int main() {
    char expr[100];
    printf("Enter expression: ");
    fgets(expr, 100, stdin);

    int i = 0, num = 0, result = 0, temp = 0;
    char op = '+', prevOp = '+';

    while (expr[i] != '\0' && expr[i] != '\n') {
        if (expr[i] == ' ') { 
            i++; continue; 
            
        } 

        if (isdigit(expr[i])) {
            num = 0;
            while (isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }

            if (op == '*'){
                temp *= num;
            } 
            else if (op == '/') {
                if (num == 0) {
                    printf("Error: Division by zero.\n");
                    return 0; 
                    
                }
                temp /= num;
            }
            else if (op == '+') {
                result += temp; temp = num; 
            }
            else if (op == '-') { 
                result += temp; temp = -num; 
            }
            continue;
        }
        else if (expr[i]=='+'||expr[i]=='-'||expr[i]=='*'||expr[i]=='/'){
            op = expr[i];
        }
        else {
            printf("Error: Invalid expression.\n");
            return 0;
        }
        i++;
    }
    result += temp;
    printf("%d\n", result);
    return 0;
}
