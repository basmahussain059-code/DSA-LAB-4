#include <iostream>
#include <string>
using namespace std;

class Node {
public:
    int bit;
    Node *next, *prev;
    Node(int b) : bit(b), next(NULL), prev(NULL) {}
};

class BinaryDLL {
private:
    Node *head, *tail;
    int size;

    void insertEnd(int b) {
        Node* n = new Node(b);
        if (!head) head = tail = n;
        else { tail->next = n; n->prev = tail; tail = n; }
        size++;
    }
    void insertFront(int b) {
        Node* n = new Node(b);
        if (!head) head = tail = n;
        else { n->next = head; head->prev = n; head = n; }
        size++;
    }
    void clear() {
        while (head) { Node* n = head->next; delete head; head = n; }
        tail = NULL; size = 0;
    }
    void padToBlocks() {                 // 8-bit grouping
        while (size % 8 != 0 || size == 0) insertFront(0);
    }
public:
    BinaryDLL() : head(NULL), tail(NULL), size(0) {}
    BinaryDLL(const BinaryDLL& o) : head(NULL), tail(NULL), size(0) {
        for (Node* t = o.head; t; t = t->next) insertEnd(t->bit);
    }
    BinaryDLL& operator=(const BinaryDLL& o) {
        if (this != &o) { clear(); for (Node* t = o.head; t; t = t->next) insertEnd(t->bit); }
        return *this;
    }
    ~BinaryDLL() { clear(); }
    bool store(const string& s) {
        if (s.empty()) return false;
        for (char c : s) if (c != '0' && c != '1') return false;  
        clear();
        for (char c : s) insertEnd(c - '0');
        padToBlocks();
        return true;
    }

    void display() const {
        int i = 0;
        for (Node* t = head; t; t = t->next, i++) {
            if (i && i % 8 == 0) cout << ' ';
            cout << t->bit;
        }
        cout << endl;
    }
    BinaryDLL onesComplement() const {
        BinaryDLL r(*this);
        for (Node* t = r.head; t; t = t->next) t->bit ^= 1;
        return r;
    }
    static BinaryDLL add(const BinaryDLL& a, const BinaryDLL& b, bool keepOverflow = true) {
        BinaryDLL r;
        Node *x = a.tail, *y = b.tail;
        int carry = 0;
        while (x || y) {
            int sum = carry + (x ? x->bit : 0) + (y ? y->bit : 0);
            r.insertFront(sum % 2);
            carry = sum / 2;
            if (x) x = x->prev;
            if (y) y = y->prev;
        }
        if (carry && keepOverflow) r.insertFront(1);
        r.padToBlocks();
        return r;
    }
    BinaryDLL twosComplement() const {
        BinaryDLL one; one.store("1");
        BinaryDLL c = onesComplement();
        BinaryDLL r = add(c, one, false);
        // keep same width as original
        while (r.size > size) { Node* n = r.head; r.head = n->next; r.head->prev = NULL; delete n; r.size--; }
        return r;
    }

    // 5. Multiplication: shift + add
    static BinaryDLL multiply(const BinaryDLL& a, const BinaryDLL& b) {
        BinaryDLL result; result.store("0");
        BinaryDLL shifted(a);                       // multiplicand, shifted left each step
        for (Node* m = b.tail; m; m = m->prev) {    // multiplier bits, LSB first
            if (m->bit) result = add(result, shifted);
            shifted.insertEnd(0);                   // shift left
        }
        return result;
    }

    // 6. Decimal Conversion: binary to decimal
    unsigned long long toDecimal() const {
        unsigned long long d = 0;
        for (Node* t = head; t; t = t->next) d = d * 2 + t->bit;
        return d;
    }
};

int main() {
    BinaryDLL num, a, b;
    int choice; string s1, s2;
      cout << "\n===== Binary DLL Menu =====\n"
                "1. Store Binary Number\n2. Display Binary Number\n"
                "3. 1's Complement\n4. 2's Complement\n5. Binary Addition\n"
                "6. Binary Multiplication\n7. Convert to Decimal\n8. Exit\n";
    do {
      cout<<"Enter Choice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); choice = 0; }
        switch (choice) {
        case 1:
            cout << "Enter Binary Number: "; cin >> s1;
            if (!num.store(s1)) cout << "Invalid! Use only 0 and 1.\n";
            break;
        case 2: cout << "Binary Number: "; num.display(); break;
        case 3: cout << "1's Complement: "; num.onesComplement().display(); break;
        case 4: cout << "2's Complement: "; num.twosComplement().display(); break;
        case 5: case 6:
            cout << "Enter First Binary: ";  cin >> s1;
            cout << "Enter Second Binary: "; cin >> s2;
            if (!a.store(s1) || !b.store(s2)) { cout << "Invalid! Use only 0 and 1.\n"; break; }
            if (choice == 5) { cout << "Sum = ";     BinaryDLL::add(a, b).display(); }
            else             { cout << "Product = "; BinaryDLL::multiply(a, b).display(); }
            break;
        case 7: cout << "Decimal = " << num.toDecimal() << endl; break;
        case 8: cout << "Program Ended\n"; break;
        default: cout << "Invalid Choice\n";
        }
    } while (choice != 8);
    return 0;
}