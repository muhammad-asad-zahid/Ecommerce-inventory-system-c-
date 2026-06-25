#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string filePath = "D:\\uni lectures\\semester 2\\oops\\EcommerceStore\\product.txt";
const string tempPath = "D:\\uni lectures\\semester 2\\oops\\EcommerceStore\\temp.txt";

class Product
{
private:
    int ProductID;
    string ProductName;
    string ProductCategory;
    double ProductPrice;
    int ProductQuantity;

public:
    Product() : ProductID(0), ProductName(""), ProductCategory(""), ProductPrice(0.0), ProductQuantity(0) {}

    Product(int id, string name, string category, double price, int stock) : ProductID(id), ProductName(name), ProductCategory(category), ProductPrice(price), ProductQuantity(stock) {}

    int getProductId() const { return ProductID; }
    string getProductName() const { return ProductName; }
    string getProductCategory() const { return ProductCategory; }
    double getProductPrice() const { return ProductPrice; }
    int getProductQuantity() const { return ProductQuantity; }

    void displayProduct() const
    {
        
        cout << left
             << " | " << setw(6) << ProductID
             << " | " << setw(22) << ProductName
             << " | " << setw(16) << ProductCategory
             << " | pkr" << setw(9) << fixed << setprecision(2) << ProductPrice
             << " | " << setw(8) << ProductQuantity << " |" << endl;
    }
};

void loadProductsFromFile()
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cout << "Database file not found or empty" << endl;
        return;
    }
    cout << "\n--- Loading Products From File Database ---" << endl;

    string idStr, name, category, priceStr, qtyStr;
    while (getline(file, idStr, ',') &&
           getline(file, name, ',') &&
           getline(file, category, ',') &&
           getline(file, priceStr, ',') &&
           getline(file, qtyStr, '\n'))
    {
        int id = stoi(idStr);
        double price = stod(priceStr);
        int qty = stoi(qtyStr);

        Product loadedProduct(id, name, category, price, qty);
        cout << " Record #" << " - ";
        loadedProduct.displayProduct();
    }
    file.close();
    cout << "====================================================\n"
         << endl;
}

class Admin
{
private:
    string adminName;
    string adminPassword;

public:
    Admin(string name, string password) : adminName(name), adminPassword(password) {}

    void addProductToFile(int id, string name, string category, double price, int stock)
    {
        cout << "\n[ADMIN ACTION] Admin '" << adminName
             << "' is attempting to add product: " << name << " (ID: " << id << ")..." << endl;

        ifstream checkfile(filePath);
        if (checkfile.is_open())
        {
            string idStr, existingName, existingCategory, priceStr, qtyStr;

            while (getline(checkfile, idStr, ',') &&
                   getline(checkfile, existingName, ',') &&
                   getline(checkfile, existingCategory, ',') &&
                   getline(checkfile, priceStr, ',') &&
                   getline(checkfile, qtyStr, '\n'))
            {
                int existingId = stoi(idStr);

                if (existingId == id || existingName == name)
                {
                    cout << "[DUPLICATION ERROR] Admin action rejected! A product with ID ["
                         << id << "] or Name [" << name << "] already exists in records." << endl;

                    checkfile.close();
                    return;
                }
            }
            checkfile.close();
            cout << "[ADMIN LOG] Validation passed. No database conflicts found." << endl;
        }

        Product newProduct(id, name, category, price, stock);

        ofstream file(filePath, ios::app);
        if (file.is_open())
        {
            file << newProduct.getProductId() << ","
                 << newProduct.getProductName() << ","
                 << newProduct.getProductCategory() << ","
                 << newProduct.getProductPrice() << ","
                 << newProduct.getProductQuantity() << "\n";

            file.close();
            cout << "[SUCCESS] Admin '" << adminName << "' successfully saved \""
                 << name << "\" to the master database file!" << endl;
        }
        else
        {
            cout << "[CRITICAL ERROR] Admin could not access filesystem path to save data." << endl;
        }
    }

    void deleteProduct(int id)
    {
        cout << "\n[ADMIN] Requesting DELETION of Product ID [" << id << "]..." << endl;

        ifstream file(filePath);

        ofstream tempFile(tempPath);

        if (!file.is_open() || !tempFile.is_open())
        {
            cout << "[SYSTEM ERROR] File access problems encountered." << endl;
            return;
        }

        string idStr, name, category, priceStr, qtyStr;
        bool productFound = false;

        while (getline(file, idStr, ',') &&
               getline(file, name, ',') &&
               getline(file, category, ',') &&
               getline(file, priceStr, ',') &&
               getline(file, qtyStr, '\n'))
        {

            if (stoi(idStr) == id)
            {
                cout << "[FILTER MATCH] Skipping/Deleting item from data stream: " << name << endl;
                productFound = true;
            }
            else
            {

                tempFile << idStr << "," << name << "," << category << "," << priceStr << "," << qtyStr << "\n";
            }
        }

        file.close();
        tempFile.close();

        if (productFound)
        {
            remove(filePath.c_str());
            rename(tempPath.c_str(), filePath.c_str());
            cout << "[ADMIN SUCCESS] Product permanently removed from storage disk." << endl;
        }
        else
        {
            remove(tempPath.c_str());
            cout << "[ADMIN ERROR] Product ID not found in database records." << endl;
        }
    }
    void editProduct(int id, string newName, string newCategory, double newPrice, int newStock)
    {
        ifstream productFile(filePath);
        ofstream tempFile(tempPath);

        if (!productFile.is_open() || !tempFile.is_open())
        {
            cout << "file access probelem" << endl;
            return;
        }
        string idStr, name, category, priceStr, qtyStr;
        bool productFound = false;

        while (getline(productFile, idStr, ',') &&
               getline(productFile, name, ',') &&
               getline(productFile, category, ',') &&
               getline(productFile, priceStr, ',') &&
               getline(productFile, qtyStr, '\n'))
        {
            if (stoi(idStr) == id)
            {
                cout << "Product found for edit";
                tempFile << id << "," << newName << "," << newCategory << "," << newPrice << "," << newStock << "\n";
                productFound = true;
            }
            else
            {
                tempFile << idStr << "," << name << "," << category << "," << priceStr << "," << qtyStr << "\n";
            }
        }

        productFile.close();
        tempFile.close();

        if (productFound)
        {
            remove(filePath.c_str());
            rename(tempPath.c_str(), filePath.c_str());
            cout << "PRODUCT UPDATE" << endl;
        }
        else
        {
            remove(tempPath.c_str());
            cout << "product cant find" << endl;
        }
    }
    void viewAllProducts() { loadProductsFromFile(); }
};

class cartItem
{
private:
    Product product;
    int OrderQuantity;

public:
    cartItem() : product(), OrderQuantity(0) {}
    cartItem(Product p, int qty) : product(p), OrderQuantity(qty) {}

    Product getProduct() const { return product; }
    int getQuantity() const { return OrderQuantity; }

    void addQuantity(int q)
    {
        cout << "[CART ITEM] Incrementing quantity for " << product.getProductName()
             << " by +" << q << " (Previous: " << OrderQuantity << ")" << endl;
        OrderQuantity += q;
    }

    double CalculateTotal() const
    {
        return product.getProductPrice() * OrderQuantity;
    }
};

class Customer
{
private:
    string UserName;
    string Userpassword;
    vector<cartItem> cart;

public:
    Customer() : UserName(""), Userpassword("") {};
    Customer(string cName, string cpassword) : UserName(cName), Userpassword(cpassword)
    {
        cout << "[USER SYSTEM] Customer instance created for user: '" << UserName << "'" << endl;
    };

    string getUsername() const { return UserName; }
    string getUserPassword() const { return Userpassword; }

    void addTocart(int id, int qty)
    {
        cout << "\n[CART ACTION] User '" << UserName << "' wants to add Product ID: " << id << " (Qty: " << qty << ")" << endl;

        ifstream fileforcart(filePath);
        if (!fileforcart.is_open())
        {
            cout << "[CRITICAL ERROR] Could not access inventory database." << endl;
            return;
        }

        string idstr, name, category, priceStr, qtyStr;
        bool productFound = false;
        int availableStock = 0;
        Product foundProduct;

        while (getline(fileforcart, idstr, ',') &&
               getline(fileforcart, name, ',') &&
               getline(fileforcart, category, ',') &&
               getline(fileforcart, priceStr, ',') &&
               getline(fileforcart, qtyStr, '\n'))
        {
            if (stoi(idstr) == id)
            {
                productFound = true;
                availableStock = stoi(qtyStr);
            
                foundProduct = Product(stoi(idstr), name, category, stod(priceStr), availableStock);
                break;
            }
        }
        fileforcart.close();

        if (!productFound)
        {
            cout << "[CART FAILURE] Aborting operation. Product ID " << id << " does not exist in backend inventory files." << endl;
            return;
        }

        int currentQtyInCart = 0;
        int cartIndex = -1;

        for (size_t i = 0; i < cart.size(); i++)
        {
            if (cart[i].getProduct().getProductId() == id)
            {
                currentQtyInCart = cart[i].getQuantity();
                cartIndex = i;
                break;
            }
        }

        int totalRequestedQty = currentQtyInCart + qty;

        if (totalRequestedQty > availableStock)
        {
            int allowedToAdd = availableStock - currentQtyInCart;

            cout << "\n| --------------------------------------------------------------- |\n";
            cout << "|  [!] STOCK LIMIT REACHED                                   |\n";
            cout << "| --------------------------------------------------------------- |\n";
            
            if (currentQtyInCart > 0) {
                cout << "│  You already have " << left << setw(3) << currentQtyInCart << " of this item in your cart.          │\n";
                cout << "│  We only have " << left << setw(3) << availableStock << " total in stock.                        │\n";
                cout << "│                                                            │\n";
                cout << "│  -> You can only add " << left << setw(3) << allowedToAdd << " more to your cart.               │\n";
            } else {
                cout << "│  We only have " << left << setw(3) << availableStock << " of this item in stock.                 │\n";
                cout << "│  Please enter a quantity of " << left << setw(3) << availableStock << " or less.                  │\n";
            }
            
            cout << "| --------------------------------------------------------------- |\n\n";
            
            return;
           
        }

        if (cartIndex != -1)
        {

            cart[cartIndex].addQuantity(qty);
            cout << "[CART SUCCESS] Updated matching item in cart. New Total Quantity: " << cart[cartIndex].getQuantity() << endl;
        }
        else
        {

            cartItem newItem(foundProduct, qty);
            cart.push_back(newItem);
            cout << "[CART SUCCESS] Initialized allocation of \"" << name << "\" [x" << qty << "] into vector." << endl;
        }
    }

    void showcart() const
    {

        cout << "\n| --------------------------------------------------------------- |\n";

        string title = "SHOPPING CART DISPLAY FOR USER: " + UserName;
        int totalWidth = 63;
        int padding = (totalWidth - title.length()) / 2;
        if (padding < 0)
            padding = 0;

        cout << "│" << string(padding, ' ') << title << string(totalWidth - title.length() - padding, ' ') << "│\n";
        cout << "| --------------------------------------------------------------- |\n";

        if (cart.empty())
        {
            cout << "|                                                                 |\n";
            cout << "|               [!] Your cart is currently EMPTY.                 |\n";
            cout << "|                                                                 |\n";
            cout << "| --------------------------------------------------------------- |\n\n";
            return;
        }

        cout << "| " << left << setw(24) << "Product Name"
             << "| " << setw(12) << "Unit Price"
             << "| " << setw(10) << "Quantity"
             << "| " << setw(11) << "Line Total" << "│\n";

        cout << "| --------------------------------------------------------------- |\n";

        double grandTotal = 0.0;

        for (const auto &item : cart)
        {
            double lineTotal = item.CalculateTotal();
            grandTotal += lineTotal;

            cout << "| " << left << setw(23) << item.getProduct().getProductName()
                 << "| pkr " << right << setw(9) << fixed << setprecision(2) << item.getProduct().getProductPrice() << " "
                 << "| " << right << setw(8) << item.getQuantity() << " "
                 << "| pkr " << right << setw(10) << lineTotal << " │\n";
        }

        cout << "| --------------------------------------------------------------- |\n";
        cout << "| " << left << setw(48) << "GRAND TOTAL:"
             << "| pkr" << right << setw(10) << grandTotal << " │\n";

        cout << "| --------------------------------------------------------------- |\n\n";
    }
    void deleteItemFromCart(int id)
    {
        cout << "\n[CART REMOVE] Request to remove Product ID [" << id << "] from cart..." << endl;

        for (size_t i = 0; i < cart.size(); i++)
        {

            if (cart[i].getProduct().getProductId() == id)
            {
                cout << "[MATCH FOUND] Erasing record match: " << cart[i].getProduct().getProductName() << endl;

                cart.erase(cart.begin() + i);

                cout << "[SUCCESS] Item dropped from active user session." << endl;
                return;
            }
        }

        cout << "[REMOVE FAILED] Product ID " << id << " was not inside this customer's active cart." << endl;
    }

    void checkout()
    {
        cout << "\n===================================================================\n";
        cout << "                       SECURE CHECKOUT HUB                         \n";
        cout << "===================================================================\n";

        if (cart.empty())
        {
            cout << "[!] Checkout aborted: Your cart is currently empty.\n\n";
            return;
        }

        // Show the cart one last time before confirming
        showcart();

        char confirmCheckout;
        cout << "\n[?] Do you want to confirm this purchase and process payment? (Y/N): ";
        cin >> confirmCheckout;

        if (confirmCheckout == 'Y' || confirmCheckout == 'y')
        {
            cout << "\n[SYSTEM] Processing transaction and updating warehouse inventory...\n";

            // 1. Open current database and a temporary file
            ifstream inFile(filePath);
            ofstream tempFile(tempPath);

            if (!inFile.is_open() || !tempFile.is_open())
            {
                cout << "[CRITICAL ERROR] Database access failed during checkout. Transaction cancelled.\n";
                return;
            }

            string idStr, name, category, priceStr, qtyStr;

            // 2. Read through the master database line by line
            while (getline(inFile, idStr, ',') &&
                   getline(inFile, name, ',') &&
                   getline(inFile, category, ',') &&
                   getline(inFile, priceStr, ',') &&
                   getline(inFile, qtyStr, '\n'))
            {
                int currentId = stoi(idStr);
                int currentStock = stoi(qtyStr);

                // 3. Check if this specific product is inside the user's cart
                for (const auto &item : cart)
                {
                    if (item.getProduct().getProductId() == currentId)
                    {
                        // Deduct the purchased quantity from the warehouse stock
                        currentStock -= item.getQuantity(); 
                        
                        // Failsafe: Ensure stock never goes below 0
                        if (currentStock < 0) currentStock = 0; 
                        break; // Stop looping the cart once the item is found
                    }
                }

                // 4. Write the (possibly updated) product back to the temporary file
                tempFile << currentId << "," << name << "," << category << ","
                         << priceStr << "," << currentStock << "\n";
            }

            inFile.close();
            tempFile.close();

            // 5. Replace the old database with the new updated one
            remove(filePath.c_str());
            rename(tempPath.c_str(), filePath.c_str());

            // 6. Memory cleanup: Safely empty the cart vector
            cart.clear();

            cout << "| --------------------------------------------------------------- |\n";
            cout << "│  [TRANSACTION SUCCESS] Order captured! Funds processed.         │\n";
            cout << "│  [INVENTORY UPDATED] Stock deducted from master database.       │\n";
            cout << "│  Your virtual cart is now empty. Returning to main menu...      │\n";
            cout << "| --------------------------------------------------------------- |\n\n";
        }
        else
        {
            cout << "\n[REJECTED] Transaction suspended. Your cart items have been saved.\n\n";
        }
    }
};

int main()
{
    int mainChoice = 0;

    while (true)
    {
        cout << "==================================================\n";
        cout << "          WELCOME TO THE Laptop STORE         \n";
        cout << "==================================================\n";
        cout << " [1] Admin Portal (Management)\n";
        cout << " [2] Customer Portal (Shopping)\n";
        cout << " [3] Exit Store Application\n";
        cout << "==================================================\n";
        cout << "Enter Your Option: ";

        if (!(cin >> mainChoice))
        {
            cout << "\n[!] Error: Invalid numeric input. Reloading portal...\n\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (mainChoice == 3)
        {
            cout << "\n Thank you for using our application!\n";
            break;
        }

        // ==================== 1. REAL ADMIN PORTAL ====================
        if (mainChoice == 1)
        {
            string inputUser, inputPass;
            cout << "\n--- SECURE ADMIN SIGN-IN ---\n";
            cout << "Enter Admin Username: ";
            cin >> inputUser;
            cout << "Enter Admin Password: ";
            cin >> inputPass;

            // Strict production credentials verification
            if (inputUser == "admin" && inputPass == "123")
            {
                Admin activeAdmin(inputUser, inputPass);
                cout << "\n[SUCCESS] Access Granted. Welcome back, system manager.\n";

                int adminChoice = 0;
                while (true)
                {
                    cout << "\n=========================================\n";
                    cout << "          ADMIN MANAGEMENT PANEL         \n";
                    cout << "=========================================\n";
                    cout << " [1] View All Product Catalog\n";
                    cout << " [2] Add New Product Entry\n";
                    cout << " [3] Edit Existing Product File\n";
                    cout << " [4] Delete Product Permanently\n";
                    cout << " [5] Secure Log Out \n";
                    cout << "-----------------------------------------\n";
                    cout << "Select operations code: ";
                    if (!(cin >> adminChoice))
                    {
                        cout << "\n[!] Error: Invalid selection. Please enter a valid number.\n";
                        cin.clear();
                        cin.ignore(10000, '\n');
                        continue;
                    }

                    if (adminChoice == 5)
                    {
                        cout << "\n[LOGOUT] Terminating administrative tokens...\n\n";
                        break;
                    }

                    if (adminChoice == 1)
                    {
                        activeAdmin.viewAllProducts();
                    }
                    else if (adminChoice == 2)
                    {
                        int id, stock;
                        string name, category;
                        double price;

                        cout << "Enter Unique Product ID: ";
                        cin >> id;
                        cin.ignore();
                        cout << "Enter Product Name: ";
                        getline(cin, name);
                        cout << "Enter Category Classification: ";
                        getline(cin, category);
                        cout << "Enter Unit Price ($): ";
                        cin >> price;
                        cout << "Enter Warehouse Initial Stock: ";
                        cin >> stock;

                        activeAdmin.addProductToFile(id, name, category, price, stock);
                    }
                    else if (adminChoice == 3)
                    {
                        int id, stock;
                        string name, category;
                        double price;

                        cout << "Enter Target Product ID to Alter: ";
                        cin >> id;
                        cin.ignore();
                        cout << "Enter Updated Product Name: ";
                        getline(cin, name);
                        cout << "Enter Updated Category Classification: ";
                        getline(cin, category);
                        cout << "Enter New Unit Price ($): ";
                        cin >> price;
                        cout << "Enter Restocked Inventory Level: ";
                        cin >> stock;

                        activeAdmin.editProduct(id, name, category, price, stock);
                    }
                    else if (adminChoice == 4)
                    {
                        int id;
                        cout << "Enter Target Product ID to Drop: ";
                        cin >> id;
                        activeAdmin.deleteProduct(id);
                    }
                    else
                    {
                        cout << "[!] Option unrecognized. Retry code entry.\n";
                    }
                }
            }
            else
            {
                cout << "\n[CRITICAL ERROR] Authentication Rejected: Invalid administrative credentials.\n\n";
            }
        }
        // ==================== 2. REAL CUSTOMER PORTAL ====================
        else if (mainChoice == 2)
        {
            string custName, custPass;
            cout << "\n--- CUSTOMER SESSION DETAILS ---\n";
            cin.ignore();
            cout << "Enter Your Full Name: ";
            getline(cin, custName);
            cout << "Create a Session Password: ";
            cin >> custPass;

            Customer dynamicCustomer(custName, custPass);
            cout << "\n[SUCCESS] Active terminal initialized for shopper: '" << custName << "'\n";

            int userChoice = 0;
            while (true)
            {
                cout << "\n=========================================\n";
                cout << "         CUSTOMER SHOPPING DASHBOARD     \n";
                cout << "=========================================\n";
                cout << " [1] Browse Store Product Catalog\n";
                cout << " [2] View Your Virtual Shopping Cart\n";
                cout << " [3] ENTER Item into Cart\n";
                cout << " [4] DELETE Item from Cart\n";
                cout << " [5] Proceed to Secure Checkout\n";
                cout << " [6] Abandone Cart & Exit Session\n";
                cout << "-----------------------------------------\n";
                cout << "Select action item: ";
                cin >> userChoice;

                if (userChoice == 6)
                {
                    cout << "\n[WARNING] Terminating dynamic cart tracking memory...\n\n";
                    break;
                }

                if (userChoice == 1)
                {
                    loadProductsFromFile();
                }
                else if (userChoice == 2)
                {
                    dynamicCustomer.showcart();
                }
                else if (userChoice == 3)
                {
                    int id, qty;
                    cout << "Enter Product ID to acquire: ";
                    cin >> id;
                    cout << "Enter Desired Quantity: ";
                    cin >> qty;
                    dynamicCustomer.addTocart(id, qty);
                }
                else if (userChoice == 4)
                {
                    int id;
                    cout << "Enter Target Product ID to purge: ";
                    cin >> id;
                    dynamicCustomer.deleteItemFromCart(id);
                }
                
                else if (userChoice == 5)
                {
                    dynamicCustomer.checkout();
                   
                }
                else
                {
                    cout << "[!] Option unrecognized. Retry code entry.\n";
                }
            }
        }
        else
        {
            cout << "[!] Option unrecognized. Main route aborted.\n\n";
        }
    }

    return 0;
}