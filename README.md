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

Further details on the functionality of the program can be found in Documentation.md