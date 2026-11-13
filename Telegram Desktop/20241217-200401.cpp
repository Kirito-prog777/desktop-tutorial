#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class Item {
public:
    string name;
    double price;
    int quantity;

    Item(string n, double p, int q) : name(n), price(p), quantity(q) {}

    string toString() const {
        return name + "," + to_string(price) + "," + to_string(quantity);
    }

    static Item fromString(const string& str) {
        size_t firstComma = str.find(',');
        size_t secondComma = str.find(',', firstComma + 1);
        
        string name = str.substr(0, firstComma);
        double price = stod(str.substr(firstComma + 1, secondComma - firstComma - 1));
        int quantity = stoi(str.substr(secondComma + 1));
        
        return Item(name, price, quantity);
    }
};

class Store {
private:
    vector<Item> items;
    vector<string> reports;
    string storeName;

public:
    void loadStoreName() {
        ifstream file("store_name.txt");
        if (file.is_open()) {
            getline(file, storeName);
            file.close();
        }
    }

    void saveStoreName() {
        ofstream file("store_name.txt");
        if (file.is_open()) {
            file << storeName;
            file.close();
        }
    }

    void loadItems() {
        ifstream file("items.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                items.push_back(Item::fromString(line));
            }
            file.close();
        }
    }

    void saveItems() {
        ofstream file("items.txt");
        if (file.is_open()) {
            for (const auto& item : items) {
                file << item.toString() << endl;
            }
            file.close();
        }
    }

    void setStoreName(string name) {
        storeName = name;
        reports.push_back("Store name set to: " + name);
    }

    string getStoreName() {
        return storeName;
    }

    void addItem(string name, double price, int quantity) {
        items.push_back(Item(name, price, quantity));
        reports.push_back("Added product: " + name + ", Price: " + to_string(price) + ", Quantity: " + to_string(quantity));
    }

    void modifyItem(string itemName, double newPrice, int newQuantity) {
        for (auto& item : items) {
            if (item.name == itemName) {
                item.price = newPrice;
                item.quantity = newQuantity;
                reports.push_back("Modified product: " + itemName + ", New Price: " + to_string(newPrice) + ", New Quantity: " + to_string(newQuantity));
                cout << "Product modified successfully." << endl;
                return;
            }
        }
        cout << "Product not found." << endl;
    }

    void deleteItem(string itemName) {
        auto it = remove_if(items.begin(), items.end(), [&](Item& item) {
            return item.name == itemName;
        });

        if (it != items.end()) {
            reports.push_back("Deleted product: " + itemName);
            items.erase(it, items.end());
            cout << "Product deleted successfully." << endl;
        } else {
            cout << "Product not found." << endl;
        }
    }

    void displayItems() {
        if (items.empty()) {
            cout << "No items in the store." << endl;
            return;
        }
        for (int i = 0; i < items.size(); i++) {
            cout << "Item " << i + 1 << ": " << items[i].name 
                 << " - " << items[i].price << " USD - Quantity: " << items[i].quantity << endl;
        }
    }

    void displayReports() {
        if (reports.empty()) {
            cout << "No reports available." << endl;
            return;
        }
        cout << "Reports:\n";
        for (const auto& report : reports) {
            cout << "- " << report << endl;
        }
    }

    void purchaseItem(int itemIndex, int amount) {
        if (itemIndex >= 0 && itemIndex < items.size()) {
            if (amount <= items[itemIndex].quantity) {
                items[itemIndex].quantity -= amount;
                reports.push_back("Purchased " + to_string(amount) + " of " + items[itemIndex].name);
                cout << "Purchased " << amount << " of " << items[itemIndex].name << ". Remaining quantity: " << items[itemIndex].quantity << endl;
            } else {
                cout << "Insufficient quantity." << endl;
            }
        } else {
            cout << "Invalid item number." << endl;
        }
    }
};

int main() {
    Store store;
    store.loadStoreName();
    store.loadItems();

    string ownerPassword;

    while (true) {
        string storeName = store.getStoreName();
        cout << "\nWelcome to " << (storeName.empty() ? "the Store" : storeName) << "!\n";
        cout << "Main Menu:\n1. Settings\n2. Employee\n3. Reports\n4. Close the program\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 4) {
            store.saveStoreName();
            store.saveItems();
            break;
        }

        switch (choice) {
        case 1: { 
            if (ownerPassword.empty()) {
                cout << "Set a new owner password: ";
                getline(cin, ownerPassword);
                cout << "Password set successfully.\n";
            } else {
                string inputPassword;
                cout << "Enter owner password: ";
                getline(cin, inputPassword);
                if (inputPassword != ownerPassword) {
                    cout << "Incorrect password. Access denied." << endl;
                    break;
                }
            }

            while (true) {
                cout << "\nSettings Menu:\n1. Store Name\n2. Amendment\n3. Stock\n4. Back to Home Page\n";
                int settingsChoice;
                cout << "Enter your choice: ";
                cin >> settingsChoice;
                cin.ignore();

                if (settingsChoice == 4) break;

                switch (settingsChoice) {
                case 1: { 
                    cout << "\nCurrent Store Name: " << (store.getStoreName().empty() ? "Not Set" : store.getStoreName()) << endl;
                    cout << "Do you want to update the store name? (yes/no): ";
                    string response;
                    getline(cin, response);

                    if (response == "yes" || response == "y") {
                        string newName;
                        cout << "Enter new store name: ";
                        getline(cin, newName);
                        store.setStoreName(newName);
                        cout << "Store name updated successfully to: " << newName << endl;
                    } else {
                        cout << "Store name remains unchanged." << endl;
                    }
                    break;
                }
                case 2: { 
                    while (true) {
                        cout << "\nAmendment Menu:\n1. Add Product\n2. Modify Products\n3. Delete Product\n4. Back\n";
                        int amendChoice;
                        cout << "Enter your choice: ";
                        cin >> amendChoice;
                        cin.ignore();

                        if (amendChoice == 4) break;

                        switch (amendChoice) {
                        case 1: {
                            string itemName;
                            double itemPrice;
                            int itemQuantity;

                            cout << "Enter product name: ";
                            getline(cin, itemName);
                            cout << "Enter product price: ";
                            cin >> itemPrice;
                            cout << "Enter product quantity: ";
                            cin >> itemQuantity;
                            cin.ignore();

                            store.addItem(itemName, itemPrice, itemQuantity);
                            cout << "Product added successfully." << endl;
                            break;
                        }
                        case 2: {
                            string itemName;
                            double newPrice;
                            int newQuantity;

                            cout << "Enter product name to modify: ";
                            getline(cin, itemName);
                            cout << "Enter new price: ";
                            cin >> newPrice;
                            cout << "Enter new quantity: ";
                            cin >> newQuantity;
                            cin.ignore();

                            store.modifyItem(itemName, newPrice, newQuantity);
                            break;
                        }
                        case 3: {
                            string itemName;
                            cout << "Enter product name to delete: ";
                            getline(cin, itemName);

                            store.deleteItem(itemName);
                            break;
                        }
                        default:
                            cout << "Invalid choice. Please try again." << endl;
                        }
                    }
                    break;
                }
                case 3: {
                    store.displayItems();
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
            break;
        }
        case 2: { 
            while (true) {
                cout << "\nEmployee Menu:\n1. Purchase Product\n2. Back\n";
                int empChoice;
                cout << "Enter your choice: ";
                cin >> empChoice;

                if (empChoice == 2) break;

                switch (empChoice) {
                case 1: {
                    store.displayItems();
                    int productIndex, amount;

                    cout << "Enter product number: ";
                    cin >> productIndex;
                    cout << "Enter quantity to purchase: ";
                    cin >> amount;

                    store.purchaseItem(productIndex - 1, amount);
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
            break;
        }
        case 3: { 
            store.displayReports();
            break;
        }
        default:
            cout << "Invalidchoice. Please try again." << endl; } }

return 0;

}
    