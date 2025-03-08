#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

using namespace std;

// Book class
class Book {
private:
    string title, author, publisher, ISBN, status, reservedBy;
    int year;

public:
    Book(string t, string a, string p, int y, string i) 
        : title(t), author(a), publisher(p), year(y), ISBN(i), status("available"), reservedBy("") {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getPublisher() const { return publisher; }
    int getYear() const { return year; }
    string getISBN() const { return ISBN; }
    string getStatus() const { return status; }
    string getReservedBy() const { return reservedBy; }
    
    void setStatus(string s) { status = s; }
    void setReservedBy(string userID) { reservedBy = userID; }

    string toCSV() const {
        ostringstream oss;
        oss << ISBN << "," << title << "," << author << "," << publisher << "," << year << "," << status << "," << reservedBy;
        return oss.str();
    }

    static Book fromCSV(const string& csv) {
        istringstream iss(csv);
        string i, t, a, p, yStr, s, r;
        getline(iss, i, ',');
        getline(iss, t, ',');
        getline(iss, a, ',');
        getline(iss, p, ',');
        getline(iss, yStr, ',');
        getline(iss, s, ',');
        getline(iss, r);
        int y = stoi(yStr);
        Book book(t, a, p, y, i);
        book.setStatus(s);
        book.setReservedBy(r);
        return book;
    }
};

// Account class
struct BorrowRecord {
    string ISBN;
    chrono::system_clock::time_point borrowDate;
};

class Account {
private:
    string userID;
    vector<BorrowRecord> borrowedBooks;
    double unpaidFine;

public:
    Account(string id) : userID(id), unpaidFine(0.0) {}

    void borrowBook(const string& ISBN) {
        borrowedBooks.push_back({ISBN, chrono::system_clock::now()});
    }

    void returnBook(const string& ISBN, double fine) {
        auto it = find_if(borrowedBooks.begin(), borrowedBooks.end(), 
                          [&ISBN](const BorrowRecord& r) { return r.ISBN == ISBN; });
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            unpaidFine += fine;
        }
    }

    double getUnpaidFine() const { return unpaidFine; }
    void payFine(double amount) { unpaidFine = max(0.0, unpaidFine - amount); }
    void setUnpaidFine(double fine) { unpaidFine = fine; }
    const vector<BorrowRecord>& getBorrowedBooks() const { return borrowedBooks; }

    void addBorrowRecord(const BorrowRecord& rec) { borrowedBooks.push_back(rec); }
};

// User base class and derived classes
class User {
protected:
    string userID, name, role;
    Account account;

public:
    User(string id, string n, string r) : userID(id), name(n), role(r), account(id) {}
    virtual ~User() {}
    
    string getUserID() const { return userID; }
    string getName() const { return name; }
    string getRole() const { return role; }
    Account& getAccount() { return account; }
    const Account& getAccount() const { return account; }
    
    virtual bool canBorrow() const = 0;
    virtual int calculateFine(int daysOverdue) const = 0;

    string toCSV() const { return userID + "," + name + "," + role; }
};

class Student : public User {
public:
    Student(string id, string n) : User(id, n, "student") {}
    bool canBorrow() const override {
        return account.getBorrowedBooks().size() < 3 && account.getUnpaidFine() == 0;
    }
    int calculateFine(int daysOverdue) const override {
        return daysOverdue > 0 ? daysOverdue * 10 : 0;
    }
};

class Faculty : public User {
public:
    Faculty(string id, string n) : User(id, n, "faculty") {}
    bool canBorrow() const override {
        if (account.getBorrowedBooks().size() >= 5) return false;
        auto now = chrono::system_clock::now();
        for (const auto& rec : account.getBorrowedBooks()) {
            int days = chrono::duration_cast<chrono::hours>(now - rec.borrowDate).count() / 24;
            if (days > 90) return false; // 30 days + 60 days overdue
        }
        return true;
    }
    int calculateFine(int daysOverdue) const override { return 0; }
};

class Librarian : public User {
public:
    Librarian(string id, string n) : User(id, n, "librarian") {}
    bool canBorrow() const override { return false; }
    int calculateFine(int daysOverdue) const override { return 0; }
};

// Library class
class Library {
private:
    vector<Book> books;
    vector<User*> users;

public:
    Library() { loadFromFile(); }
    ~Library() { saveToFile(); for (User* u : users) delete u; }

    void addBook(const Book& book) { books.push_back(book); }
    void removeBook(const string& ISBN) {
        books.erase(remove_if(books.begin(), books.end(), 
                              [&ISBN](const Book& b) { return b.getISBN() == ISBN; }), books.end());
    }

    void addUser(User* user) { users.push_back(user); }
    void removeUser(const string& userID) {
        users.erase(remove_if(users.begin(), users.end(), 
                              [&userID](const User* u) { return u->getUserID() == userID; }), users.end());
    }

    Book* findBook(const string& ISBN) {
        for (auto& book : books) {
            if (book.getISBN() == ISBN) return &book;
        }
        return nullptr;
    }

    User* findUser(const string& userID) {
        for (User* user : users) {
            if (user->getUserID() == userID) return user;
        }
        return nullptr;
    }

    void borrowBook(User* user, const string& ISBN) {
        Book* book = findBook(ISBN);
        if (!book) { cout << "Book not found.\n"; return; }
        if (book->getStatus() == "borrowed") { cout << "Book is already borrowed.\n"; return; }
        if (book->getStatus() == "reserved" && book->getReservedBy() != user->getUserID()) {
            cout << "Book is reserved by another user.\n"; return;
        }
        if (!user->canBorrow()) { cout << "You cannot borrow books at this time.\n"; return; }
        
        bool wasReservedByUser = (book->getStatus() == "reserved" && book->getReservedBy() == user->getUserID());
        user->getAccount().borrowBook(ISBN);
        book->setStatus("borrowed");
        book->setReservedBy("");  // Clear the reservation
        cout << "Book borrowed successfully.\n";
        
        if (wasReservedByUser) {
            int newReservedCount = getReservedBooksCount(user->getUserID());
            cout << "Reservation fulfilled. Remaining reserved books: " << newReservedCount << endl;
        }
    }

    void returnBook(User* user, const string& ISBN) {
        Book* book = findBook(ISBN);
        if (!book) { cout << "Book not found.\n"; return; }
        if (book->getStatus() != "borrowed") { cout << "Book is not borrowed.\n"; return; }
        auto& borrowed = user->getAccount().getBorrowedBooks();
        auto it = find_if(borrowed.begin(), borrowed.end(), 
                          [&ISBN](const BorrowRecord& r) { return r.ISBN == ISBN; });
        if (it == borrowed.end()) { cout << "You have not borrowed this book.\n"; return; }
        auto now = chrono::system_clock::now();
        int days = chrono::duration_cast<chrono::hours>(now - it->borrowDate).count() / 24;
        int maxDays = (user->getRole() == "student") ? 15 : 30;
        int daysOverdue = max(0, days - maxDays);
        int fine = user->calculateFine(daysOverdue);
        user->getAccount().returnBook(ISBN, fine);
        book->setStatus(book->getReservedBy().empty() ? "available" : "reserved");
        cout << "Book returned successfully.\n";
        if (fine > 0) cout << "Fined " << fine << " rupees for overdue.\n";
    }

    void reserveBook(User* user, const string& ISBN) {
        Book* book = findBook(ISBN);
        if (!book) { cout << "Book not found.\n"; return; }
        if (book->getStatus() != "borrowed") { cout << "Book is not available for reservation.\n"; return; }
        if (!book->getReservedBy().empty()) { cout << "Book is already reserved.\n"; return; }
        book->setReservedBy(user->getUserID());
        cout << "Book reserved successfully.\n";
    }

    int getReservedBooksCount(const string& userID) const {
        int count = 0;
        for (const auto& book : books) {
            if (book.getReservedBy() == userID) count++;
        }
        return count;
    }

    void searchBooks(const string& query) {
        if (books.empty()) { cout << "No book available.\n"; return; }
        bool found = false;
        for (const auto& book : books) {
            if (book.getTitle().find(query) != string::npos || book.getAuthor().find(query) != string::npos) {
                cout << book.getISBN() << " - " << book.getTitle() << " by " << book.getAuthor() 
                     << " (" << book.getStatus() << ")\n";
                found = true;
            }
        }
        if (!found) cout << "No matching books found.\n";
    }

    void viewAllBooks() {
        if (books.empty()) { cout << "No book available.\n"; return; }
        for (const auto& b : books) {
            cout << b.getISBN() << " - " << b.getTitle() << " (" << b.getStatus() << ")\n";
        }
    }

    void viewAllUsers() {
        for (const auto& u : users) {
            cout << u->getUserID() << " - " << u->getName() << " (" << u->getRole() << ")\n";
        }
    }

    void loadFromFile() {
        ifstream booksFile("books.csv");
        if (booksFile.is_open()) {
            string line;
            while (getline(booksFile, line)) books.push_back(Book::fromCSV(line));
            booksFile.close();
        } else {
            addBook(Book("Book1", "Author1", "Pub1", 2020, "ISBN1"));
            addBook(Book("Book2", "Author2", "Pub2", 2021, "ISBN2"));
            addBook(Book("Book3", "Author3", "Pub3", 2019, "ISBN3"));
            addBook(Book("Book4", "Author4", "Pub4", 2022, "ISBN4"));
            addBook(Book("Book5", "Author5", "Pub5", 2018, "ISBN5"));
        }

        ifstream usersFile("users.csv");
        if (usersFile.is_open()) {
            string line;
            while (getline(usersFile, line)) {
                istringstream iss(line);
                string id, n, r;
                getline(iss, id, ',');
                getline(iss, n, ',');
                getline(iss, r);
                if (r == "student") addUser(new Student(id, n));
                else if (r == "faculty") addUser(new Faculty(id, n));
                else if (r == "librarian") addUser(new Librarian(id, n));
            }
            usersFile.close();
        } else {
            addUser(new Student("S1", "Student1"));
            addUser(new Student("S2", "Student2"));
            addUser(new Student("S3", "Student3"));
            addUser(new Student("S4", "Student4"));
            addUser(new Student("S5", "Student5"));
            addUser(new Faculty("F1", "Faculty1"));
            addUser(new Faculty("F2", "Faculty2"));
            addUser(new Faculty("F3", "Faculty3"));
            addUser(new Librarian("L1", "Librarian1"));
        }

        ifstream borrowingsFile("borrowings.csv");
        if (borrowingsFile.is_open()) {
            string line;
            while (getline(borrowingsFile, line)) {
                istringstream iss(line);
                string uid, isbn, tsStr;
                getline(iss, uid, ',');
                getline(iss, isbn, ',');
                getline(iss, tsStr);
                long long ts = stoll(tsStr);
                auto tp = chrono::system_clock::time_point(chrono::seconds(ts));
                User* u = findUser(uid);
                if (u) u->getAccount().addBorrowRecord({isbn, tp});
            }
            borrowingsFile.close();
        }

        ifstream finesFile("fines.csv");
        if (finesFile.is_open()) {
            string line;
            while (getline(finesFile, line)) {
                istringstream iss(line);
                string uid, fStr;
                getline(iss, uid, ',');
                getline(iss, fStr);
                double f = stod(fStr);
                User* u = findUser(uid);
                if (u) u->getAccount().setUnpaidFine(f);
            }
            finesFile.close();
        }
    }

    void saveToFile() {
        ofstream booksFile("books.csv");
        for (const auto& b : books) booksFile << b.toCSV() << "\n";
        booksFile.close();

        ofstream usersFile("users.csv");
        for (const auto& u : users) usersFile << u->toCSV() << "\n";
        usersFile.close();

        ofstream borrowingsFile("borrowings.csv");
        for (const auto& u : users) {
            for (const auto& r : u->getAccount().getBorrowedBooks()) {
                long long ts = chrono::duration_cast<chrono::seconds>(r.borrowDate.time_since_epoch()).count();
                borrowingsFile << u->getUserID() << "," << r.ISBN << "," << ts << "\n";
            }
        }
        borrowingsFile.close();

        ofstream finesFile("fines.csv");
        for (const auto& u : users) {
            double f = u->getAccount().getUnpaidFine();
            if (f > 0) finesFile << u->getUserID() << "," << f << "\n";
        }
        finesFile.close();
    }
};

int main() {
    Library library;

    while (true) {
        cout << "Enter userID to login (or 'exit' to quit): ";
        string input;
        cin >> input;
        if (input == "exit") {
            cout << "Saving data..." << endl;
            break;
        }

        User* user = library.findUser(input);
        if (!user) { 
            cout << "User not found.\n"; 
            continue; 
        }

        // Display login message
        cout << "Logged in as " << user->getName() << " (" << user->getRole() << ")" << endl;

        // For students and faculty, display borrowed and reserved book counts
        if (user->getRole() == "student" || user->getRole() == "faculty") {
            int borrowed = user->getAccount().getBorrowedBooks().size();
            int reserved = library.getReservedBooksCount(user->getUserID());
            cout << "Borrowed books: " << borrowed << endl;
            cout << "Reserved books: " << reserved << endl;
        }

        // Student or Faculty menu
        if (user->getRole() == "student" || user->getRole() == "faculty") {
            while (true) {
                cout << "\n1. Borrow book\n2. Return book\n3. View borrowed books\n4. View fines\n5. Pay fines\n6. Reserve book\n7. Search books\n8. Logout\nChoice: ";
                int choice;
                cin >> choice;
                if (choice == 8) break;

                string isbn, query;
                switch (choice) {
                    case 1:
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        library.borrowBook(user, isbn);
                        break;
                    case 2:
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        library.returnBook(user, isbn);
                        break;
                    case 3:
                        for (const auto& r : user->getAccount().getBorrowedBooks()) {
                            cout << r.ISBN << "\n";
                        }
                        if (user->getAccount().getBorrowedBooks().empty()) cout << "No books borrowed.\n";
                        break;
                    case 4:
                        cout << "Unpaid fines: " << user->getAccount().getUnpaidFine() << " rupees\n";
                        break;
                    case 5:
                        cout << "Paying all fines: " << user->getAccount().getUnpaidFine() << " rupees\n";
                        user->getAccount().payFine(user->getAccount().getUnpaidFine());
                        cout << "Fines cleared.\n";
                        break;
                    case 6:
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        library.reserveBook(user, isbn);
                        break;
                    case 7:
                        cout << "Enter search query: ";
                        cin.ignore();
                        getline(cin, query);
                        library.searchBooks(query);
                        break;
                    default:
                        cout << "Invalid choice.\n";
                }
            }
        } 
        // Librarian menu
        else if (user->getRole() == "librarian") {
            while (true) {
                cout << "\n1. Add book\n2. Remove book\n3. Add user\n4. Remove user\n5. View all books\n6. View all users\n7. Search books\n8. Logout\nChoice: ";
                int choice;
                cin >> choice;
                if (choice == 8) break;

                string isbn, id, name, role, title, author, publisher, query;
                int year;
                switch (choice) {
                    case 1:
                        cout << "Title: ";
                        cin.ignore();
                        getline(cin, title);
                        cout << "Author: ";
                        getline(cin, author);
                        cout << "Publisher: ";
                        getline(cin, publisher);
                        cout << "Year: ";
                        cin >> year;
                        cout << "ISBN: ";
                        cin >> isbn;
                        library.addBook(Book(title, author, publisher, year, isbn));
                        cout << "Book added.\n";
                        break;
                    case 2:
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        library.removeBook(isbn);
                        cout << "Book removed.\n";
                        break;
                    case 3:
                        cout << "UserID: ";
                        cin >> id;
                        cout << "Name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Role (student/faculty/librarian): ";
                        cin >> role;
                        if (role == "student") library.addUser(new Student(id, name));
                        else if (role == "faculty") library.addUser(new Faculty(id, name));
                        else if (role == "librarian") library.addUser(new Librarian(id, name));
                        else cout << "Invalid role.\n";
                        cout << "User added.\n";
                        break;
                    case 4:
                        cout << "Enter userID: ";
                        cin >> id;
                        library.removeUser(id);
                        cout << "User removed.\n";
                        break;
                    case 5:
                        library.viewAllBooks();
                        break;
                    case 6:
                        library.viewAllUsers();
                        break;
                    case 7:
                        cout << "Enter search query: ";
                        cin.ignore();
                        getline(cin, query);
                        library.searchBooks(query);
                        break;
                    default:
                        cout << "Invalid choice.\n";
                }
            }
        }
    }
    return 0;
}