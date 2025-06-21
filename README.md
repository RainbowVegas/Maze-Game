# Maze Zombie Slayer

A thrilling maze game developed in **C and C++**, where your goal is to kill all zombies and escape the maze before time runs out!

## Table of Contents

- [About](#about)
- [Features](#features)
- [How to Play](#how-to-play)
- [Installation](#installation)
- [Usage](#usage)
- [Controls](#controls)
- [Contributing](#contributing)
- [License](#license)

## About

Maze Zombie Slayer is a console-based maze game combining strategic movement and action. Navigate the maze, eliminate all zombies, and reach the exit before the timer expires.

## Features

- Maze navigation with user input
- Randomly placed zombies to hunt
- Time-limited gameplay adding urgency
- Clear win and lose conditions
- Simple console graphics

## How to Play

1. Compile the project (instructions below).  
2. Run the executable.  
3. Use the controls to move your character through the maze.  
4. Kill all zombies by moving onto their positions.  
5. Reach the exit before time runs out to win.

## Installation

Make sure you have a C/C++ compiler installed (e.g., `gcc` or `g++`).

Clone this repository:

```bash
git clone https://github.com/yourusername/maze-zombie-slayer.git
cd maze-zombie-slayer
```

Compile the game:

```bash
gcc -o maze_game main.c maze.c zombies.c -lstdc++  # Adjust files as necessary
```

Or, if you use a Makefile, just run:

```bash
make
```

## Usage

Run the compiled executable:

```bash
./maze_game
```

Follow the on-screen instructions to play.

## Controls

- **W** - Move Up  
- **A** - Move Left  
- **S** - Move Down  
- **D** - Move Right  

## Contributing

Contributions, issues, and feature requests are welcome! Feel free to open an issue or pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
