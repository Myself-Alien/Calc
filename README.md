<h3>GTK-4 Calculator (C)</h3>
A simple desktop calculator application built using GTK-4 and C, designed to demonstrate modern GTK-4 UI concepts such as grid layouts, signal handling, CSS styling, and application lifecycle management.
This project was created as a learning-oriented implementation, focusing on correct GTK-4 practices rather than legacy GTK-3 patterns.

Features
GTK-4 based graphical user interface

Grid-based calculator layout

Basic arithmetic operations: +, -, *, /

Expression-style input (e.g. 2 + 2 = 4)

Custom CSS styling using GtkCssProvider

Clean separation between UI logic and styling

Minimal, dependency-free C code (no external frameworks)

Technologies Used

C (C17 compatible)

GTK-4

GObject / GLib

CSS (GTK-4 styling system)

pkg-config

Make

Project Structure
Calc/
├── src/
│   └── main.c        # Application logic and UI
├── style.css         # GTK-4 CSS styling
├── Makefile          # Build instructions
├── compile_commands.json
└── README.md

License

MIT License — free to use, modify, and learn from.


