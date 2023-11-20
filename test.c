//204125774 zy12345 Yuhan Sun
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 100

// Function declarations
double evaluateExpression(const char *expression);
int getPrecedence(char operator);
double applyOp(double a, double b, char op);
bool isOperator(char c);
bool isNumber(char c);

// Helper function to apply an operator 'op' on operands 'a' and 'b'
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': 
            if ((b > 0 && a > DBL_MAX - b) || (b < 0 && a < -DBL_MAX - b)) return NAN; // Overflow and underflow check
            return a + b;
        case '-': 
            if ((b < 0 && a > DBL_MAX + b) || (b > 0 && a < -DBL_MAX + b)) return NAN; // Underflow and overflow check
            return a - b;
        case '*': 
            if (a != 0 && (b > DBL_MAX / fabs(a) || b < -DBL_MAX / fabs(a))) return NAN; // Overflow and underflow check
            return a * b;
        case '/': 
            if (b == 0) return NAN; // Division by zero check
            return a / b;
        case '%':
            if (b == 0) return NAN; // Modulus by zero error
            return fmod(a, b);
        default: 
            return NAN; // Unknown operator
    }
}

// Function to evaluate the expression
double evaluateExpression(const char *expression) {
    double values[MAX_INPUT_LENGTH] = {0};
    char ops[MAX_INPUT_LENGTH] = {0};
    int valuesTop = -1, opsTop = -1;
    int i;

    for (i = 0; i < strlen(expression); i++) {
        if (expression[i] == ' ') continue;

        if (expression[i] == '(') {
            ops[++opsTop] = expression[i];
        } else if (expression[i] == ')') {
            while (opsTop != -1 && ops[opsTop] != '(') {
                double val2 = values[valuesTop--];
                double val1 = values[valuesTop--];
                char op = ops[opsTop--];

                double result = applyOp(val1, val2, op);
                if (isnan(result)) return NAN; // Check for errors in operation
                values[++valuesTop] = result;
            }
            if (opsTop != -1) opsTop--; // Pop the '('
            else return NAN; // Unmatched parenthesis error
        } else if (isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && (i == 0 || expression[i-1] == '(' || isOperator(expression[i-1])))) {
            double val = 0;
            int sign = 1;
            if (expression[i] == '-') {
                sign = -1;
                i++;
            }
            int decimalFound = 0;
            double decimalFactor = 1.0;

            while (i < strlen(expression) && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    if (decimalFound) return NAN; // Multiple decimals not allowed
                    decimalFound = 1;
                } else {
                    val = val * 10.0 + (expression[i] - '0');
                    if (decimalFound) decimalFactor *= 10.0;
                }
                i++;
            }
            if (decimalFound) val /= decimalFactor;
            values[++valuesTop] = sign * val;
            i--;
        } else if (isOperator(expression[i])) {
            while (opsTop != -1 && getPrecedence(ops[opsTop]) >= getPrecedence(expression[i])) {
                double val2 = values[valuesTop--];
                double val1 = values[valuesTop--];
                char op = ops[opsTop--];

                double result = applyOp(val1, val2, op);
                if (isnan(result)) return NAN; // Check for errors in operation
                values[++valuesTop] = result;
            }
            ops[++opsTop] = expression[i];
        } else {
            return NAN; // Invalid character error
        }
    }

    while (opsTop != -1) {
        if (opsTop == 0 && ops[opsTop] == '(') return NAN; // Unmatched parenthesis error
        double val2 = values[valuesTop--];
        double val1 = values[valuesTop--];
        char op = ops[opsTop--];

        double result = applyOp(val1, val2, op);
        if (isnan(result)) return NAN; // Check for errors in operation
        values[++valuesTop] = result;
    }

    return values[valuesTop];
}

// Helper function to return precedence of operators
int getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return -1; // Not an operator
    }
}

// Function to check if the character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

// Function to check if the character is a number or a decimal point
bool isNumber(char c) {
    return isdigit(c) || c == '.';
}

int main() {
    char input[MAX_INPUT_LENGTH];
    printf("Enter an expression: ");
    fgets(input, MAX_INPUT_LENGTH, stdin);
    
    // Remove newline character if present
    size_t length = strlen(input);
    if (input[length - 1] == '\n') {
        input[length - 1] = '\0';
    }
    
    double result = evaluateExpression(input);
    if (isnan(result)) {
        printf("Invalid input or calculation error\n");
    } else {
        printf("Result: %f\n", result);
    }
    
    return 0;
}
