/* Nama: Wildan Aziz Hidayat
   NIM: 235150301111028
   Kelas: Teknik Komputer B
   Tugas: Tugas Bonus Graph BFS-DFS
*/

#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>


struct Node
{
    int data;
    
    Node *next;
};

struct Queue
{
    Node *front;
    Node *rear;

    Queue() : front(nullptr), rear(nullptr) {}

    bool isEmpty() {
        return front == nullptr;
    }

    void enqueue(int value) {
        Node *newNode = new Node;
        newNode->data = value;
        newNode->next = nullptr;
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    int dequeue() {
        if (isEmpty()) {
            std::cerr << "Queue is empty, cannot dequeue." << std::endl;
            return -1;
        }
        Node *temp = front;
        int value = front->data;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return value;
    }

    int frontElement(){
        if (isEmpty())
        {
            std::cerr << "Queue is empty, cannot return front element!" << std::endl;
            return -1;
        }
        return front->data;
    }
};



struct Book {
    int primary_key;
    char title[100];
    int year;
    char author[100];
    char publisher[100];
    bool is_deleted;
};

int inOption();
void checkDatabase(std::fstream &data);
void addBook(std::fstream &data);
void writeData(std::fstream &data, int position, Book &inputBook);
int getDataSize(std::fstream &data);
Book readData(std::fstream &data, int position);
void displayBook(std::fstream &data);
void updateData(std::fstream &data);
void deleteData(std::fstream &data);
void gc(std::fstream &data);

int main() {
    std::fstream data;
    checkDatabase(data);

    int rOption = inOption();
    char isContinue;
    enum { SHOW = 1, ADD, UPDATE, DELETE, END };

    while (rOption != END) {
        switch (rOption) {
        case SHOW:
            std::cout << "Show books on library" << std::endl;
            displayBook(data);
            break;

        case ADD:
            std::cout << "Add books on library" << std::endl;
            addBook(data);
            break;

        case UPDATE:
            std::cout << "Update books on library" << std::endl;
            displayBook(data);
            updateData(data);
            displayBook(data);
            break;

        case DELETE:
            std::cout << "Delete books on library" << std::endl;
            displayBook(data);
            deleteData(data);
            gc(data);
            displayBook(data);
            break;
        default:
            std::cout << "Input has not found!" << std::endl;
            break;
        }

        std::cout << "continue (y/n): ";
        std::cin >> isContinue;
        if (isContinue == 'n') {
            std::cout << "End of Program" << std::endl;
            break;
        } else if (isContinue == 'y') {
            rOption = inOption();
        } else {
            std::cout << "Invalid input!" << std::endl;
            break;
        }
    }

    return 0;
}

int inOption() {
    int option;
    std::cout << "Welcome to Simple Book Management System" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "1. Show books has available on Library" << std::endl;
    std::cout << "2. Add book on Library" << std::endl;
    std::cout << "3. Update book on Library" << std::endl;
    std::cout << "4. Delete book on Library" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Your input [1-5]: ";
    std::cin >> option;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return option;
}

void checkDatabase(std::fstream &data) {
    data.open("database.bin", std::ios::in | std::ios::out | std::ios::binary);

    if (data.is_open()) {
        std::cout << "Database has found" << std::endl;
    } else {
        std::cout << "Database has not found, New database will created" << std::endl;
        data.close();
        data.open("database.bin", std::ios::trunc | std::ios::in | std::ios::out | std::ios::binary);
    }
}

void writeData(std::fstream &data, int position, Book &inputBook) {
    data.seekp((position - 1) * sizeof(Book), std::ios::beg);
    data.write(reinterpret_cast<char*>(&inputBook), sizeof(Book));
}

int getDataSize(std::fstream &data) {
    int start, end;
    data.seekg(0, std::ios::beg);
    start = data.tellg();
    data.seekg(0, std::ios::end);
    end = data.tellg();
    return (end - start) / sizeof(Book);
}

Book readData(std::fstream &data, int position) {
    Book readBook;
    data.seekg((position - 1) * sizeof(Book), std::ios::beg);
    data.read(reinterpret_cast<char*>(&readBook), sizeof(Book));
    return readBook;
}

void addBook(std::fstream &data) {
    Book inputBook;
    int size = getDataSize(data);

    std::cout << "Data size: " << size << std::endl;

    if(size == 0) {
        inputBook.primary_key = 1;
    } else {
        Book lastBook = readData(data, size);
        std::cout << "Primary Key: " << lastBook.primary_key << std::endl;
        inputBook.primary_key = lastBook.primary_key + 1;
    }

    std::cout << "Input title: ";
    std::cin.getline(inputBook.title, 100);
    std::cout << "Input year: ";
    std::cin >> inputBook.year;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Input author: ";
    std::cin.getline(inputBook.author, 100);
    std::cout << "Input publisher: ";
    std::cin.getline(inputBook.publisher, 100);
    inputBook.is_deleted = false;

    writeData(data, size + 1, inputBook);
}

void displayBook(std::fstream &data) {
    int size = getDataSize(data);
    Book showBook;

    // Print table header
    std::cout << "+-------+----------------+----------------------------------+-----------+-------------------------------+------------------------------+" << std::endl;
    std::cout << "| " << std::left << std::setw(6) << "No."
              << "| " << std::setw(15) << "Primary Key"
              << "| " << std::setw(33) << "Title"
              << "| " << std::setw(10) << "Year"
              << "| " << std::setw(30) << "Author"
              << "| " << std::setw(28) << "Publisher" << " |" << std::endl;
    std::cout << "+-------+----------------+----------------------------------+-----------+-------------------------------+------------------------------+" << std::endl;

    // Print table rows
    for (int i = 1; i <= size; i++) {
        showBook = readData(data, i);
        if (!showBook.is_deleted) {
            std::cout << "| " << std::left << std::setw(6) << i
                      << "| " << std::setw(15) << showBook.primary_key
                      << "| " << std::setw(33) << showBook.title
                      << "| " << std::setw(10) << showBook.year
                      << "| " << std::setw(30) << showBook.author
                      << "| " << std::setw(28) << showBook.publisher << " |" << std::endl;
        }
    }

    // Print table footer
    std::cout << "+-------+----------------+----------------------------------+-----------+-------------------------------+------------------------------+" << std::endl;
}

void updateData(std::fstream &data) {
    int id;
    Book updateBook;
    int size = getDataSize(data);

    if (size == 0) {
        std::cout << "Database is empty, No data to update, Please add data first -_-" << std::endl;
        return;
    }

    std::cout << "Choose the id want to update the data: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    updateBook = readData(data, id);

    while (id < 1 || id > size || updateBook.is_deleted) {
        std::cout << "Invalid position to update. Please enter a valid position: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        updateBook = readData(data, id);
    }

    std::cout << "\nYour option" << std::endl;
    std::cout << "Title: " << updateBook.title << std::endl;
    std::cout << "Year: " << updateBook.year << std::endl;
    std::cout << "Author: " << updateBook.author << std::endl;
    std::cout << "Publisher: " << updateBook.publisher << std::endl;

    std::cout << "\nUpdate data from your option" << std::endl;
    std::cout << "Title: ";
    std::cin.getline(updateBook.title, 100);
    std::cout << "Year: ";
    std::cin >> updateBook.year;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Author: ";
    std::cin.getline(updateBook.author, 100);
    std::cout << "Publisher: ";
    std::cin.getline(updateBook.publisher, 100);

    writeData(data, id, updateBook);
    std::cout << "Your option has been updated ^_^" << std::endl;
}

void deleteData(std::fstream &data) {
    int id, size;
    Book book;
    size = getDataSize(data);

    if (size == 0) {
        std::cout << "Database is empty, No data to delete, Please add data first -_-" << std::endl;
        return;
    }

    std::cout << "Choose the data will be deleted: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (id < 1 || id > size) {
        std::cout << "Invalid position to delete. Please enter a valid position: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    book = readData(data, id);
    book.is_deleted = true;
    writeData(data, id, book);

    std::cout << "Book with ID " << id << " has been marked as deleted." << std::endl;
}

void mark(std::fstream &data, int position, bool *marked) {
    Queue queue;
    queue.enqueue(position);
    while (!queue.isEmpty()) {
        int pos = queue.frontElement();
        queue.dequeue();
        if (!marked[pos - 1]) {
            marked[pos - 1] = true;
            Book book = readData(data, pos);
            // Add additional logic if you have references in books
        }
    }
}

void sweep(std::fstream &data) {
    int size = getDataSize(data);
    bool *marked = new bool[size];

    for (int i = 1; i <= size; ++i) {
        Book book = readData(data, i);
        if (!book.is_deleted) {
            mark(data, i, marked);
        }
    }

    std::fstream temp;
    temp.open("temp.dat", std::ios::trunc | std::ios::out | std::ios::binary);

    int offset = 0;
    for (int i = 1; i <= size; ++i) {
        Book tempData = readData(data, i);
        if (!marked[i - 1] || tempData.is_deleted) {
            offset++;
        } else {
            writeData(temp, i - offset, tempData);
        }
    }

    temp.close();
    data.close();
    remove("database.bin");
    rename("temp.dat", "database.bin");
    data.open("database.bin", std::ios::in | std::ios::out | std::ios::binary);
}

void gc(std::fstream &data) {
    std::cout << "Starting garbage collection..." << std::endl;
    std::cout << "This process will delete the data that has been marked as deleted" << std::endl;
    sweep(data);
    std::cout << "Garbage collection completed." << std::endl;
}
