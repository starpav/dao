#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            //printf("step2");
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; j < candidate_count; j++)
        {
            if (j < candidate_count && i + j < candidate_count)
            {
                preferences[ranks[i]][ranks[i + j]]++;
            }
        }
    }
    //printf("success!");
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    int lose_pair = 0;
    do
    {
        for (int i = 0; i < candidate_count; i++)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                if ((preferences[i][j] > preferences[j][i]) && (i != j))
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    //printf("%i%i ", pairs[pair_count].winner, pairs[pair_count].loser);
                    pair_count++;
                }
                else
                {
                    lose_pair++;
                }
            }
        }
    }
    while (pair_count + lose_pair < candidate_count * (candidate_count - 1) / 2);
    //printf("%i\n", pair_count);
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    pair temp_pair;

    int swap_counter = -1;
    do
    {
        swap_counter = 0;
        for (int r = 0; r < pair_count; r++)
        {
            //printf("%i", pairs[r].winner);
            if (preferences[pairs[r].winner][pairs[r].loser] < preferences[pairs[r + 1].winner][pairs[r + 1].loser])
            {
                temp_pair = pairs[r];
                pairs[r] = pairs[r + 1];
                pairs[r + 1] = temp_pair;
                swap_counter++;
            }
            //printf("%i%i ", pairs[r].winner, pairs[r].loser);
        }
    }
    while (swap_counter != 0);
    //printf("%i\n", pair_count);
    return;
}


//check path "loser to winner"
bool check(int candidate_index_1, int candidate_index_2)
{
    // Check cycle
    if (candidate_index_1 == candidate_index_2)
    {
        return true;
    }

    // find next candidate
    for (int i = 0; i < candidate_count; i++)
    {
        //if he is locked go next node
        if (locked[candidate_index_1][i])
        {
            //find next
            if (check(i, candidate_index_2))
            {
                return true;
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles

void lock_pairs(void)
{
    // Loop through pairs
    for (int i = 0; i < pair_count; i++)
    {
        // If check returns false, lock the pair
        if (!check(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    int winner_index = -1;
    int counter = -1;

    do
    {
        winner_index++;
        counter = 0;
        for (int i = 0; i < candidate_count; i++)
        {
            if (winner_index != i && locked[i][winner_index])
            {
                counter++;
            }
        }
    }
    while (counter != 0);

    printf("%s\n", candidates[winner_index]);
    return;
}
