---
# Library Management System

## Overview
This is a C++ based Library Management System that manages books, users, borrowing, returning, reservations, and fines. It supports three types of users:
- Students (max 3 books, 15-day loan period, fines applicable)
- Faculty (max 5 books, 30-day loan period, no fines)
- Librarians (cannot borrow, can manage books and users)

The system persists data in CSV files and includes features like book searching, reservation system, and fine management.

## Features
- Book management (add/remove books)
- User management (add/remove users)
- Borrowing and returning books
- Book reservation system
- Fine calculation and payment (for students)
- Search functionality
- Persistent storage in CSV files

## Prerequisites
- C++11 compatible compiler (g++, clang++, etc.)
- Standard C++ libraries

## File Structure
- `main.cpp` - Main source code file
- `books.csv` - Stores book data
- `users.csv` - Stores user data
- `borrowings.csv` - Stores borrowing records
- `fines.csv` - Stores fine records

## Compilation Instructions
To compile the program, use one of the following commands:

### Using g++
```bash
g++ main.cpp -o library_management_system
```

## Execution Instructions
1. After compilation, run the program using:
```bash
library_management_system.exe
```

2. The program will:
   - Load existing data from CSV files if present
   - Create default data if no CSV files exist
   - Present a login prompt

3. To login:
   - Enter a user ID (e.g., "S1" for student, "F1" for faculty, "L1" for librarian)
   - Type "exit" to quit the program

## Default Users
The system creates these default users if no users.csv exists:
- Students: S1-S5 (Student1-Student5)
- Faculty: F1-F3 (Faculty1-Faculty3)
- Librarian: L1 (Librarian1)

## Default Books
The system creates these default books if no books.csv exists:
- Book1 (ISBN1) - Author1, Pub1, 2020
- Book2 (ISBN2) - Author2, Pub2, 2021
- Book3 (ISBN3) - Author3, Pub3, 2019
- Book4 (ISBN4) - Author4, Pub4, 2022
- Book5 (ISBN5) - Author5, Pub5, 2018

## Usage
### Student/Faculty Menu
1. Borrow book
2. Return book
3. View borrowed books
4. View fines
5. Pay fines
6. Reserve book
7. Search books
8. Logout

### Librarian Menu
1. Add book
2. Remove book
3. Add user
4. Remove user
5. View all books
6. View all users
7. Search books
8. Logout

## Data Persistence
- Data is automatically saved to CSV files when the program exits
- Files are loaded automatically when the program starts
- CSV files must be in the same directory as the executable

## Limitations
- No password authentication
- Simple fine system (10 rupees/day for students only)
- No GUI interface
- Basic error handling

## Troubleshooting
- Ensure CSV files are writable in the execution directory
- Check compiler version supports C++11
- Verify sufficient memory for larger datasets

## Contributing
Feel free to fork and submit pull requests for:
- Bug fixes
- Feature enhancements
- Improved error handling
- GUI implementation