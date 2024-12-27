## Project Overview
This project involves designing a simple calculator using the TM4C123GH6PM microcontroller. The calculator takes input from a 4x4 keypad and displays the results on a 20x4 Liquid Crystal Display (LCD). The primary objective is to implement a functional embedded system capable of basic calculations and potentially some extended features for enhanced functionality.

## Features

### Keypad Interface
- Input from a 4x4 matrix keypad.
- Handles key presses for digits, basic arithmetic operators (`+`, `-`, `*`, `/`), and other commands (e.g., clear, equals).
- Optionally supports a shift key for additional functions such as exponentiation or clearing input entirely.

### LCD Interface
- Displays results and intermediate operations.
- Operates in 4-bit mode to save GPIO pins.
- Provides feedback for user input and results of calculations.
- Supports cursor movement and screen clearing for usability.

### Calculator Functionality
- Basic arithmetic operations.
- Support for floating-point calculations and operator precedence.
- Handles nested calculations.

### Extended Features (Planned)
- User authentication with a configurable password.
- Graphics display on the LCD.
- Persistent data storage using the microcontroller's flash memory (e.g., for saving user preferences or results).
- Trigonometric functions
  
