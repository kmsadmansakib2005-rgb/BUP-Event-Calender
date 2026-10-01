#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX_EVENTS 300

void saveEvents();
void loadEvents();
int isOnlineClassDay(int d, int m, int y);

struct Event {
    int d, m, y; //d,m and y represents date, month and year
    char name[100];
};

struct Event events[MAX_EVENTS];
int eventCount = 0;
int cursorDay = 1; // Highligting day by cursor


void clearScreen() {
    system("cls");
}
//determinig leap year
int isLeap(int year) {
    return ( (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) );
}

int daysInMonth(int month, int year) {
    int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && isLeap(year)) 
    return 29;
    
    return days[month - 1];
}
//m for month, d for date and y for year
// 0 = Sunday, 6 = Saturday
int dayOfWeek(int d, int m, int y) {
    if (m < 3) {
        m += 12; //by following zeller's method where March is month 3
        y--;
    }
    int K = y % 100; //year within century
    int J = y / 100; //century year
    int h = (d + 13*(m + 1)/5 + K + K/4 + J/4 + 5*J) % 7;
    //zeller's formula
    return (h + 6) % 7;
}

int eventOn(int d, int m, int y) {
    for (int i = 0; i < eventCount; i++)
        if (events[i].d == d && events[i].m == m && events[i].y == y)
            return 1;
    return 0;
}

//function for displaying event

void showEvents(int d, int m, int y) {
    clearScreen();
    printf("Events on %02d/%02d/%04d:\n\n", d, m, y);
    int found = 0; //no event found
    if (isOnlineClassDay(d, m, y)) {
    printf(" - BUP Online Class Day\n");
    found = 1; //events found
}

    for (int i = 0; i < eventCount; i++) {
        if (events[i].d == d && events[i].m == m && events[i].y == y) {
            printf(" - %s\n", events[i].name);
            found = 1;
        }
    }
    if (!found)
        printf("No events.\n");
    printf("\nPress any key...");
    _getch();
}

//adding event to the calender

void addEvent(int d, int m, int y) {
    clearScreen();
    printf("Add event for %02d/%02d/%04d\n\n", d, m, y);
    printf("Event Name: ");
    fflush(stdin); // clear input buffer
    fgets(events[eventCount].name, 100, stdin);
    events[eventCount].name[strcspn(events[eventCount].name, "\r\n")] = 0;
    events[eventCount].d = d;
    events[eventCount].m = m;
    events[eventCount].y = y;
    eventCount++;
    saveEvents();
    printf("\nEvent Added! Press any key...");
    _getch();
}

//deleting event

void deleteEvent(int d, int m, int y) {
    clearScreen();
    printf("Delete events on %02d/%02d/%04d:\n\n", d, m, y);

    int foundIndex[MAX_EVENTS];
    int foundCount = 0;

    // list events for this date
    for (int i = 0; i < eventCount; i++) {
        if (events[i].d == d && events[i].m == m && events[i].y == y) {
            printf("%d. %s\n", foundCount + 1, events[i].name);
            foundIndex[foundCount] = i;
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("\nNo events found for this date.\n");
        printf("Press any key...");
        _getch();
        return;
    }

    printf("\nEnter the event number to delete (0 to cancel): ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > foundCount) {
        printf("\nCancelled.\n");
        _getch();
        return;
    }

    int delIndex = foundIndex[choice - 1];

    // shift events to remove chosen one(chosen date)
    for (int i = delIndex; i < eventCount - 1; i++) {
        events[i] = events[i + 1];
    }
    eventCount--;
    saveEvents();

    printf("\nEvent deleted successfully!\n");
    printf("Press any key...");
    _getch();
}

//drawing the structure of the main function

void drawCalendar(int month, int year) {
    clearScreen();
    char *months[] = {
        "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };
    printf("\n\t   BUP EVENT CALENDER\n");
    printf("\t      %s %d      \n\n", months[month - 1], year);
    printf(" Su   Mo   Tu   We   Th   Fr   Sa\n");
    printf(" --------------------------------\n");

    int start = dayOfWeek(1, month, year);
    int dim = daysInMonth(month, year);
    int cell = 0;

    for (int i = 0; i < start; i++) {
        printf("    ");
        cell++;
    }

    for (int d = 1; d <= dim; d++) {
        int cursor = (d == cursorDay);
        if (cursor)
            printf("[");
        else
            printf(" ");
        if (eventOn(d, month, year) || isOnlineClassDay(d, month, year))
    printf("*%2d", d);
else
    printf(" %2d", d);

        if (cursor)
            printf("]");
        else
            printf(" ");

        cell++;
        if (cell % 7 == 0) printf("\n");
    }

    printf("\n\nControls:\n");
    printf("Arrow keys = move cursor\n");
    printf("Enter = view events\n");
    printf("A = Add Event\n");
    printf("D = Delete Event\n");
    printf("Q = Quit\n");
}

//saving events in the file

void saveEvents() {
    FILE *fp = fopen("events.txt", "w");
    if (fp == NULL) {
        printf("Error saving events!\n");
        return;
    }
    for (int i = 0; i < eventCount; i++) {
        fprintf(fp, "%d %d %d | %s\n",
            events[i].d, events[i].m, events[i].y, events[i].name);
    }
    fclose(fp);
}
//loading events

void loadEvents() {
    FILE *fp = fopen("events.txt", "r");
    if (fp == NULL){
        printf("Error in file opening.\n");
        return;
    }
    char line[200];
    while(fgets(line, sizeof(line), fp)){
        //removing the new line at the end
        line[strcspn(line, "\r\n")] =0;

        //spliting date and event name at '|'

        int d, m, y;
        char name[100];
        if(sscanf(line, "%d %d %d %[^\n]", &d, &m, &y, name)==4) {
            events[eventCount].d=d;
            events[eventCount].m=m;
            events[eventCount].y=y;
            strcpy(events[eventCount].name, name);
            eventCount++;
            if(eventCount>=MAX_EVENTS)
            {
                break;
            }
        }
    }
    fclose(fp);
}
int isOnlineClassDay(int d, int m, int y) {
    // 0 = Sunday, 1 = Monday, 2 = Tuesday, ... 
    int dow = dayOfWeek(d, m, y);
    if (dow != 2) 
    return 0; // Not Tuesday

    // Count which Tuesday it is
    int count = 0;
    for (int day = 1; day <= d; day++) {
        if (dayOfWeek(day, m, y) == 2)
            count++;
    }

    return (count == 2 || count == 4);
}


//the main function

int main() {
    int month, year;
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    month = tm->tm_mon + 1;
    year = tm->tm_year + 1900;
    cursorDay = tm->tm_mday;

    loadEvents();

    while (1) {
        int maxDay = daysInMonth(month, year);
        if (cursorDay > maxDay) cursorDay = maxDay;

        drawCalendar(month, year);

        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 75) {             // left
                cursorDay--;
                if (cursorDay < 1) {
                    month--;
                    if (month < 1) { month = 12; year--; }
                    cursorDay = daysInMonth(month, year);
                }
            } else if (ch == 77) {        // right
                cursorDay++;
                if (cursorDay > daysInMonth(month, year)) {
                    cursorDay = 1;
                    month++;
                    if (month > 12) { month = 1; year++; }
                }
            } else if (ch == 72) {        // up (-7 days)
                cursorDay -= 7;
                if (cursorDay < 1) {
                    month--;
                    if (month < 1) { month = 12; year--; }
                    cursorDay += daysInMonth(month, year);
                }
            } else if (ch == 80) {        // down (+7 days)
                cursorDay += 7;
                if (cursorDay > daysInMonth(month, year)) {
                    cursorDay -= daysInMonth(month, year);
                    month++;
                    if (month > 12) { month = 1; year++; }
                }
            }
        }
        else if (ch == 13) { // Enter
            showEvents(cursorDay, month, year);
        }
        else if (ch == 'A' || ch == 'a') {
            addEvent(cursorDay, month, year);
        }
        else if (ch == 'D' || ch == 'd') {
            deleteEvent(cursorDay, month, year);
        }
        else if (ch == 'Q' || ch == 'q') {
            saveEvents();
            break;
        }
    }

    printf("\nThank you for your time!\n");
    printf("\n\n\t\t\t\tThis project is submitted by: \n");
    printf("\t\t\t\tK. M. Sadman Sakib\n");
    printf("\t\t\t\tDepartment of Computer Science and Engineering\n");
    printf("\t\t\t\tBangladesh University of Professionls(BUP)\n");

    return 0;
}

