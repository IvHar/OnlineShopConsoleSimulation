#pragma once
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
#include <chrono>
#include "Product.h"
using namespace std;

class Cart {
private:
	set<Product> products;
	set<Product> freeProducts;
	set<Product>timerDiscountProducts;
	double deliveryPrice = 50;
public:
	Cart() {
		timerDiscountProducts = { Product(15), Product(1), Product(2), Product(14), Product(9) };
	}
	set<Product> &getProducts() { return products; }

	void addProduct(Product product, int count, double discount = 0.0) {
		auto i = products.find(product);
		double price = product.getPrice();
		if (discount > 0.0) {
			price *= (1 - discount);
		}
		if (i != products.end()) {
			Product updatedProduct = *i;
			updatedProduct.setQuantity(updatedProduct.getQuantity() + count);
			updatedProduct.setPrice(price);
			products.erase(i);
			products.insert(updatedProduct);
		}
		else {
			product.setQuantity(count);
			product.setPrice(price);
			products.insert(product);
		}
		
	}
	void removeProduct(set<Product> &prods, int id) {
		for (auto i = products.begin(); i != products.end(); i++) {
			if (i->getId() == id) {
				int quantity = i->getQuantity();
				products.erase(i);

				for (auto& product : prods) {
					if (product.getId() == id) {
						Product updatedProduct = product;
						updatedProduct.setQuantity(updatedProduct.getQuantity() + quantity);
						prods.erase(product);
						prods.insert(updatedProduct);
						system("cls");
						return;
					}
				}

			}
		}
		system("cls");
		cout << "There's no such product in your cart" << endl;
	}

	//5. Free delivery
	void countDeliveryPrice(double& total) {
		if (total <= 1000) {
			total += deliveryPrice;
			cout << "Delivery price: " << deliveryPrice << endl;
		}
	}
	
	//4. Promocodes
	void applyPromocode(map<string, double> &promocodes, double& total) {
		string promocode;
		cout << "Enter a promocode? ('n' for 'no')"; cin >> promocode;
		auto i = promocodes.find(promocode);
		if (i != promocodes.end()) {
			int productId;
			cout << "Select product from your cart: "; cin >> productId;
			for (auto& cartProduct : products) {
				if (cartProduct.getId() == productId) {
					total -= cartProduct.getPrice() * i->second;
					cout << "You have successfully applied promocode for" << cartProduct.getName()<<endl;
					return;
				}
				cout << "Product not found" << endl;
			}
		}
		else {
			cout << "Invalid promocode!" << endl;
		}
	}

	void getDiscountByDuration(long long duration) {
		if (duration <= 60) {
			set<Product> updatedProducts;
			for (auto& product : products) {
				if (timerDiscountProducts.count(product)) {
					Product discountedProduct = product;
					double discountRate = 0.2;
					discountedProduct.setPrice(discountedProduct.getPrice() * (1 - discountRate));
					updatedProducts.insert(discountedProduct);
				}
				else
					updatedProducts.insert(product);
			}
			products = updatedProducts;
			cout << "Discount applied for the first minute " << duration<< endl;
		}
	}

	double total(map<string, double> &promocodes, long long duration) {
		getDiscountByDuration(duration);
		double total = 0;
		for (auto& product : products) {
			total += product.getPrice() * product.getQuantity();
		}
		applyPromocode(promocodes, total);
		countDeliveryPrice(total);
		return total;
	}
	double total() {
		double total = 0;
		for (auto& product : products) {
			total += product.getPrice() * product.getQuantity();
		}
		countDeliveryPrice(total);
		return total;
	}


	void showCart() {
		cout << left << setw(5) << "id"
			<< setw(20) << "Product"
			<< setw(10) << "Price"
			<< setw(10) << "Quantity" << endl;
		for (auto& product : products) {
			product.showProductInfo();
		}
		cout << "Total: " << this->total()<<"$"<<endl;
	}
};

