#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp> // Header file include kar rahe hain
#include <SFML/Audio.hpp> // Header file include kar rahe hain
#include <iostream> // Header file include kar rahe hain
#include <fstream> // Header file include kar rahe hain
#include <string> // Header file include kar rahe hain
#include <cstring> // Header file include kar rahe hain
#include <ctime> // Header file include kar rahe hain
#include <cmath> // Header file include kar rahe hain


using namespace sf; // Standard namespace use kar rahe hain taake std:: likhne ki zarurat na ho
using namespace std; // Standard namespace use kar rahe hain taake std:: likhne ki zarurat na ho

const int MAX_FRIENDS = 10;
const int MAX_HISTORY = 20;
const int MAX_LEADERBOARD = 10;

// Game grid dimensions and tile size
const int M = 25;
const int N = 40;
const int ts = 32;

// Forward declaration for grid // For loop - repeat karne ke liye block of code
int grid[M][N];

// ----- Structures -----
struct PlayerProfile { // Code block ka start
    char username[50];
    int totalPoints;
    char friends[MAX_FRIENDS][50];
    int friendCount;
    // Store match history outcome: 'W' win, 'L' loss, 'D' draw
    char matchHistory[MAX_HISTORY];
    int matchCount;
    int highScore; // Added: To store the highest score achieved in a single game
    char pendingSent[MAX_FRIENDS][50];
    char pendingReceived[MAX_FRIENDS][50];
    int pendingSentCount;
    int pendingReceivedCount;


    PlayerProfile() { // Code block ka start
        username[0] = '\0';
        totalPoints = 0;
        friendCount = 0;
        matchCount = 0;
        highScore = 0; // Initialize high score
        pendingSentCount = 0;
        pendingReceivedCount = 0;

    } // Code block ka end
}; // Code block ka end

struct HeapNode { // Code block ka start
    char username[50];
    int totalPoints;
}; // Code block ka end

// Forward declarations for file operations related to PlayerProfile // For loop - repeat karne ke liye block of code
bool loadPlayerProfile(const char* uname, PlayerProfile& profile);
void saveOrUpdateProfile(PlayerProfile& profile); // Function define kar rahe hain jo kuch return nahi karta

struct Leaderboard { // Code block ka start
    HeapNode heap[MAX_LEADERBOARD];
    int size;

    Leaderboard() { // Code block ka start
        size = 0;
    } // Code block ka end

    void saveLeaderboardToFile() { // Function define kar rahe hain jo kuch return nahi karta
        ofstream fout("leaderboard.txt");
        if (!fout.is_open()) { // Condition check ho rahi hai
            cerr << "Error: Could not open leaderboard.txt for saving." << endl; // For loop - repeat karne ke liye block of code
            return; // Program successful end pe return 0 karta hai
        } // Code block ka end
        for (int i = 0; i < size; i++) { // For loop - repeat karne ke liye block of code
            fout << heap[i].username << " " << heap[i].totalPoints << "\n";
        } // Code block ka end
        fout.close();
    } // Code block ka end

    void swap(int i, int j) { // Function define kar rahe hain jo kuch return nahi karta
        HeapNode temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    } // Code block ka end

    void heapifyDown(int i) { // Condition check ho rahi hai
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < size && heap[l].totalPoints < heap[smallest].totalPoints) smallest = l; // Condition check ho rahi hai
        if (r < size && heap[r].totalPoints < heap[smallest].totalPoints) smallest = r; // Condition check ho rahi hai

        if (smallest != i) { // Condition check ho rahi hai
            swap(i, smallest);
            heapifyDown(smallest); // Condition check ho rahi hai
        } // Code block ka end
    } // Code block ka end

    void heapifyUp(int i) { // Condition check ho rahi hai
        int parent = (i - 1) / 2;
        if (i > 0 && heap[i].totalPoints < heap[parent].totalPoints) { // Min-heap property: parent is smaller // Condition check ho rahi hai
            swap(i, parent);
            heapifyUp(parent); // Condition check ho rahi hai
        } // Code block ka end
    } // Code block ka end

    void insertOrUpdate(const char* uname, int points) { // Function define kar rahe hain jo kuch return nahi karta
        // Check if user already exists in the heap // Condition check ho rahi hai
        for (int i = 0; i < size; i++) { // For loop - repeat karne ke liye block of code
            if (strcmp(heap[i].username, uname) == 0) { // Condition check ho rahi hai
                // Only update if the new score is higher // Condition check ho rahi hai
                if (points > heap[i].totalPoints) { // Condition check ho rahi hai
                    heap[i].totalPoints = points;
                    heapifyDown(i); // Restore heap property if updated score makes it "larger" relative to children // Condition check ho rahi hai
                } // Code block ka end
                saveLeaderboardToFile(); // Save after update
                return; // User found and updated or score not higher // Program successful end pe return 0 karta hai
            } // Code block ka end
        } // Code block ka end

        // If user not in heap and heap is not full, insert
        if (size < MAX_LEADERBOARD) { // Condition check ho rahi hai
            strcpy(heap[size].username, uname);
            heap[size].totalPoints = points;
            heapifyUp(size); // Bubble up to maintain min-heap property // Condition check ho rahi hai
            size++;
        } // Code block ka end
        // If user not in heap and heap is full, check if new score is higher than minimum (root) // Condition check ho rahi hai
        else if (points > heap[0].totalPoints) { // Condition check ho rahi hai
            strcpy(heap[0].username, uname);
            heap[0].totalPoints = points;
            heapifyDown(0); // Restore heap property from the root // Condition check ho rahi hai
        } // Code block ka end

        saveLeaderboardToFile();
    } // Code block ka end

    void display(RenderWindow& window, Font& font) { // Function define kar rahe hain jo kuch return nahi karta
        // Copy heap elements to a temporary array for sorting // For loop - repeat karne ke liye block of code
        HeapNode sorted_heap[MAX_LEADERBOARD];
        for (int i = 0; i < size; i++) { // For loop - repeat karne ke liye block of code
            sorted_heap[i] = heap[i];
        } // Code block ka end

        // Sort the temporary array in descending order of total points (manual sort, e.g., Bubble Sort)
        for (int i = 0; i < size - 1; ++i) { // For loop - repeat karne ke liye block of code
            for (int j = 0; j < size - i - 1; ++j) { // For loop - repeat karne ke liye block of code
                if (sorted_heap[j].totalPoints < sorted_heap[j + 1].totalPoints) { // Condition check ho rahi hai
                    // Direct swap of HeapNode objects
                    HeapNode temp = sorted_heap[j];
                    sorted_heap[j] = sorted_heap[j + 1];
                    sorted_heap[j + 1] = temp;
                } // Code block ka end
            } // Code block ka end
        } // Code block ka end


        window.clear(Color::Black);
        Text title("Leaderboard (Top 10)", font, 30);
        title.setPosition(100, 30);
        title.setFillColor(Color::Cyan);
        window.draw(title);

        for (int i = 0; i < size; i++) { // For loop - repeat karne ke liye block of code
            char line[100];
            sprintf(line, "%d. %s - %d pts", i + 1, sorted_heap[i].username, sorted_heap[i].totalPoints);
            Text t(line, font, 20);
            t.setPosition(100, 80 + i * 30);
            t.setFillColor(Color::White);
            window.draw(t);
        } // Code block ka end

        Text pressEscape("Press ESC to return to Main Menu", font, 18);
        pressEscape.setPosition(100, 80 + size * 30 + 20);
        pressEscape.setFillColor(Color::Yellow);
        window.draw(pressEscape);


        window.display();

        while (window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
            Event e;
            while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
                if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) // Condition check ho rahi hai
                    return; // Program successful end pe return 0 karta hai
            } // Code block ka end
        } // Code block ka end
    } // Code block ka end
}; // Code block ka end
// ---- Matchmaking Structures ----

const int MAX_QUEUE_SIZE = 100;

struct MatchPlayer { // Code block ka start
    char username[50];
    int score;
}; // Code block ka end

class MatchMakingQueue { // Class define kar rahe hain - object banane ke liye
    MatchPlayer queue[MAX_QUEUE_SIZE];
    int size;

    void swap(int i, int j) { // Function define kar rahe hain jo kuch return nahi karta
        MatchPlayer temp = queue[i];
        queue[i] = queue[j];
        queue[j] = temp;
    } // Code block ka end

    void heapifyUp(int i) { // Condition check ho rahi hai
        while (i > 0 && queue[i].score > queue[(i - 1) / 2].score) { // Max-heap: parent is smaller // Jab tak condition true hai, loop chalta rahega
            swap(i, (i - 1) / 2);
            i = (i - 1) / 2;
        } // Code block ka end
    } // Code block ka end

    void heapifyDown(int i) { // Condition check ho rahi hai
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && queue[left].score > queue[largest].score) largest = left; // Condition check ho rahi hai
        if (right < size && queue[right].score > queue[largest].score) largest = right; // Condition check ho rahi hai

        if (largest != i) { // Condition check ho rahi hai
            swap(i, largest);
            heapifyDown(largest); // Condition check ho rahi hai
        } // Code block ka end
    } // Code block ka end

public:
    MatchMakingQueue() { size = 0; }

    void insert(const char* name, int score) { // Function define kar rahe hain jo kuch return nahi karta
        if (size >= MAX_QUEUE_SIZE) return; // Condition check ho rahi hai
        strcpy(queue[size].username, name);
        queue[size].score = score;
        heapifyUp(size); // Condition check ho rahi hai
        size++;
    } // Code block ka end

    // Modified `match` function to update profiles and leaderboard // Condition check ho rahi hai
    // It now takes SFML window and font for displaying messages, and a reference to the global leaderboard // For loop - repeat karne ke liye block of code
    bool match(RenderWindow& window, Font& font, Leaderboard& leaderboard_ref) {
        if (size < 2) return false;

        // Extract top two players
        MatchPlayer p1 = queue[0]; removeTop();
        MatchPlayer p2 = queue[0]; removeTop();

        int preScore1 = p1.score;
        int preScore2 = p2.score;

        string message_str = "Matched Players:\n";
        message_str += "1. " + string(p1.username) + " (" + to_string(preScore1) + " pts)\n";
        message_str += "2. " + string(p2.username) + " (" + to_string(preScore2) + " pts)\n";

        char p1_outcome_char, p2_outcome_char;

        if (preScore1 == preScore2) {
            message_str += "Result: Draw\n";
            p1.score += 10;
            p2.score += 10;
            p1_outcome_char = p2_outcome_char = 'D';

            // ✅ Requeue both players for future matchmaking
            insert(p1.username, p1.score);
            insert(p2.username, p2.score);
        }
        else if (preScore1 > preScore2) {
            message_str += "Result: " + string(p1.username) + " wins\n";
            p1.score += 20;
            p2.score += 5;
            p1_outcome_char = 'W';
            p2_outcome_char = 'L';

            // ✅ Requeue only the **loser**
            insert(p2.username, p2.score);
        }
        else {
            message_str += "Result: " + string(p2.username) + " wins\n";
            p2.score += 20;
            p1.score += 5;
            p1_outcome_char = 'L';
            p2_outcome_char = 'W';

            // ✅ Requeue only the **loser**
            insert(p1.username, p1.score);
        }

        cout << message_str << endl;

        // Update profiles and leaderboard
        PlayerProfile profile_p1, profile_p2;
        if (loadPlayerProfile(p1.username, profile_p1)) {
            profile_p1.totalPoints = p1.score;
            if (profile_p1.matchCount < MAX_HISTORY)
                profile_p1.matchHistory[profile_p1.matchCount++] = p1_outcome_char;
            saveOrUpdateProfile(profile_p1);
            leaderboard_ref.insertOrUpdate(profile_p1.username, profile_p1.totalPoints);
        }

        if (loadPlayerProfile(p2.username, profile_p2)) {
            profile_p2.totalPoints = p2.score;
            if (profile_p2.matchCount < MAX_HISTORY)
                profile_p2.matchHistory[profile_p2.matchCount++] = p2_outcome_char;
            saveOrUpdateProfile(profile_p2);
            leaderboard_ref.insertOrUpdate(profile_p2.username, profile_p2.totalPoints);
        }

        // Display in SFML window
        Text tempMsg(message_str, font, 18);
        tempMsg.setFillColor(Color::Yellow);
        tempMsg.setOrigin(tempMsg.getLocalBounds().width / 2.0f, tempMsg.getLocalBounds().height / 2.0f);
        tempMsg.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 100.0f);

        window.clear(Color::Black);
        window.draw(tempMsg);
        window.display();

        Clock msgClock;
        while (msgClock.getElapsedTime().asSeconds() < 3.0f && window.isOpen()) {
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) window.close();
            }
        }

        return true;
    }



    void removeTop() { // Function define kar rahe hain jo kuch return nahi karta
        if (size == 0) return; // Condition check ho rahi hai
        queue[0] = queue[size - 1];
        size--;
        heapifyDown(0); // Condition check ho rahi hai
    } // Code block ka end

    void saveToFile(const char* filename) { // Function define kar rahe hain jo kuch return nahi karta
        ofstream fout(filename);
        if (!fout.is_open()) { // Condition check ho rahi hai
            cerr << "Error: Could not open " << filename << " for saving matchmaking queue." << endl; // For loop - repeat karne ke liye block of code
            return; // Program successful end pe return 0 karta hai
        } // Code block ka end
        for (int i = 0; i < size; i++) { // For loop - repeat karne ke liye block of code
            fout << queue[i].username << " " << queue[i].score << "\n";
        } // Code block ka end
        fout.close();
    } // Code block ka end

    void loadFromFile(const char* filename) { // Function define kar rahe hain jo kuch return nahi karta
        ifstream fin(filename); // Condition check ho rahi hai
        if (!fin.is_open()) { // Condition check ho rahi hai
            cerr << "Warning: Could not open " << filename << " for loading matchmaking queue." << endl; // For loop - repeat karne ke liye block of code
            return; // Program successful end pe return 0 karta hai
        } // Code block ka end
        size = 0;
        char name[50];
        int sc;
        while (fin >> name >> sc) { // Jab tak condition true hai, loop chalta rahega
            insert(name, sc); // Re-insert to maintain heap property
        } // Code block ka end
        fin.close();
    } // Code block ka end
}; // Code block ka end


struct Enemy
{ // Code block ka start
    int x, y, dx, dy;
    // Flag to indicate if the enemy is frozen by a power-up // Condition check ho rahi hai
    bool isFrozen;

    Enemy() { // Code block ka start
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
        isFrozen = false;
    } // Code block ka end
    void move() { // Function define kar rahe hain jo kuch return nahi karta
        if (isFrozen) return; // Don't move if frozen // Condition check ho rahi hai
        x += dx; if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; } // Condition check ho rahi hai
        y += dy; if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; } // Condition check ho rahi hai
    } // Code block ka end
}; // Code block ka end

struct Player { // Code block ka start
    int x, y, dx, dy, score;
    bool isAlive;
    bool isConstructing; // True if player is drawing a line // Condition check ho rahi hai

    // Points System variables
    int tilesCapturedInMove;
    int rewardCounter; // Tracks how many times bonus has been earned
    int bonusThreshold; // Current threshold for bonus points (10, then 5) // For loop - repeat karne ke liye block of code
    int currentMultiplier; // Multiplier for bonus points (1, 2, or 4) // For loop - repeat karne ke liye block of code

    // Power-up variables
    int powerUpCount;
    bool powerUpActive;
    float powerUpTimer; // Duration of active power-up

    Player() : x(10), y(0), dx(0), dy(0), score(0), isAlive(true), isConstructing(false),
        tilesCapturedInMove(0), rewardCounter(0), bonusThreshold(10), currentMultiplier(1),
        powerUpCount(0), powerUpActive(false), powerUpTimer(0.0f) {}

    void grantPowerUp() { // Function define kar rahe hain jo kuch return nahi karta
        powerUpCount++;
    } // Code block ka end

    void activatePowerUp() { // Function define kar rahe hain jo kuch return nahi karta
        if (powerUpCount > 0 && !powerUpActive) { // Condition check ho rahi hai
            powerUpCount--;
            powerUpActive = true;
            powerUpTimer = 3.0f; // 3 seconds duration
        } // Code block ka end
    } // Code block ka end
}; // Code block ka end
struct TileNode { // Code block ka start
    int x, y;
    TileNode* next;
    TileNode(int xx, int yy) : x(xx), y(yy), next(nullptr) {}
}; // Code block ka end

struct GameState { // Code block ka start
    char username[50];
    time_t timestamp;
    Player playerData;
    // Uses linked list for player trail (grid == 2 for P1 in single player) // For loop - repeat karne ke liye block of code
    TileNode* tiles;

    Enemy enemies[10]; // Store enemy states (assuming max 10 enemies for simplicity) // For loop - repeat karne ke liye block of code
    int enemyCount;
    int grid[M][N];

    GameState() : timestamp(time(0)), tiles(nullptr), enemyCount(0) { // Code block ka start
        username[0] = '\0';
    } // Code block ka end
}; // Code block ka end


// ----- Global Variables -----
PlayerProfile currentProfile; // Assuming Player 1 is the logged-in player in multiplayer
Leaderboard leaderboard;
MatchMakingQueue mmq;


// ----- Function Definitions -----
void loadLeaderboardFromFile() { // Function define kar rahe hain jo kuch return nahi karta
    ifstream fin("leaderboard.txt"); // Condition check ho rahi hai
    leaderboard.size = 0;
    if (!fin.is_open()) { // Condition check ho rahi hai
        cerr << "Warning: Could not open leaderboard.txt for loading. Starting with empty leaderboard." << endl; // For loop - repeat karne ke liye block of code
        return; // Program successful end pe return 0 karta hai
    } // Code block ka end
    while (leaderboard.size < MAX_LEADERBOARD && fin >> leaderboard.heap[leaderboard.size].username >> leaderboard.heap[leaderboard.size].totalPoints) { // Jab tak condition true hai, loop chalta rahega
        leaderboard.size++;
    } // Code block ka end

    // Rebuild the min-heap from the loaded data
    for (int i = leaderboard.size / 2 - 1; i >= 0; i--) { // For loop - repeat karne ke liye block of code
        leaderboard.heapifyDown(i); // Condition check ho rahi hai
    } // Code block ka end

    fin.close();
} // Code block ka end

// Load a player profile from file
bool loadPlayerProfile(const char* uname, PlayerProfile& profile) { // Code block ka start
    ifstream fin("profiles.txt"); // Condition check ho rahi hai
    if (!fin.is_open()) { // Condition check ho rahi hai
        cerr << "Warning: Could not open profiles.txt for loading. A new profile will be created on login." << endl; // For loop - repeat karne ke liye block of code
        return false; // File not found or cannot be opened // Program successful end pe return 0 karta hai
    } // Code block ka end

    // Read all profiles into a temporary array
    PlayerProfile temp_profiles[100]; // Assuming max 100 profiles for simplicity // For loop - repeat karne ke liye block of code
    int count = 0;
    // Updated to read highScore as well
    while (count < 100 && fin >> temp_profiles[count].username >> temp_profiles[count].totalPoints >> temp_profiles[count].friendCount >> temp_profiles[count].matchCount >> temp_profiles[count].highScore) { // Jab tak condition true hai, loop chalta rahega
        // Read friends
        for (int i = 0; i < temp_profiles[count].friendCount; i++) { // For loop - repeat karne ke liye block of code
            fin >> temp_profiles[count].friends[i];
        } // Code block ka end

        // Read match history (as a single string of chars for simplicity in file I/O) // For loop - repeat karne ke liye block of code
        string history_str;
        if (temp_profiles[count].matchCount > 0) { // Condition check ho rahi hai
            fin >> history_str; // Read the whole string of characters
            strncpy(temp_profiles[count].matchHistory, history_str.c_str(), temp_profiles[count].matchCount);
            temp_profiles[count].matchHistory[temp_profiles[count].matchCount] = '\0'; // Null-terminate
        } // Code block ka end

        // 🔽 Add this: Read pending sent and received requests
        fin >> temp_profiles[count].pendingSentCount;
        for (int i = 0; i < temp_profiles[count].pendingSentCount; i++) // For loop - repeat karne ke liye block of code
            fin >> temp_profiles[count].pendingSent[i];

        fin >> temp_profiles[count].pendingReceivedCount;
        for (int i = 0; i < temp_profiles[count].pendingReceivedCount; i++) // For loop - repeat karne ke liye block of code
            fin >> temp_profiles[count].pendingReceived[i];

        count++;
    } // Code block ka end
    fin.close();

    // Search for the profile by username // For loop - repeat karne ke liye block of code
    for (int i = 0; i < count; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(temp_profiles[i].username, uname) == 0) { // Condition check ho rahi hai
            profile = temp_profiles[i]; // Copy found profile to the output parameter
            return true; // Profile found // Program successful end pe return 0 karta hai
        } // Code block ka end
    } // Code block ka end

    return false; // Profile not found // Program successful end pe return 0 karta hai
} // Code block ka end
bool sendFriendRequest(PlayerProfile& sender, const char* recipientUsername) { // Code block ka start
    if (strcmp(sender.username, recipientUsername) == 0) return false; // Can't send to self // Condition check ho rahi hai

    PlayerProfile recipient;
    if (!loadPlayerProfile(recipientUsername, recipient)) return false; // Condition check ho rahi hai

    // Already friends?
    for (int i = 0; i < sender.friendCount; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(sender.friends[i], recipientUsername) == 0) // Condition check ho rahi hai
            return false; // Program successful end pe return 0 karta hai
    } // Code block ka end

    // Already sent?
    for (int i = 0; i < sender.pendingSentCount; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(sender.pendingSent[i], recipientUsername) == 0) // Condition check ho rahi hai
            return false; // Program successful end pe return 0 karta hai
    } // Code block ka end

    // Already received?
    for (int i = 0; i < sender.pendingReceivedCount; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(sender.pendingReceived[i], recipientUsername) == 0) // Condition check ho rahi hai
            return false; // Program successful end pe return 0 karta hai
    } // Code block ka end

    // Add to sender’s pendingSent
    strcpy(sender.pendingSent[sender.pendingSentCount++], recipientUsername);

    // Add to recipient’s pendingReceived
    strcpy(recipient.pendingReceived[recipient.pendingReceivedCount++], sender.username);

    // Save both
    saveOrUpdateProfile(sender);
    saveOrUpdateProfile(recipient);

    return true; // Program successful end pe return 0 karta hai
} // Code block ka end
bool acceptFriendRequest(PlayerProfile& receiver, const char* senderUsername) { // Code block ka start
    PlayerProfile sender;
    if (!loadPlayerProfile(senderUsername, sender)) return false; // Condition check ho rahi hai

    // Check if the request exists // Condition check ho rahi hai
    bool found = false;
    for (int i = 0; i < receiver.pendingReceivedCount; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(receiver.pendingReceived[i], senderUsername) == 0) { // Condition check ho rahi hai
            // Remove sender from receiver's pendingReceived
            for (int j = i; j < receiver.pendingReceivedCount - 1; j++) // For loop - repeat karne ke liye block of code
                strcpy(receiver.pendingReceived[j], receiver.pendingReceived[j + 1]);
            receiver.pendingReceivedCount--;
            found = true;
            break;
        } // Code block ka end
    } // Code block ka end
    if (!found) return false; // Condition check ho rahi hai

    // Remove receiver from sender's pendingSent
    for (int i = 0; i < sender.pendingSentCount; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(sender.pendingSent[i], receiver.username) == 0) { // Condition check ho rahi hai
            for (int j = i; j < sender.pendingSentCount - 1; j++) // For loop - repeat karne ke liye block of code
                strcpy(sender.pendingSent[j], sender.pendingSent[j + 1]);
            sender.pendingSentCount--;
            break;
        } // Code block ka end
    } // Code block ka end

    // Add each other to friends list
    if (receiver.friendCount < MAX_FRIENDS && sender.friendCount < MAX_FRIENDS) { // Condition check ho rahi hai
        strcpy(receiver.friends[receiver.friendCount++], sender.username);
        strcpy(sender.friends[sender.friendCount++], receiver.username);
    } // Code block ka end
    else { // Agar if condition false ho to yeh block chalega
        return false; // Friends list full // Program successful end pe return 0 karta hai
    } // Code block ka end

    // Save both
    saveOrUpdateProfile(receiver);
    saveOrUpdateProfile(sender);

    return true; // Program successful end pe return 0 karta hai
} // Code block ka end
bool rejectFriendRequest(PlayerProfile& receiver, const char* senderUsername) { // Code block ka start
    // Remove from pendingReceived
    for (int i = 0; i < receiver.pendingReceivedCount; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(receiver.pendingReceived[i], senderUsername) == 0) { // Condition check ho rahi hai
            for (int j = i; j < receiver.pendingReceivedCount - 1; j++) // For loop - repeat karne ke liye block of code
                strcpy(receiver.pendingReceived[j], receiver.pendingReceived[j + 1]);
            receiver.pendingReceivedCount--;
            saveOrUpdateProfile(receiver);
            return true; // Program successful end pe return 0 karta hai
        } // Code block ka end
    } // Code block ka end
    return false; // Program successful end pe return 0 karta hai
} // Code block ka end


// Save or update a profile in profiles.txt
void saveOrUpdateProfile(PlayerProfile& profile) { // Function define kar rahe hain jo kuch return nahi karta
    // Load all existing profiles into a temporary array
    PlayerProfile temp_profiles[100]; // Assuming max 100 profiles
    int count = 0;
    ifstream fin("profiles.txt"); // Condition check ho rahi hai
    if (fin.is_open()) { // Condition check ho rahi hai
        // Updated to read highScore as well
        while (count < 100 && fin >> temp_profiles[count].username >> temp_profiles[count].totalPoints >> temp_profiles[count].friendCount >> temp_profiles[count].matchCount >> temp_profiles[count].highScore) { // Jab tak condition true hai, loop chalta rahega
            for (int i = 0; i < temp_profiles[count].friendCount; i++) fin >> temp_profiles[count].friends[i]; // For loop - repeat karne ke liye block of code
            string history_str;
            if (temp_profiles[count].matchCount > 0) { // Condition check ho rahi hai
                fin >> history_str; // Read the whole string of characters
                strncpy(temp_profiles[count].matchHistory, history_str.c_str(), temp_profiles[count].matchCount);
                temp_profiles[count].matchHistory[temp_profiles[count].matchCount] = '\0';
            } // Code block ka end

            // 🔽 Add: read pendingSent
            fin >> temp_profiles[count].pendingSentCount;
            for (int i = 0; i < temp_profiles[count].pendingSentCount; i++) // For loop - repeat karne ke liye block of code
                fin >> temp_profiles[count].pendingSent[i];

            // 🔽 Add: read pendingReceived
            fin >> temp_profiles[count].pendingReceivedCount;
            for (int i = 0; i < temp_profiles[count].pendingReceivedCount; i++) // For loop - repeat karne ke liye block of code
                fin >> temp_profiles[count].pendingReceived[i];

            count++;
        } // Code block ka end
        fin.close();
    } // Code block ka end

    // Update or append the current profile
    bool found = false;
    for (int i = 0; i < count; i++) { // For loop - repeat karne ke liye block of code
        if (strcmp(temp_profiles[i].username, profile.username) == 0) { // Condition check ho rahi hai
            temp_profiles[i] = profile; // Update existing profile
            found = true;
            break;
        } // Code block ka end
    } // Code block ka end
    if (!found && count < 100) { // Condition check ho rahi hai
        temp_profiles[count++] = profile; // Append new profile
    } // Code block ka end
    else if (!found) { // Condition check ho rahi hai
        cerr << "Warning: Maximum number of profiles reached. Cannot save new profile." << endl;
        return; // Program successful end pe return 0 karta hai
    } // Code block ka end

    // Save all profiles back to the file
    ofstream fout("profiles.txt");
    if (!fout.is_open()) { // Condition check ho rahi hai
        cerr << "Error: Could not open profiles.txt for saving." << endl; // For loop - repeat karne ke liye block of code
        return; // Program successful end pe return 0 karta hai
    } // Code block ka end
    for (int i = 0; i < count; i++) { // For loop - repeat karne ke liye block of code
        fout << temp_profiles[i].username << " " << temp_profiles[i].totalPoints << " "
            << temp_profiles[i].friendCount << " " << temp_profiles[i].matchCount << " "
            << temp_profiles[i].highScore << "\n";

        // Write friends on the same line
        for (int j = 0; j < temp_profiles[i].friendCount; j++) // For loop - repeat karne ke liye block of code
            fout << temp_profiles[i].friends[j] << (j == temp_profiles[i].friendCount - 1 ? "" : " ");
        fout << "\n";

        // Write match history
        for (int j = 0; j < temp_profiles[i].matchCount; j++) // For loop - repeat karne ke liye block of code
            fout << temp_profiles[i].matchHistory[j];
        fout << "\n";

        // 🔽 Write pendingSent
        fout << temp_profiles[i].pendingSentCount << "\n";
        for (int j = 0; j < temp_profiles[i].pendingSentCount; j++) // For loop - repeat karne ke liye block of code
            fout << temp_profiles[i].pendingSent[j] << (j == temp_profiles[i].pendingSentCount - 1 ? "" : " ");
        fout << "\n";

        // 🔽 Write pendingReceived
        fout << temp_profiles[i].pendingReceivedCount << "\n";
        for (int j = 0; j < temp_profiles[i].pendingReceivedCount; j++) // For loop - repeat karne ke liye block of code
            fout << temp_profiles[i].pendingReceived[j] << (j == temp_profiles[i].pendingReceivedCount - 1 ? "" : " ");
        fout << "\n";
    } // Code block ka end
    fout.close();
} // Code block ka end


void saveGame(GameState* state, const char* saveID) { // Function define kar rahe hain jo kuch return nahi karta
    string filename = "gamestate_";
    filename += saveID;

    ofstream fout(filename.c_str(), ios::binary);
    if (!fout) { // Condition check ho rahi hai
        cerr << "Error opening file '" << filename << "' for saving.\n"; // For loop - repeat karne ke liye block of code
        return; // Program successful end pe return 0 karta hai
    } // Code block ka end

    fout.write((char*)&state->timestamp, sizeof(time_t));
    fout.write((char*)&state->playerData, sizeof(Player));
    fout.write(state->username, sizeof(state->username));

    // Save enemy states
    fout.write((char*)&state->enemyCount, sizeof(int));
    fout.write((char*)&state->enemies, sizeof(Enemy) * state->enemyCount);

    // Save player 1 trail tiles
    TileNode* current = state->tiles;
    int tileCount = 0;
    TileNode* tempHead = state->tiles; // Count first
    while (tempHead) { // Jab tak condition true hai, loop chalta rahega
        tileCount++;
        tempHead = tempHead->next;
    } // Code block ka end
    fout.write((char*)&tileCount, sizeof(int)); // Save count of tiles
    current = state->tiles; // Reset to head for writing // For loop - repeat karne ke liye block of code
    while (current) { // Jab tak condition true hai, loop chalta rahega
        fout.write((char*)&current->x, sizeof(int));
        fout.write((char*)&current->y, sizeof(int));
        current = current->next;
    } // Code block ka end
    for (int i = 0; i < M; ++i) {
        fout.write((char*)state->grid[i], sizeof(int) * N);
    }

    fout.close();
    cout << "Game saved to file '" << filename << "'\n"; // Output screen pe kuch print kar rahe hain
} // Code block ka end


GameState* loadGame(const char* saveID) {
    string filename = "gamestate_";
    filename += saveID;

    ifstream fin(filename.c_str(), ios::binary);
    if (!fin) {
        cerr << "Error opening file '" << filename << "' for loading.\n";
        return nullptr;
    }

    GameState* state = new GameState;
    fin.read((char*)&state->timestamp, sizeof(time_t));
    fin.read((char*)&state->playerData, sizeof(Player));
    fin.read(state->username, sizeof(state->username));

    // Load enemy states
    fin.read((char*)&state->enemyCount, sizeof(int));
    fin.read((char*)&state->enemies, sizeof(Enemy) * state->enemyCount);

    // Load player 1 trail tiles
    int tileCount = 0;
    fin.read((char*)&tileCount, sizeof(int));
    TileNode* head = nullptr, * tail = nullptr;
    for (int i = 0; i < tileCount; ++i) {
        int x, y;
        if (!fin.read((char*)&x, sizeof(int))) break;
        if (!fin.read((char*)&y, sizeof(int))) break;
        TileNode* node = new TileNode(x, y);
        if (!head) head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    state->tiles = head;

    // Load the grid
    for (int i = 0; i < M; ++i) {
        fin.read((char*)state->grid[i], sizeof(int) * N);
    }

    fin.close();
    cout << "Game loaded from file '" << filename << "'\n";
    return state;
}// Code block ka end


// Helper function to display a temporary message on the screen
void show_message(RenderWindow& window, Font& font, const string& message, Color color, Vector2f pos, float duration_seconds) { // Function define kar rahe hain jo kuch return nahi karta
    Text msgText(message, font, 20);
    msgText.setPosition(pos);
    msgText.setFillColor(color);

    Clock msgClock;
    // To avoid flickering, we'll redraw the background and the message // Function define kar rahe hain jo kuch return nahi karta
    // The calling function should handle its own drawing before this is called if context matters. // Condition check ho rahi hai
    // Here we just clear and draw message to be visible.
    while (window.isOpen() && msgClock.getElapsedTime().asSeconds() < duration_seconds) { // Jab tak condition true hai, loop chalta rahega
        Event e;
        while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
            if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
        } // Code block ka end
        window.clear(Color::Black);
        window.draw(msgText);
        window.display();
    } // Code block ka end
} // Code block ka end


// ----- Window-based Login/Registration Functions -----

string draw_textbox(RenderWindow& window, Font& font, string label, Vector2f pos, bool is_password = false) { // Code block ka start
    Text title(label, font, 20);
    title.setPosition(pos.x, pos.y - 30);
    title.setFillColor(Color::White);

    RectangleShape box(Vector2f(300, 40));
    box.setPosition(pos);
    box.setFillColor(Color(30, 30, 30));
    box.setOutlineThickness(2);
    box.setOutlineColor(Color::White);

    Text input_display("", font, 20);
    input_display.setPosition(pos.x + 10, pos.y + 5);
    input_display.setFillColor(Color::Cyan);

    string input = "";
    bool active = true; // Textbox is active initially

    // Add a blinking cursor
    RectangleShape cursor(Vector2f(2, 30));
    cursor.setFillColor(Color::White);
    bool cursorVisible = true;
    Clock cursorClock;

    while (window.isOpen() && active) { // Jab tak condition true hai, loop chalta rahega
        Event e;
        while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
            if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
            if (e.type == Event::KeyPressed) { // Condition check ho rahi hai
                if (e.key.code == Keyboard::Enter) { // Condition check ho rahi hai
                    active = false; // Deactivate textbox on Enter
                } // Code block ka end
                else if (e.key.code == Keyboard::Escape) { // Condition check ho rahi hai
                    return ""; // Allow cancellation with Escape // Program successful end pe return 0 karta hai
                } // Code block ka end
            } // Code block ka end
            if (e.type == Event::TextEntered) { // Condition check ho rahi hai
                if (e.text.unicode == 8) { // Backspace // Condition check ho rahi hai
                    if (!input.empty()) { // Condition check ho rahi hai
                        input.pop_back();
                    } // Code block ka end
                } // Code block ka end
                else if (e.text.unicode >= 32 && e.text.unicode <= 126) { // Printable characters // Condition check ho rahi hai
                    input += static_cast<char>(e.text.unicode);
                } // Code block ka end
                // Reset cursor visibility on text input
                cursorVisible = true;
                cursorClock.restart();
            } // Code block ka end
        } // Code block ka end

        // Update cursor blinking
        if (cursorClock.getElapsedTime().asMilliseconds() > 500) { // Condition check ho rahi hai
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        } // Code block ka end

        input_display.setString(is_password ? string(input.size(), '*') : input);
        cursor.setPosition(pos.x + 10 + input_display.getGlobalBounds().width + 2, pos.y + 5);


        window.clear(Color::Black);
        window.draw(title);
        window.draw(box);
        window.draw(input_display);
        if (active && cursorVisible) { // Condition check ho rahi hai
            window.draw(cursor);
        } // Code block ka end
        window.display();
    } // Code block ka end
    return input; // Program successful end pe return 0 karta hai
} // Code block ka end

bool username_exists(string uname) { // Code block ka start
    ifstream fin("users.txt"); // Condition check ho rahi hai
    if (!fin.is_open()) return false; // File doesn't exist, so username can't exist // Condition check ho rahi hai

    string u, p;
    while (fin >> u >> p) { // Jab tak condition true hai, loop chalta rahega
        if (u == uname) return true; // Condition check ho rahi hai
    } // Code block ka end
    return false; // Program successful end pe return 0 karta hai
} // Code block ka end

bool check_credentials(string uname, string pass) { // Code block ka start
    ifstream fin("users.txt"); // Condition check ho rahi hai
    if (!fin.is_open()) return false; // File doesn't exist, no users to check // Condition check ho rahi hai

    string u, p;
    while (fin >> u >> p) { // Jab tak condition true hai, loop chalta rahega
        if (u == uname && p == pass) return true; // Condition check ho rahi hai
    } // Code block ka end
    return false; // Program successful end pe return 0 karta hai
} // Code block ka end

void save_user(string uname, string pass) { // Function define kar rahe hain jo kuch return nahi karta
    ofstream fout("users.txt", ios::app);
    if (!fout.is_open()) { // Condition check ho rahi hai
        cerr << "Error: Could not open users.txt for saving new user." << endl; // For loop - repeat karne ke liye block of code
        return; // Program successful end pe return 0 karta hai
    } // Code block ka end
    fout << uname << " " << pass << "\n";
    fout.close();
} // Code block ka end


bool registration(RenderWindow& window, Font& font) { // Code block ka start
    Text regTitle("Register", font, 30);
    regTitle.setPosition(150, 50);
    regTitle.setFillColor(Color::White);

    string uname, pass;

    while (window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
        window.clear(Color::Black);
        window.draw(regTitle);
        window.display(); // Show title before drawing textbox // For loop - repeat karne ke liye block of code

        uname = draw_textbox(window, font, "Enter Username:", { 150, 150 });
        if (uname.empty() && window.isOpen()) return false; // Cancelled or window closed // Condition check ho rahi hai
        if (!window.isOpen()) return false; // Condition check ho rahi hai


        if (username_exists(uname)) { // Condition check ho rahi hai
            show_message(window, font, "Username already exists!", Color::Red, { 150, 300 }, 2.0f);
            continue; // Ask for username again // For loop - repeat karne ke liye block of code
        } // Code block ka end

        window.clear(Color::Black);
        window.draw(regTitle);
        Text unameLabel("Username: " + uname, font, 20);
        unameLabel.setPosition(150, 120);
        unameLabel.setFillColor(Color::White);
        window.draw(unameLabel);
        window.display(); // Show title and username before drawing password box // For loop - repeat karne ke liye block of code

        pass = draw_textbox(window, font, "Enter Password (min 4 chars):", { 150, 220 }, true);
        if (pass.empty() && window.isOpen()) { // Check window.isOpen() in case it was closed // Condition check ho rahi hai
            // User might have pressed escape on password box
            // Return false if cancelled on password, allowing user to restart reg // Condition check ho rahi hai
            return false; // Program successful end pe return 0 karta hai
        } // Code block ka end
        if (!window.isOpen()) return false; // Condition check ho rahi hai


        if (pass.length() < 4) { // Condition check ho rahi hai
            show_message(window, font, "Password must be at least 4 characters!", Color::Red, { 150, 300 }, 2.0f);
            continue; // Ask for password again // For loop - repeat karne ke liye block of code
        } // Code block ka end

        save_user(uname, pass);
        // Create an initial player profile for the new user // For loop - repeat karne ke liye block of code
        PlayerProfile newProfile;
        strcpy(newProfile.username, uname.c_str());
        newProfile.totalPoints = 0;
        newProfile.friendCount = 0;
        newProfile.matchCount = 0;
        newProfile.highScore = 0; // Initialize high score for new profile // For loop - repeat karne ke liye block of code
        saveOrUpdateProfile(newProfile);

        show_message(window, font, "Registration successful! Press any key to continue.", Color::Green, { 150, 300 }, 2.0f);
        return true; // Program successful end pe return 0 karta hai
    } // Code block ka end
    return false; // Should not reach here unless window is closed // Program successful end pe return 0 karta hai
} // Code block ka end

bool login(RenderWindow& window, Font& font) { // Code block ka start
    Text loginTitle("Login", font, 30);
    loginTitle.setPosition(150, 50);
    loginTitle.setFillColor(Color::White);

    string uname, pass;

    while (window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
        window.clear(Color::Black);
        window.draw(loginTitle);
        window.display(); // Show title before drawing textbox // For loop - repeat karne ke liye block of code

        uname = draw_textbox(window, font, "Enter Username:", { 150, 150 });
        if (uname.empty() && window.isOpen()) return false; // Cancelled or window closed // Condition check ho rahi hai
        if (!window.isOpen()) return false; // Condition check ho rahi hai


        window.clear(Color::Black);
        window.draw(loginTitle);
        Text unameLabel("Username: " + uname, font, 20);
        unameLabel.setPosition(150, 120);
        unameLabel.setFillColor(Color::White);
        window.draw(unameLabel);
        window.display(); // Show title and username before drawing password box // For loop - repeat karne ke liye block of code

        pass = draw_textbox(window, font, "Enter Password:", { 150, 220 }, true);
        if (pass.empty() && window.isOpen()) { // Condition check ho rahi hai
            // User might have pressed escape on password box
            return false; // Program successful end pe return 0 karta hai
        } // Code block ka end
        if (!window.isOpen()) return false; // Condition check ho rahi hai


        if (check_credentials(uname, pass)) { // Condition check ho rahi hai
            if (!loadPlayerProfile(uname.c_str(), currentProfile)) { // Condition check ho rahi hai
                // This case should ideally not happen if registration creates a profile, // Condition check ho rahi hai
                // but as a fallback, create a new profile if login succeeds but profile not found. // Condition check ho rahi hai
                cout << "Profile not found for existing user. Creating new profile." << endl; // Output screen pe kuch print kar rahe hain
                strcpy(currentProfile.username, uname.c_str());
                currentProfile.totalPoints = 0;
                currentProfile.friendCount = 0;
                currentProfile.matchCount = 0;
                currentProfile.highScore = 0; // Initialize high score
                saveOrUpdateProfile(currentProfile);
            } // Code block ka end
            show_message(window, font, "Login successful!", Color::Green, { 150, 300 }, 1.0f);
            return true; // Login successful // Program successful end pe return 0 karta hai
        } // Code block ka end
        else { // Agar if condition false ho to yeh block chalega
            show_message(window, font, "Invalid username or password!", Color::Red, { 150, 300 }, 2.0f);
        } // Code block ka end
    } // Code block ka end
    return false; // Should not reach here unless window is closed // Program successful end pe return 0 karta hai
} // Code block ka end

// Recursive flood fill algorithm to identify enclosed areas // Condition check ho rahi hai
void drop(int y, int x) { // Function define kar rahe hain jo kuch return nahi karta
    if (grid[y][x] == 0) grid[y][x] = -1; // Mark as visited and outside // Condition check ho rahi hai
    if (y > 0 && grid[y - 1][x] == 0) drop(y - 1, x); // Condition check ho rahi hai
    if (y < M - 1 && grid[y + 1][x] == 0) drop(y + 1, x); // Condition check ho rahi hai
    if (x > 0 && grid[y][x - 1] == 0) drop(y, x - 1); // Condition check ho rahi hai
    if (x < N - 1 && grid[y][x + 1] == 0) drop(y, x + 1); // Condition check ho rahi hai
} // Code block ka end

// Function to reset the game grid
void resetGrid() { // Function define kar rahe hain jo kuch return nahi karta
    for (int i = 0; i < M; i++) { // For loop - repeat karne ke liye block of code
        for (int j = 0; j < N; j++) { // For loop - repeat karne ke liye block of code
            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
        } // Code block ka end
    } // Code block ka end
} // Code block ka end
bool get_user_input(RenderWindow& window, Font& font, string& result) { // Code block ka start
    result = draw_textbox(window, font, "Enter Username:", { N * ts / 2.0f - 150.0f, M * ts / 2.0f });
    return !result.empty() && window.isOpen(); // Program successful end pe return 0 karta hai
} // Code block ka end


// End Game Menu
// Returns true if player chooses to Restart, false otherwise (Main Menu or Exit) // Condition check ho rahi hai
bool showEndMenu(RenderWindow& window, Font& font, int finalScore1, int finalScore2 = -1, int player1_highScore = -1) { // Code block ka start
    const char* endMenuOptions[] = { "Restart", "Main Menu", "Exit Game" };
    int endMenuSize = 3;
    int selected = 0;

    Text gameOverText("Game Over!", font, 50);
    gameOverText.setPosition(N * ts / 2.0f - gameOverText.getGlobalBounds().width / 2.0f, M * ts / 2.0f - 150.0f);
    gameOverText.setFillColor(Color::Red);

    char scoreStr1[50];
    sprintf(scoreStr1, "Your Score: %d", finalScore1);
    Text scoreText1(scoreStr1, font, 30);
    scoreText1.setPosition(N * ts / 2.0f - scoreText1.getGlobalBounds().width / 2.0f, M * ts / 2.0f - 80.0f);
    scoreText1.setFillColor(Color::White);

    Text highScoreText; // For displaying high score
    if (player1_highScore != -1) { // Only for single player or Player 1 (logged in user) // Condition check ho rahi hai
        char hsStr[100];
        sprintf(hsStr, "High Score: %d %s", player1_highScore, (finalScore1 == player1_highScore && finalScore1 > 0) ? "(New!)" : "");
        highScoreText.setString(hsStr);
        highScoreText.setFont(font);
        highScoreText.setCharacterSize(25);
        highScoreText.setPosition(N * ts / 2.0f - highScoreText.getGlobalBounds().width / 2.0f, M * ts / 2.0f - 40.0f);
        highScoreText.setFillColor((finalScore1 == player1_highScore && finalScore1 > 0) ? Color::Green : Color::Yellow);
    } // Code block ka end


    Text scoreText2; // For multiplayer (reused for winner text) // For loop - repeat karne ke liye block of code
    if (finalScore2 != -1) { // Condition check ho rahi hai
        Text winnerText; // For multiplayer winner
        winnerText.setFont(font);
        winnerText.setCharacterSize(30);
        winnerText.setPosition(N * ts / 2.0f - winnerText.getGlobalBounds().width / 2.0f, M * ts / 2.0f - 10.0f);
        winnerText.setFillColor(Color::Green);

        if (finalScore1 > finalScore2) { // Condition check ho rahi hai
            winnerText.setString("Player 1 Wins!");
        } // Code block ka end
        else if (finalScore2 > finalScore1) { // Condition check ho rahi hai
            winnerText.setString("Player 2 Wins!");
        } // Code block ka end
        else { // Agar if condition false ho to yeh block chalega
            winnerText.setString("It's a Tie!");
        } // Code block ka end
        scoreText2 = winnerText; // Reuse scoreText2 for winner display // For loop - repeat karne ke liye block of code
    } // Code block ka end


    while (window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
        Event e;
        while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
            if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
            if (e.type == Event::KeyPressed) { // Condition check ho rahi hai
                if (e.key.code == Keyboard::Up) { // Condition check ho rahi hai
                    selected = (selected - 1 + endMenuSize) % endMenuSize;
                } // Code block ka end
                else if (e.key.code == Keyboard::Down) { // Condition check ho rahi hai
                    selected = (selected + 1) % endMenuSize;
                } // Code block ka end
                else if (e.key.code == Keyboard::Enter) { // Condition check ho rahi hai
                    if (selected == 0) return true; // Restart // Condition check ho rahi hai
                    if (selected == 1) return false; // Main Menu // Condition check ho rahi hai
                    if (selected == 2) { // Exit Game // Condition check ho rahi hai
                        window.close();
                        return false; // Indicate not restarting/going to main menu // Program successful end pe return 0 karta hai
                    } // Code block ka end
                } // Code block ka end
            } // Code block ka end
        } // Code block ka end

        window.clear(Color::Black);
        window.draw(gameOverText);
        window.draw(scoreText1);
        if (player1_highScore != -1) { // Draw high score for Player 1 // Condition check ho rahi hai
            window.draw(highScoreText);
        } // Code block ka end

        if (finalScore2 != -1) { // Condition check ho rahi hai
            // If multiplayer, draw the winner text which is stored in scoreText2 now
            window.draw(scoreText2);
        } // Code block ka end


        for (int i = 0; i < endMenuSize; ++i) { // For loop - repeat karne ke liye block of code
            Text text(endMenuOptions[i], font, 24);
            // Adjust position based on whether high score / multiplayer winner text is displayed
            float offset_y = 50;
            if (player1_highScore != -1) offset_y += 30; // Shift down if high score text is present // Condition check ho rahi hai
            if (finalScore2 != -1) offset_y += 30; // Shift down if multiplayer winner text is present // Condition check ho rahi hai

            text.setPosition(N * ts / 2.0f - text.getGlobalBounds().width / 2.0f, M * ts / 2.0f + offset_y + i * 40.0f);
            text.setFillColor(i == selected ? Color::Yellow : Color::White);
            window.draw(text);
        } // Code block ka end

        window.display();
    } // Code block ka end
    return false; // Default return if window is closed // Program successful end pe return 0 karta hai
} // Code block ka end


// Function to handle game logic for a single player // For loop - repeat karne ke liye block of code
void runSinglePlayerGame(RenderWindow& window, Sprite& sTile, Sprite& sGameover, Sprite& sEnemy, Font& font) {
    resetGrid();
    Player player1;
    int enemyCount = 4;
    Enemy a[10]; // Max 10 enemies, though we only use enemyCount

    float timer = 0, delay = 0.07;
    Clock clock;
    bool Game = true; // True while game is in progress

    while (window.isOpen() && Game) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        // Update power-up timer if active
        if (player1.powerUpActive) {
            player1.powerUpTimer -= time;
            if (player1.powerUpTimer <= 0) {
                player1.powerUpActive = false;
                cout << "Power-up ended." << endl;
            }
        }

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                Game = false;
                break;
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    Game = false;
                    break;
                }

                // Power-up activation
                if (e.key.code == Keyboard::Space) {
                    player1.activatePowerUp();
                }
                // Save Game (press S)
                if (e.key.code == Keyboard::S) {
                    string saveID = draw_textbox(window, font, "Enter Save ID to Save:", { 150, 200 });
                    if (saveID.empty() && window.isOpen()) continue;
                    if (!window.isOpen()) {
                        Game = false;
                        break;
                    }

                    GameState* state = new GameState;
                    strcpy(state->username, currentProfile.username);
                    state->playerData = player1;

                    // Save enemy states
                    state->enemyCount = enemyCount;
                    for (int i = 0; i < enemyCount; ++i) {
                        state->enemies[i] = a[i];
                    }

                    // Save player trail tiles and the grid
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            state->grid[i][j] = grid[i][j]; // Copy the grid
                            if (grid[i][j] == 2) { // Only save Player 1 trail
                                TileNode* node = new TileNode(j, i);
                                node->next = state->tiles;
                                state->tiles = node;
                            }
                        }
                    }
                    saveGame(state, saveID.c_str());
                    show_message(window, font, "Game saved successfully!", Color::Green, { N * ts / 2.0f - 150.0f, M * ts - 50.0f }, 1.5f);

                    // Clean up allocated TileNodes
                    while (state->tiles) {
                        TileNode* tmp = state->tiles;
                        state->tiles = state->tiles->next;
                        delete tmp;
                    }
                    delete state;
                }

                // Load game
                if (e.key.code == Keyboard::L) {
                    string saveID = draw_textbox(window, font, "Enter Save ID to Load:", { 150, 200 });
                    if (saveID.empty() && window.isOpen()) continue;
                    if (!window.isOpen()) {
                        Game = false;
                        break;
                    }

                    GameState* state = loadGame(saveID.c_str());
                    if (state) {
                        player1 = state->playerData;
                        resetGrid(); // Make sure this happens BEFORE loading tiles

                        // Restore the grid
                        for (int i = 0; i < M; i++) {
                            for (int j = 0; j < N; j++) {
                                grid[i][j] = state->grid[i][j];
                            }
                        }

                        // Reconstruct grid from loaded tiles (this might not be necessary anymore since we load the whole grid)
                        //TileNode* curr = state->tiles;
                        //while (curr) {
                        //    grid[curr->y][curr->x] = 2; // Set loaded trails to Player 1 trail
                        //    curr = curr->next;
                        //}

                        // Load enemy states
                        enemyCount = state->enemyCount;
                        for (int i = 0; i < enemyCount; ++i) {
                            a[i] = state->enemies[i];
                        }

                        show_message(window, font, "Game loaded successfully!", Color::Green, { N * ts / 2.0f - 150.0f, M * ts - 50.0f }, 1.5f);

                        // Clean up allocated TileNodes and GameState
                        while (state->tiles) {
                            TileNode* tmp = state->tiles;
                            state->tiles = state->tiles->next;
                            delete tmp;
                        }
                        delete state;
                    }
                    else {
                        show_message(window, font, "Failed to load game! Invalid ID?", Color::Red, { N * ts / 2.0f - 150.0f, M * ts - 50.0f }, 2.0f);
                    }
                }
            }
        }
        if (!Game) break; // Exit game loop if window closed or Escape was pressed

        // Player 1 Controls
        int prev_dx = player1.dx;
        int prev_dy = player1.dy;
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            player1.dx = -1;
            player1.dy = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player1.dx = 1;
            player1.dy = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up)) {
            player1.dx = 0;
            player1.dy = -1;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down)) {
            player1.dx = 0;
            player1.dy = 1;
        }
        // If no movement keys are pressed, dx and dy remain from the previous frame, allowing continuous movement.

        // Check if player direction changed or stopped (dx=0, dy=0 implies stopping if it wasn't already 0)
        if (player1.dx != prev_dx || player1.dy != prev_dy || (player1.dx == 0 && player1.dy == 0 && (prev_dx != 0 || prev_dy != 0))) {
            // Player changed direction or stopped - reset tiles captured in move
            player1.tilesCapturedInMove = 0;
            player1.isConstructing = false; // Stop constructing if changed direction or stopped
        }

        if (timer > delay) {
            // Only move if player is alive (always true here until collision)
            player1.x += player1.dx;
            player1.y += player1.dy;

            // Boundary checks
            if (player1.x < 0) player1.x = 0;
            if (player1.x >= N) player1.x = N - 1;
            if (player1.y < 0) player1.y = 0;
            if (player1.y >= M) player1.y = M - 1;

            // Collision with trail (grid value 2)
            if (grid[player1.y][player1.x] == 2) {
                Game = false;
                player1.isAlive = false;
            } // Player hits their own trail

            // Marking the trail or hitting solid boundary
            if (grid[player1.y][player1.x] == 0) {
                grid[player1.y][player1.x] = 2; // Mark as Player 1 trail
                player1.score++; // Basic point for capturing a tile
                player1.tilesCapturedInMove++;
                player1.isConstructing = true;
            }
            else if (grid[player1.y][player1.x] == 1) {
                player1.dx = player1.dy = 0; // Stop player movement
                player1.isConstructing = false;

                // Calculate and apply bonus points if applicable
                if (player1.tilesCapturedInMove > player1.bonusThreshold) {
                    int bonusPoints = player1.tilesCapturedInMove * (player1.currentMultiplier - 1); // Bonus on top of 1 point per tile
                    player1.score += bonusPoints;
                    player1.rewardCounter++;
                    cout << "Bonus! Tiles: " << player1.tilesCapturedInMove << ", Multiplier: x" << player1.currentMultiplier << ", Bonus Points: " << bonusPoints << ", Total Score: " << player1.score << endl;

                    // Update bonus threshold and multiplier based on reward counter
                    if (player1.rewardCounter == 3) { // Exactly 3 occurrences
                        player1.bonusThreshold = 5; // Threshold reduces to 5
                        player1.currentMultiplier = 2; // Multiplier is x2
                    }
                    else if (player1.rewardCounter == 5) { // Exactly 5 total occurrences
                        player1.bonusThreshold = 5; // Threshold remains 5
                        player1.currentMultiplier = 4; // Multiplier becomes x4
                    }
                    else if (player1.rewardCounter > 5) { // More than 5 total occurrences
                        player1.bonusThreshold = 5; // Threshold remains 5
                        player1.currentMultiplier = 4; // Multiplier remains x4
                    }
                    else if (player1.rewardCounter < 3 && player1.tilesCapturedInMove > 10) {
                        player1.currentMultiplier = 2; // Multiplier is x2 for >10 tiles before 3 occurrences
                    }
                    else {
                        player1.currentMultiplier = 1; // Default multiplier
                    }
                }
                player1.tilesCapturedInMove = 0; // Reset for the next move sequence

                // Use the drop function to identify enclosed areas
                for (int i = 0; i < enemyCount; i++) {
                    // Only drop from un-filled areas where enemy is inside
                    if (grid[a[i].y / ts][a[i].x / ts] == 0)
                        drop(a[i].y / ts, a[i].x / ts);
                }

                // Fill enclosed areas or convert trails to solid
                for (int i = 0; i < M; i++) {
                    for (int j = 0; j < N; j++) {
                        if (grid[i][j] == -1) grid[i][j] = 0; // Keep outside areas empty
                        else if (grid[i][j] == 2) grid[i][j] = 1; // Convert Player 1's trail to solid
                        else if (grid[i][j] == 0) { // This is an enclosed area
                            grid[i][j] = 1; // Fill enclosed areas
                            player1.score += 2; // Bonus points per filled cell
                        }
                    }
                }
            }
            timer = 0;
        }

        // Power-up granting based on score thresholds
        if (player1.score >= 50 && player1.powerUpCount < 1) player1.grantPowerUp();
        if (player1.score >= 70 && player1.powerUpCount < 2) player1.grantPowerUp();
        if (player1.score >= 100 && player1.powerUpCount < 3) player1.grantPowerUp();
        if (player1.score >= 130) {
            int powerUpsDue = 3 + (player1.score - 130) / 30;
            while (player1.powerUpCount < powerUpsDue) player1.grantPowerUp();
        }

        // Enemy movement and collision
        for (int i = 0; i < enemyCount; i++) {
            // Only move enemy if power-up is not active
            if (!player1.powerUpActive) {
                a[i].move();
            }
            a[i].isFrozen = player1.powerUpActive; // Set enemy frozen status for drawing/visuals

            // Check for enemy collision with player
            if (player1.isAlive && a[i].x / ts == player1.x && a[i].y / ts == player1.y) {
                Game = false;
                player1.isAlive = false;
            } // Enemy hits player
            // Check for enemy collision with player's trail
            if (player1.isAlive && grid[a[i].y / ts][a[i].x / ts] == 2) {
                Game = false;
                player1.isAlive = false;
            } // Enemy hits trail
        }
        if (!Game) break; // Exit game loop if player died

        // Drawing
        window.clear();
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 0) continue;
                if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts)); // Solid
                if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts)); // Player 1 trail (green)
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            }
        }

        // Draw player
        if (player1.isAlive) {
            sTile.setTextureRect(IntRect(36, 0, ts, ts)); // Player tile
            sTile.setPosition(player1.x * ts, player1.y * ts);
            window.draw(sTile);
        }

        // Draw enemies
        sEnemy.rotate(player1.powerUpActive ? 0 : 10); // Stop rotating if frozen
        for (int i = 0; i < enemyCount; i++) {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }

        // Display score
        char scoreStr[20];
        sprintf(scoreStr, "Score: %d", player1.score);
        Text scoreText(scoreStr, font, 20);
        scoreText.setPosition(10, 10);
        scoreText.setFillColor(Color::White);
        window.draw(scoreText);

        // Display power-up count
        char powerUpStr[20];
        sprintf(powerUpStr, "Power-ups: %d", player1.powerUpCount);
        Text powerUpText(powerUpStr, font, 20);
        powerUpText.setPosition(10, 40);
        powerUpText.setFillColor(Color::Yellow);
        window.draw(powerUpText);

        // Display power-up active status
        if (player1.powerUpActive) {
            Text activeText("POWER UP ACTIVE!", font, 24);
            activeText.setPosition(N * ts / 2.0f - activeText.getGlobalBounds().width / 2.0f, 10.0f);
            activeText.setFillColor(Color::Red);
            window.draw(activeText);
        }

        window.display();
    }

    // Game Over sequence
    // Update player profile with final score and match history
    currentProfile.totalPoints += player1.score;
    if (currentProfile.matchCount < MAX_HISTORY) {
        currentProfile.matchHistory[currentProfile.matchCount++] = 'W'; // Assuming single player completion is a 'Win'
    }
    // Update high score if applicable
    if (player1.score > currentProfile.highScore) {
        currentProfile.highScore = player1.score;
    }
    saveOrUpdateProfile(currentProfile);

    // Update leaderboard
    leaderboard.insertOrUpdate(currentProfile.username, currentProfile.totalPoints);

    // Show End Menu
    while (window.isOpen()) {
        bool restart = showEndMenu(window, font, player1.score, -1, currentProfile.highScore); // Pass currentProfile.highScore
        if (restart) {
            runSinglePlayerGame(window, sTile, sGameover, sEnemy, font); // Restart the game
            return; // Exit the current game instance to prevent recursive calls
        }
        else {
            return; // Go back to main menu
        }
    }
}

// Function to handle game logic for two players // For loop - repeat karne ke liye block of code
void runMultiplayerGame(RenderWindow& window, Sprite& sTile, Sprite& sGameover, Sprite& sEnemy, Font& font) { // Function define kar rahe hain jo kuch return nahi karta
    resetGrid();
    Player player1; // Player 1 (Arrow keys)
    Player player2; // Player 2 (WASD)
    player2.x = N - 11; // Set a different starting position for player 2 // Condition check ho rahi hai
    player2.y = M - 1;
    player2.dx = 0; player2.dy = 0; // Ensure player 2 starts stationary

    int enemyCount = 4;
    Enemy a[10]; // Max 10 enemies

    float timer = 0, delay = 0.07;
    Clock clock;

    // Grid values: 0 = empty, 1 = solid, 2 = Player 1 trail, 3 = Player 2 trail

    while (window.isOpen() && (player1.isAlive || player2.isAlive)) { // Jab tak condition true hai, loop chalta rahega
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        // Update power-up timers
        if (player1.powerUpActive) { // Condition check ho rahi hai
            player1.powerUpTimer -= time;
            if (player1.powerUpTimer <= 0) { // Condition check ho rahi hai
                player1.powerUpActive = false;
                cout << "Player 1 Power-up ended." << endl; // Output screen pe kuch print kar rahe hain
            } // Code block ka end
        } // Code block ka end
        if (player2.powerUpActive) { // Condition check ho rahi hai
            player2.powerUpTimer -= time;
            if (player2.powerUpTimer <= 0) { // Condition check ho rahi hai
                player2.powerUpActive = false;
                cout << "Player 2 Power-up ended." << endl; // Output screen pe kuch print kar rahe hain
            } // Code block ka end
        } // Code block ka end

        Event e;
        while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
            if (e.type == Event::Closed) return; // Return to menu if window closed // Condition check ho rahi hai
            if (e.type == Event::KeyPressed) { // Condition check ho rahi hai
                if (e.key.code == Keyboard::Escape) return; // Return to menu on Escape // Condition check ho rahi hai

                // Player 1 Power-up activation
                if (player1.isAlive && e.key.code == Keyboard::Space) { // Condition check ho rahi hai
                    player1.activatePowerUp();
                } // Code block ka end
                // Player 2 Power-up activation (using Right Shift key for example) // Condition check ho rahi hai
                if (player2.isAlive && e.key.code == Keyboard::RShift) { // Condition check ho rahi hai
                    player2.activatePowerUp();
                } // Code block ka end
            } // Code block ka end
        } // Code block ka end


        // --- Input Handling ---
        // Players cannot move if the *other* player's power-up is active // Condition check ho rahi hai
        if (player1.isAlive && !player2.powerUpActive) { // Condition check ho rahi hai
            int prev_dx = player1.dx;
            int prev_dy = player1.dy;
            if (Keyboard::isKeyPressed(Keyboard::Left)) { player1.dx = -1; player1.dy = 0; } // Condition check ho rahi hai
            else if (Keyboard::isKeyPressed(Keyboard::Right)) { player1.dx = 1; player1.dy = 0; } // Condition check ho rahi hai
            else if (Keyboard::isKeyPressed(Keyboard::Up)) { player1.dx = 0; player1.dy = -1; } // Condition check ho rahi hai
            else if (Keyboard::isKeyPressed(Keyboard::Down)) { player1.dx = 0; player1.dy = 1; } // Condition check ho rahi hai
            // If no movement keys are pressed, dx and dy remain from the previous frame, allowing continuous movement.

            // Check if player 1 direction changed or stopped (dx=0, dy=0 implies stopping if it wasn't already 0) // Condition check ho rahi hai
            if (player1.dx != prev_dx || player1.dy != prev_dy || (player1.dx == 0 && player1.dy == 0 && (prev_dx != 0 || prev_dy != 0))) { // Condition check ho rahi hai
                player1.tilesCapturedInMove = 0;
                player1.isConstructing = false;
            } // Code block ka end

        } // Code block ka end
        else if (player1.isAlive && player2.powerUpActive) { // Condition check ho rahi hai
            // Player 1 is frozen by Player 2's power-up
            player1.dx = player1.dy = 0;
        } // Code block ka end
        if (!player1.isAlive) { // Stop movement if not alive // Condition check ho rahi hai
            player1.dx = player1.dy = 0;
        } // Code block ka end


        if (player2.isAlive && !player1.powerUpActive) { // Player 2 cannot move if Player 1's power-up is active // Condition check ho rahi hai
            int prev_dx = player2.dx;
            int prev_dy = player2.dy;
            if (Keyboard::isKeyPressed(Keyboard::A)) { player2.dx = -1; player2.dy = 0; } // Condition check ho rahi hai
            else if (Keyboard::isKeyPressed(Keyboard::D)) { player2.dx = 1; player2.dy = 0; } // Condition check ho rahi hai
            else if (Keyboard::isKeyPressed(Keyboard::W)) { player2.dx = 0; player2.dy = -1; } // Condition check ho rahi hai
            else if (Keyboard::isKeyPressed(Keyboard::S)) { player2.dx = 0; player2.dy = 1; } // Condition check ho rahi hai
            // If no movement keys are pressed, dx and dy remain from the previous frame, allowing continuous movement.

            // Check if player 2 direction changed or stopped // Condition check ho rahi hai
            if (player2.dx != prev_dx || player2.dy != prev_dy || (player2.dx == 0 && player2.dy == 0 && (prev_dx != 0 || prev_dy != 0))) { // Condition check ho rahi hai
                player2.tilesCapturedInMove = 0;
                player2.isConstructing = false;
            } // Code block ka end
        } // Code block ka end
        else if (player2.isAlive && player1.powerUpActive) { // Condition check ho rahi hai
            // Player 2 is frozen by Player 1's power-up
            player2.dx = player2.dy = 0;
        } // Code block ka end
        if (!player2.isAlive) { // Stop movement if not alive // Condition check ho rahi hai
            player2.dx = player2.dy = 0;
        } // Code block ka end


        // --- Game Logic ---
        if (timer > delay) { // Condition check ho rahi hai
            // Player 1 Movement and Grid Update
            if (player1.isAlive) { // Condition check ho rahi hai
                player1.x += player1.dx; player1.y += player1.dy;

                // Boundary checks
                if (player1.x < 0) player1.x = 0; if (player1.x >= N) player1.x = N - 1; // Condition check ho rahi hai
                if (player1.y < 0) player1.y = 0; if (player1.y >= M) player1.y = M - 1; // Condition check ho rahi hai

                // Collision with own trail (grid value 2)
                if (grid[player1.y][player1.x] == 2) { player1.isAlive = false; } // Condition check ho rahi hai
                // Collision with other player's trail (grid value 3) - Handled below in player-player trail collision


               // Marking the trail or hitting solid boundary
                if (grid[player1.y][player1.x] == 0) { // Condition check ho rahi hai
                    grid[player1.y][player1.x] = 2; // Mark as Player 1 trail
                    player1.score++; // Basic point for capturing a tile // For loop - repeat karne ke liye block of code
                    player1.tilesCapturedInMove++;
                    player1.isConstructing = true;
                } // Code block ka end
                else if (grid[player1.y][player1.x] == 1) { // Hit a solid boundary // Condition check ho rahi hai
                    player1.dx = player1.dy = 0; // Stop player movement
                    player1.isConstructing = false;

                    // Calculate and apply bonus points if applicable // Condition check ho rahi hai
                    if (player1.tilesCapturedInMove > player1.bonusThreshold) { // Condition check ho rahi hai
                        int bonusPoints = player1.tilesCapturedInMove * (player1.currentMultiplier - 1);
                        player1.score += bonusPoints;
                        player1.rewardCounter++;
                        cout << "P1 Bonus! Tiles: " << player1.tilesCapturedInMove << ", Mult: x" << player1.currentMultiplier << ", Bonus Pts: " << bonusPoints << ", Total Score: " << player1.score << endl; // Output screen pe kuch print kar rahe hain


                        if (player1.rewardCounter == 3) { // Condition check ho rahi hai
                            player1.bonusThreshold = 5;
                            player1.currentMultiplier = 2;
                        } // Code block ka end
                        else if (player1.rewardCounter == 5) { // Condition check ho rahi hai
                            player1.bonusThreshold = 5;
                            player1.currentMultiplier = 4;
                        } // Code block ka end
                        else if (player1.rewardCounter > 5) { // Condition check ho rahi hai
                            player1.bonusThreshold = 5;
                            player1.currentMultiplier = 4;
                        } // Code block ka end
                        else if (player1.rewardCounter < 3 && player1.tilesCapturedInMove > 10) { // Condition check ho rahi hai
                            player1.currentMultiplier = 2;
                        } // Code block ka end
                        else { // Agar if condition false ho to yeh block chalega
                            player1.currentMultiplier = 1;
                        } // Code block ka end
                    } // Code block ka end
                    player1.tilesCapturedInMove = 0;


                    // Use drop function to identify enclosed areas // Condition check ho rahi hai
                    for (int i = 0; i < enemyCount; i++) { // For loop - repeat karne ke liye block of code
                        if (grid[a[i].y / ts][a[i].x / ts] == 0) // Only drop from un-filled areas where enemy is inside // Condition check ho rahi hai
                            drop(a[i].y / ts, a[i].x / ts);
                    } // Code block ka end

                    // Fill enclosed areas or convert trails to solid
                    for (int i = 0; i < M; i++) { // For loop - repeat karne ke liye block of code
                        for (int j = 0; j < N; j++) { // For loop - repeat karne ke liye block of code
                            if (grid[i][j] == -1) grid[i][j] = 0; // Keep outside areas empty // Condition check ho rahi hai
                            else if (grid[i][j] == 2) grid[i][j] = 1; // Convert Player 1's trail to solid // Condition check ho rahi hai
                            else if (grid[i][j] == 3) grid[i][j] = 1; // Also convert Player 2's trail if enclosed // Condition check ho rahi hai
                            else if (grid[i][j] == 0) { // This is an enclosed area // Condition check ho rahi hai
                                grid[i][j] = 1; // Fill enclosed areas
                                player1.score += 2; // Bonus points per filled cell - Award to Player 1 who enclosed the area
                            } // Code block ka end
                        } // Code block ka end
                    } // Code block ka end
                } // Code block ka end
            } // Code block ka end

            // Player 2 Movement and Grid Update
            if (player2.isAlive) { // Condition check ho rahi hai
                player2.x += player2.dx; player2.y += player2.dy;

                // Boundary checks
                if (player2.x < 0) player2.x = 0; if (player2.x >= N) player2.x = N - 1; // Condition check ho rahi hai
                if (player2.y < 0) player2.y = 0; if (player2.y >= M) player2.y = M - 1; // Condition check ho rahi hai

                // Collision with own trail (grid value 3)
                if (grid[player2.y][player2.x] == 3) { player2.isAlive = false; } // Condition check ho rahi hai
                // Collision with other player's trail (grid value 2) - Handled below in player-player trail collision

                // Marking the trail or hitting solid boundary
                if (grid[player2.y][player2.x] == 0) { // Condition check ho rahi hai
                    grid[player2.y][player2.x] = 3; // Mark as Player 2 trail
                    player2.score++; // Basic point for capturing a tile // For loop - repeat karne ke liye block of code
                    player2.tilesCapturedInMove++;
                    player2.isConstructing = true;
                } // Code block ka end
                else if (grid[player2.y][player2.x] == 1) { // Hit a solid boundary // Condition check ho rahi hai
                    player2.dx = player2.dy = 0; // Stop player movement
                    player2.isConstructing = false;

                    // Calculate and apply bonus points if applicable // Condition check ho rahi hai
                    if (player2.tilesCapturedInMove > player2.bonusThreshold) { // Condition check ho rahi hai
                        int bonusPoints = player2.tilesCapturedInMove * (player2.currentMultiplier - 1);
                        player2.score += bonusPoints;
                        player2.rewardCounter++;
                        cout << "P2 Bonus! Tiles: " << player2.tilesCapturedInMove << ", Mult: x" << player2.currentMultiplier << ", Bonus Pts: " << bonusPoints << ", Total Score: " << player2.score << endl; // Output screen pe kuch print kar rahe hain


                        if (player2.rewardCounter == 3) { // Condition check ho rahi hai
                            player2.bonusThreshold = 5;
                            player2.currentMultiplier = 2;
                        } // Code block ka end
                        else if (player2.rewardCounter == 5) { // Condition check ho rahi hai
                            player2.bonusThreshold = 5;
                            player2.currentMultiplier = 4;
                        } // Code block ka end
                        else { // Agar if condition false ho to yeh block chalega
                            player2.currentMultiplier = 1;
                        } // Code block ka end
                    } // Code block ka end
                    player2.tilesCapturedInMove = 0;


                    // Use drop function to mark outside areas before filling // For loop - repeat karne ke liye block of code
                    for (int i = 0; i < enemyCount; i++) { // For loop - repeat karne ke liye block of code
                        if (grid[a[i].y / ts][a[i].x / ts] == 0) // Condition check ho rahi hai
                            drop(a[i].y / ts, a[i].x / ts);
                    } // Code block ka end

                    // Fill enclosed areas or convert trails to solid for Player 2 // For loop - repeat karne ke liye block of code
                    for (int i = 0; i < M; i++) { // For loop - repeat karne ke liye block of code
                        for (int j = 0; j < N; j++) { // For loop - repeat karne ke liye block of code
                            if (grid[i][j] == -1) grid[i][j] = 0; // Reset outside areas // Condition check ho rahi hai
                            else if (grid[i][j] == 3) grid[i][j] = 1; // Convert Player 2's trail to solid // Condition check ho rahi hai
                            else if (grid[i][j] == 2) grid[i][j] = 1; // Also convert Player 1's trail if enclosed // Condition check ho rahi hai
                            else if (grid[i][j] == 0) { // This is an enclosed area // Condition check ho rahi hai
                                grid[i][j] = 1; // Fill enclosed areas
                                player2.score += 2; // Bonus points per filled cell - Award to Player 2 who enclosed the area
                            } // Code block ka end
                        } // Code block ka end
                    } // Code block ka end
                } // Code block ka end
            } // Code block ka end
            timer = 0;
        } // Code block ka end

        // Power-up granting based on score thresholds (for both players) // For loop - repeat karne ke liye block of code
        // Check Player 1's score and grant power-ups to Player 1
        if (player1.score >= 50 && player1.powerUpCount < 1) player1.grantPowerUp(); // Condition check ho rahi hai
        if (player1.score >= 70 && player1.powerUpCount < 2) player1.grantPowerUp(); // Condition check ho rahi hai
        if (player1.score >= 100 && player1.powerUpCount < 3) player1.grantPowerUp(); // Condition check ho rahi hai
        if (player1.score >= 130) { // Condition check ho rahi hai
            int powerUpsDue = 3 + (player1.score - 130) / 30;
            while (player1.powerUpCount < powerUpsDue) player1.grantPowerUp(); // Jab tak condition true hai, loop chalta rahega
        } // Code block ka end

        // Check Player 2's score and grant power-ups to Player 2
        if (player2.score >= 50 && player2.powerUpCount < 1) player2.grantPowerUp(); // Condition check ho rahi hai
        if (player2.score >= 70 && player2.powerUpCount < 2) player2.grantPowerUp(); // Condition check ho rahi hai
        if (player2.score >= 100 && player2.powerUpCount < 3) player2.grantPowerUp(); // Condition check ho rahi hai
        if (player2.score >= 130) { // Condition check ho rahi hai
            int powerUpsDue = 3 + (player2.score - 130) / 30;
            while (player2.powerUpCount < powerUpsDue) player2.grantPowerUp(); // Jab tak condition true hai, loop chalta rahega
        } // Code block ka end


        // --- Player-Player Collision ---
        if (player1.isAlive && player2.isAlive && player1.x == player2.x && player1.y == player2.y) { // Condition check ho rahi hai
            // Rule: If both are moving (constructing), both die. If one is moving into the other (stationary or also moving), the moving one(s) die.
            bool p1_moving = (player1.dx != 0 || player1.dy != 0);
            bool p2_moving = (player2.dx != 0 || player2.dy != 0);

            if (p1_moving && p2_moving) { // Both moving, both die // Condition check ho rahi hai
                player1.isAlive = false;
                player2.isAlive = false;
            } // Code block ka end
            else if (p1_moving) { // P1 moving // Condition check ho rahi hai
                player1.isAlive = false;
            } // Code block ka end
            else if (p2_moving) { // P2 moving // Condition check ho rahi hai
                player2.isAlive = false;
            } // Code block ka end
            // If neither is moving and they collide (shouldn't happen with current movement logic), they don't die.
        } // Code block ka end

        // --- Player-Trail Collision ---
        // Handled within player movement: player hits own trail (grid[y][x] == 2 or 3)


        // --- Player-Other Player's Trail Collision (while constructing) --- // Jab tak condition true hai, loop chalta rahega
        if (player1.isAlive && player2.isAlive) { // Condition check ho rahi hai
            // If Player 1 touches Player 2's trail while P1 is constructing // Jab tak condition true hai, loop chalta rahega
            if (player1.isConstructing && grid[player1.y][player1.x] == 3) { player1.isAlive = false; player1.dx = player1.dy = 0; } // Condition check ho rahi hai
            // If Player 2 touches Player 1's trail while P2 is constructing // Jab tak condition true hai, loop chalta rahega
            if (player2.isConstructing && grid[player2.y][player2.x] == 2) { player2.isAlive = false; player2.dx = player2.dy = 0; } // Condition check ho rahi hai
        } // Code block ka end


        // Enemy movement and collision
        for (int i = 0; i < enemyCount; i++) { // For loop - repeat karne ke liye block of code
            // Enemies are frozen if either player's power-up is active // Condition check ho rahi hai
            bool enemiesFrozen = player1.powerUpActive || player2.powerUpActive;
            if (!enemiesFrozen) { // Condition check ho rahi hai
                a[i].move();
            } // Code block ka end
            a[i].isFrozen = enemiesFrozen; // Update enemy frozen status for drawing/visuals // For loop - repeat karne ke liye block of code

            // Check for enemy collision with player characters and trails // For loop - repeat karne ke liye block of code
            if (player1.isAlive && a[i].x / ts == player1.x && a[i].y / ts == player1.y) { player1.isAlive = false; } // Condition check ho rahi hai
            if (player2.isAlive && a[i].x / ts == player2.x && a[i].y / ts == player2.y) { player2.isAlive = false; } // Condition check ho rahi hai
            if (player1.isAlive && grid[a[i].y / ts][a[i].x / ts] == 2) { player1.isAlive = false; } // Condition check ho rahi hai
            if (player2.isAlive && grid[a[i].y / ts][a[i].x / ts] == 3) { player2.isAlive = false; } // Condition check ho rahi hai
        } // Code block ka end


        // --- Drawing ---
        window.clear();
        for (int i = 0; i < M; i++) { // For loop - repeat karne ke liye block of code
            for (int j = 0; j < N; j++) { // For loop - repeat karne ke liye block of code
                if (grid[i][j] == 0) continue; // Condition check ho rahi hai
                if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts)); // Solid // Condition check ho rahi hai
                if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts)); // Player 1 trail (green) // Condition check ho rahi hai
                if (grid[i][j] == 3) sTile.setTextureRect(IntRect(18, 0, ts, ts)); // Player 2 trail (darker blue/purple-ish from tiles.png) // Condition check ho rahi hai
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            } // Code block ka end
        } // Code block ka end

        // Draw players
        if (player1.isAlive) { // Condition check ho rahi hai
            sTile.setTextureRect(IntRect(36, 0, ts, ts)); // Player 1 tile
            sTile.setPosition(player1.x * ts, player1.y * ts);
            window.draw(sTile);
        } // Code block ka end
        if (player2.isAlive) { // Condition check ho rahi hai
            sTile.setTextureRect(IntRect(36, 0, ts, ts)); // Player 2 tile (same appearance for now) // For loop - repeat karne ke liye block of code
            sTile.setPosition(player2.x * ts, player2.y * ts);
            window.draw(sTile);
        } // Code block ka end

        // Draw enemies
        // Rotate enemies only if not frozen by power-up // Condition check ho rahi hai
        bool enemiesFrozen = player1.powerUpActive || player2.powerUpActive;
        sEnemy.rotate(enemiesFrozen ? 0 : 10);
        for (int i = 0; i < enemyCount; i++) { // For loop - repeat karne ke liye block of code
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        } // Code block ka end

        // Display scores
        char scoreStr1[20], scoreStr2[20];
        sprintf(scoreStr1, "P1 Score: %d", player1.score);
        sprintf(scoreStr2, "P2 Score: %d", player2.score);

        Text scoreText1(scoreStr1, font, 20);
        scoreText1.setPosition(10, 10);
        scoreText1.setFillColor(Color::White);
        window.draw(scoreText1);

        Text scoreText2(scoreStr2, font, 20);
        scoreText2.setPosition(N * ts - scoreText2.getGlobalBounds().width - 10, 10); // Position on the right side
        scoreText2.setFillColor(Color::White);
        window.draw(scoreText2);

        // Display power-up counts
        char powerUpStr1[20], powerUpStr2[20];
        sprintf(powerUpStr1, "P1 Power-ups: %d", player1.powerUpCount);
        sprintf(powerUpStr2, "P2 Power-ups: %d", player2.powerUpCount);

        Text powerUpText1(powerUpStr1, font, 20);
        powerUpText1.setPosition(10, 40);
        powerUpText1.setFillColor(Color::Yellow);
        window.draw(powerUpText1);

        Text powerUpText2(powerUpStr2, font, 20);
        powerUpText2.setPosition(N * ts - powerUpText2.getGlobalBounds().width - 10, 40);
        powerUpText2.setFillColor(Color::Yellow);
        window.draw(powerUpText2);

        // Display power-up active status
        if (player1.powerUpActive) { // Condition check ho rahi hai
            Text activeText("P1 POWER UP ACTIVE!", font, 24);
            activeText.setPosition(N * ts / 2.0f - activeText.getGlobalBounds().width / 2.0f, 10.0f);
            activeText.setFillColor(Color::Red);
            window.draw(activeText);
        } // Code block ka end
        if (player2.powerUpActive) { // Condition check ho rahi hai
            Text activeText("P2 POWER UP ACTIVE!", font, 24);
            activeText.setPosition(N * ts / 2.0f - activeText.getGlobalBounds().width / 2.0f, 40.0f);
            activeText.setFillColor(Color::Red);
            window.draw(activeText);
        } // Code block ka end


        window.display();
    } // Code block ka end

    // Game Over for Multiplayer sequence // For loop - repeat karne ke liye block of code
    // Determine Match Outcome for Player 1 (logged-in user) // For loop - repeat karne ke liye block of code
    char outcome = 'D'; // Assume Draw initially
    if (player1.score > player2.score) outcome = 'W'; // Player 1 wins // Condition check ho rahi hai
    else if (player2.score > player1.score) outcome = 'L'; // Player 1 loses // Condition check ho rahi hai

    // Update Player 1's profile with final score and match history
    currentProfile.totalPoints += player1.score; // Add score to total points
    if (currentProfile.matchCount < MAX_HISTORY) { // Condition check ho rahi hai
        currentProfile.matchHistory[currentProfile.matchCount++] = outcome;
    } // Code block ka end
    // Update high score if applicable for Player 1 // Condition check ho rahi hai
    if (player1.score > currentProfile.highScore) { // Condition check ho rahi hai
        currentProfile.highScore = player1.score;
    } // Code block ka end
    saveOrUpdateProfile(currentProfile);

    // Update leaderboard with Player 1's total score
    leaderboard.insertOrUpdate(currentProfile.username, currentProfile.totalPoints);

    // Note: Player 2's score and match history are not saved persistently in this implementation
    // unless you implement a separate login/profile system for Player 2 as well. // For loop - repeat karne ke liye block of code

    // Show End Menu
    while (window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
        // Pass both scores to the End Menu for display, Player 1's high score is also passed // For loop - repeat karne ke liye block of code
        bool restart = showEndMenu(window, font, player1.score, player2.score, currentProfile.highScore);
        if (restart) { // Condition check ho rahi hai
            // Reset player states and scores for a new game // For loop - repeat karne ke liye block of code
            runMultiplayerGame(window, sTile, sGameover, sEnemy, font); // Restart the game
            return; // Exit the current game instance // Program successful end pe return 0 karta hai
        } // Code block ka end
        else { // Agar if condition false ho to yeh block chalega
            return; // Go back to main menu // Program successful end pe return 0 karta hai
        } // Code block ka end
    } // Code block ka end
} // Code block ka end

void showLeaderboard(RenderWindow& window, Font& font) { // Function define kar rahe hain jo kuch return nahi karta
    leaderboard.display(window, font);
} // Code block ka end

void showPlayerProfile(RenderWindow& window, Font& font) { // Function define kar rahe hain jo kuch return nahi karta
    // Options for profile menu // For loop - repeat karne ke liye block of code
    const char* profileMenuOptions[] = { "Add Friend", "Remove Friend", "Back" };
    int profileMenuSize = 3;
    int profileSelected = 0;

    bool inProfileMenu = true;

    while (window.isOpen() && inProfileMenu) { // Jab tak condition true hai, loop chalta rahega
        window.clear(Color::Black);

        char buffer[200]; // Increased buffer size
        Text title("Player Profile", font, 30);
        title.setPosition(100, 30);
        title.setFillColor(Color::Cyan);
        window.draw(title);

        // Username
        sprintf(buffer, "Username: %s", currentProfile.username);
        Text t1(buffer, font, 20);
        t1.setPosition(100, 80);
        t1.setFillColor(Color::White);
        window.draw(t1);

        // Total Points
        sprintf(buffer, "Total Points: %d", currentProfile.totalPoints);
        Text t2(buffer, font, 20);
        t2.setPosition(100, 110);
        t2.setFillColor(Color::White);
        window.draw(t2);

        // High Score
        sprintf(buffer, "High Score: %d", currentProfile.highScore);
        Text t_hs(buffer, font, 20);
        t_hs.setPosition(100, 140);
        t_hs.setFillColor(Color::White);
        window.draw(t_hs);


        // Match History
        Text t3_label("Match History:", font, 20);
        t3_label.setPosition(100, 170);
        t3_label.setFillColor(Color::White);
        window.draw(t3_label);

        if (currentProfile.matchCount == 0) { // Condition check ho rahi hai
            Text t3_history("No matches played yet.", font, 18);
            t3_history.setPosition(120, 200);
            t3_history.setFillColor(Color::White);
            window.draw(t3_history);
        } // Code block ka end
        else { // Agar if condition false ho to yeh block chalega
            // Display match history more clearly
            for (int i = 0; i < currentProfile.matchCount; ++i) { // For loop - repeat karne ke liye block of code
                char match_outcome[30];
                const char* outcome_str = "";
                Color outcome_color = Color::White;

                if (currentProfile.matchHistory[i] == 'W') { // Condition check ho rahi hai
                    outcome_str = "Win";
                    outcome_color = Color::Green;
                } // Code block ka end
                else if (currentProfile.matchHistory[i] == 'L') { // Condition check ho rahi hai
                    outcome_str = "Loss";
                    outcome_color = Color::Red;
                } // Code block ka end
                else if (currentProfile.matchHistory[i] == 'D') { // Condition check ho rahi hai
                    outcome_str = "Draw";
                    outcome_color = Color::Yellow;
                } // Code block ka end
                else { // Agar if condition false ho to yeh block chalega
                    outcome_str = "Unknown"; // Handle unexpected values
                } // Code block ka end
                sprintf(match_outcome, "Match %d: %s", i + 1, outcome_str);
                Text t3_history(match_outcome, font, 18);
                t3_history.setPosition(120, 200 + i * 25);
                t3_history.setFillColor(outcome_color);
                window.draw(t3_history);
            } // Code block ka end
        } // Code block ka end


        // Friends List
        Text t4("Friends:", font, 20);
        // Calculate position based on previous content
        float friends_list_y = 200.0f + (currentProfile.matchCount > 0 ? currentProfile.matchCount : 1) * 25.0f + 20.0f;
        t4.setPosition(100, friends_list_y);
        t4.setFillColor(Color::White);
        window.draw(t4);

        if (currentProfile.friendCount == 0) { // Condition check ho rahi hai
            Text fr("No friends added yet.", font, 18);
            fr.setPosition(120, t4.getPosition().y + 30);
            fr.setFillColor(Color::White);
            window.draw(fr);
        } // Code block ka end
        else { // Agar if condition false ho to yeh block chalega
            for (int i = 0; i < currentProfile.friendCount; i++) { // For loop - repeat karne ke liye block of code
                Text fr(currentProfile.friends[i], font, 18);
                fr.setPosition(120, t4.getPosition().y + 30 + i * 25);
                fr.setFillColor(Color::Green);
                window.draw(fr);
            } // Code block ka end
        } // Code block ka end

        // Draw Profile Menu options
        float menu_start_y = t4.getPosition().y + 30.0f + (currentProfile.friendCount > 0 ? currentProfile.friendCount : 1) * 25.0f + 30.0f;

        for (int i = 0; i < profileMenuSize; ++i) { // For loop - repeat karne ke liye block of code
            Text menuText(profileMenuOptions[i], font, 24);
            menuText.setPosition(100, menu_start_y + i * 40.0f);
            menuText.setFillColor(i == profileSelected ? Color::Yellow : Color::White);
            window.draw(menuText);
        } // Code block ka end

        window.display();

        // Handle events for profile menu // For loop - repeat karne ke liye block of code
        Event e;
        while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
            if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
            if (e.type == Event::KeyPressed) { // Condition check ho rahi hai
                if (e.key.code == Keyboard::Up) { // Condition check ho rahi hai
                    profileSelected = (profileSelected - 1 + profileMenuSize) % profileMenuSize;
                } // Code block ka end
                else if (e.key.code == Keyboard::Down) { // Condition check ho rahi hai
                    profileSelected = (profileSelected + 1) % profileMenuSize;
                } // Code block ka end
                else if (e.key.code == Keyboard::Enter) { // Condition check ho rahi hai
                    if (profileSelected == 0) { // Add Friend // Condition check ho rahi hai
                        string friendName = draw_textbox(window, font, "Enter friend's username:", { 150, 300 });
                        if (friendName.empty() && window.isOpen()) continue; // Condition check ho rahi hai
                        if (!window.isOpen()) { inProfileMenu = false; break; } // Condition check ho rahi hai

                        if (friendName.length() >= 50) { // Condition check ho rahi hai
                            show_message(window, font, "Username too long!", Color::Red, { 150, 400 }, 2.0f);
                            continue;
                        } // Code block ka end

                        if (strcmp(currentProfile.username, friendName.c_str()) == 0) { // Condition check ho rahi hai
                            show_message(window, font, "Cannot add yourself as a friend!", Color::Red, { 150, 400 }, 2.0f);
                            continue;
                        } // Code block ka end

                        // Check if friend exists in profiles.txt // Condition check ho rahi hai
                        PlayerProfile tempFriendProfile;
                        if (!loadPlayerProfile(friendName.c_str(), tempFriendProfile)) { // Condition check ho rahi hai
                            show_message(window, font, "Friend not found!", Color::Red, { 150, 400 }, 2.0f);
                            continue;
                        } // Code block ka end

                        // Check if already friends // Condition check ho rahi hai
                        bool alreadyFriends = false;
                        for (int i = 0; i < currentProfile.friendCount; ++i) { // For loop - repeat karne ke liye block of code
                            if (strcmp(currentProfile.friends[i], friendName.c_str()) == 0) { // Condition check ho rahi hai
                                alreadyFriends = true;
                                break;
                            } // Code block ka end
                        } // Code block ka end

                        if (alreadyFriends) { // Condition check ho rahi hai
                            show_message(window, font, "Already friends with " + friendName + "!", Color::Red, { 150, 400 }, 2.0f);
                        } // Code block ka end
                        else if (currentProfile.friendCount < MAX_FRIENDS) { // Condition check ho rahi hai
                            strcpy(currentProfile.friends[currentProfile.friendCount++], friendName.c_str());
                            saveOrUpdateProfile(currentProfile);
                            show_message(window, font, friendName + " added as friend!", Color::Green, { 150, 400 }, 2.0f);
                        } // Code block ka end
                        else { // Agar if condition false ho to yeh block chalega
                            show_message(window, font, "Friend list is full!", Color::Red, { 150, 400 }, 2.0f);
                        } // Code block ka end
                    } // Code block ka end
                    else if (profileSelected == 1) { // Remove Friend // Condition check ho rahi hai
                        string friendName = draw_textbox(window, font, "Enter friend's username to remove:", { 150, 300 });
                        if (friendName.empty() && window.isOpen()) continue; // Condition check ho rahi hai
                        if (!window.isOpen()) { inProfileMenu = false; break; } // Condition check ho rahi hai

                        bool found = false;
                        for (int i = 0; i < currentProfile.friendCount; ++i) { // For loop - repeat karne ke liye block of code
                            if (strcmp(currentProfile.friends[i], friendName.c_str()) == 0) { // Condition check ho rahi hai
                                // Shift elements to remove // Condition check ho rahi hai
                                for (int j = i; j < currentProfile.friendCount - 1; ++j) { // For loop - repeat karne ke liye block of code
                                    strcpy(currentProfile.friends[j], currentProfile.friends[j + 1]);
                                } // Code block ka end
                                currentProfile.friendCount--;
                                found = true;
                                saveOrUpdateProfile(currentProfile);
                                show_message(window, font, friendName + " removed from friends!", Color::Green, { 150, 400 }, 2.0f);
                                break;
                            } // Code block ka end
                        } // Code block ka end
                        if (!found) { // Condition check ho rahi hai
                            show_message(window, font, friendName + " not found in friends list!", Color::Red, { 150, 400 }, 2.0f);
                        } // Code block ka end
                    } // Code block ka end
                    else if (profileSelected == 2) { // Back // Condition check ho rahi hai
                        inProfileMenu = false;
                    } // Code block ka end
                } // Code block ka end
                else if (e.key.code == Keyboard::Escape) { // Allow escape to go back // Condition check ho rahi hai
                    inProfileMenu = false;
                } // Code block ka end
            } // Code block ka end
        } // Code block ka end
    } // Code block ka end
} // Code block ka end


// Function to show game instructions
void showInstructions(RenderWindow& window, Font& font) { // Function define kar rahe hain jo kuch return nahi karta
    window.clear(Color::Black);

    Text title("How to Play Xonix", font, 30);
    title.setPosition(window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, 30.0f);
    title.setFillColor(Color::Cyan);
    window.draw(title);

    string instructions_text =
        "Objective:\n"
        "Capture as much of the game board as possible without being hit by enemies or your own trail.\n\n"
        "Controls:\n"
        "Player 1: Arrow Keys (Move), Space (Activate Power-up)\n"
        "Player 2 (Multiplayer): WASD (Move), Right Shift (Activate Power-up)\n\n" // Condition check ho rahi hai
        "Gameplay:\n"
        "- Move to draw a trail. When you hit a solid wall, the area you enclosed will be filled.\n"
        "- Avoid enemies! If they hit you or your trail while you are constructing, you lose.\n" // Jab tak condition true hai, loop chalta rahega
        "- In Multiplayer, players can also hit each other or their trails (see Profile section for rules).\n\n" // For loop - repeat karne ke liye block of code
        "Points System:\n"
        "- Capture a tile: 1 point.\n"
        "- Capture >10 tiles in a move: x2 points.\n"
        "- After 3 x2 bonuses, threshold for x2 bonus reduces to 5 tiles.\n" // For loop - repeat karne ke liye block of code
        "- After 5 total x2 bonuses, capturing >5 tiles gives x4 points.\n\n"
        "Power-ups:\n"
        "- Earned at scores of 50, 70, 100, 130, and every 30 points thereafter.\n"
        "- Activate to freeze all enemies (and the other player in multiplayer) for 3 seconds.\n" // For loop - repeat karne ke liye block of code
        "- Unused power-ups stack.\n\n"
        "Press ESC to return to Main Menu";

    Text instructions(instructions_text, font, 18);
    instructions.setPosition(50, 100);
    instructions.setFillColor(Color::White);
    window.draw(instructions);

    window.display();

    while (window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
        Event e;
        while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
            if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) // Condition check ho rahi hai
                return; // Program successful end pe return 0 karta hai
        } // Code block ka end
    } // Code block ka end
} // Code block ka end


int main() { // Main function start ho raha hai, yahan se program run hota hai
    srand(time(0));

    // Game Setup - use a slightly larger window for authentication screens initially // For loop - repeat karne ke liye block of code
    RenderWindow window(VideoMode(1024, 768), "Xonix Game");
    window.setFramerateLimit(60);

    Texture t1, t2, t3;
    if (!t1.loadFromFile("tiles.png")) { // Condition check ho rahi hai
        cerr << "Error loading tiles.png" << endl; return -1;
    } // Code block ka end
    if (!t2.loadFromFile("gameover.png")) { // Condition check ho rahi hai
        cerr << "Error loading gameover.png" << endl; return -1;
    } // Code block ka end
    if (!t3.loadFromFile("enemy.png")) { // Condition check ho rahi hai
        cerr << "Error loading enemy.png" << endl; return -1;
    } // Code block ka end

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(window.getSize().x / 2.0f - sGameover.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f - sGameover.getGlobalBounds().height / 2.0f);
    sEnemy.setOrigin(sEnemy.getLocalBounds().width / 2.0f, sEnemy.getLocalBounds().height / 2.0f);

    Font font;
    if (!font.loadFromFile("arial.ttf")) { // Condition check ho rahi hai
        cerr << "Error loading arial.ttf" << endl; return -1;
    } // Code block ka end

    loadLeaderboardFromFile();
    mmq.loadFromFile("match_queue.txt");
    resetGrid();

    bool authenticated = false;
    enum AuthState { AUTH_MENU, LOGIN, REGISTER };
    AuthState authState = AUTH_MENU;

    while (window.isOpen() && !authenticated) { // Jab tak condition true hai, loop chalta rahega
        if (authState == AUTH_MENU) { // Condition check ho rahi hai
            const char* authMenu[] = { "Login", "Register", "Exit" };
            int authMenuSize = 3;
            int authSelected = 0;
            bool menuSelectionMade = false;

            while (!menuSelectionMade && window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
                window.clear(Color::Black);
                Text welcomeText("Welcome to Xonix Game", font, 30);
                welcomeText.setPosition(window.getSize().x / 2.0f - welcomeText.getGlobalBounds().width / 2.0f, 50.0f);
                welcomeText.setFillColor(Color::Cyan);
                window.draw(welcomeText);

                Text instructionText("Use UP/DOWN keys to navigate, ENTER to select", font, 18);
                instructionText.setPosition(window.getSize().x / 2.0f - instructionText.getGlobalBounds().width / 2.0f, 100.0f);
                instructionText.setFillColor(Color::White);
                window.draw(instructionText);

                for (int i = 0; i < authMenuSize; ++i) { // For loop - repeat karne ke liye block of code
                    Text text(authMenu[i], font, 24);
                    text.setPosition(window.getSize().x / 2.0f - text.getGlobalBounds().width / 2.0f, 200.0f + i * 50.0f);
                    text.setFillColor(i == authSelected ? Color::Yellow : Color::White);
                    window.draw(text);
                } // Code block ka end

                window.display();

                Event e;
                while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
                    if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
                    if (e.type == Event::KeyPressed) { // Condition check ho rahi hai
                        if (e.key.code == Keyboard::Up) authSelected = (authSelected - 1 + authMenuSize) % authMenuSize; // Condition check ho rahi hai
                        else if (e.key.code == Keyboard::Down) authSelected = (authSelected + 1) % authMenuSize; // Condition check ho rahi hai
                        else if (e.key.code == Keyboard::Enter) { // Condition check ho rahi hai
                            menuSelectionMade = true;
                            if (authSelected == 0) authState = LOGIN; // Condition check ho rahi hai
                            else if (authSelected == 1) authState = REGISTER; // Condition check ho rahi hai
                            else if (authSelected == 2) { window.close(); return 0; } // Condition check ho rahi hai
                        } // Code block ka end
                        if (e.key.code == Keyboard::Escape) { window.close(); return 0; } // Condition check ho rahi hai
                    } // Code block ka end
                } // Code block ka end
            } // Code block ka end
        } // Code block ka end
        else if (authState == LOGIN) { // Condition check ho rahi hai
            authenticated = login(window, font);
            if (!authenticated && window.isOpen()) authState = AUTH_MENU; // Condition check ho rahi hai
            else if (!window.isOpen()) return 0; // Condition check ho rahi hai
        } // Code block ka end
        else if (authState == REGISTER) { // Condition check ho rahi hai
            bool registered = registration(window, font);
            if (window.isOpen()) authState = AUTH_MENU; // Condition check ho rahi hai
            else return 0; // Agar if condition false ho to yeh block chalega
        } // Code block ka end
    } // Code block ka end

    if (!authenticated && !window.isOpen()) return 0; // Condition check ho rahi hai
    window.close();
    window.create(VideoMode(N * ts, M * ts), "Xonix Game");
    window.setFramerateLimit(60);

    const char* mainMenu[] = { // Code block ka start
     "Single Player", "Multiplayer", "Leaderboard", "Profile", "How to Play",
     "Join Matchmaking Queue", "Match Players", "Manage Friend Requests", "Send Friend Request", "Exit"
    }; // Code block ka end
    int menuSize = 10;


    int selected = 0;
    enum GameState { MENU, SINGLE_PLAYER, MULTIPLAYER, LEADERBOARD, PROFILE, INSTRUCTIONS }; // Updated enum
    GameState currentState = MENU;

    while (window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
        if (currentState == MENU) { // Condition check ho rahi hai
            window.clear(Color::Black);
            Text titleText("Xonix Game", font, 36);
            titleText.setPosition(N * ts / 2.0f - titleText.getGlobalBounds().width / 2.0f, 50.0f);
            titleText.setFillColor(Color::Cyan);
            window.draw(titleText);

            for (int i = 0; i < menuSize; ++i) { // For loop - repeat karne ke liye block of code
                Text text(mainMenu[i], font, 30);
                text.setPosition(N * ts / 2.0f - text.getGlobalBounds().width / 2.0f, M * ts / 2.0f - 40.0f + i * 40.0f);
                text.setFillColor(i == selected ? Color::Yellow : Color::White);
                window.draw(text);
            } // Code block ka end

            window.display();

            Event e;
            while (window.pollEvent(e)) { // Jab tak condition true hai, loop chalta rahega
                if (e.type == Event::Closed) window.close(); // Condition check ho rahi hai
                if (e.type == Event::KeyPressed) { // Condition check ho rahi hai
                    if (e.key.code == Keyboard::Up) selected = (selected - 1 + menuSize) % menuSize; // Condition check ho rahi hai
                    else if (e.key.code == Keyboard::Down) selected = (selected + 1) % menuSize; // Condition check ho rahi hai
                    else if (e.key.code == Keyboard::Enter) { // Condition check ho rahi hai
                        if (selected == 0) currentState = SINGLE_PLAYER; // Condition check ho rahi hai
                        else if (selected == 1) currentState = MULTIPLAYER; // Condition check ho rahi hai
                        else if (selected == 2) currentState = LEADERBOARD; // Condition check ho rahi hai
                        else if (selected == 3) currentState = PROFILE; // Condition check ho rahi hai
                        else if (selected == 4) currentState = INSTRUCTIONS; // Added Instructions option // Condition check ho rahi hai
                        else if (selected == 5) { // Join Matchmaking Queue // Condition check ho rahi hai
                            mmq.insert(currentProfile.username, currentProfile.totalPoints);
                            mmq.saveToFile("match_queue.txt");
                            show_message(window, font, "Added to matchmaking queue!", Color::Green, { N * ts / 2.0f - 150.0f, M * ts - 50.0f }, 2.0f);
                        } // Code block ka end
                        else if (selected == 6) { // Match Players // Condition check ho rahi hai
                            bool matched = mmq.match(window, font, leaderboard); // Pass window, font, and leaderboard for updates // For loop - repeat karne ke liye block of code
                            mmq.saveToFile("match_queue.txt");
                            if (!matched) { // Condition check ho rahi hai
                                show_message(window, font, "Not enough players to match!", Color::Red, { N * ts / 2.0f - 150.0f, M * ts - 50.0f }, 2.0f);
                            } // Code block ka end
                        } // Code block ka end
                        else if (selected == 7) { // Condition check ho rahi hai
                            // 🔽 Manage Friend Requests
                            if (currentProfile.pendingReceivedCount == 0) { // Condition check ho rahi hai
                                show_message(window, font, "No pending requests!", Color::Yellow, { N * ts / 2.0f - 150.0f, M * ts - 50.0f }, 2.0f);
                            } // Code block ka end
                            else { // Agar if condition false ho to yeh block chalega
                                for (int i = 0; i < currentProfile.pendingReceivedCount;) { // For loop - repeat karne ke liye block of code
                                    string requester = currentProfile.pendingReceived[i];
                                    string msg = "Friend request from: " + requester + " (A)ccept / (R)eject?";
                                    show_message(window, font, msg, Color::Cyan, { N * ts / 2.0f - 200.0f, M * ts - 150.0f }, 1.0f);

                                    bool decisionMade = false;
                                    while (!decisionMade && window.isOpen()) { // Jab tak condition true hai, loop chalta rahega
                                        Event e2;
                                        while (window.pollEvent(e2)) { // Jab tak condition true hai, loop chalta rahega
                                            if (e2.type == Event::Closed) window.close(); // Condition check ho rahi hai
                                            if (e2.type == Event::KeyPressed) { // Condition check ho rahi hai
                                                if (e2.key.code == Keyboard::A) { // Condition check ho rahi hai
                                                    if (acceptFriendRequest(currentProfile, requester.c_str())) { // Condition check ho rahi hai
                                                        show_message(window, font, "Friend added!", Color::Green, { N * ts / 2.0f - 150.0f, M * ts - 100.0f }, 1.5f);
                                                    } // Code block ka end
                                                    decisionMade = true;
                                                } // Code block ka end
                                                else if (e2.key.code == Keyboard::R) { // Condition check ho rahi hai
                                                    if (rejectFriendRequest(currentProfile, requester.c_str())) { // Condition check ho rahi hai
                                                        show_message(window, font, "Request rejected.", Color::Red, { N * ts / 2.0f - 150.0f, M * ts - 100.0f }, 1.5f);
                                                    } // Code block ka end
                                                    decisionMade = true;
                                                } // Code block ka end
                                            } // Code block ka end
                                        } // Code block ka end
                                    } // Code block ka end
                                } // Code block ka end
                            } // Code block ka end
                        } // Code block ka end
                        else if (selected == 8) { // Condition check ho rahi hai
                            // 🔽 Send Friend Request
                            string target;
                            show_message(window, font, "Enter username to send request to:", Color::White, { N * ts / 2.0f - 200.0f, M * ts - 150.0f }, 1.0f);

                            if (get_user_input(window, font, target)) { // Condition check ho rahi hai
                                if (sendFriendRequest(currentProfile, target.c_str())) { // Condition check ho rahi hai
                                    show_message(window, font, "Request sent!", Color::Green, { N * ts / 2.0f - 150.0f, M * ts - 100.0f }, 1.5f);
                                } // Code block ka end
                                else { // Agar if condition false ho to yeh block chalega
                                    show_message(window, font, "Failed to send request.", Color::Red, { N * ts / 2.0f - 150.0f, M * ts - 100.0f }, 1.5f);
                                } // Code block ka end
                            } // Code block ka end
                        } // Code block ka end
                        else if (selected == 9) window.close(); // Exit // Condition check ho rahi hai


                    } // Code block ka end
                    if (e.key.code == Keyboard::Escape) window.close(); // Condition check ho rahi hai
                } // Code block ka end
            } // Code block ka end
        } // Code block ka end
        else if (currentState == SINGLE_PLAYER) { // Condition check ho rahi hai
            runSinglePlayerGame(window, sTile, sGameover, sEnemy, font);
            currentState = MENU;
        } // Code block ka end
        else if (currentState == MULTIPLAYER) { // Condition check ho rahi hai
            runMultiplayerGame(window, sTile, sGameover, sEnemy, font);
            currentState = MENU;
        } // Code block ka end
        else if (currentState == LEADERBOARD) { // Condition check ho rahi hai
            showLeaderboard(window, font);
            currentState = MENU;
        } // Code block ka end
        else if (currentState == PROFILE) { // Condition check ho rahi hai
            showPlayerProfile(window, font);
            currentState = MENU;
        } // Code block ka end
        else if (currentState == INSTRUCTIONS) { // Added Instructions state // Condition check ho rahi hai
            showInstructions(window, font);
            currentState = MENU;
        } // Code block ka end
    } // Code block ka end
    return 0; // Program successful end pe return 0 karta hai
} // Code block ka end
