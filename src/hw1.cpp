//Marwa Saleh
//hw1.cpp
//Purpose:Create power and log functions.
#include <iostream>
using namespace std;

//
// Function:      power
// Purpose:       computes num to the exp power
// Parameters:  The number – num, and the exponent – exp
// Returns:       0 if exp is negative; 1 if exp is 0; raise num to the exp
// power otherwise.
int power ( int num, int exp) {
    if (exp < 0){
       return 0;
    }

    int value= 1;

    for(int i = 1; i <= exp; i++){
        value *= num;
    }

    return value;
}

//
// Function:      myLog
// Purpose:       computes log of num in base
// Parameters:    The number – num, the base – base, and the counter - count
// Returns:        log.
int myLog (int num, int base, int count) {
     num /= base;
        count ++;
    return num >= base ? myLog (num,base,count) : count;
}


int main() {
    int base;
    int exponent;
    int num;
    int value;

    cout << "Enter a base: ";
    cin >> base;

    cout << "Enter an exponent: ";
    cin >> exponent;

    value = power(base, exponent);
    cout << std::to_string( base ) + " to the " + std::to_string( exponent ) + " is " + std::to_string( value ) <<endl;

    cout << "Enter a base: ";
    cin >> base;

    cout << "Enter a number: ";
    cin >> num;

    if(base <= 1 || num <= 0){
        cout << "The base should be greater than 1 and the number value must be greater than 0"<<endl;
    } else {
        value = myLog (num,base,0);
        cout << "Log of " + std::to_string( num ) + " in base " + std::to_string( base ) + " is " + std::to_string( value ) <<endl;
    }

    return 0;
}

