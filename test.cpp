#include <iostream>
#include <vector>

// int main() {
//     std::vector<int> numbers;
// 	numbers.push_back(1);
// 	numbers.push_back(2);
// 	numbers.push_back(3);
// 	numbers.push_back(4);
// 	numbers.push_back(5);


//     // Erase a single element
// 	std::cout << "B:: size: " << numbers.size() << std::endl;
//     numbers.erase(numbers.begin() + 1);
// 	std::cout << "A:: size: " << numbers.size() << std::endl;
//     // Erase a range of elements
//     // numbers.erase(numbers.begin() + 1, numbers.begin() + 3); // Erase elements from index 1 to 2 (exclusive)

//     // Print the resulting vector
// 	for(int i = 0; i < numbers.size(); i++)
// 		std::cout << numbers[i] << " ";
//     std::cout << std::endl;

// 	std::cout << "Capacity: " << numbers.capacity() << std::endl;

//     return 0;
// }

#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers;
	numbers.push_back(1);
	numbers.push_back(2);
	numbers.push_back(3);
	numbers.push_back(4);
	numbers.push_back(5);

    std::cout << "Size before shrink_to_fit(): " << numbers.size() << std::endl;
    std::cout << "Capacity before shrink_to_fit(): " << numbers.capacity() << std::endl;

    numbers.shrink_to_fit();

    std::cout << "Size after shrink_to_fit(): " << numbers.size() << std::endl;
    std::cout << "Capacity after shrink_to_fit(): " << numbers.capacity() << std::endl;

	std::cout << "add more elemets" << std::endl;
	for (int i = 0; i < 200; i++)
			numbers.push_back(i + 5);

	std::cout << "Size after adding more elements: " << numbers.size() << std::endl;
	std::cout << "Capacity after adding more elements: " << numbers.capacity() << std::endl;
	
	std::cout << "delete more elemets" << std::endl;
	for (int i = 0; i < 200; i++)
			numbers.erase( numbers.begin() + 5);
	
	std::cout << "Size after adding more elements: " << numbers.size() << std::endl;
	std::cout << "Capacity after adding more elements: " << numbers.capacity() << std::endl;

    return 0;
}
