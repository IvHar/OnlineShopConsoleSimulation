#pragma once
#include <iostream>
#include <map>
#include "Cart.h"
class User
{
private:
	string name;
	double balance;
	Cart cart;
public:
	User(string name, double balance) {
		this->name = name;
		this->balance = balance;
	}
	string getName() const { return name; }
	double getBalance() const { return balance; }
	Cart &getCart() { return cart; }
	void setBalance(double balance) {
		this->balance = balance;
	}

	void checkout(map<string, double> &promocodes, long long duration) {
		balance -= cart.total(promocodes, duration);
		cart.getProducts().clear();
	}
};

