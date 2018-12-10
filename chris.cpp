/*
TODO:
- take PICKS_PER_TURN as argument
- give option to see more detail of each trial (maybe if num_trials is small enough)
*/

#include <stdio.h>
#include <random>
#include "defines.h"

const uint MAX_LENGTH = 50;
const uint PICKS_PER_TURN = 3;

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

uint rand_int(uint min, uint max)
{
    std::uniform_int_distribution<uint> uni(min,max); // guaranteed unbiased
    return uni(rng);
}

bool in_array(uint array[], uint length, uint value)
{
    for(uint i = 0; i < length; i++)
    {
        if(array[i] == value)
            return true;
    }
    return false;
}

bool run_game(uint starting_length)
{
    bool list[MAX_LENGTH] = {};
    uint current_length = starting_length;

    while (current_length < MAX_LENGTH)
    {
        uint picks[PICKS_PER_TURN] = {};
        for (uint i = 0; i < PICKS_PER_TURN; ++i)
        {
            uint rand = rand_int(1, current_length);
            while(in_array(picks, PICKS_PER_TURN, rand))
            {
                ++rand;
                if(rand > current_length)
                    rand %= current_length;
            }
            picks[i] = rand;
            list[rand - 1] = true;
        }

        for (uint i = 0; i < current_length; ++i)
        {
            if (!list[i])
            {
                ++current_length;
                goto goroundagain;
            }
        }

        return true;

        goroundagain:;
    }

    return false;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "I need 2 arguments: <num_trials> <starting_length>\n");
        return 1;
    }

    int num_trials, starting_length;
    sscanf(argv[1], "%d", &num_trials);
    sscanf(argv[2], "%d", &starting_length);

    if((num_trials < 1) || (starting_length < 3) || (starting_length >= MAX_LENGTH))
    {
        fprintf(
            stderr,
            "Invalid arguments. Num trials must be >0; starting length must be >=3 and <%d.\n",
            MAX_LENGTH);
        return 1;
    }

    uint wins = 0;

    for(int i = 0; i < num_trials; ++i)
    {
        if(run_game(starting_length))
        {
            ++wins;
            printf("W");
        }
        else
        {
            printf("-");
        }
        if(i % 10 == 0)
            fflush(stdout);
    }
    printf("\n\n");

    float percent = wins / (float)num_trials * 100.0;
    printf(
        "Won %d/%d when starting at length=%d (%.2f%%)\n",
        wins,
        num_trials,
        starting_length,
        percent);

    return 0;
}
