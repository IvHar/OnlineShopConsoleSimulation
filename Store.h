#pragma once
#include "Product.h"
#include "User.h"
#include "Store.h"
#include "Cart.h"
#include <set>
#include <map>
#include <chrono>
#include "validate_input.h"
using namespace std;

class Store{
private:
	set<Product> products;
	set<Product>bogoProducts;
	map<string, double> promocodes;
	double discountRate;
public:
	Store() {
		products.insert(Product(1, "Laptop", 2000, 10));
		products.insert(Product(2, "Smartphone", 1000, 25));
		products.insert(Product(3, "Tablet", 800, 15));
		products.insert(Product(4, "Smartwatch", 500, 20));
		products.insert(Product(5, "Headphones", 100, 50));
		products.insert(Product(6, "Charger", 20, 50));
		products.insert(Product(7, "Keyboard", 100, 40));
		products.insert(Product(8, "Mouse", 75, 60));
		products.insert(Product(9, "Monitor", 300, 25));
		products.insert(Product(10, "Webcam", 90, 10));
		products.insert(Product(11, "CPU", 400, 15));
		products.insert(Product(12, "GPU", 1500, 15));
		products.insert(Product(13, "Motherboard", 250, 15));
		products.insert(Product(14, "SSD", 300, 15));
		products.insert(Product(15, "RAM", 250, 15));

		bogoProducts = { Product(1), Product(4), Product(7), Product(11), Product(14) };

		promocodes["HUI"] = 0.1;
		promocodes["CV123"] = 0.15;
		promocodes["QWERTY13"] = 0.20;
		promocodes["IFELSE"] = 0.25;
		promocodes["BONUS22"] = 0.5;

		discountRate = 0.10;
		applyDiscounts();
	}

	set<Product>& getProducts() { return products; }

	void showProducts() {
		for (const auto& product : products) {
			product.showProductInfo();
		}
		cout<<endl;
	}

	void addProductToCart(User& user, int productId) {
		int count;
		for (auto i = products.begin(); i != products.end(); i++) {
			if (i->getId() == productId) {
				cout << "Enter how many products you want to add in cart: ";
				validated_input(count);
				if (count > i->getQuantity()) {
					system("cls");
					cout << "There are not so many products available" << endl;
					return;
				}
				user.getCart().addProduct(*i, count);
				Product updatedProduct = *i;
				updatedProduct.setQuantity(updatedProduct.getQuantity() - count);
				products.erase(i);
				products.insert(updatedProduct);
				system("cls");

				break;
			}
		}
	}

	//1. Discount
	void applyDiscounts() {
		set<Product> updatedProducts;
		for (auto& product : products) {
			if (product.getId() % 3 == 0) {
				Product updatedProduct = product;
				updatedProduct.setName(updatedProduct.getName() + " (" + to_string((int)(discountRate * 100)) + "%)");
				updatedProduct.setPrice(updatedProduct.getPrice() * (1 - discountRate));
				updatedProducts.insert(updatedProduct);
			}
			else {
				updatedProducts.insert(product);
			}
		}
		products = updatedProducts;
	}

	//2. bogo
	void applyBogo(User& user) {
		const auto& cartProducts = user.getCart().getProducts();
		for (const auto& cartProduct : cartProducts) {
			if (bogoProducts.count(cartProduct)) {
				char response;
				cout << "Would you like to buy another " << cartProduct.getName()
					<< " at 50% discount? (y/n): ";
				cin >> response;
				if (response == 'y' || response == 'Y') {
					auto productIt = find_if(products.begin(), products.end(), [&](const Product& p) {
						return p.getId() == cartProduct.getId();
						});
					if (productIt != products.end() && productIt->getQuantity() > 0) {
						Product updatedProduct = *productIt;
						double discountedPrice = updatedProduct.getPrice() * 0.5;

						if (user.getBalance() >= discountedPrice) {
							user.setBalance(user.getBalance() - discountedPrice);
							updatedProduct.setQuantity(updatedProduct.getQuantity() - 1);

							products.erase(productIt);
							products.insert(updatedProduct);

							cout << "You have successfully bought another " << updatedProduct.getName()
								<< " for $" << discountedPrice << "!" << endl;
						}
						else cout << "Insufficient balance for the discounted product." << endl;
					}
					else cout << "Product not available." << endl;
				}
			}
		}
	}

	static void menu() {
		Store store;
		string username;
		cout << "Enter name: "; cin >> username;
		User user(username, 25000);
		int choice, id;
		auto start = chrono::steady_clock::now();
		while (true) {
			cout << user.getName() << ", your balance: " << user.getBalance()<<endl;
			cout << "Catalog:\n";
			cout << left << setw(5) << "Id"
				<< setw(20) << "Product"
				<< setw(10) << "Price"
				<< setw(10) << "Quantity"<< endl;
			store.showProducts();
			cout << "1. Add product to cart\n2. View cart\n3. Exit\n";
			cout << "Choose option: "; validated_input(choice);
			auto now = chrono::steady_clock::now();
			auto duration = chrono::duration_cast<chrono::seconds>(now - start).count();
			switch (choice){
			case 1:
				cout << "\nEnter product id: "; validated_input(id);
				store.addProductToCart(user, id);
				break;
			case 2:
				cout << endl;
				if (user.getCart().getProducts().empty()) {
					system("cls");
					cout << "The cart is empty" << endl;
					break;
				}
				user.getCart().showCart();
				cout << "\n1.Checkout\n2.Remove from cart\n3.Return\n"; cin>>choice;
				switch (choice){
				case 1:
					store.applyBogo(user);
					user.checkout(store.promocodes, duration);
					break;
				case 2:
					cout << "Enter product's id: "; validated_input(id);
					user.getCart().removeProduct(store.products, id);
					break;
				default:
					system("cls");
					break; 
				}
				break;
			default:
				exit(0);
			}
		}
	}
};

