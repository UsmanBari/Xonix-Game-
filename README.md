# 🎮 Advanced Xonix Game - Data Structures Project

This is a fully featured and modern reimplementation of the classic **Xonix** game. Built using C++ and fundamental data structures like heaps, queues, hash tables, and linked lists, it simulates a complete game environment with multiplayer support, login/authentication, leaderboard management, matchmaking, social features, and game persistence.

---

## 📁 Project Structure

📦XonixGame
┣ 📄main.cpp # Main driver with the full game logic
┣ 📄background.png # Background image
┣ 📄enemy.png # Enemy tile image
┣ 📄gameover.png # Game Over screen
┣ 📄players.txt # Stores registered players
┣ 📄scores.txt # Stores player scores
┣ 📄leaderboard.txt # Leaderboard top 10 data
┣ 📄savedgames/ # Folder containing saved games
┣ 📄friendships.txt # Stores friend relationships
┣ 📄matchqueue.txt # Queue file for match-making
┣ 📄README.md # Project documentation


---

## 🧩 Features Implemented

### 🎮 Game Modes

- **Single Player Mode**
- **Multiplayer Mode**
  - Player 1: Arrow keys
  - Player 2: W, A, S, D
  - Collision rules apply
  - Shared board and timer
  - Independent scores and power-ups
  - Winner declared on game over

---

### 🧾 Menus and UI

- **Main Menu**
  - Login / Register
  - Start Game
  - Leaderboard
  - Profile
  - Exit

- **Start Menu**
  - Start New Game
  - Select Level
  - Resume Saved Game
  - Multiplayer Queue

- **End Menu**
  - Final score shown
  - Highlights new high scores
  - Options to Restart, Main Menu, or Exit

---

### 🔐 Login & Registration System

Implemented using **file handling** and **input validation**.

- Unique usernames
- Secure password validation
- Player ID assigned
- Stored in `players.txt`

### ✅ Login Validation

- Checks existing credentials
- Allows only valid users to play
- After login: access to profile, friends, matchmaking, etc.

---

### 🏆 Points System

- 1 point per tile
- Double points for >10 tiles in one move
- After 3 double-points: threshold reduced to 5
- After 5 bonuses: 4x multiplier on >5 tiles
- Power-ups earned at 50, 70, 100, 130 and every +30
- Power-up = stop all enemies (and other player in multiplayer) for 3 seconds

---

### 🧑‍💼 Player Profile

Each player has:
- Username
- Friends List
- Match History (Win/Loss)
- Total Score
- Viewable after login

---

### 👫 Friend System

**Data Structures Used:**
- Array List of Players
- Hash Table for Player ID lookup
- Linked List for Friends

**Functionalities:**
- Send/Accept/Reject friend requests
- View friend list
- Fast player search using hash table

---

### 🏁 Leaderboard (Top 10 Players)

**Implemented using Min-Heap**:
- Always holds top 10 scores
- Sorted for display (descending)
- Auto-updates after each match
- Format: Player ID | Score

---

### 🧠 Matchmaking System

**Implemented using Priority Queue:**
- Players added to queue based on score
- Highest priority = highest score
- Top 2 players matched for multiplayer game
- Score updated after game
- Queue continues processing automatically

---

### 🕹️ Game Room

- Queue-based game pairing
- Uses leaderboard score to determine priority
- Multiplayer sessions launched from queue

---

### 💾 Save and Resume Game

- Game state stored in `savedgames/` folder
- Includes:
  - Player ID
  - Timestamp
  - Game Board State (linked list of tiles)
- Resume by entering unique Save ID
- Full state restored

---

## 🧱 Data Structures Used

| Feature              | Data Structure     |
|---------------------|--------------------|
| Leaderboard         | Min-Heap           |
| Friend System       | Array List + Hash Table + Linked List |
| Matchmaking Queue   | Priority Queue     |
| Game Room Queue     | Queue              |
| Player Profiles     | File + Linked List |
| Saved Game State    | File + Linked List |
| Authentication      | File + Hashing     |

---

## 💡 Bonus Features

- Background and custom images
- Game Over screen
- Full-screen toggle
- Sound effects (if enabled)
- Clean UI experience

---

## 🛠️ How to Run

1. Clone the repo.
2. Also you should have SFML pre-installed.
3. Make sure all image files and text files are in the same directory as `main.cpp`.
4. Compile and run `main.cpp` using your preferred C++ IDE (e.g., Visual Studio).
5. Enjoy the game!

---

## 📌 Future Improvements (Ideas)

- Online multiplayer support
- GUI version using SFML/SDL
- Achievements and XP system
- Custom maps and level editor

---

## 👨‍💻 Contributors

- Usman Bari
- Team Members (Multiplayer, Leaderboard, Matching Modules)

---

> 🎯 *This project demonstrates mastery of file I/O, dynamic memory management, advanced data structures, and object-oriented programming in C++.*

