#include <stdio.h>
#include <string.h>
#include "Players_data.h"
#define MAX_PLAYERS 10000

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
} Player_PI;

typedef struct
{
    int team_id;
    char name[30];
    int total_players;
    float avg_batting_strike_rate;
} Team;

Player_PI allPlayers[MAX_PLAYERS];
int totalCount = 0;
Team allTeams[1000];

float calculate_avg(char *name);
int calculate_total_players(char *name);

void initPlayers()
{
    for (int i = 0; i < playerCount; i++)
    {
        allPlayers[i].player_id = players[i].id;
        strcpy(allPlayers[i].name, players[i].name);
        strcpy(allPlayers[i].team_name, players[i].team);
        strcpy(allPlayers[i].role, players[i].role);
        allPlayers[i].total_runs = players[i].totalRuns;
        allPlayers[i].batting_avg = players[i].battingAverage;
        allPlayers[i].strike_rate = players[i].strikeRate;
        allPlayers[i].wickets = players[i].wickets;
        allPlayers[i].economy_rate = players[i].economyRate;
        allPlayers[i].performance_index = 0.0f;
    }
    totalCount = playerCount;
}

void initTeam()
{
    for (int i = 0; i < teamCount; i++)
    {
        allTeams[i].team_id = i + 1;
        strcpy(allTeams[i].name, teams[i]);
        allTeams[i].total_players = calculate_total_players(allTeams[i].name);
        allTeams[i].avg_batting_strike_rate = calculate_avg(allTeams[i].name);
    }
}

int calculate_total_players(char *name)
{
    int count = 0;
    for (int i = 0; i < totalCount; i++)
    {
        if (strcmp(name, allPlayers[i].team_name) == 0)
        {
            count++;
        }
    }
    return count;
}

float calculate_avg(char *name)
{
    float avg = 0;
    int count = 0;
    for (int i = 0; i < totalCount; i++)
    {
        if (strcmp(name, allPlayers[i].team_name) == 0 && strcmp(allPlayers[i].role, "Batsman") == 0)
        {
            count++;
            avg += allPlayers[i].strike_rate;
        }
    }
    return (count > 0) ? (avg / count) : 0.0f;
}

/*Batsman: (BattingAverage × StrikeRate) / 100
Bowlers: (Wickets × 2) + (100 − EconomyRate)
All-Rounders: [(BattingAverage × StrikeRate) / 100] + (Wickets × 2)*/

void calculate_performance_index()
{
    for (int i = 0; i < totalCount; i++)
    {
        if (strcmp(allPlayers[i].role, "All-rounder") == 0)
        {
            allPlayers[i].performance_index = ((allPlayers[i].batting_avg * allPlayers[i].strike_rate) / 100) + (allPlayers[i].wickets * 2);
        }
        else if (strcmp(allPlayers[i].role, "Bowler") == 0)
        {
            allPlayers[i].performance_index = (allPlayers[i].wickets * 2) + (100 - allPlayers[i].economy_rate);
        }
        else if (strcmp(allPlayers[i].role, "Batsman") == 0)
        {
            allPlayers[i].performance_index = (allPlayers[i].batting_avg * allPlayers[i].strike_rate) / 100;
        }
    }
}
int select_team(int *team_no)
{
    printf("enter the team number that u want to select:");
    scanf("%d", team_no);
    if (*team_no > 0 && *team_no <= teamCount)
    {
        return *team_no;
    }
    printf("team with no. %d not available\n", *team_no);
    return -1;
}
// void add_player();
void display_players(int team_no)
{
    printf("Players of Team %s:\n", teams[team_no]);
    printf("=========================================================================================================\n");
    printf("  %-4s    %-24s   %-12s   %-5s   %-4s   %-4s   %-5s   %-4s   %-4s \n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "PI");
    printf("=========================================================================================================\n");
    for (int i = 0; i < playerCount; i++)
    {
        if (strcmp(allPlayers[i].team_name, teams[team_no]) == 0)
        {
            printf("  %-4d    %-21s     %-11s   %5d   %5.1f   %5.1f   %4d   %4.1f  %4.1f   \n",
                   allPlayers[i].player_id,
                   allPlayers[i].name,
                   allPlayers[i].role,
                   allPlayers[i].total_runs,
                   allPlayers[i].batting_avg,
                   allPlayers[i].strike_rate,
                   allPlayers[i].wickets,
                   allPlayers[i].economy_rate,
                   allPlayers[i].performance_index);
        }
    }
    printf("=============================================================================================================\n");
    printf("Total Players: %d\n", allTeams[team_no].total_players);
    printf("Average Batting Strike Rate: %.2f\n", allTeams[team_no].avg_batting_strike_rate);
    printf("\n");
}
// void team_avg_strike_rate();
// void top_k_players_by_role();
// void all_players_specific_role_access();

int main()
{

    int choice;
    printf("========================================================\n");
    printf("ICC ODI Player Performance Analyzer\n");
    printf("============================================================\n");
    initPlayers();
    initTeam();
    calculate_performance_index();
    /*printf("team_count=%d",teamCount);
    printf("player_count=%d",playerCount);
    return 0;*/
    while (1)
    {
        printf("1.Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");

        printf("==============================================================\n");

        printf("enter ur choice\n");
        scanf("%d", &choice);
        switch (choice)
        {
        /*case 1:
            add_player();
            break;*/
        case 2:
        {
            int team;
            team = select_team(&team);
            display_players(team);
            break;
        }
            /*case 3:
                team_avg_strike_rate();
                break;
            case 4:
                top_k_players_by_role();
                break;
            case 5:
                all_players_specific_role_access();
                break;*/
        }
    }
}
