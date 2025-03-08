---
# Library Management System Documentation

This document provides a detailed explanation of the C++ code for the Library Management System, including its classes, methods, and overall functionality.

## Overview

The program is a console-based library management system written in C++11. It manages books and users with different roles (students, faculty, librarians), handling tasks like borrowing, returning, reserving books, and maintaining user accounts. Data is stored in CSV files for persistence.

## Classes

### Book

Represents a book in the library.

- **Attributes**:
  - `title`: `string` - The book’s title.
  - `author`: `string` - The book’s author.
  - `publisher`: `string` - The publisher of the book.
  - `year`: `int` - The publication year.
  - `ISBN`: `string` - The unique ISBN identifier.
  - `status`: `string` - The book’s status ("available", "borrowed", "reserved").
  - `reservedBy`: `string` - The userID of the user who reserved the book (empty if not reserved).

- **Methods**:
  - **Constructor**: `Book(string t, string a, string p, int y, string i)` - Initializes a book with title, author, publisher, year, and ISBN. Status defaults to "available".
  - **Getters**: `getTitle()`, `getAuthor()`, `getPublisher()`, `getYear()`, `getISBN()`, `getStatus()`, `getReservedBy()` - Return the respective attributes.
  - `setStatus(string s)`: Updates the book’s status.
  - `setReservedBy(string userID)`: Sets or clears the reserving userID.
  - `toCSV()`: Returns a CSV-formatted string of the book’s data (e.g., `ISBN1,Book1,Author1,Pub1,2020,available,`).
  - `static fromCSV(string csv)`: Creates a `Book` object from a CSV string.

### Account

Manages a user’s borrowing records and fines.

- **Attributes**:
  - `userID`: `string` - The associated user’s ID.
  - `borrowedBooks`: `vector<BorrowRecord>` - List of borrowed books, where `BorrowRecord` is a struct with:
    - `ISBN`: `string` - The borrowed book’s ISBN.
    - `borrowDate`: `chrono::system_clock::time_point` - The time the book was borrowed.
  - `unpaidFine`: `double` - Total unpaid fines in rupees.

- **Methods**:
  - **Constructor**: `Account(string id)` - Initializes with a userID and zero fines.
  - `borrowBook(string ISBN)`: Adds a new borrow record with the current timestamp.
  - `returnBook(string ISBN, double fine)`: Removes the borrow record and adds the fine to `unpaidFine`.
  - `getUnpaidFine()`: Returns the total unpaid fine.
  - `payFine(double amount)`: Reduces `unpaidFine` by the specified amount (minimum 0).
  - `setUnpaidFine(double fine)`: Sets the fine amount directly (used when loading from file).
  - `getBorrowedBooks()`: Returns the list of borrowed books.
  - `addBorrowRecord(BorrowRecord rec)`: Adds a borrow record (used when loading from file).

### User (Base Class)

Abstract base class for different user types.

- **Attributes**:
  - `userID`: `string` - Unique identifier for the user.
  - `name`: `string` - The user’s name.
  - `role`: `string` - The user’s role ("student", "faculty", "librarian").
  - `account`: `Account` - The user’s account for managing borrowings and fines.

- **Methods**:
  - **Constructor**: `User(string id, string n, string r)` - Initializes user details and creates an `Account`.
  - **Virtual Destructor**: Ensures proper cleanup of derived classes.
  - **Getters**: `getUserID()`, `getName()`, `getRole()`, `getAccount()` - Access user details and account.
  - `virtual bool canBorrow() const = 0`: Pure virtual method to check borrowing eligibility.
  - `virtual int calculateFine(int daysOverdue) const = 0`: Pure virtual method to compute fines.
  - `toCSV()`: Returns a CSV string (e.g., `S1,Student1,student`).

#### Student (Derived from User)

Represents a student user.

- **Constructor**: `Student(string id, string n)` - Initializes with role "student".
- **Overrides**:
  - `canBorrow()`: Returns `true` if fewer than 3 books are borrowed and no unpaid fines exist.
  - `calculateFine(int daysOverdue)`: Returns 10 rupees per day overdue (0 if not overdue).

#### Faculty (Derived from User)

Represents a faculty user.

- **Constructor**: `Faculty(string id, string n)` - Initializes with role "faculty".
- **Overrides**:
  - `canBorrow()`: Returns `true` if fewer than 5 books are borrowed and no book is overdue by more than 90 days (30-day borrowing period + 60-day grace period).
  - `calculateFine(int daysOverdue)`: Returns 0 (no fines for faculty).

#### Librarian (Derived from User)

Represents a librarian user.

- **Constructor**: `Librarian(string id, string n)` - Initializes with role "librarian".
- **Overrides**:
  - `canBorrow()`: Returns `false` (librarians cannot borrow).
  - `calculateFine(int daysOverdue)`: Returns 0 (no fines).

### Library

Manages the collection of books and users.

- **Attributes**:
  - `books`: `vector<Book>` - List of all books in the library.
  - `users`: `vector<User*>` - List of pointers to user objects (polymorphic).

- **Methods**:
  - **Constructor**: `Library()` - Loads data from files.
  - **Destructor**: `~Library()` - Saves data to files and deletes user pointers.
  - `addBook(Book book)`: Adds a book to the collection.
  - `removeBook(string ISBN)`: Removes a book by ISBN.
  - `addUser(User* user)`: Adds a user to the system.
  - `removeUser(string userID)`: Removes a user by userID.
  - `findBook(string ISBN)`: Returns a pointer to the book or `nullptr` if not found.
  - `findUser(string userID)`: Returns a pointer to the user or `nullptr` if not found.
  - `borrowBook(User* user, string ISBN)`: Handles borrowing logic (checks availability, reservation, and eligibility).
  - `returnBook(User* user, string ISBN)`: Handles return logic, calculates fines (15-day limit for students, 30-day for faculty).
  - `reserveBook(User* user, string ISBN)`: Reserves a borrowed book if available for reservation.
  - `getReservedBooksCount(string userID)`: Counts books reserved by a user.
  - `searchBooks(string query)`: Displays books matching the query in title or author.
  - `viewAllBooks()`: Lists all books with ISBN, title, and status.
  - `viewAllUsers()`: Lists all users with userID, name, and role.
  - `loadFromFile()`: Loads data from `books.csv`, `users.csv`, `borrowings.csv`, and `fines.csv`; initializes defaults if files are missing.
  - `saveToFile()`: Saves all data to the respective CSV files.

## Main Function

The `main` function provides a console-based interface:

1. **Login Loop**:
   - Prompts for a `userID` or `exit` to quit.
   - Validates the user and displays their name and role.

2. **User Menus**:
   - **Student/Faculty**:
     - Options: Borrow, return, view borrowed books, view/pay fines, reserve, search, logout.
     - Displays borrowed and reserved book counts on login.
   - **Librarian**:
     - Options: Add/remove books/users, view all books/users, search, logout.
   - Input is handled via `cin`, with prompts guiding the user.

3. **Data Persistence**:
   - Data is loaded when the `Library` object is created and saved when it’s destroyed (on program exit).

## Data Files

- `books.csv`: Format: `ISBN,title,author,publisher,year,status,reservedBy`.
- `users.csv`: Format: `userID,name,role`.
- `borrowings.csv`: Format: `userID,ISBN,timestamp` (timestamp in seconds since epoch).
- `fines.csv`: Format: `userID,fine`.

## Notes

- **Time Handling**: Uses `<chrono>` for borrowing timestamps and overdue calculations.
- **Default Data**: If CSV files are absent, the program initializes with 5 books and 9 users (5 students, 3 faculty, 1 librarian).
- **Borrowing Rules**:
  - Students: 3-book limit, 15-day borrowing period, 10 rupees/day fine.
  - Faculty: 5-book limit, 30-day borrowing period, no fines but blocked if overdue > 90 days.
  - Librarians: Cannot borrow.
- **Error Handling**: Basic console messages for invalid inputs or actions.