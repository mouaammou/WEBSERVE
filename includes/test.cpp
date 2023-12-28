#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base class constructor called" << std::endl;
    }
	Base(int value) {
        std::cout << "Base 2   : " << value << std::endl;
    }
};

class Subclass : public Base {
public:
    Subclass(): Base() {
        std::cout << "Subclass constructor called" << std::endl;
    }
};

int main() {
    Subclass obj;  // Creating an instance of the Subclass
    return 0;
}