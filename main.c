
#include <stdio.h>
#include <string.h>

#define MAX_GAMES 20
#define TOTAL_MEMORY 1024

typedef struct {
    int id;
    char name[50];
    int memory;
    int priority;
    int burst;
    int active;
} Game;

Game games[MAX_GAMES];
int count = 0;
int nextId = 1;
int usedMemory = 0;

void addGame(void) {
    Game g;

    if (count >= MAX_GAMES) {
        printf("Game list is full!\n");
        return;
    }

    printf("Enter game/process name: ");
    scanf(" %49[^\n]", g.name);

    printf("Enter memory required (MB): ");
    if (scanf("%d", &g.memory) != 1) {
        printf("Invalid input!\n");
        return;
    }

    printf("Enter priority (1 = highest): ");
    if (scanf("%d", &g.priority) != 1) {
        printf("Invalid input!\n");
        return;
    }

    printf("Enter CPU burst time (units): ");
    if (scanf("%d", &g.burst) != 1) {
        printf("Invalid input!\n");
        return;
    }

    if (g.memory <= 0 || g.priority <= 0 || g.burst <= 0) {
        printf("Values must be positive!\n");
        return;
    }

    if (g.memory > TOTAL_MEMORY - usedMemory) {
        printf("Not enough simulated memory!\n");
        return;
    }

    g.id = nextId++;
    g.active = 1;
    games[count++] = g;
    usedMemory += g.memory;

    printf("Game added successfully! ID: %d\n", g.id);
}

void showGames(void) {
    int i;

    if (count == 0) {
        printf("No games registered.\n");
        return;
    }

    printf("\n--- GAMING OS PROCESS LIST ---\n");
    printf("%-5s %-20s %-10s %-10s %-10s\n",
           "ID", "Name", "Memory", "Priority", "Burst");

    for (i = 0; i < count; i++) {
        printf("%-5d %-20s %-10d %-10d %-10d\n",
               games[i].id, games[i].name,
               games[i].memory, games[i].priority,
               games[i].burst);
    }
}

void runScheduler(void) {
    Game temp[MAX_GAMES];
    int i, j, total = 0;
    double waiting = 0, turnaround = 0;

    if (count == 0) {
        printf("Add a game before running the scheduler.\n");
        return;
    }

    memcpy(temp, games, count * sizeof(Game));

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (temp[j].priority < temp[i].priority) {
                Game swap = temp[i];
                temp[i] = temp[j];
                temp[j] = swap;
            }
        }
    }

    printf("\n--- PRIORITY CPU SCHEDULER ---\n");
    printf("Execution order:\n");

    for (i = 0; i < count; i++) {
        printf("%s", temp[i].name);
        if (i < count - 1)
            printf(" -> ");

        waiting += total;
        total += temp[i].burst;
        turnaround += total;
    }

    printf("\nTotal CPU burst: %d units\n", total);
    printf("Average waiting time: %.2f units\n",
           waiting / count);
    printf("Average turnaround time: %.2f units\n",
           turnaround / count);
}

void memoryReport(void) {
    printf("\n--- MEMORY MANAGER ---\n");
    printf("Total simulated memory : %d MB\n", TOTAL_MEMORY);
    printf("Allocated memory       : %d MB\n", usedMemory);
    printf("Free memory            : %d MB\n",
           TOTAL_MEMORY - usedMemory);
    printf("Memory utilization     : %.2f%%\n",
           usedMemory * 100.0 / TOTAL_MEMORY);
}

void performanceReport(void) {
    int i, totalBurst = 0;

    printf("\n--- GAMING OS MONITOR ---\n");
    printf("Registered processes: %d\n", count);

    for (i = 0; i < count; i++)
        totalBurst += games[i].burst;

    printf("Total CPU burst units: %d\n", totalBurst);
    printf("Simulated memory used: %d MB\n", usedMemory);
    printf("Available memory: %d MB\n",
           TOTAL_MEMORY - usedMemory);
    printf("CPU scheduling: Priority-based\n");
}

int main(void) {
    int choice;

    printf("=================================\n");
    printf("       GAMING OS SIMULATOR       \n");
    printf("=================================\n");

    do {
        printf("\n1. Add Game Process\n");
        printf("2. View Game Processes\n");
        printf("3. Run Priority Scheduler\n");
        printf("4. View Memory Usage\n");
        printf("5. Performance Report\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Restart the program.\n");
            return 1;
        }

        switch (choice) {
            case 1: addGame(); break;
            case 2: showGames(); break;
            case 3: runScheduler(); break;
            case 4: memoryReport(); break;
            case 5: performanceReport(); break;
            case 6: printf("Gaming OS Simulator closed.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}
