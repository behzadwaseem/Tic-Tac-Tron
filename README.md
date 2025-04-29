# 🤖 Tic-Tac-Tron

**Tic-Tac-Tron** is a LEGO EV3 robot programmed in [RobotC](https://www.robotc.net/), designed to play an interactive game of Tic-Tac-Toe against a human opponent. It uses a color sensor to scan the board for moves and a stamping mechanism to physically mark its move on the game grid. Think you're smart? This bot's here to outplay you—mechanically.

---

## 🎮 How It Works

1. **Board Scanning:**  
   The robot uses an EV3 color sensor to detect human moves on a 3x3 Tic-Tac-Toe grid. It scans each cell for black markings indicating a player's move.

2. **Cheat Detection:**  
   Tic-Tac-Tron keeps track of the board state. If it detects a player has overwritten one of its previous moves or added extra marks between turns, it halts the game and declares **"Cheating Detected!"**

3. **Move Selection:**  
   The robot selects a legal move randomly from the remaining free cells (future versions could implement a smarter strategy like Minimax).

4. **Move Execution:**  
   Once a move is selected, Tic-Tac-Tron:
   - Positions its stamping arm using 3 motors (forward/back, vertical, and lateral).
   - Physically stamps the chosen cell.
   - Updates the internal game state.

5. **Game Progression:**  
   After each turn, the robot evaluates whether someone has won or if the game ends in a draw.

---

## 🛠️ Hardware Setup

- **Motors:**
  - `motorA`: Drives the robot forward/backward across rows.
  - `motorB`: Controls vertical movement of the stamping arm.
  - `motorD`: Slides the arm sideways between columns.

- **Sensors:**
  - `S4`: EV3 Color Sensor (used to scan for player marks).
  - `S1`: EV3 Touch Sensor (for emergency stop functionality).

- **Stamping Mechanism:**  
  Attach a small object to the arm that can leave a physical mark (e.g., chalk, marker, or mechanical stamp).

- **Tic-Tac-Toe Grid:**  
  A 3x3 board printed or drawn on a flat surface with clear cell boundaries and space for the robot to move.

---

## 🧠 Software Overview

- Written in **RobotC** (C++-like syntax).
- Key Functions:
  - `scanBoard()`: Detects human moves and updates the game state.
  - `makeMove(x, y)`: Moves to a chosen cell and performs the stamping action.
  - `findEmpty()`: Identifies all available cells.
  - `findRandom()`: Randomly selects one available move.
  - `evaluateBoard()`: Checks for a win, loss, or draw.
  - `stopAllTasks(message)`: Emergency kill switch on cheating or system fault.

---

## 🚀 Getting Started

1. Load the `Tic-Tac-Tron` source code onto your EV3 brick via the RobotC IDE.
2. Assemble the robot with mobility across a 3x3 board and an articulated stamping arm.
3. Power on and run the program.
4. Choose who plays first (robot or human) via the EV3 buttons.
5. Play and try to beat the bot!

---

## ⚠️ Known Limitations

- Only detects **black marks** for human moves.
- Uses **random** strategy (can be upgraded with AI like Minimax).
- Relies on **precise board alignment** for accurate scanning and stamping.
- Board must be **free of obstructions** to prevent mechanical missteps.

---

## 📦 Future Improvements

- Implement a smarter move strategy (Minimax or heuristic-based AI).
- Add support for re-calibration or vision-based recognition for more flexible play.
- Introduce a GUI interface on the EV3 display for better UX.
- Record game logs for post-game analysis.

---

## 📸 Pics

![Tic Tac Tron Photo](https://github.com/user-attachments/assets/86e4175e-8c26-473e-84bf-95300acbd6e6)
![Tic Tac Tron Photo 2](https://github.com/user-attachments/assets/50c49e09-6e9b-42e5-a3b5-90846127bf36)
(^btw the chopstick was not in the final version... we just used it while our actual part was 3D printing 😅)

[🤳 Us fooling around with the prototype](https://youtube.com/shorts/Xm68uUlmOJA?feature=share)

---

## 👨‍💻 Authors

Built and programmed by **Behzad Waseem, Ben Tran, Shilo Jeyarajasingam, & Parsa Ahmadi** using LEGO EV3 and RobotC for the MTE100 course at UWaterloo.

🚨 Btw if you're taking MTE100, do NOT use this code for your project; you will be caught for plagarism!

---

## 📄 License

MIT License. Feel free to remix, modify, and improve Tic-Tac-Tron. Just don't let it beat you too badly 😉.
