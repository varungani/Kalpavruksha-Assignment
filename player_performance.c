#include "Players_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int id_tracker[1000];

typedef struct TeamBattingStats
{
    int id;
    char name[50];
    float avg_sr;
    int count;
} TeamBattingStats;

typedef struct PlayerMetrics
{
    int id;
    char name[50];
    char team[50];
    char role[50];
    int runs;
    float avg;
    float sr;
    int wkt;
    float econ;
    float performance_score;
} PlayerMetrics;

void duplicate_player(Player *dest, const Player src)
{
    *dest = src;
}

void duplicate_batting_stats(TeamBattingStats *dest, const TeamBattingStats src)
{
    *dest = src;
}

void duplicate_metrics(PlayerMetrics *dest, const PlayerMetrics src)
{
    *dest = src;
}

int validate_id_bounds(const int id)
{
    if (id < 1 || id > 1500)
    {
        return 1;
    }
    return 0;
}

int find_player_index_by_id(const Player roster[], const int id, const int count)
{
    int low = 0, high = count - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (roster[mid].id == id)
        {
            return mid;
        }
        else if (roster[mid].id > id)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

void show_unused_ids()
{
    printf("\nThe available IDs are: ");
    for (int i = 1; i <= 1000; i++)
    {
        if (!id_tracker[i])
        {
            printf("%d ", i);
        }
    }
}

int acquire_unique_id(const Player roster[], const int count)
{
    int id, processed = 0;
    do
    {
        if (processed)
        {
            printf("\nPlease enter and valid unique id\n");
            show_unused_ids();
        }
        printf("\nEnter Team ID to add player: ");
        scanf("%d", &id);
        processed = 1;
    } while (find_player_index_by_id(roster, id, count) != -1 && validate_id_bounds(id));
    return id;
}

int validate_name_syntax(const char buffer[])
{
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == ' ')
        {
            continue;
        }
        if (i == 0 || buffer[i - 1] == ' ')
        {
            if (!isupper(buffer[i]))
            {
                return 1;
            }
        }
        if (!isalpha(buffer[i]))
        {
            return 1;
        }
    }
    if (strlen(buffer) > 50)
    {
        return 1;
    }
    return 0;
}

void input_player_name(char *buffer, const int size)
{
    int processed = 0;
    do
    {
        if (processed)
        {
            printf("\nName should have following things: ");
            printf("\n1. First name and Last Name should have 1st letter capital and the rest small");
            printf("\n2. Name should not contain non alphabetic letters");
        }
        getchar();
        printf("\nName: ");
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        processed = 1;
    } while (validate_name_syntax(buffer));
}

int verify_team_exists(const char *query_team)
{
    for (int i = 0; i < teamCount; i++)
    {
        if (strcmp(teams[i], query_team) == 0)
        {
            return i;
        }
    }
    return -1;
}

void input_team_name(char *buffer, const int size)
{
    int processed = 0;
    do
    {
        if (processed)
        {
            printf("\nTeam entered doesn't exists");
        }
        printf("\nEnter team Name: ");
        getchar();
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        processed = 1;
    } while (verify_team_exists(buffer) == -1);
}

void select_role(char *buffer)
{
    int option;
    while (1)
    {
        printf("\n Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
        scanf("%d", &option);
        switch (option)
        {
            case 1:
                strcpy(buffer, "Batsman");
                return;
            case 2:
                strcpy(buffer, "Bowler");
                return;
            case 3:
                strcpy(buffer, "All-rounder");
                return;
            default:
                printf("\nPlease enter valid choice");
        }
    }
}

int is_negative(const float val)
{
    if (val < 0)
    {
        return 1;
    }
    return 0;
}

int input_total_runs()
{
    int processed = 0, val;
    do
    {
        if (processed)
        {
            printf("\nPlease enter valid total runs");
        }
        printf("\n Total Runs: ");
        scanf("%d", &val);
        processed = 1;
    } while (val < 0);
    return val;
}

float input_batting_avg()
{
    int processed = 0;
    float val;
    do
    {
        if (processed)
        {
            printf("\nEnter valid average");
        }
        printf("\nBatting Average: ");
        scanf("%f", &val);
        processed = 1;
    } while (is_negative(val));
    return val;
}

float input_strike_rate()
{
    int processed = 0;
    float val;
    do
    {
        if (processed)
        {
            printf("\nPlease enter valid strike rate");
        }
        printf("\nStrike Rate: ");
        scanf("%f", &val);
        processed = 1;
    } while (is_negative(val));
    return val;
}

int input_wickets()
{
    int processed = 0, val;
    do
    {
        if (processed)
        {
            printf("\nEnter valid number of wickets");
        }
        printf("\nWickets: ");
        scanf("%d", &val);
        processed = 1;
    } while (val < 0);
    return val;
}

float input_economy_rate()
{
    int processed = 0;
    float val;
    do
    {
        if (processed)
        {
            printf("\nEnter valid economy rate");
        }
        printf("\n Economy Rate: ");
        scanf("%f", &val);
        processed = 1;
    } while (is_negative(val));
    return val;
}

void merge_id_subarrays(Player roster[], const int l, const int m, const int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    Player left[n1];
    Player right[n2];

    for (int i = 0; i < n1; i++)
    {
        duplicate_player(&left[i], roster[i + l]);
    }
    for (int i = 0; i < n2; i++)
    {
        duplicate_player(&right[i], roster[i + m + 1]);
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (left[i].id < right[j].id)
        {
            duplicate_player(&roster[k++], left[i++]);
        }
        else
        {
            duplicate_player(&roster[k++], right[j++]);
        }
    }
    while (i < n1)
    {
        duplicate_player(&roster[k++], left[i++]);
    }
    while (j < n2)
    {
        duplicate_player(&roster[k++], right[j++]);
    }
}

void execute_id_sort(Player roster[], const int l, const int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        execute_id_sort(roster, l, m);
        execute_id_sort(roster, m + 1, r);
        merge_id_subarrays(roster, l, m, r);
    }
}

void merge_teams_alpha(Player roster[], const int l, const int m, const int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    Player left[n1];
    Player right[n2];

    for (int i = 0; i < n1; i++)
    {
        duplicate_player(&left[i], roster[i + l]);
    }
    for (int i = 0; i < n2; i++)
    {
        duplicate_player(&right[i], roster[i + m + 1]);
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (strcmp(left[i].team, right[j].team) < 0)
        {
            duplicate_player(&roster[k++], left[i++]);
        }
        else
        {
            duplicate_player(&roster[k++], right[j++]);
        }
    }
    while (i < n1)
    {
        duplicate_player(&roster[k++], left[i++]);
    }
    while (j < n2)
    {
        duplicate_player(&roster[k++], right[j++]);
    }
}

void execute_team_sort(Player roster[], const int l, const int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        execute_team_sort(roster, l, m);
        execute_team_sort(roster, m + 1, r);
        merge_teams_alpha(roster, l, m, r);
    }
}

void merge_stats_by_sr(TeamBattingStats stats[], const int l, const int m, const int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    TeamBattingStats left[n1];
    TeamBattingStats right[n2];

    for (int i = 0; i < n1; i++)
    {
        duplicate_batting_stats(&left[i], stats[i + l]);
    }
    for (int i = 0; i < n2; i++)
    {
        duplicate_batting_stats(&right[i], stats[i + m + 1]);
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (left[i].avg_sr > right[j].avg_sr)
        {
            duplicate_batting_stats(&stats[k++], left[i++]);
        }
        else
        {
            duplicate_batting_stats(&stats[k++], right[j++]);
        }
    }
    while (i < n1)
    {
        duplicate_batting_stats(&stats[k++], left[i++]);
    }
    while (j < n2)
    {
        duplicate_batting_stats(&stats[k++], right[j++]);
    }
}

void execute_sr_sort(TeamBattingStats stats[], const int l, const int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        execute_sr_sort(stats, l, m);
        execute_sr_sort(stats, m + 1, r);
        merge_stats_by_sr(stats, l, m, r);
    }
}

void merge_metrics_by_role(PlayerMetrics metrics[], const int l, const int m, const int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    PlayerMetrics left[n1];
    PlayerMetrics right[n2];

    for (int i = 0; i < n1; i++)
    {
        duplicate_metrics(&left[i], metrics[i + l]);
    }
    for (int i = 0; i < n2; i++)
    {
        duplicate_metrics(&right[i], metrics[i + m + 1]);
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        int cmp = strcmp(left[i].role, right[j].role);
        if (cmp < 0 || (cmp == 0 && left[i].performance_score > right[j].performance_score))
        {
            duplicate_metrics(&metrics[k++], left[i++]);
        }
        else
        {
            duplicate_metrics(&metrics[k++], right[j++]);
        }
    }
    while (i < n1)
    {
        duplicate_metrics(&metrics[k++], left[i++]);
    }
    while (j < n2)
    {
        duplicate_metrics(&metrics[k++], right[j++]);
    }
}

void execute_metrics_sort(PlayerMetrics metrics[], const int l, const int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        execute_metrics_sort(metrics, l, m);
        execute_metrics_sort(metrics, m + 1, r);
        merge_metrics_by_role(metrics, l, m, r);
    }
}

void register_new_player(Player **roster, int *count)
{
    execute_id_sort(*roster, 0, *count - 1);
    char *name = malloc(50 * sizeof(char));
    char *team = malloc(50 * sizeof(char));
    char *role = malloc(50 * sizeof(char));

    input_player_name(name, 50);
    input_team_name(team, 50);
    select_role(role);

    Player entry = {
        acquire_unique_id(*roster, *count),
        name,
        team,
        role,
        input_total_runs(),
        input_batting_avg(),
        input_strike_rate(),
        input_wickets(),
        input_economy_rate()};

    *roster = realloc(*roster, (*count + 1) * sizeof(Player));
    duplicate_player(&((*roster)[*count]), entry);
    *count = *count + 1;

    free(name);
    free(team);
    free(role);
    id_tracker[entry.id] = 1;
}

int find_team_start_index(const Player roster[], const int n, const char team[])
{
    int start = 0, end = n - 1, res = -1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (strcmp(roster[mid].team, team) == 0)
        {
            res = mid;
            end = mid - 1;
        }
        else if (strcmp(roster[mid].team, team) < 0)
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }
    return res;
}

int find_team_end_index(const Player roster[], const int n, const char team[])
{
    int start = 0, end = n - 1, res = -1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (strcmp(roster[mid].team, team) == 0)
        {
            res = mid;
            start = mid + 1;
        }
        else if (strcmp(roster[mid].team, team) < 0)
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }
    return res;
}

void show_team_roster(Player roster[], const int n)
{
    execute_team_sort(roster, 0, n - 1);
    printf("\nEnter team name: ");
    getchar();
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    int start = find_team_start_index(roster, n, buffer);
    int end = find_team_end_index(roster, n, buffer);
    int total = 0;
    float accum_sr = 0.0;
    int i = start;

    printf("\n ====================================================================================\n");
    printf("\nID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("\n ====================================================================================\n");

    while (i <= end)
    {
        printf("\n%d\t%s\t%s\t%s\t%d\t%f\t%f\t%d\t%f", roster[i].id, roster[i].name, roster[i].team, roster[i].role, roster[i].totalRuns, roster[i].battingAverage, roster[i].economyRate, roster[i].wickets, roster[i].strikeRate);
        i++;
        total++;
        accum_sr += roster[i].strikeRate;
    }

    if (total > 0)
    {
        accum_sr /= total;
    }
    printf("\nTotal Number Of Players: %d", total);
    printf("\nAverage strike rate: %f", accum_sr);
}

void analyze_team_batting_power(Player roster[], const int n)
{
    TeamBattingStats stats[teamCount];
    execute_team_sort(roster, 0, n - 1);

    for (int i = 0; i < teamCount; i++)
    {
        char temp_team[50];
        strcpy(temp_team, teams[i]);
        int start = find_team_start_index(roster, n, temp_team);
        int end = find_team_end_index(roster, n, temp_team);
        float sum_sr = 0;
        int count = 0;

        while (start <= end)
        {
            if (strcmp(roster[start].role, "Bowler") != 0)
            {
                sum_sr += roster[start].strikeRate;
                count++;
            }
            start++;
        }

        stats[i].id = i;
        strcpy(stats[i].name, temp_team);
        stats[i].avg_sr = (count > 0) ? sum_sr / count : 0;
        stats[i].count = count;
    }

    execute_sr_sort(stats, 0, teamCount - 1);
    printf("\nTeams Sorted by Average Batting Strike Rate");
    printf("\n =========================================================\n");
    printf("\n ID\tTeam Name\tAvg Bat SR\tTotal Players");
    printf("\n =========================================================\n");

    for (int i = 0; i < teamCount; i++)
    {
        printf("\n%d\t%s\t%f\t%d", stats[i].id, stats[i].name, stats[i].avg_sr, stats[i].count);
    }
}

int resolve_role_string(const int opt, char buffer[])
{
    switch (opt)
    {
        case 1:
            strcpy(buffer, "Batsman");
            break;
        case 2:
            strcpy(buffer, "Bowler");
            break;
        case 3:
            strcpy(buffer, "All-rounder");
            break;
        default:
            printf("Invalid choice");
            return -1;
    }
    return 1;
}

void compute_performance_data(const Player roster[], PlayerMetrics metrics[], const int start, const int end)
{
    for (int i = start, k = 0; i <= end; i++, k++)
    {
        metrics[k].avg = roster[i].battingAverage;
        metrics[k].econ = roster[i].economyRate;
        metrics[k].id = roster[i].id;
        strcpy(metrics[k].name, roster[i].name);
        strcpy(metrics[k].role, roster[i].role);
        strcpy(metrics[k].team, roster[i].team);
        metrics[k].sr = roster[i].strikeRate;
        metrics[k].runs = roster[i].totalRuns;
        metrics[k].wkt = roster[i].wickets;

        if (strcmp(metrics[k].role, "Batsman") == 0)
        {
            metrics[k].performance_score = ((float)metrics[k].avg * metrics[k].sr) / 100;
        }
        else if (strcmp(metrics[k].role, "Bowler") == 0)
        {
            metrics[k].performance_score = (metrics[k].wkt * 2) + (100 - metrics[k].econ);
        }
        else
        {
            metrics[k].performance_score = (((float)metrics[k].avg * metrics[k].sr) / 100) + (metrics[k].wkt * 2);
        }
    }
}

int find_role_start_index(const PlayerMetrics metrics[], int start, int end, const int choice)
{
    char query[50];
    if (resolve_role_string(choice, query) == -1)
    {
        return -1;
    }
    int res = -1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (strcmp(metrics[mid].role, query) == 0)
        {
            res = mid;
            end = mid - 1;
        }
        else if (strcmp(metrics[mid].role, query) > 0)
        {
            end = mid - 1;
        }
        else
        {
            start = mid + 1;
        }
    }
    return res;
}

int find_role_end_index(const PlayerMetrics metrics[], int start, int end, const int choice)
{
    char query[50];
    if (resolve_role_string(choice, query) == -1)
    {
        return -1;
    }
    int res = -1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (strcmp(metrics[mid].role, query) == 0)
        {
            res = mid;
            start = mid + 1;
        }
        else if (strcmp(metrics[mid].role, query) > 0)
        {
            end = mid - 1;
        }
        else
        {
            start = mid + 1;
        }
    }
    return res;
}

void render_performance_table(const PlayerMetrics metrics[], const int start, const int k, const int choice)
{
    char query[50];
    if (resolve_role_string(choice, query) == -1)
    {
        return;
    }
    printf("\n====================================================================================\n");
    printf("\nID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("\n====================================================================================\n");
    for (int i = start; i < start + k && strcmp(metrics[i].role, query) == 0; i++)
    {
        printf("\n%d\t%s\t%s\t%d\t%f\t%f\t%d\t%f\t%f", metrics[i].id, metrics[i].name, metrics[i].role, metrics[i].runs, metrics[i].avg, metrics[i].sr, metrics[i].wkt, metrics[i].econ, metrics[i].performance_score);
    }
}

void show_top_performers(Player roster[], const int n)
{
    execute_team_sort(roster, 0, n - 1);
    char buffer[50];
    printf("\nEnter team Name: ");
    getchar();
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    int start = find_team_start_index(roster, n, buffer);
    int end = find_team_end_index(roster, n, buffer);
    PlayerMetrics per[end - start + 1];

    compute_performance_data(roster, per, start, end);
    execute_metrics_sort(per, 0, (end - start));

    int choice;
    printf("\nEnter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &choice);

    int k;
    printf("\nEnter number of players: ");
    scanf("%d", &k);

    if (k > end - start + 1)
    {
        k = end - start + 1;
    }
    start = find_role_start_index(per, 0, end - start + 1, choice);
    if (start == -1)
    {
        return;
    }

    render_performance_table(per, start, k, choice);
}

void global_role_ranking(const Player roster[], const int n)
{
    int choice;
    printf("\nEnter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &choice);

    PlayerMetrics *per = (PlayerMetrics *)malloc(n * sizeof(PlayerMetrics));
    compute_performance_data(roster, per, 0, n - 1);
    execute_metrics_sort(per, 0, n - 1);

    int start = find_role_start_index(per, 0, n - 1, choice);
    int end = find_role_end_index(per, 0, n - 1, choice);
    render_performance_table(per, start, end + 1, choice);
    free(per);
}

int main()
{
    Player *roster = (Player *)malloc(playerCount * sizeof(Player));
    for (int i = 0; i < playerCount; i++)
    {
        duplicate_player(&roster[i], players[i]);
        id_tracker[roster[i].id] = 1;
    }

    int current_size = playerCount;
    int option;

    while (1)
    {
        printf("\n==============================================================================\n");
        printf("\n ICC ODI Player Performance Analyzer\n");
        printf("\n==============================================================================\n");
        printf("\n 1. Add Player to Team");
        printf("\n 2. Display Players of a Specific Team");
        printf("\n 3. Display Teams by Average Batting Strike Rate");
        printf("\n 4. Display Top K Players of a Specific Team by Role");
        printf("\n 5. Display all Players of specific role Across All Teams by performance index");
        printf("\n 6. Exit");
        printf("\n==============================================================================\n");
        printf("\n Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                register_new_player(&roster, &current_size);
                break;
            case 2:
                show_team_roster(roster, current_size);
                break;
            case 3:
                analyze_team_batting_power(roster, current_size);
                break;
            case 4:
                show_top_performers(roster, current_size);
                break;
            case 5:
                global_role_ranking(roster, current_size);
                break;
            case 6:
                free(roster);
                exit(0);
        }
    }
    return 0;
}
