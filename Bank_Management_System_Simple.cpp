#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    string customerName;
    double balance;

public:
    BankAccount(int number, const string& name, double startingBalance)
        : accountNumber(number), customerName(name), balance(startingBalance) {}

    int getAccountNumber() const { return accountNumber; }
    string getCustomerName() const { return customerName; }
    double getBalance() const { return balance; }

    void deposit(double amount) { balance += amount; }

    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }
};

bool readInteger(const string& prompt, int& number) {
    cout << prompt;
    string line;
    getline(cin, line);
    stringstream input(line);
    char extra;

    if (!(input >> number) || (input >> extra)) {
        cout << "Please enter a valid whole number.\n";
        return false;
    }
    return true;
}

bool readAmount(const string& prompt, double& amount, bool allowZero = false) {
    cout << prompt;
    string line;
    getline(cin, line);
    stringstream input(line);
    double value;
    char extra;

    if (!(input >> value) || (input >> extra)) {
        cout << "Please enter a valid amount, such as 250 or 250.50.\n";
        return false;
    }
    if (value < 0 || (!allowZero && value == 0)) {
        cout << (allowZero ? "Amount cannot be negative.\n"
                           : "Amount must be greater than zero.\n");
        return false;
    }
    if (value > 1000000000.0) {
        cout << "Amount is too large.\n";
        return false;
    }
    // Keep the example simple: accept at most two decimal places.
    double cents = value * 100.0;
    long long roundedCents = static_cast<long long>(cents + 0.000001);
    if (cents - roundedCents > 0.0001 || roundedCents - cents > 0.0001) {
        cout << "Use at most two decimal places.\n";
        return false;
    }
    amount = roundedCents / 100.0;
    return true;
}

int findAccount(const vector<BankAccount>& accounts, int number) {
    for (int i = 0; i < static_cast<int>(accounts.size()); i++) {
        if (accounts[i].getAccountNumber() == number) return i;
    }
    return -1;
}

bool saveAccounts(const vector<BankAccount>& accounts) {
    ofstream file("accounts.txt");
    if (!file) {
        cout << "Error: Could not save account data.\n";
        return false;
    }

    file << fixed << setprecision(2);
    for (const BankAccount& account : accounts) {
        file << account.getAccountNumber() << "|"
             << account.getCustomerName() << "|"
             << account.getBalance() << "\n";
    }
    file.close();
    if (!file) {
        cout << "Error: Could not finish saving account data.\n";
        return false;
    }
    return true;
}

void loadAccounts(vector<BankAccount>& accounts) {
    ifstream file("accounts.txt");
    string line;

    while (getline(file, line)) {
        stringstream row(line);
        string numberText, name, balanceText;
        if (!getline(row, numberText, '|') ||
            !getline(row, name, '|') ||
            !getline(row, balanceText)) continue;

        stringstream numberInput(numberText), balanceInput(balanceText);
        int number;
        double balance;
        char extra;

        if (!(numberInput >> number) || (numberInput >> extra)) continue;
        if (!(balanceInput >> balance) || (balanceInput >> extra)) continue;
        if (number <= 0 || balance < 0 || findAccount(accounts, number) != -1) continue;

        accounts.push_back(BankAccount(number, name, balance));
    }
}

bool saveTransaction(int number, const string& type,
                     double amount, double balanceAfter) {
    ofstream file("transactions.txt", ios::app);
    if (!file) {
        cout << "Warning: Could not save transaction history.\n";
        return false;
    }

    file << fixed << setprecision(2)
         << number << "|" << type << "|" << amount
         << "|" << balanceAfter << "\n";
    file.close();

    if (!file) {
        cout << "Warning: Could not save transaction history.\n";
        return false;
    }
    return true;
}

void displayAccounts(const vector<BankAccount>& accounts) {
    if (accounts.empty()) {
        cout << "No accounts available.\n";
        return;
    }

    cout << left << setw(15) << "Account No."
         << setw(25) << "Customer Name"
         << right << setw(12) << "Balance\n";
    cout << string(52, '-') << "\n" << fixed << setprecision(2);

    for (const BankAccount& account : accounts) {
        cout << left << setw(15) << account.getAccountNumber()
             << setw(25) << account.getCustomerName()
             << right << setw(12) << account.getBalance() << "\n";
    }
}

void createAccount(vector<BankAccount>& accounts) {
    int number;
    double initialDeposit;

    if (!readInteger("Enter a positive account number: ", number)) return;
    if (number <= 0) {
        cout << "Account number must be positive.\n";
        return;
    }
    if (findAccount(accounts, number) != -1) {
        cout << "That account number already exists.\n";
        return;
    }

    cout << "Enter customer name: ";
    string name;
    getline(cin, name);
    if (name.empty() || name.find('|') != string::npos) {
        cout << "Name cannot be empty or contain the | character.\n";
        return;
    }

    if (!readAmount("Enter initial deposit (0 or more): ", initialDeposit, true)) return;

    accounts.push_back(BankAccount(number, name, initialDeposit));
    if (!saveAccounts(accounts)) {
        accounts.pop_back();
        return;
    }

    if (initialDeposit > 0) {
        saveTransaction(number, "DEPOSIT", initialDeposit, initialDeposit);
    }
    cout << "Account created successfully.\n";
}

void depositMoney(vector<BankAccount>& accounts) {
    int number;
    double amount;

    if (!readInteger("Enter account number: ", number)) return;
    int index = findAccount(accounts, number);
    if (index == -1) {
        cout << "Account not found.\n";
        return;
    }
    if (!readAmount("Enter deposit amount: ", amount)) return;

    accounts[index].deposit(amount);

    if (!saveAccounts(accounts)) {
        // Undo the deposit if account data could not be saved.
        accounts[index].withdraw(amount);
        return;
    }

    saveTransaction(number, "DEPOSIT", amount, accounts[index].getBalance());
    cout << fixed << setprecision(2)
         << "Deposit successful. New balance: "
         << accounts[index].getBalance() << "\n";
}

void withdrawMoney(vector<BankAccount>& accounts) {
    int number;
    double amount;

    if (!readInteger("Enter account number: ", number)) return;
    int index = findAccount(accounts, number);
    if (index == -1) {
        cout << "Account not found.\n";
        return;
    }
    if (!readAmount("Enter withdrawal amount: ", amount)) return;

    if (!accounts[index].withdraw(amount)) {
        cout << fixed << setprecision(2)
             << "Insufficient balance. Current balance: "
             << accounts[index].getBalance() << "\n";
        return;
    }

    if (!saveAccounts(accounts)) {
        // Undo the withdrawal if account data could not be saved.
        accounts[index].deposit(amount);
        return;
    }

    saveTransaction(number, "WITHDRAW", amount, accounts[index].getBalance());
    cout << fixed << setprecision(2)
         << "Withdrawal successful. New balance: "
         << accounts[index].getBalance() << "\n";
}

void searchAccount(const vector<BankAccount>& accounts, bool balanceOnly = false) {
    int number;
    if (!readInteger("Enter account number: ", number)) return;

    int index = findAccount(accounts, number);
    if (index == -1) {
        cout << "Account not found.\n";
        return;
    }

    cout << fixed << setprecision(2);
    if (!balanceOnly) {
        cout << "Account number: " << accounts[index].getAccountNumber() << "\n";
        cout << "Customer name: " << accounts[index].getCustomerName() << "\n";
    }
    cout << "Current balance: " << accounts[index].getBalance() << "\n";
}

void showTransactionHistory(const vector<BankAccount>& accounts) {
    int number;
    if (!readInteger("Enter account number: ", number)) return;
    if (findAccount(accounts, number) == -1) {
        cout << "Account not found.\n";
        return;
    }

    ifstream file("transactions.txt");
    if (!file) {
        cout << "No transaction history available.\n";
        return;
    }

    cout << "\nTransaction history for account " << number << ":\n";
    cout << left << setw(15) << "Type"
         << right << setw(12) << "Amount"
         << setw(18) << "Balance After\n";
    cout << string(45, '-') << "\n" << fixed << setprecision(2);

    string line;
    int count = 0;
    while (getline(file, line)) {
        stringstream row(line);
        string numberText, type, amountText, balanceText;

        if (!getline(row, numberText, '|') ||
            !getline(row, type, '|') ||
            !getline(row, amountText, '|') ||
            !getline(row, balanceText)) continue;

        stringstream numberInput(numberText);
        int savedNumber;
        if (!(numberInput >> savedNumber) || savedNumber != number) continue;

        cout << left << setw(15) << type
             << right << setw(12) << amountText
             << setw(18) << balanceText << "\n";
        count++;
    }

    if (count == 0) cout << "No transactions recorded for this account.\n";
}

void showMenu() {
    cout << "\n========== BANK MANAGEMENT SYSTEM ==========\n"
         << "1. Create Account\n"
         << "2. Display All Accounts\n"
         << "3. Deposit Money\n"
         << "4. Withdraw Money\n"
         << "5. Search Account\n"
         << "6. Check Balance\n"
         << "7. Transaction History\n"
         << "8. Exit\n"
         << "============================================\n";
}

int main() {
    vector<BankAccount> accounts;
    loadAccounts(accounts);

    cout << accounts.size() << " account(s) loaded.\n";

    int choice;
    while (true) {
        showMenu();

        if (!readInteger("Enter your choice: ", choice)) continue;

        if (choice == 1) createAccount(accounts);
        else if (choice == 2) displayAccounts(accounts);
        else if (choice == 3) depositMoney(accounts);
        else if (choice == 4) withdrawMoney(accounts);
        else if (choice == 5) searchAccount(accounts);
        else if (choice == 6) searchAccount(accounts, true);
        else if (choice == 7) showTransactionHistory(accounts);
        else if (choice == 8) {
            cout << "Thank you for using the Bank Management System.\n";
            break;
        } else {
            cout << "Invalid choice. Enter a number from 1 to 8.\n";
        }
    }

    return 0;
}
