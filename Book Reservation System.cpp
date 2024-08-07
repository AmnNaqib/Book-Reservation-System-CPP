// DATE       : DEC 1, 2023
// DESCRIPTION: This program provides a bookstore interface
//              for users to interact with, select books,
//              manage deliveries, and receive receipts
//              based on their choices and membership status.

// lib declaration
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

// text color
#define RED_TEXT "\033[31m"
#define RESET_COLOR "\033[0m"

// Global variables declaration
const int BOOKNUM = 5;
const int DESTNUM = 5;
const int MEMNUM = 3;

// structured data types
struct bookType {
    int code;
    string title;
    double price;
};

struct destType {
    int code;
    string place;
    double price;
};

struct memType {
    int code;
    string name;
    double price;
};

// class declaration
class BookStore {
    // member declaration
   protected:
    string name;
    string id;
    string phoneNum;

    int bookCode;
    int desCode;

    double price;
    double desFee;

    double payment;
    double balance;

    static double total;
    static double discount;
    static int quantity[BOOKNUM];

    fstream file;
    string filename;

    bookType books[BOOKNUM] = {{1, "The Great Gatsby", 86.0},
                               {2, "Gone Girl", 53.0},
                               {3, "Dune", 119.0},
                               {4, "The Name of the Wind", 51.0},
                               {5, "Little Women", 32.0}};

    destType destinations[DESTNUM] = {
        {1, "Klang Valley", 10.0}, {2, "Negeri Sembilan", 20.0},
        {3, "Melaka", 35.0},       {4, "Johor", 40.0},
        {5, "Perak", 50},
    };

    memType members[MEMNUM] = {{1, "GOLD", 0.0}, {2, "SILVER", 0.0}, {3, "BRONZE", 0.0}};

   public:
    // pure virtual functions
    virtual void processOrder() = 0;
    virtual void saveReceipt() = 0;
    virtual void displayReceipt() = 0;

    // construstor
    BookStore(string n, string i, string p, int b, int q)
        : name(n), id(i), phoneNum(p), bookCode(b) {
        price = 0;
        desFee = 0;
        payment = 0;
        balance = 0;
        filename = id;
        price = books[bookCode - 1].price * q;
        total += price;
        quantity[bookCode - 1] += q;
    }

    // display table of books
    void displayBooks() {
        bookType books[BOOKNUM] = {{1, "The Great Gatsby", 86.0},
                                   {2, "Gone Girl", 53.0},
                                   {3, "Dune", 119.0},
                                   {4, "The Name of the Wind", 51.0},
                                   {5, "Little Women", 32.0}};
        cout << "\nTable of Books\n";
        cout << "+------+----------------------------------------+-----------"
                "---+\n";
        cout << "| Code | Title                                  | Price "
                "(RM)   |\n";
        cout << "+------+----------------------------------------+-----------"
                "---+\n";

        for (int i = 0; i < BOOKNUM; ++i) {
            cout << "| " << setw(4) << left << books[i].code << " | " << setw(38)
                 << books[i].title << " | " << setw(12) << fixed << setprecision(2)
                 << books[i].price << " |\n";
        }

        cout << "+------+----------------------------------------+-----------"
                "---+\n";
    }

    // display table of destinations
    void displayDestinations() {
        cout << "\nTable of Destinations\n";
        cout << "+------+------------------------+-----------------+\n";
        cout << "| Code | Destination            | Delivery charge |\n";
        cout << "+------+------------------------+-----------------+\n";

        for (int i = 0; i < DESTNUM; ++i) {
            cout << "| " << setw(4) << left << destinations[i].code << " | " << setw(22)
                 << destinations[i].place << " | " << setw(15) << fixed << setprecision(2)
                 << destinations[i].price << " |\n";
        }

        cout << "+------+------------------------+-----------------+\n";
    }

    // display member promotion table
    void displayMemberPromotionTable() {
        cout << "\nHERE'S PROMOTION FOR YOU\n\n";
        cout << "+----------------------------+-------------+-----------+\n";
        cout << "|               | Gold       | Silver      | Bronze    |\n";
        cout << "+Total Price(RM)+------------+-------------------------+\n";
        cout << "|               | Discount   |  Discount   |  Discount |\n";
        cout << "+---------------+------------+-------------------------+\n";
        cout << "|     > 500     | 80%        | 70%         | 60%       |\n";
        cout << "|   200 - 500   | 70%        | 60%         | 40%       |\n";
        cout << "|     < 200     | 50%        | 30%         | 20%       |\n";
        cout << "+---------------+------------+-------------+-----------+\n";
    }

    // display non member promotion table
    void displayNonMemberPromotionTable() {
        cout << "\nHERE'S PROMOTION FOR YOU\n\n";
        cout << "+-----------------+------------+\n";
        cout << "| Total Price(RM) | Discount   |\n";
        cout << "+-----------------+------------+\n";
        cout << "|      > 500      | 15%        |\n";
        cout << "|    200 - 500    | 10%        |\n";
        cout << "|      < 200      | 5%         |\n";
        cout << "+-----------------+------------+\n";
    }

    // set delivery fee
    void Delivery(int d) {
        desCode = d;
        if (desCode >= 1 && desCode <= 5) {
            desFee = destinations[desCode - 1].price;
        }
    }

    // display total and input payment
    void displayTotal() {
        cout << "Total books  : " << calculateTotalQuantity() << endl;
        cout << "Net Price    : RM " << fixed << setprecision(2)
             << (total + desFee - discount) << endl;
        do {
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Total Payment: RM ";
            cin >> payment;
            if (payment < 0 || payment < (total + desFee - discount)) {
                cout << RED_TEXT << "\t\t    *******************************\n\n";
                cout << "\t\t             INVALID INPUT        " << std::endl;
                cout << "\n\t\t    *******************************";
                cout << "\n\t\t    Please re-enter\n";
                cout << RESET_COLOR;  // Reset color to default
            }
        } while (payment < 0 || payment < (total + desFee - discount) || cin.fail());
        balance = payment - (total + desFee - discount);
    }

    // recursive function to count the num of books
    int calculateTotalQuantity(int index = 0) {
        if (index == BOOKNUM) {
            return 0;
        }
        return quantity[index] + calculateTotalQuantity(index + 1);
    }
};

double BookStore::total = 0;
double BookStore::discount = 0;
int BookStore::quantity[BOOKNUM] = {0};

class Member : public BookStore {
   private:
    int memCode;

   public:
    // constructor
    Member(string n, string i, string p, int bookCode, int q, int t)
        : BookStore(n, i, p, bookCode, q), memCode(t) {}

    // set discount rate and discount
    void processOrder() {
        double discountRate = 0.0;
        if (memCode == 1) {
            if (total > 500)
                discountRate = 0.8;
            else if (total >= 200 && total <= 500)
                discountRate = 0.7;
            else
                discountRate = 0.5;
        } else if (memCode == 2) {
            if (total > 500)
                discountRate = 0.7;
            else if (total >= 200 && total <= 500)
                discountRate = 0.6;
            else
                discountRate = 0.3;
        } else if (memCode == 3) {
            if (total > 500)
                discountRate = 0.6;
            else if (total >= 200 && total <= 500)
                discountRate = 0.4;
            else
                discountRate = 0.2;
        }

        discount = total * discountRate;
    }

    // save receipt to fstream file
    virtual void saveReceipt() {
        file.open((filename + ".txt").c_str(), ios::in);
        if (!file.fail()) {
            file.close();
            filename += "(1)";
            file.open((filename + ".txt").c_str(), std::ios::in);
            while (true) {
                if (file.fail()) {
                    file.close();
                    break;
                } else {
                    file.close();
                    int count;
                    string temp = "";
                    for (int i = 0; i < filename.size(); i++) {
                        if (filename[i] == ')') {
                            break;
                        } else {
                            temp += filename[13 + i];
                        }
                    }
                    count = atoi(temp.c_str());
                    count++;
                    stringstream ss;
                    ss << count;
                    temp = ss.str();

                    filename[12] = '(';
                    for (int i = 0; i < temp.size(); i++) {
                        filename[13 + i] = temp[i];
                    }
                    filename[13 + temp.size()] = ')';

                    file.open((filename + ".txt").c_str(), ios::in);
                }
            }
        } else {
            file.close();
        }
        filename += ".txt";
        file.open(filename.c_str(), ios::out);

        if (file.fail()) {
            cerr << "Error" << endl;
        }

        time_t tt;
        struct tm* ti;
        time(&tt);
        ti = localtime(&tt);

        // Header
        // Header
        file << "============================================================"
                "============"
             << endl;
        file << "                         BUKKU'S BOOKSTORE RECEIPT          "
                "            "
             << endl;
        file << "============================================================"
                "============"
             << endl;
        file << "DATE        | " << asctime(ti);
        file << "NAME        | " << name << endl;
        file << "IC          | " << id << endl;
        file << "PHONE       | " << phoneNum << endl;
        file << "MEMBERSHIP  | " << members[memCode - 1].name << endl;
        if (desFee != 0) {
            file << "DELIVERED TO| " << destinations[desCode - 1].place << endl;
        } else {
            file << "DELIVERED TO| "
                 << "SELF-PICKUP" << endl;
        }
        // Book Details
        file << "+-----------------------+----------+------------+-----------"
                "----------+\n";
        file << "| Book                  | Quantity | Unit Price | Total "
                "Price         |\n";
        file << "+-----------------------+----------+------------+-----------"
                "----------+\n";
        for (int i = 0; i < BOOKNUM; ++i) {
            if (quantity[i] != 0) {
                file << "|" << setw(22) << left << books[i].title << setw(9) << right
                     << quantity[i] << setw(12) << right << fixed << setprecision(2)
                     << books[i].price << setw(20) << right << fixed << setprecision(2)
                     << books[i].price * quantity[i] << setw(8) << " |\n";
            }
        }
        file << "+-----------------------+----------+------------+-----------"
                "----------+\n";

        // Totals
        file << "\nTotal books   : " << calculateTotalQuantity() << endl;
        file << "Total Price   : RM " << fixed << setprecision(2) << total << endl;
        file << "Discount      :-RM " << fixed << setprecision(2) << discount << endl;
        if (desFee != 0)
            file << "Delivery fee  : RM" << fixed << setprecision(2) << desFee << endl;

        file << "Net price     : RM " << fixed << setprecision(2)
             << total + desFee - discount << endl;
        file << "Total payment : RM " << fixed << setprecision(2) << payment << endl;
        file << "Balance       : RM " << fixed << setprecision(2) << balance << endl;

        // Footer
        file << "============================================================"
                "============"
             << endl;
        file << "                    THANK YOU FOR PURCHASING WITH US        "
                "            "
             << endl;
        file << "============================================================"
                "============"
             << endl;

        file.close();
    }

    // display receipt from fstream file
    virtual void displayReceipt() {
        string line;
        file.open(filename.c_str(), ios::in);
        if (file.fail()) {
            cerr << "Error" << endl;
        }
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};

class NonMember : public BookStore {
   public:
    // constructor
    NonMember(string n, string i, string p, int bookCode, int q)
        : BookStore(n, i, p, bookCode, q) {}

    // set discount rate and discount // error handling implementation
    void processOrder() {
        double discountRate = 0.0;
        try {
            if (total > 500)
                discountRate = 0.15;
            else if (total >= 200 && total <= 500)
                discountRate = 0.1;
            else
                throw 0.05;
        } catch (double discountRATE) {
            discountRate = discountRATE;
        }
        discount = total * discountRate;
    }

    // save receipt to fstream file
    virtual void saveReceipt() override {
        file.open((filename + ".txt").c_str(), ios::in);
        if (!file.fail()) {
            file.close();
            filename += "(1)";
            file.open((filename + ".txt").c_str(), std::ios::in);
            while (true) {
                if (file.fail()) {
                    file.close();
                    break;
                } else {
                    file.close();
                    int count;
                    string temp = "";
                    for (int i = 0; i < filename.size(); i++) {
                        if (filename[i] == ')') {
                            break;
                        } else {
                            temp += filename[13 + i];
                        }
                    }
                    count = atoi(temp.c_str());
                    count++;
                    stringstream ss;
                    ss << count;
                    temp = ss.str();

                    filename[12] = '(';
                    for (int i = 0; i < temp.size(); i++) {
                        filename[13 + i] = temp[i];
                    }
                    filename[13 + temp.size()] = ')';

                    file.open((filename + ".txt").c_str(), ios::in);
                }
            }
        } else {
            file.close();
        }
        filename += ".txt";
        file.open(filename.c_str(), ios::out);
        if (file.fail()) {
            cerr << "Error" << endl;
        }
        time_t tt;
        struct tm* ti;
        time(&tt);
        ti = localtime(&tt);

        // Header
        file << "============================================================"
                "============"
             << endl;
        file << "                         BUKKU'S BOOKSTORE RECEIPT          "
                "            "
             << endl;
        file << "============================================================"
                "============"
             << endl;
        file << "DATE        | " << asctime(ti);
        file << "NAME        | " << name << endl;
        file << "IC          | " << id << endl;
        file << "PHONE       | " << phoneNum << endl;
        file << "MEMBERSHIP  | NONE" << endl;
        // file << "\nReception type: ";
        if (desFee != 0) {
            file << "DELIVERED TO| " << destinations[desCode - 1].place << endl;
        } else {
            file << "DELIVERED TO| "
                 << "SELF-PICKUP" << endl;
        }

        // Book Details
        file << "+-----------------------+----------+------------+-----------"
                "----------+\n";
        file << "| Book                  | Quantity | Unit Price | Total "
                "Price         |\n";
        file << "+-----------------------+----------+------------+-----------"
                "----------+\n";
        for (int i = 0; i < BOOKNUM; ++i) {
            if (quantity[i] != 0) {
                file << "|" << setw(22) << left << books[i].title << setw(9) << right
                     << quantity[i] << setw(12) << right << fixed << setprecision(2)
                     << books[i].price << setw(20) << right << fixed << setprecision(2)
                     << books[i].price * quantity[i] << setw(8) << " |\n";
            }
        }
        file << "+-----------------------+----------+------------+-----------"
                "----------+\n";

        // Totals
        file << "\nTotal books   : " << calculateTotalQuantity() << endl;
        file << "Total Price   : RM " << fixed << setprecision(2) << total << endl;
        file << "Discount      :-RM " << fixed << setprecision(2) << discount << endl;
        if (desFee != 0)
            file << "Delivery fee  : RM" << fixed << setprecision(2) << desFee << endl;

        file << "Net price     : RM " << fixed << setprecision(2)
             << total + desFee - discount << endl;
        file << "Total payment : RM " << fixed << setprecision(2) << payment << endl;
        file << "Balance       : RM " << fixed << setprecision(2) << balance << endl;

        // Footer
        file << "============================================================"
                "============"
             << endl;
        file << "                    THANK YOU FOR PURCHASING WITH US        "
                "            "
             << endl;
        file << "============================================================"
                "============"
             << endl;

        file.close();
    }

    // display receipt from fstream file
    void displayReceipt() override {
        string line;
        file.open(filename.c_str(), ios::in);
        if (file.fail()) {
            cerr << "Error" << endl;
        }
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};

// error message
void invalid() {
    cout << RED_TEXT << "\t\t    *******************************\n\n";
    cout << "\t\t             INVALID INPUT        " << std::endl;
    cout << "\n\t\t    *******************************";
    cout << "\n\t\t    Please re-enter\n";
    cout << RESET_COLOR;  // Reset color to default
}

int main() {
    BookStore* user = NULL;
    // Inputs declaration
    string name, phoneNum;
    string id;
    int input1, input2, input3, input4, input5, memCode;
    bool valid;

    // Menu
    cout << "\n\n\n\n\t\t\t\t      __...--~~~~~-._   _.-~~~~~--...__" << endl;
    cout << "\t\t\t\t    //               `V'               \\\\ " << endl;
    cout << "\t\t\t\t   //                 |                 \\\\ " << endl;
    cout << "\t\t\t\t  //__...--~~~~~~-._  |  _.-~~~~~~--...__\\\\ " << endl;
    cout << "\t\t\t\t //__.....----~~~~._\\ | /_.~~~~----.....__\\\\" << endl;
    cout << "\t\t\t\t====================\\\\|//====================" << endl;
    cout << "\t\t\t\t                    `---`" << endl;
    std::cout << ".______    __    __   __  ___  __  ___  __    __     __     "
                 "_______.\n"
                 "|   _  \\  |  |  |  | |  |/  / |  |/  / |  |  |  |   (_ )   "
                 "/       |\n"
                 "|  |_)  | |  |  |  | |  '  /  |  '  /  |  |  |  |    |/   | "
                 "  (----`\n"
                 "|   _  <  |  |  |  | |    <   |    <   |  |  |  |          "
                 "\\   \\    \n"
                 "|  |_)  | |  `--'  | |  .  \\  |  .  \\  |  `--'  |      "
                 ".----)   |   \n"
                 "|______/   \\______/  |__|\\__\\ |__|\\__\\  \\______/      "
                 " |_______/    \n\n";

    system("pause");
    system("cls");
    cout << "\n----------------------------------BUKKU'S "
            "BOOKSTORE----------------------------------\n";

    // ask for name
    do {
        cout << "Please enter your name        | ";
        getline(cin, name);
        if (name == "") {
            invalid();
        }
    } while (name == "");

    // ask for ic num
    do {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter IC number    (without -)| ";
        cin >> id;
        valid = true;
        for (int i = 0; i < id.size(); i++) {
            if (id.size() < 12 || id.size() > 12) {
                valid = false;
                invalid();
                break;
            }
            if (isdigit(id[i]) == 0) {
                valid = false;
                invalid();
                break;
            }
        }
    } while (cin.fail() || valid == 0);

    // ask for phone num
    do {
        cout << "Enter phone number (without -)| ";
        cin >> phoneNum;
        valid = true;
        for (int i = 0; i < phoneNum.size(); i++) {
            if (isdigit(phoneNum[i]) == 0) {
                valid = false;
                invalid();
                break;
            }
        }
    } while (valid == 0);

    // ask for membership status
    cout << "________________\n";
    cout << " 1  - MEMBER    |\n"
         << " 2  - NON-MEMBER|" << endl;
    do {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        // display indicator
        cout << "Are you a member| ";
        cin >> input1;
        if (input1 != 1 && input1 != 2) {
            invalid();
        }
    } while (input1 != 1 && input1 != 2 || cin.fail());

    // if a member, ask for member type
    if (input1 == 1) {
        user->displayMemberPromotionTable();
        cout << "________________\n";
        cout << "  1  -  GOLD    |\n"
             << "  2  -  SILVER  |\n"
             << "  3  -  BRONZE  |" << endl;
        do {
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Member type     | ";
            cin >> memCode;
            if (memCode < 1 || memCode > 3) {
                invalid();
            }
        } while (memCode < 1 || memCode > 3 || cin.fail());
    } else {
        user->displayNonMemberPromotionTable();
        system("pause");
    }
    system("cls");

    // Display table of books for the user to choose
    cout << "\nHere's the book choices for you \n";
    user->displayBooks();
    // Prompt user to input book info
    // User can press '0' to stop
    while (true) {
        do {
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "\nEnter Book Code (1-5, 0 to stop)| ";
            cin >> input2;
            if (input2 < 0 || input2 > 5 || cin.fail()) invalid();
        } while (input2 < 0 || input2 > 5 || cin.fail());
        if (input2 == 0) break;
        do {
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Enter Quantity                  | ";
            cin >> input3;
            if (input3 < 0) {
                invalid();
            }
        } while (input3 < 0 || cin.fail());
        // Process the order based on user type (Member/Non-Member)
        // creating object instance
        if (input1 == 1) {
            user = new Member(name, id, phoneNum, input2, input3, memCode);
        } else {
            user = new NonMember(name, id, phoneNum, input2, input3);
        }
    }

    user->processOrder();
    system("cls");

    // Ask for delivery 1-deli 2-pic
    cout << "_________________\n";
    cout << "  1  -  DELIVERY |\n"
         << "  2  -  PICKUP   |" << endl;
    do {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Your selection   | ";
        cin >> input4;
        if (input4 != 1 && input4 != 2) {
            invalid();
        }
    } while (input4 != 1 && input4 != 2 || cin.fail());

    // if delivery, ask for location
    if (input4 == 1) {
        user->displayDestinations();
        do {
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "\nChoose your Destination Code    | ";
            cin >> input5;
            if (input5 < 1 || input5 > 5) {
                invalid();
            }
        } while (input5 < 1 || input5 > 5 || cin.fail());
        user->Delivery(input5);
    }
    system("cls");
    // Display total
    user->displayTotal();
    system("cls");
    // save receipt
    user->saveReceipt();
    // read n display receipt
    user->displayReceipt();
    delete user;  // Free allocated memory

    return 0;
}

