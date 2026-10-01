# BUP Event Calendar

A console-based event calendar developed in **C** during my **2nd semester** at **Bangladesh University of Professionals (BUP)**.

## Features

* Monthly calendar display
* Automatic detection of leap years
* Day-of-week calculation
* Keyboard-based calendar navigation
* Add events to specific dates
* View events for a selected date
* Delete events
* Persistent event storage using file handling
* Automatic highlighting of dates containing events
* BUP online class day indicator for the **2nd and 4th Tuesday** of each month
* Automatically starts at the current date

## Technologies Used

* C
* File Handling
* Structures
* Arrays
* Functions
* Date and Calendar Algorithms
* Windows Console Input

## Controls

| Key        | Function        |
| ---------- | --------------- |
| Arrow Keys | Move the cursor |
| Enter      | View events     |
| A          | Add an event    |
| D          | Delete an event |
| Q          | Quit            |

## How It Works

The program dynamically generates the calendar based on the selected month and year. It calculates leap years and the number of days in each month and uses **Zeller's Congruence** to determine the day of the week.

Events are stored in `events.txt`, allowing them to remain available when the program is opened again.

The program also includes a BUP-specific feature that identifies the **2nd and 4th Tuesday** of each month as online class days.

## Project Structure

```text
BUP-Event-Calender/
│
├── calender.c
├── events.txt
├── .gitignore
└── README.md
```

## Academic Project

**Course:** C Programming
**Semester:** 2nd Semester
**University:** Bangladesh University of Professionals (BUP)
**Department:** Computer Science and Engineering

## Author

**K. M. Sadman Sakib**

Computer Science and Engineering
Bangladesh University of Professionals (BUP)
