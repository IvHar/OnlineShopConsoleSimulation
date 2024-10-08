#include "validate_input.h"
#include <iostream>
using namespace std;

void validated_input(int &number) {
	while(!(cin>>number)){
		cout << "Invalid input!!!" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}