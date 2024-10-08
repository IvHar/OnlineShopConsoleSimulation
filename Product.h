#pragma once
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Product{
private:
	int id;
	string name;
	double price;
	int quantity;
public:
	Product(int id) { this->id = id; }
	Product(int id, string name, double price, int quantity) {
		this->id = id;
		this->name = name;
		this->price = price;
		this->quantity = quantity;
	}

	int getId() const { return id; }
	string getName() const{ return name; }
	double getPrice() const { return price; }
	double getQuantity() const { return quantity; }
	void setName(string name) { this->name = name; }
	void setPrice(int price) { this->price = price; }
	void setQuantity(int quantity) { this->quantity = quantity; }

	void showProductInfo() const{
		cout << left << setw(5)<< id
			<< setw(20) << name
			<< "$" <<setw(10)<< price
			<< setw(10) << quantity << endl;
	}
	bool operator<(const Product& other) const {
		return id < other.id;
	}

};

