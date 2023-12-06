#include <iostream>

class ShallowCopyExample {
	public:
		ShallowCopyExample(int val) : data(new int(val)) {}
		
		// Copy constructor performs a shallow copy
		ShallowCopyExample(const ShallowCopyExample& other) : data(other.data) {}

		void display() {
			std::cout << "Data: " << *data << std::endl;
		}

	public:
		int* data;
};

int main() {
    ShallowCopyExample original(42);
    ShallowCopyExample copy(original);

    original.display();  // Output: Data: 42
    copy.display();      // Output: Data: 42

    // Modifying the original affects the copy
    *original.data = 99;

    original.display();  // Output: Data: 99
    copy.display();      // Output: Data: 99

    return 0;
}
