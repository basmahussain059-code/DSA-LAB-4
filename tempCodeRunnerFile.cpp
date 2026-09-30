#include <iostream>
#include <string>
using namespace std;

class Song {
public:
    int songID;
    string songName;
    string duration;   // MM:SS
    Song* next;
    Song* prev;
    Song(int id, string name, string dur)
        : songID(id), songName(name), duration(dur), next(NULL), prev(NULL) {}
};

class Playlist {
private:
    Song* head;
    Song* tail;      // makes addSong O(1)
    Song* current;
public:
    Playlist() : head(NULL), tail(NULL), current(NULL) {}

    // FIX: free all nodes (original leaked memory)
    ~Playlist() {
        Song* t = head;
        while (t) { Song* n = t->next; delete t; t = n; }
    }

    void addSong(int id, string name, string dur) {
        Song* s = new Song(id, name, dur);
        if (!head) { head = tail = current = s; return; }
        tail->next = s;
        s->prev = tail;
        tail = s;
    }

    void deleteSong(int id) {
        Song* t = head;
        while (t && t->songID != id) t = t->next;
        if (!t) { cout << "Song not found!\n"; return; }

        // FIX: keep 'current' valid (original left a dangling pointer)
        if (t == current) current = t->next ? t->next : t->prev;

        if (t->prev) t->prev->next = t->next; else head = t->next;
        if (t->next) t->next->prev = t->prev; else tail = t->prev;  // FIX: tail update
        delete t;
        cout << "Song Deleted Successfully!\n";
    }

    void displayForward() {
        if (!head) { cout << "Playlist is empty.\n"; return; }
        cout << "\nPlaylist Forward:\n";
        for (Song* t = head; t; t = t->next)
            cout << t->songID << " " << t->songName << " " << t->duration << endl;
    }

    void displayBackward() {
        if (!tail) { cout << "Playlist is empty.\n"; return; }
        cout << "\nPlaylist Backward:\n";
        for (Song* t = tail; t; t = t->prev)
            cout << t->songID << " " << t->songName << " " << t->duration << endl;
    }

    void searchSong(int id) {
        for (Song* t = head; t; t = t->next) {
            if (t->songID == id) {
                cout << "\nSong Found\nID: " << t->songID
                     << "\nName: " << t->songName
                     << "\nDuration: " << t->duration << endl;
                return;
            }
        }
        cout << "Song Not Found!\n";
    }

    void playNext() {
        if (!current) { cout << "Playlist Empty\n"; return; }
        if (current->next) current = current->next;
        else cout << "(End of playlist) ";
        cout << "Playing: " << current->songName << endl;
    }

    void playPrevious() {
        if (!current) { cout << "Playlist Empty\n"; return; }
        if (current->prev) current = current->prev;
        else cout << "(Start of playlist) ";
        cout << "Playing: " << current->songName << endl;
    }

    void reversePlaylist() {
        Song* t = head;
        Song* last = NULL;
        while (t) {
            last = t->prev;
            t->prev = t->next;
            t->next = last;
            t = t->prev;
        }
        Song* oldHead = head;
        head = tail;
        tail = oldHead;
        cout << "Playlist Reversed!\n";
    }
};

int main() {
    Playlist p;
    int choice, id;
    string name, duration;
     cout << "\n=== PLAYLIST MANAGEMENT SYSTEM ===";
    cout << "\n1. Add Song\n2. Delete Song\n3. Display Playlist Forward"
                "\n4. Display Playlist Backward\n5. Search Song"
                "\n6. Play Next Song\n7. Play Previous Song"
                "\n8. Reverse Playlist\n" ;
                cout << "\n  9. Exit";
    do {
        cout << " \nEnter Choice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); choice = 0; }

        switch (choice) {
        case 1:
            cout << "Enter Song ID: "; cin >> id; cin.ignore();
            cout << "Enter Song Name: "; getline(cin, name);
            cout << "Enter Duration (MM:SS): "; getline(cin, duration);
            p.addSong(id, name, duration);
            cout << "Song Added Successfully!\n";
            break;
        case 2: cout << "Enter Song ID to Delete: "; cin >> id; p.deleteSong(id); break;
        case 3: p.displayForward(); break;
        case 4: p.displayBackward(); break;
        case 5: cout << "Enter Song ID to Search: "; cin >> id; p.searchSong(id); break;
        case 6: p.playNext(); break;
        case 7: p.playPrevious(); break;
        case 8: p.reversePlaylist(); break;
        case 9: cout << "Exiting Program...\n"; break;
        default: cout << "Invalid Choice!\n";
        }
    } while (choice != 9);
    return 0;
}