#include <iostream>
#include <string>
#include <fstream>
#include <vector> 
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <random> 
using namespace std;

// Song structure with added album information
struct Song {
    string title;
    string artist;
    string album;
    string genre;
    int duration; // Duration in seconds
    int rating;
    int playCount;
    bool favorite;
    Song* prev;
    Song* next;

    Song(string t, string a, string al, int d) 
        : title(t), artist(a), album(al), genre(""), duration(d), rating(0), 
          playCount(0), favorite(false), prev(nullptr), next(nullptr) {}
};

// Binary Search Tree node to organize songs
struct TreeNode {
    Song* song;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Song* s) : song(s), left(nullptr), right(nullptr) {}
};

// Playlist class with DSA enhancements
class Playlist {
private:
    Song* head;
    Song* tail;
    Song* currentSong;
    int totalDuration;
    bool repeat;
    TreeNode* root;  // Root of BST for songs
    vector<Song*> songVector;  // For sorting operations
    vector<Song*> history; // Track history of played songs

public:
    Playlist() : head(nullptr), tail(nullptr), currentSong(nullptr), totalDuration(0), repeat(false), root(nullptr) {}

    void addSong(string title, string artist, string album, int duration, string genre = "") {
        Song* newSong = new Song(title, artist, album, duration);
        newSong->genre = genre;

        if (head == nullptr) {
            head = tail = newSong;
        } else {
            tail->next = newSong;
            newSong->prev = tail;
            tail = newSong;
        }
        totalDuration += duration;
        cout << "Song added: " << title << " by " << artist << " [" << album << "] [" << genre << "]" << endl;

        root = addToBST(root, newSong);
        songVector.push_back(newSong);
    }

    TreeNode* addToBST(TreeNode* node, Song* song) {
        if (node == nullptr) return new TreeNode(song);

        if (song->title < node->song->title) {
            node->left = addToBST(node->left, song);
        } else {
            node->right = addToBST(node->right, song);
        }
        return node;
    }

    void removeSong(string title) {
        Song* temp = head;
        while (temp != nullptr) {
            if (temp->title == title) {
                totalDuration -= temp->duration;
                if (temp->prev != nullptr) temp->prev->next = temp->next;
                if (temp->next != nullptr) temp->next->prev = temp->prev;
                if (temp == head) head = temp->next;
                if (temp == tail) tail = temp->prev;
                root = removeFromBST(root, title);
                songVector.erase(remove(songVector.begin(), songVector.end(), temp), songVector.end());
                delete temp;
                cout << "Song removed: " << title << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Song not found!" << endl;
    }

    TreeNode* removeFromBST(TreeNode* node, string title) {
        if (!node) return nullptr;
        if (title < node->song->title) {
            node->left = removeFromBST(node->left, title);
        } else if (title > node->song->title) {
            node->right = removeFromBST(node->right, title);
        } else {
            if (!node->left) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }
            TreeNode* temp = minValueNode(node->right);
            node->song = temp->song;
            node->right = removeFromBST(node->right, temp->song->title);
        }
        return node;
    }

    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void displayPlaylist() {
        Song* temp = head;
        if (!temp) {
            cout << "Playlist is empty!" << endl;
            return;
        }
        while (temp) {
            cout << temp->title << " by " << temp->artist << " [" << temp->album << "] [" << temp->duration << " sec]";
            if (temp->favorite) cout << " [Favorite]";
            cout << endl;
            temp = temp->next;
        }
        cout << "Total Playlist Duration: " << totalDuration << " seconds" << endl;
    }

    void playNextSong() {
        if (currentSong == nullptr) {
            currentSong = head;
        } else if (currentSong->next != nullptr) {
            currentSong = currentSong->next;
        } else if (repeat) {
            currentSong = head;
        } else {
            currentSong = nullptr;
        }

        if (currentSong != nullptr) {
            currentSong->playCount++;
            history.push_back(currentSong);
            cout << "Now playing: " << currentSong->title << " by " << currentSong->artist << " [" << currentSong->playCount << " plays]" << endl;
        } else {
            cout << "End of playlist!" << endl;
        }
    }

    void playPreviousSong() {
        if (currentSong != nullptr && currentSong->prev != nullptr) {
            currentSong = currentSong->prev;
        }

        if (currentSong != nullptr) {
            cout << "Now playing: " << currentSong->title << " by " << currentSong->artist << endl;
        } else {
            cout << "Start of playlist!" << endl;
        }
    }

    void playSongByIndex(int index) {
        Song* temp = head;
        for (int i = 0; i < index && temp != nullptr; ++i) {
            temp = temp->next;
        }
        if (temp != nullptr) {
            currentSong = temp;
            cout << "Now playing: " << currentSong->title << " by " << currentSong->artist << endl;
            currentSong->playCount++;
            history.push_back(currentSong);
        } else {
            cout << "Index out of range!" << endl;
        }
    }


void shufflePlaylist() {
    if (songVector.size() <= 1) {
        cout << "Not enough songs to shuffle!" << endl;
        return;
    }

    // Create a random engine
    random_device rd;
    mt19937 g(rd()); // Mersenne Twister for random number generation

    // Shuffle the vector
    shuffle(songVector.begin(), songVector.end(), g);

    cout << "Shuffled Playlist:" << endl;
    for (Song* song : songVector) {
        cout << song->title << " by " << song->artist << " [" << song->album << "]" << endl;
    }
    cout << "Total Playlist Duration: " << totalDuration << " seconds" << endl;
}

    void searchSong(string keyword) {
        Song* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->title.find(keyword) != string::npos || temp->artist.find(keyword) != string::npos) {
                cout << "Found: " << temp->title << " by " << temp->artist << " [" << temp->album << "]" << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "No song found with the keyword: " << keyword << endl;
        }
    }

    void deepSearchByArtistDFS(string artist) {
        cout << "DFS search for artist: " << artist << endl;
        stack<TreeNode*> s;
        TreeNode* current = root;
        while (current != nullptr || !s.empty()) {
            while (current != nullptr) {
                s.push(current);
                current = current->left;
            }

            current = s.top();
            s.pop();

            if (current->song->artist == artist) {
                cout << "Found: " << current->song->title << " by " << current->song->artist << " [" << current->song->album << "]" << endl;
            }

            current = current->right;
        }
    }

    void toggleFavorite(string title) {
        Song* temp = head;
        while (temp != nullptr) {
            if (temp->title == title) {
                temp->favorite = !temp->favorite;
                cout << "Song marked as " << (temp->favorite ? "favorite: " : "not favorite: ") << title << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Song not found!" << endl;
    }

    void displayFavorites() {
        Song* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->favorite) {
                cout << temp->title << " by " << temp->artist << " [" << temp->duration << " sec] [" << temp->album << "]" << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "No favorite songs in the playlist!" << endl;
        }
    }

    void toggleRepeat() {
        repeat = !repeat;
        cout << "Repeat is now " << (repeat ? "on" : "off") << endl;
    }

    void showCurrentSong() {
        if (currentSong != nullptr) {
            cout << "Current song: " << currentSong->title << " by " << currentSong->artist << endl;
        } else {
            cout << "No song is currently playing!" << endl;
        }
    }

    void savePlaylist(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Failed to open the file!" << endl;
            return;
        }
        Song* temp = head;
        while (temp != nullptr) {
            file << temp->title << "," << temp->artist << "," << temp->album << ","
                 << temp->duration << "," << temp->favorite << "," << temp->genre << "," << temp->rating << "," << temp->playCount << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Playlist saved to " << filename << endl;
    }

    void loadPlaylist(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Failed to open the file!" << endl;
            return;
        }

        string title, artist, album, genre, durationStr, favoriteStr, ratingStr, playCountStr;
        int duration, rating, playCount;
        totalDuration = 0;
        while (getline(file, title, ',') && getline(file, artist, ',') && getline(file, album, ',') &&
               getline(file, durationStr, ',') && getline(file, favoriteStr, ',') &&
               getline(file, genre, ',') && getline(file, ratingStr, ',') && getline(file, playCountStr)) {
            duration = stoi(durationStr);
            rating = stoi(ratingStr);
            playCount = stoi(playCountStr);
            Song* newSong = new Song(title, artist, album, duration);
            newSong->favorite = (favoriteStr == "1");
            newSong->rating = rating;
            newSong->playCount = playCount;
            newSong->genre = genre;
            addSong(newSong->title, newSong->artist, newSong->album, newSong->duration, newSong->genre);
        }
        file.close();
        cout << "Playlist loaded from " << filename << endl;
    }

    // Sort playlist by title using vector and lambda function
    void sortPlaylistByTitle() {
        sort(songVector.begin(), songVector.end(), [](Song* a, Song* b) {
            return a->title < b->title;
        });
        cout << "Playlist sorted by title:" << endl;
        for (Song* song : songVector) {
            cout << song->title << " by " << song->artist << " [" << song->album << "]" << endl;
        }
    }

    void rateSong(string title, int rating) {
        Song* temp = head;
        while (temp != nullptr) {
            if (temp->title == title) {
                temp->rating = rating;
                cout << "Rated " << title << " with " << rating << " stars." << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Song not found!" << endl;
    }

    void displayTopRatedSongs() {
        vector<Song*> ratedSongs;
        for (Song* song : songVector)
            if (song->rating > 0) 
                ratedSongs.push_back(song);

        sort(ratedSongs.begin(), ratedSongs.end(), [](Song* a, Song* b) {
            return a->rating > b->rating;
        });

        cout << "Top Rated Songs:" << endl;
        for (Song* song : ratedSongs) {
            cout << song->title << " by " << song->artist << " Rating: " << song->rating << " stars" << endl;
        }
    }

    void displayPlayHistory() {
        if (history.empty()) {
            cout << "No play history!" << endl;
            return;
        }
        cout << "Play History:" << endl;
        for (Song* song : history) {
            cout << song->title << " by " << song->artist << endl;
        }
    }

    void filterByGenre(string genre) {
        bool found = false;
        cout << "Songs in genre [" << genre << "]:" << endl;
        Song* temp = head;
        while (temp != nullptr) {
            if (temp->genre == genre) {
                cout << temp->title << " by " << temp->artist << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "No songs found for genre: " << genre << endl;
        }
    }

    ~Playlist() {
        while (head != nullptr) {
            Song* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    Playlist myPlaylist;
    int choice;
    do {
        cout << "\nMusic Playlist Manager\n";
        cout << "1. Add Song\n";
        cout << "2. Remove Song\n";
        cout << "3. Display Playlist\n";
        cout << "4. Play Next Song\n";
        cout << "5. Play Previous Song\n";
        cout << "6. Shuffle Playlist\n";
        cout << "7. Search for a Song\n";
        cout << "8. Save Playlist\n";
        cout << "9. Load Playlist\n";
        cout << "10. Play Song by Index\n";
        cout << "11. Toggle Favorite Status\n";
        cout << "12. Display Favorite Songs\n";
        cout << "13. Toggle Repeat\n";
        cout << "14. Show Current Song\n";
        cout << "15. Deep Search By Artist (DFS)\n";
        cout << "16. Sort Playlist by Title\n";
        cout << "17. Rate a Song\n";
        cout << "18. Display Top Rated Songs\n";
        cout << "19. Display Play History\n";
        cout << "20. Filter by Genre\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        string title, artist, album, genre;
        int duration, index, rating;
        switch (choice) {
            case 1:
                cout << "Enter song title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter artist: ";
                getline(cin, artist);
                cout << "Enter album: ";
                getline(cin, album);
                cout << "Enter genre: ";
                getline(cin, genre);
                cout << "Enter duration (in seconds): ";
                cin >> duration;
                myPlaylist.addSong(title, artist, album, duration, genre);
                break;

            case 2:
                cout << "Enter song title to remove: ";
                cin.ignore();
                getline(cin, title);
                myPlaylist.removeSong(title);
                break;

            case 3:
                myPlaylist.displayPlaylist();
                break;

            case 4:
                myPlaylist.playNextSong();
                break;

            case 5:
                myPlaylist.playPreviousSong();
                break;

            case 6:
                myPlaylist.shufflePlaylist();
                break;

            case 7:
                cout << "Enter keyword to search for: ";
                cin.ignore();
                getline(cin, title);
                myPlaylist.searchSong(title);
                break;

            case 8:
                myPlaylist.savePlaylist("playlist.txt");
                break;

            case 9:
                myPlaylist.loadPlaylist("playlist.txt");
                break;

            case 10:
                cout << "Enter index of song to play: ";
                cin >> index;
                myPlaylist.playSongByIndex(index);
                break;

            case 11:
                cout << "Enter song title to toggle favorite status: ";
                cin.ignore();
                getline(cin, title);
                myPlaylist.toggleFavorite(title);
                break;

            case 12:
                myPlaylist.displayFavorites();
                break;

            case 13:
                myPlaylist.toggleRepeat();
                break;

            case 14:
                myPlaylist.showCurrentSong();
                break;

            case 15:
                cout << "Enter artist for deep search: ";
                cin.ignore();
                getline(cin, artist);
                myPlaylist.deepSearchByArtistDFS(artist);
                break;

            case 16:
                myPlaylist.sortPlaylistByTitle();
                break;

            case 17:
                cout << "Enter song title to rate: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter rating (1-5): ";
                cin >> rating;
                myPlaylist.rateSong(title, rating);
                break;

            case 18:
                myPlaylist.displayTopRatedSongs();
                break;

            case 19:
                myPlaylist.displayPlayHistory();
                break;

            case 20:
                cout << "Enter genre to filter by: ";
                cin.ignore();
                getline(cin, genre);
                myPlaylist.filterByGenre(genre);
                break;

            case 0:
                cout << "Exiting..." << endl;
                break;

            default:
                cout << "Invalid choice, try again!" << endl;
        }
    } while (choice != 0);

    return 0;
}