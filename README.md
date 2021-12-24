# Bomberman-Arduino

### Introduction

This project was developed by applying the knowledge obtained during the [Introduction To Robotics Course](https://github.com/cul1n/IntroductionToRobotics) @ FMI UNIBUC. It's a game inspired by the classic 1983 NES game, [Bomberman](https://en.wikipedia.org/wiki/Bomberman_(1983_video_game)). Taking into account the technical limitations of developing a game on an 8x8 LED matrix, I chose this theme because I considered the bomb mechanics and the room-structure of Bomberman as a perfect match for this project.

### How To Play

Once the game starts, a random room will be generated and the player will be spawned in the starting gate. The player is tasked with clearing the room by controlling a dot on the 8x8 matrix with the joystick and passing though the exit gate before the time runs out. Each room is filled with breakable walls and enemies that may block the way or damage you, but the player has a handy trick: placing bombs and destroying obstacles by pressing the joystick's "switch" button. Every two levels the player meets a shop keeper that sells power-ups which help them on their journey in exchange for score points. The objective of the game is to beat the 10 levels, reaching the end of the dungeon.

### Game Description And Technical Details

Turning on the game you will be met by the loading screen and the starting animation of a bomb exploding.

The menu is displayed on the LCD and contains the following options:
- `Play`: jump right into the game.
- `Settings`: all settings are saved in EEPROM (even if the game is powered off).
  - Name: insert the player's name using the joystick's X and Y axis for a maximum of 6 characters, this name will be displayed during the game and in the high score section.
  - Level: choose the starting level of the game (range: 1 to 10).
  - Contrast: adjust the LCD's contrast (range: 1 to 255).
  - LCD Brightness: adjust the LCD's brightness (range: 1 to 255).
  - Matrix Brightness: adjust the matrix's brightness (range: 1 to 15).
  - Reset High Scores.
  - Disable Sounds.
- `High Scores`: list of the three best scores and the name of the player that achieved them.
- `Credits`: name of the game, author and a scrolling link to the Github repository.

Each sub-menu has a `Back` option that returns the player to the main menu and is represented on the matrix by displaying a specific icon.

<img src= https://github.com/cul1n/Bomberman-Arduino/blob/main/GameInfo.jpeg width=50% height=50%>

When the `Play` option is chosen, the first room will be rendered on the matrix, and the game info will be displayed on the LCD: player's name, current level, time left, number of lives (initially 3), number of bombs (initially 1) and current score.

<img src= https://github.com/cul1n/Bomberman-Arduino/blob/main/RoomExemples.gif>

Levels are procedurally generated as follows:
- Each level has a number of rooms equal to the number of the level, capped at 5 rooms.
- A room has a unbreakable border around it with two gates, on the left side the spawning gate, and on a random edge of the level the exit gate.
- Breakable walls will be placed in the room, the further the player goes, more walls will be placed.
- Enemies navigate the room and damage the player by colliding with him.
- For each room in the level, the timer increases with the base time of 25 seconds.
- Every two levels a shop will be displayed on the LCD, where the player can buy power-ups.

<img src= https://github.com/cul1n/Bomberman-Arduino/blob/main/Shop.jpeg width=50% height=50%>

The score is calculated by adding 5 points per wall destroyed, 20 points per enemy slayed and one third of the leftover time when finishing the level. When the shop screen is displayed, the score will be visible and 2 of 3 power-ups will be available to be bought, as the games progresses this items become more expensive:
- Health (base price of 25): Increases the player health by one.
- Bomb (base price of 50): Increases the number of bombs a player can place in quick succession. Maximum number of bombs is 3.
- Extra spread (base price of 75): Increases the range of the bomb by one more unit. Only one available during the game.

Bomb Power-Up           |  Extra Spread Power-Up
:-------------------------:|:-------------------------:
![](https://github.com/cul1n/Bomberman-Arduino/blob/main/BombsPowerUp.gif)  |  ![](https://github.com/cul1n/Bomberman-Arduino/blob/main/SpreadPowerUp.gif)

The game ends if the player has no lives left, the timer reaches 0 or the player finishes level 10. The game over screen displays the level reached, and after a button press, another screen with stats about the game will be revealed: player's score (it announces if the player entered the high score leaderboard), total time played, number of bombs placed and damage taken. Another button press will take the player back to the main menu.

<img src= https://github.com/cul1n/Bomberman-Arduino/blob/main/GameStats.jpeg width=50% height=50%>

### Project files
- Bomberman (.ino) - Main file where the game state is checked.
- Consts (.h) - File containing constant variables used in the project.
- Controls (.ino / .h) - Class used for utility functions, like controlling the joystick.
- State (.ino / .h) - The generic game state class.
  - Intro (.ino / .h) - Renders the intro message.
  - Menu (.ino / .h) - The generic menu class, used for defining the logic of the scrolling menu.
    - MainMenu (.ino / .h) - Access all of the game features.
    - SettingsMenu (.ino / .h) - Edit user preferences.
    - HighScoreMenu (.ino / .h) - Contains the leaderboard of the top 3 scores.
    - Credits (.ino / .h) - Contains Info about the game and its creator.
  - InGame (.ino / .h) - Logic of the game.
  - GameOver (.ino / .h) - Two screens, detailing the player's stats.
- Entity (.ino / .h) - Contains all of the game objects: the player, bombs, enemies, explosions etc.
- Position (.ino / .h) - Basic class describing the position of an object on a XY axis
- List (.ino / .h) - Implementations of long and short lists, used to store entities.

### Hardware
The following components were used for building the project:
- Arduino Uno.
- LCD display, using a potentiometer to control the contrast.
- 8x8 LED Matrix.
- Joystick.
- Buzzer, using a 100 Ohm resistor.
- MAX7219 Shift register, using a 10k Ohm resistor, a 10 uF capacitor and a 104 pF ceramic capacitor.
- Wires per logic.

### Setup
<img src= https://github.com/cul1n/Bomberman-Arduino/blob/main/Setup.jpeg width=75% height=75%>

### Demo
[![DEMO](https://img.youtube.com/vi/jlRoAc5g8Qs/0.jpg)](https://www.youtube.com/watch?v=jlRoAc5g8Qs "DEMO")
