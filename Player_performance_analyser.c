#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Players_data.h"

#define MAX_PLAYERS 10000
#define MAX_TEAMS 1000
#define ROLE_COUNT 3

/* Independent Player structure: does not reuse the header's Player structure. */
typedef struct
{
    int player_id;
    char name[50];
    char team_name[30];
    char role[20];
    int total_runs;
    float batting_avg;
    float strike_rate;
    int wickets;
    float economy_rate;
    float performance_index;
} CricketPlayer;

typedef struct
{
    int team_id;
    char name[30];
    int total_players;
    float avg_batting_strike_rate;
} Team;

/* Each linked-list node refers to one dynamically allocated Player. */
typedef struct Node
{
    CricketPlayer *player;
    struct Node *next;
} Node;

CricketPlayer *allPlayers[MAX_PLAYERS];
int totalCount = 0;
Team allTeams[MAX_TEAMS];
int numberOfTeams = 0;

/* One performance-index-sorted list for every team and role. */
Node *roleLists[MAX_TEAMS][ROLE_COUNT] = {{NULL}};

const char *roleNames[ROLE_COUNT] = {
    "Batsman", "Bowler", "All-rounder"
};

int roleIndex(const char *role)
{
    for (int i = 0; i < ROLE_COUNT; i++)
        if (strcmp(role, roleNames[i]) == 0)
            return i;
    return -1;
}

int readRole(void)
{
    int choice;
    while (1)
    {
        printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
        if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 3)
            return choice - 1;

        printf("Invalid role. Enter 1, 2 or 3.\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
}

void calculatePerformanceIndex(CricketPlayer *p)
{
    if (strcmp(p->role, "Batsman") == 0)
        p->performance_index = (p->batting_avg * p->strike_rate) / 100.0f;
    else if (strcmp(p->role, "Bowler") == 0)
        p->performance_index = (p->wickets * 2.0f) + (100.0f - p->economy_rate);
    else
        p->performance_index =
            ((p->batting_avg * p->strike_rate) / 100.0f) + (p->wickets * 2.0f);
}


void insertSorted(Node **head, CricketPlayer *player)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->player = player;
    newNode->next = NULL;

    if (*head == NULL ||
        (*head)->player->performance_index < player->performance_index)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    Node *current = *head;
    while (current->next != NULL &&
           current->next->player->performance_index >= player->performance_index)
        current = current->next;

    newNode->next = current->next;
    current->next = newNode;
}

void addPlayerToRoleList(CricketPlayer *p)
{
    int t = -1;
    for (int i = 0; i < numberOfTeams; i++)
        if (strcmp(allTeams[i].name, p->team_name) == 0)
        {
            t = i;
            break;
        }

    int r = roleIndex(p->role);
    if (t >= 0 && r >= 0)
        insertSorted(&roleLists[t][r], p);
}

void initPlayers(void)
{
    for (int i = 0; i < playerCount; i++)
    {
        CricketPlayer *p = (CricketPlayer *)malloc(sizeof(CricketPlayer));
        if (p == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        p->player_id = players[i].id;
        strcpy(p->name, players[i].name);
        strcpy(p->team_name, players[i].team);
        strcpy(p->role, players[i].role);
        p->total_runs = players[i].totalRuns;
        p->batting_avg = players[i].battingAverage;
        p->strike_rate = players[i].strikeRate;
        p->wickets = players[i].wickets;
        p->economy_rate = players[i].economyRate;
        calculatePerformanceIndex(p);

        allPlayers[totalCount++] = p;
    }
}

void initTeams(void)
{
    numberOfTeams = teamCount;

    for (int i = 0; i < numberOfTeams; i++)
    {
        allTeams[i].team_id = i + 1;
        strcpy(allTeams[i].name, teams[i]);
        allTeams[i].total_players = 0;
        allTeams[i].avg_batting_strike_rate = 0.0f;
    }
}


int searchTeam(int teamId)
{
    int low = 0, high = numberOfTeams - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (allTeams[mid].team_id == teamId)
            return mid;
        if (allTeams[mid].team_id < teamId)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

void updateTeamStatistics(void)
{
    for (int i = 0; i < numberOfTeams; i++)
    {
        int count = 0;
        float sum = 0.0f;
        allTeams[i].total_players = 0;

        for (int j = 0; j < totalCount; j++)
        {
            if (strcmp(allPlayers[j]->team_name, allTeams[i].name) == 0)
            {
                allTeams[i].total_players++;

                if (strcmp(allPlayers[j]->role, "Batsman") == 0 ||
                    strcmp(allPlayers[j]->role, "All-rounder") == 0)
                {
                    sum += allPlayers[j]->strike_rate;
                    count++;
                }
            }
        }

        allTeams[i].avg_batting_strike_rate =
            (count > 0) ? sum / count : 0.0f;
    }
}

void buildRoleLists(void)
{
    for (int i = 0; i < numberOfTeams; i++)
        for (int r = 0; r < ROLE_COUNT; r++)
            roleLists[i][r] = NULL;

    for (int i = 0; i < totalCount; i++)
        addPlayerToRoleList(allPlayers[i]);
}

bool validatePlayerId(int id)
{
    if (id < 1 || id > 1000)
        return false;

    for (int i = 0; i < totalCount; i++)
        if (allPlayers[i]->player_id == id)
            return false;

    return true;
}

void printPlayerHeader(void)
{
    printf("====================================================================================================\n");
    printf("%-6s %-24s %-13s %-7s %-7s %-7s %-7s %-7s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("====================================================================================================\n");
}

void printPlayer(CricketPlayer *p)
{
    printf("%-6d %-24s %-13s %-7d %-7.1f %-7.1f %-7d %-7.1f %-10.2f\n",
           p->player_id, p->name, p->role, p->total_runs,
           p->batting_avg, p->strike_rate, p->wickets,
           p->economy_rate, p->performance_index);
}

void displayPlayers(int teamIndex)
{
    printf("\nPlayers of Team %s:\n", allTeams[teamIndex].name);
    printPlayerHeader();

    for (int i = 0; i < totalCount; i++)
        if (strcmp(allPlayers[i]->team_name, allTeams[teamIndex].name) == 0)
            printPlayer(allPlayers[i]);

    printf("====================================================================================================\n");
    printf("Total Players: %d\n", allTeams[teamIndex].total_players);
    printf("Average Batting Strike Rate: %.2f\n",
           allTeams[teamIndex].avg_batting_strike_rate);
}


void addPlayer(void)
{
    if (totalCount >= MAX_PLAYERS)
    {
        printf("Player storage is full.\n");
        return;
    }

    int teamId, id, runs, wickets;
    float avg, sr, economy;
    char name[51];

    printf("Enter Team ID to add player: ");
    if (scanf("%d", &teamId) != 1)
        return;

    int teamIndex = searchTeam(teamId);
    if (teamIndex == -1)
    {
        printf("Invalid Team ID.\n");
        return;
    }

    printf("Player ID: ");
    if (scanf("%d", &id) != 1 || !validatePlayerId(id))
    {
        printf("Invalid or duplicate Player ID.\n");
        return;
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
    printf("Name: ");
    if (fgets(name, sizeof(name), stdin) == NULL)
        return;
    name[strcspn(name, "\n")] = '\0';

    if (strlen(name) == 0)
    {
        printf("Name cannot be empty.\n");
        return;
    }

    int r = readRole();

    printf("Total Runs: ");
    if (scanf("%d", &runs) != 1 || runs < 0)
        return;
    printf("Batting Average: ");
    if (scanf("%f", &avg) != 1 || avg < 0)
        return;
    printf("Strike Rate: ");
    if (scanf("%f", &sr) != 1 || sr < 0)
        return;
    printf("Wickets: ");
    if (scanf("%d", &wickets) != 1 || wickets < 0)
        return;
    printf("Economy Rate: ");
    if (scanf("%f", &economy) != 1 || economy < 0)
        return;

    CricketPlayer *p = (CricketPlayer *)malloc(sizeof(CricketPlayer));
    if (p == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    p->player_id = id;
    strcpy(p->name, name);
    strcpy(p->team_name, allTeams[teamIndex].name);
    strcpy(p->role, roleNames[r]);
    p->total_runs = runs;
    p->batting_avg = avg;
    p->strike_rate = sr;
    p->wickets = wickets;
    p->economy_rate = economy;
    calculatePerformanceIndex(p);

    allPlayers[totalCount++] = p;
    insertSorted(&roleLists[teamIndex][r], p);

    updateTeamStatistics();
    printf("Player added successfully to Team %s!\n", allTeams[teamIndex].name);
}



void displayTopK(void)
{
    int teamId, k;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    int teamIndex = searchTeam(teamId);
    if (teamIndex == -1)
    {
        printf("Invalid Team ID.\n");
        return;
    }

    int r = readRole();
    printf("Enter number of players (K): ");
    if (scanf("%d", &k) != 1 || k <= 0)
    {
        printf("K must be positive.\n");
        return;
    }

    Node *current = roleLists[teamIndex][r];
    if (current == NULL)
    {
        printf("No players found for this role in %s.\n",
               allTeams[teamIndex].name);
        return;
    }

    printf("\nTop %d %s of Team %s:\n",
           k, roleNames[r], allTeams[teamIndex].name);
    printPlayerHeader();

    int count = 0;
    while (current != NULL && count < k)
    {
        printPlayer(current->player);
        current = current->next;
        count++;
    }

    if (count < k)
        printf("Only %d players are available for this role.\n", count);
    printf("====================================================================================================\n");
}

void displayAllPlayersOfRole(void)
{
    int r = readRole();
    Node *current[MAX_TEAMS];

    for (int i = 0; i < numberOfTeams; i++)
        current[i] = roleLists[i][r];

    printf("\nAll %s Players Across All Teams (by Performance Index):\n",
           roleNames[r]);

    printf("============================================================================================================\n");
    printf("%-6s %-22s %-18s %-13s %-7s %-7s %-7s %-7s %-7s %-10s\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR",
           "Wkts", "ER", "Perf.Index");
    printf("============================================================================================================\n");

    while (1)
    {
        int maxIndex = -1;

        for (int i = 0; i < numberOfTeams; i++)
        {
            if (current[i] != NULL)
            {
                if (maxIndex == -1 ||
                    current[i]->player->performance_index >
                    current[maxIndex]->player->performance_index)
                {
                    maxIndex = i;
                }
            }
        }

        if (maxIndex == -1)
            break;

        CricketPlayer *p = current[maxIndex]->player;

        printf("%-6d %-22s %-18s %-13s %-7d %-7.1f %-7.1f %-7d %-7.1f %-10.2f\n",
               p->player_id, p->name, p->team_name, p->role,
               p->total_runs, p->batting_avg, p->strike_rate,
               p->wickets, p->economy_rate, p->performance_index);

        current[maxIndex] = current[maxIndex]->next;
    }

    printf("============================================================================================================\n");
}


int main(void)
{
    int choice;

    printf("============================================================\n");
    printf("          ICC ODI Player Performance Analyzer\n");
    printf("============================================================\n");

    if (teamCount > MAX_TEAMS || playerCount > MAX_PLAYERS)
    {
        printf("Initial dataset exceeds configured storage limits.\n");
        return 1;
    }

    initTeams();
    initPlayers();
    buildRoleLists();
    updateTeamStatistics();

    while (1)
    {
        printf("\n1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of a Specific Role Across All Teams\n");
        printf("6. Exit\n");
        printf("============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice)
        {
            case 1:
                addPlayer();
                break;

            case 2:
            {
                int teamId;
                printf("Enter Team ID: ");
                scanf("%d", &teamId);

                int teamIndex = searchTeam(teamId);
                if (teamIndex == -1)
                    printf("Invalid Team ID.\n");
                else
                    displayPlayers(teamIndex);
                break;
            }

            case 3:
                displayTeamsByAverage();
                break;

            case 4:
                displayTopK();
                break;

            case 5:
                displayAllPlayersOfRole();
                break;

            case 6:
                freeMemory();
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Please enter 1 to 6.\n");
        }
    }

    freeMemory();
    return 0;
}
