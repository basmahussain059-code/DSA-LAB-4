#include <iostream>
using namespace std;

class Node {
public:
    int id;
    Node* next;
    Node(int v) : id(v), next(NULL) {}
};

class CircularLinkedList {
private:
    Node* head;
public:
    CircularLinkedList() : head(NULL) {}
    ~CircularLinkedList() {
        if (!head) return;
        Node* t = head->next;
        while (t != head) { Node* n = t->next; delete t; t = n; }
        delete head;
    }

    void createCircle(int n) {
        if (n <= 0) return;
        head = new Node(1);
        Node* temp = head;
        for (int i = 2; i <= n; i++) {
            temp->next = new Node(i);
            temp = temp->next;
        }
        temp->next = head;          // make circular
    }

    void josephus(int k) {
        if (!head || k < 1) return;
        Node* prev = head;
        while (prev->next != head) prev = prev->next;
        Node* curr = head;

        while (curr->next != curr) {
            for (int i = 1; i < k; i++) { prev = curr; curr = curr->next; }
            cout << "Eliminated Person: " << curr->id << endl;
            prev->next = curr->next;      // unlink
            Node* del = curr;
            curr = curr->next;
            delete del;
        }
        cout << "\nSurvivor is Person: " << curr->id << endl;
        head = curr;
    }
};

int main() {
    int N, K;
    cout << "Enter Number of People: "; cin >> N;
    cout << "Enter Step Count (K): ";   cin >> K;
    if (N <= 0 || K <= 0) { cout << "N and K must be positive.\n"; return 1; }

    CircularLinkedList circle;
    circle.createCircle(N);
    cout << "\nElimination Order:\n";
    circle.josephus(K);
    return 0;
}