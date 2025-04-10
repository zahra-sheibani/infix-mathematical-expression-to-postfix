#include <iostream>
#include <vector>
#include <cctype>
using namespace std;

template <class A, int Max>
class Stack {
private:
    A items[Max];
    int top;

public:
    Stack() : top(-1) {}

    bool Is_Empty() {
        return top == -1;
    }

    A pop() {
        if (Is_Empty()) {
            cerr << "Error: Stack underflow!" << endl;
            exit(1);
        }
        return items[top--];
    }

    bool push(A x) {
        if (top == Max - 1) {
            cerr << "Error: Stack overflow!" << endl;
            return false;
        }
        items[++top] = x;
        return true;
    }

    A Stack_top() {
        if (Is_Empty()) {
            cerr << "Error: Stack is empty!" << endl;
            exit(1);
        }
        return items[top];
    }
};

// Converts a character digit to its numerical value
float value1(char c) {
    return c - '0';
}

// Handles variable x in expressions
float value2(char c, float x) {
    return (c == 'x') ? x : 0;
}

// Recursive power function (optimized)
float tav(float a, float b) {
    if (b == 0) return 1;
    if (static_cast<int>(b) % 2 == 0) {
        float half = tav(a, b / 2);
        return half * half;
    }
    return a * tav(a, b - 1);
}

// Evaluates binary operations
float eval(float f1, float f2, char x) {
    switch (x) {
        case '^': return tav(f1, f2);
        case '*': return f1 * f2;
        case '/': return f1 / f2;
        case '-': return f1 - f2;
        case '+': return f1 + f2;
        default: return 0;
    }
}

// Handles factorial and negation
float factornegetive(float x, char y) {
    if (y == '!') {
        float fact = 1;
        for (int i = 1; i <= x; i++) fact *= i;
        return fact;
    }
    if (y == '#') return -x;
    return x;
}

// Determines operator precedence
bool prcd(char x, char y) {
    if (y == '!') return false;
    if (x == '(' || y == '(') return false;
    if (x == '^') return y != '^';
    if (x == y) return true;
    if (x == '+' || x == '-') return y == '+' || y == '-';
    if (x == '*' || x == '/') return y != '^' && y != '!';
    return x == '!';
}

// Converts infix expression to postfix
string intopo(string infix) {
    char postfix[100];
    Stack<char, 100> stack1;
    int i = 0, j = 0;

    while (infix[i] != '\0') {
        if (isalpha(infix[i]) || isdigit(infix[i])) {
            postfix[j++] = infix[i];
        } else if (infix[i] == ')') {
            while (stack1.Stack_top() != '(') postfix[j++] = stack1.pop();
            stack1.pop();
        } else {
            while (!stack1.Is_Empty() && prcd(stack1.Stack_top(), infix[i])) {
                postfix[j++] = stack1.pop();
            }
            if (infix[i] == '-' && (i == 0 || infix[i - 1] == '(' || infix[i - 1] == '*' || infix[i - 1] == '^' || infix[i - 1] == '/')) {
                stack1.push('#');
            } else {
                stack1.push(infix[i]);
            }
        }
        i++;
    }

    while (!stack1.Is_Empty()) {
        postfix[j++] = stack1.pop();
    }
    postfix[j] = '\0';
    return string(postfix);
}

// Evaluates a postfix expression
float postfixcal(string postfix, float x) {
    int i = 0;
    Stack<float, 100> stack2;

    while (postfix[i] != '\0') {
        if (isalpha(postfix[i])) {
            stack2.push(value2(postfix[i], x));
        } else if (isdigit(postfix[i])) {
            stack2.push(value1(postfix[i]));
        } else if (postfix[i] == '#' || postfix[i] == '!') {
            float w = stack2.pop();
            stack2.push(factornegetive(w, postfix[i]));
        } else {
            float val2 = stack2.pop();
            float val1 = stack2.pop();
            stack2.push(eval(val1, val2, postfix[i]));
        }
        i++;
    }
    return stack2.pop();
}

// Main function for user input and execution
int main() {
    while (true) {
        cout << "Please enter range [a, b]: ";
        double a, b;
        cin >> a >> b;

        cout << "Enter the number of divisions (n): ";
        int n;
        cin >> n;

        cout << "Enter your function: ";
        string f;
        cin >> f;

        string w = intopo(f);
        cout << "The postfix form of your function is: " << w << endl;

        for (int i = 1; i <= n; i++) {
            float x = a + (i - 1) * (b - a) / (n - 1);
            cout << "f(" << x << ") = " << postfixcal(w, x) << endl;
        }
    }
}
