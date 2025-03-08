# CS253-Library-Management-SystemBased on your request to "change the readme to markdown also," I’ll assume you want the `readme.md` file provided in markdown format. Since the file is already in markdown as shown below, I’ll present it fully formatted and ready for use. Here is the complete `readme.md` in markdown:

---

# Library Management System

This is a simple library management system implemented in C++. It allows users to borrow, return, and reserve books, as well as manage their accounts. Different user roles (students, faculty, and librarians) have specific privileges, such as borrowing limits or administrative functions like adding and removing books and users.

## Compilation

To compile the program, you need a C++ compiler that supports C++11 or later (e.g., g++). Use the following command in your terminal or command prompt:

```bash
g++ -std=c++11 -o library_system main.cpp
```

- The `-std=c++11` flag ensures support for C++11 features like `chrono` used in the code.
- This command compiles `main.cpp` into an executable named `library_system`.

## Execution

To run the program after compilation, use the following command:

```bash
./library_system
```

On Windows, if using a different compiler or environment, you might need to run:

```bash
library_system.exe
```

## Usage

1. **Login**: 
   - When the program starts, enter your `userID` to log in.
   - Type `exit` to quit the program at the login prompt.
   - Example userIDs from the default data: `S1` (student), `F1` (faculty), `L1` (librarian).

2. **Student/Faculty Menu**:
   - **Options**:
     - **Borrow book**: Enter an ISBN to borrow a book (e.g., `ISBN1`).
     - **Return book**: Return a borrowed book by ISBN.
     - **View borrowed books**: See the list of books you’ve borrowed.
     - **View fines**: Check your unpaid fines (students only).
     - **Pay fines**: Clear your fines (students only).
     - **Reserve book**: Reserve a borrowed book by ISBN.
     - **Search books**: Search by title or author.
     - **Logout**: Return to the login screen.
   - Follow the prompts to enter required information (e.g., ISBNs).

3. **Librarian Menu**:
   - **Options**:
     - **Add book**: Enter book details (title, author, publisher, year, ISBN).
     - **Remove book**: Remove a book by ISBN.
     - **Add user**: Add a new user with a userID, name, and role.
     - **Remove user**: Remove a user by userID.
     - **View all books**: List all books in the library.
     - **View all users**: List all registered users.
     - **Search books**: Search by title or author.
     - **Logout**: Return to the login screen.

## Data Persistence

The program uses CSV files to store data, which are created or updated when the program runs:

- `books.csv`: Stores book details (ISBN, title, author, publisher, year, status, reservedBy).
- `users.csv`: Stores user details (userID, name, role).
- `borrowings.csv`: Tracks borrowed books (userID, ISBN, borrow timestamp).
- `fines.csv`: Records unpaid fines (userID, fine amount).

- **Note**: If these files don’t exist when the program starts, it initializes with default books and users. Data is saved to these files when the program exits.

## Prerequisites

- A C++ compiler supporting C++11 (e.g., g++ version 4.8 or later).
- No external libraries are required beyond the C++ Standard Library.

---

This `readme.md` is already in markdown format, as requested, and can be directly copied into your project. Let me know if you need any additional changes!