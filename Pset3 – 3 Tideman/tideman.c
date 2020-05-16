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

// Custom struct for my print-winner function
typedef struct
{
    int winner;
    int points;
}
finalwinner;

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

    // I'm essentially going through the candidates
    // When the name matches the candidate
    // I store the candidate's position into the rank

    for (int a = 0; a < candidate_count; a++)
    {
        if (strcmp(name, candidates[a]) == 0)
        {
            ranks[rank] = a;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks

void record_preferences(int ranks[])
{


    // I'm storing the rank of each candidate
    // In an array where the index
    // Matches the index of the candidate
    // So if at position 0 I store rank 2
    // That means that candidate 0 has a rank 2

    int candidate[candidate_count];

    for (int a = 0; a < candidate_count; a++)
    {

        for (int b = 0; b < candidate_count; b++)
        {
            if (ranks[a] == b)
            {
                candidate[b] = a;
            }
        }

    }


    // Now I'm comparing ranks
    // I compare each candidate to all candidates
    // If her rank is below the compared candidate
    // I add a +1 to that combination


    for (int c = 0; c < candidate_count; c++)
    {
        for (int d = 0; d < candidate_count; d++)
        {
            if (candidate[c] < candidate[d])
            {
                preferences[c][d]++;
            }
        }
    }


    return;

}

// Record pairs of candidates where one is preferred over the other

void add_pairs(void)
{

    // I cycle through the candidates
    // Compare them to each other
    // Whoever has advantage
    // I store it in the pairs array
    // Trick: I define the position of the pair
    // Based on pair count


    for (int a = 0; a < candidate_count; a++)
    {

        for (int b = 0; b < candidate_count; b++)
        {

            if (preferences[a][b] > preferences[b][a])
            {
                pairs[pair_count].winner = a;
                pairs[pair_count].loser = b;
                pair_count++;
            }

        }

    }

    return;
}

// Sort pairs in decreasing order by strength of victory

void sort_pairs(void)
{


    // Store the results in an array

    int score[pair_count];

    for (int a = 0; a < pair_count; a++)
    {
        score[a] = preferences[pairs[a].winner][pairs[a].loser];
    }

    // Define my buffer

    pair bufferpair[1];

    // Sort them

    for (int b = 0; b < pair_count; b++)
    {

        int c = 0;

        while (c < pair_count - 1)
        {

            // If the first score is smaller than the second

            if (score[c] < score[c + 1])
            {
                // Move the looser to the buffer
                bufferpair[0].winner = pairs[c].winner;
                bufferpair[0].loser = pairs[c].loser;

                // Move the winner up
                pairs[c].winner = pairs[c + 1].winner;
                pairs[c].loser = pairs[c + 1].loser;

                // Move looser down
                pairs[c + 1].winner = bufferpair[0].winner;
                pairs[c + 1].loser = bufferpair[0].loser;
            }

            c++;
        }

    }



    return;
}

// Lock pairs into the candidate graph in order, without creating cycles

void lock_pairs(void)
{

    // First, set them all to false

    for (int a = 0; a < candidate_count; a++)
    {

        for (int b = 0; b < candidate_count; b++)
        {
            locked[a][b] = false;
        }

    }

    // Now, set only the right ones to true

    for (int c = 0; c < pair_count; c++)
    {

        // I essentially make sure the looser going forward
        // Isn't the winner of the very first round
        // If so, then thats a cycle

        if (pairs[0].winner != pairs[c].loser)
        {
            locked[pairs[c].winner][pairs[c].loser] = true;
        }

    }

    return;
}

// Print the winner of the election

void print_winner(void)
{


    // Created a struct for final winners
    // With the candidate index and and no. of falses in the lock index

    finalwinner finalwinners[candidate_count];


    for (int a = 0; a < candidate_count; a++)
    {
        for (int b = 0; b < candidate_count; b++)
        {

            // If a candidate has no one pointing back at her
            // Then store her on my array of final winners
            // And add a point to the candidate

            if (locked[b][a] == false)
            {
                finalwinners[a].winner = a;
                finalwinners[a].points++;
            }
        }
    }

    // Whoever's points match the candidate count
    // Had all pointers as FALSE
    // As per above
    // That is the source of the graph

    for (int c = 0; c < candidate_count; c++)
    {
        if (finalwinners[c].points == candidate_count)
        {
            printf("%s\n", candidates[finalwinners[c].winner]);
        }
    }

    return;
}

