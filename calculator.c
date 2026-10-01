#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define stack_size 20 
 
void push(int value, int *top, int s[]) { 
    if (*top == stack_size - 1) { 
        printf("Stack overflow\n"); 
        return; 
    } 
 
    (*top)++; 
    s[*top] = value; 
} 
 
int pop(int *top, int s[]) { 
    if (*top == -1) { 
        printf("Invalid expression\n"); 
        exit(1); 
    } 
 
    return s[(*top)--]; 
} 
 
int precedence(char symbol) { 
    switch (symbol) { 
        case '+': 
        case '-':return 1; 
        case '*': 
        case '/':return 2; 
        case '(':return 0; 
        default:return -1; 
    } 
} 
 
void infix_to_postfix(char infix[], char postfix[]) { 
    int st[stack_size]; 
    int top = -1; 
    int j = 0; 
    int n = strlen(infix); 
    for (int i = 0; i < n; i++) { 
        char symbol = infix[i]; 
        if (symbol >= '0' && symbol <= '9') { 
            while (i < n && infix[i] >= '0' && infix[i] <= '9') { 
                postfix[j++] = infix[i]; 
                i++; 
            } 
            postfix[j++] = ' '; 
            i--; 
        } 
        else if (symbol == '(') { 
            push(symbol, &top, st); 
        } 
        else if (symbol == ')') { 
            while (top != -1 && st[top] != '(') { 
                postfix[j++] = pop(&top, st); 
                postfix[j++] = ' '; 
            } 
            if (top == -1) { 
                printf("Invalid expression\n"); 
                exit(1); 
            } 
            pop(&top, st); 
        } 
 
        else if (symbol == '+' || symbol == '-' ||symbol == '*' || symbol == '/') { 
            while (top != -1 && st[top] != '(' && precedence(st[top]) >= precedence(symbol)) { 
                postfix[j++] = pop(&top, st); 
                postfix[j++] = ' '; 
            } 
            push(symbol, &top, st); 
        } 
        else { 
            printf("Invalid expression\n"); 
            exit(1); 
        } 
    } 
    while (top != -1) { 
        if (st[top] == '(') { 
            printf("Invalid expression\n"); 
            exit(1); 
        } 
        postfix[j++] = pop(&top, st); 
        postfix[j++] = ' '; 
    } 
    postfix[j] = '\0'; 
} 
 
int evaluate(char postfix[]) { 
    int s[stack_size]; 
    int top = -1; 
    int n = strlen(postfix); 
    for (int i = 0; i < n; i++) { 
        char symbol = postfix[i]; 
        if (symbol == ' ') { 
            continue; 
        } 
        if (symbol >= '0' && symbol <= '9') { 
            int num = 0; 
            while (i < n && postfix[i] >= '0' && postfix[i] <= '9') { 
                num = num * 10 + (postfix[i] - '0'); 
                i++; 
            } 
            push(num, &top, s); 
            i--;  
        } 
        else { 
            switch(symbol) { 
                case '+': { 
                    int b = pop(&top, s); 
                    int a = pop(&top, s); 
                    push(a + b, &top, s); 
                    break; 
                } 
                case '-': { 
                    int b = pop(&top, s); 
                    int a = pop(&top, s); 
                    push(a - b, &top, s); 
                    break; 
                } 
                case '*': { 
                    int b = pop(&top, s); 
                    int a = pop(&top, s); 
                    push(a * b, &top, s); 
                    break; 
                } 
                case '/': { 
                    int b = pop(&top, s); 
                    int a = pop(&top, s); 
                    if (b == 0) { 
                        printf("Error: Division by zero\n"); 
                        exit(1); 
                    } 
                    push(a / b, &top, s); 
                    break; 
                } 
                default: 
                    printf("Invalid expression\n"); 
                    exit(1); 
            } 
        } 
    } 
    if (top != 0) { 
        printf("Invalid expression\n"); 
        exit(1); 
    } 
    return pop(&top, s); 
} 
 
int main(void) { 
    char infix[100], postfix[100]; 
    printf("Enter an infix expression: "); 
    scanf("%s", infix); 
    infix_to_postfix(infix, postfix); 
    printf("Postfix expression: %s\n", postfix); 
    int result = evaluate(postfix); 
    printf("Result: %d\n", result); 
    return 0; 
}