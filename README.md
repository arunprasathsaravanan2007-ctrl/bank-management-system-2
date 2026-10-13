# Bank Management System in C++

A simple, menu-driven **Bank Management System** developed using C++ to demonstrate fundamental programming concepts and object-oriented programming (OOP).

## About the Project

This project was developed as a practical application of the C++ concepts learned during my internship at **Thiranex Skill Development & Future Tech**.

The application allows users to create bank accounts, view account details, deposit and withdraw money, search for accounts, check balances, and view transaction history through a console-based interface.

## Features

- **Create Account** — Create a new bank account.
- **Display Accounts** — View available account details.
- **Deposit Money** — Add money to an account.
- **Withdraw Money** — Withdraw money when sufficient balance is available.
- **Search Account** — Find an account using its account number.
- **Check Balance** — View the balance of an account.
- **Transaction History** — View recorded transactions.
- **File Handling** — Store account information and transaction records in text files.
- **Input Validation** — Handle invalid inputs and prevent duplicate account numbers.

## Technologies Used

- **Programming Language:** C++
- **Concepts:** Classes, Objects, Functions, Conditional Statements, Loops, Vectors, and File Handling
- **Compiler:** GCC (g++)
- **Standard:** C++11

## Project Structure

```text
bank-management-system/
├── Bank_Management_System_Simple.cpp
├── README.md
├── accounts.txt
└── transactions.txt
```

The `accounts.txt` and `transactions.txt` files are created or updated by the application at runtime. They do not need to be uploaded beforehand.

## How to Run the Project

### 1. Install a C++ Compiler

Install GCC through MSYS2 or use another C++ compiler that supports C++11.

### 2. Clone the Repository

```bash
git clone YOUR_REPOSITORY_URL
```

Replace `YOUR_REPOSITORY_URL` with your GitHub repository URL.

### 3. Open the Project Folder

```bash
cd bank-management-system
```

### 4. Compile the Program

```bash
g++ -std=c++11 Bank_Management_System_Simple.cpp -o bank
```

### 5. Run the Program

On Windows PowerShell:

```powershell
.\bank.exe
```

On Linux or macOS:

```bash
./bank
```

## Learning Outcomes

Through this project, I gained practical experience in:

- Applying object-oriented programming concepts.
- Organizing code using classes and functions.
- Managing multiple accounts using vectors.
- Implementing basic deposit and withdrawal operations.
- Reading and writing data using file handling.
- Validating user input and handling common errors.
- Compiling and running C++ programs using GCC.

## Limitations

This is an educational console application. It does not implement production-grade banking security, database-backed storage, user authentication, or real financial transactions.

## Author

**Arun Prasath S**  
B.E. Computer Science and Engineering

Developed as a learning project during my C++ programming internship.

---

*This project is intended for educational purposes and demonstrates basic C++ programming concepts.*
