#include "my_forvard_list.h"

int main() {

	ForwardList<int> IntList1 = { 1,2,3,4 };

	IntList1.push_front(0);	// {0,1,2,3,4}

	IntList1.pop_front(); // {1,2,3,4}

	int x = 9;
	IntList1.push_front(x);	// {9,1,2,3,4}
	IntList1.push_front(16);	// {16,9,1,2,3,4}

	std::cout << IntList1.front() << std::endl;	// 16

	std::cout << "Iterating:\n";
	for (int n : IntList1) {
		std::cout << n << " ";
	}

	return 0;
}