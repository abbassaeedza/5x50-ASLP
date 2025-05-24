// redesigning for 5 lines and 50 sequential stations
#include <iostream>
#include <algorithm>
using namespace std;

const int LINES = 5;
const int JOBS = 50;

const int ENTRIES[LINES] = {1, 4, 2, 1, 2};
const int EXITS[LINES] = {5, 4, 4, 4, 1};

// const int BASE_COST = 20;
// const int FAILURE_PENALTY = 10;

const int PROCESSING_COST[LINES][JOBS] = {
    {840, 1190, 4665, 115},
    {705, 350, 390, 2225},
    {320, 195, 30, 85},
    {20, 65, 85, 205}};

// const int FAILURE[LINES][JOBS] = {
//	{13, 48, 15, 6},
//	{13, 9, 11, 20},
//	{6, 4, 1, 2},
//	{1, 3, 3, 1} };

const int TRANSFER_COST[LINES][LINES][JOBS] = {};

int (*stationCost())[JOBS]
{
    static int stations[LINES][JOBS];
    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j < JOBS; j++)
        {
            stations[i][j] = BASE_COST + PROCESSING_COST[i][j] + (FAILURE_PENALTY * FAILURE[i][j]);
            cout << stations[i][j] << " ";
        }
        cout << endl;
    }

    return stations;
}

int fastestWay(int stations[][JOBS])
{
    int f[LINES][JOBS + 1];
    int l[LINES][JOBS + 1];

    f[0][JOBS] = 0;
    f[1][JOBS] = 0;
    f[2][JOBS] = 0;
    f[3][JOBS] = 0;
    l[0][JOBS] = 0;
    l[1][JOBS] = 0;
    l[2][JOBS] = 0;
    l[3][JOBS] = 0;

    for (int i = 0; i < LINES; i++)
    {
        f[i][0] = ENTRIES[i] + stations[i][0];
        l[i][0] = i + 1;
    }

    for (int j = 1; j < JOBS; j++)
    {
        if (f[0][j - 1] + stations[0][j] <=
            min(f[1][j - 1] + TRANSFER_COST + stations[0][j],
                min(f[2][j - 1] + TRANSFER_COST + stations[0][j],
                    f[3][j - 1] + TRANSFER_COST + stations[0][j])))
        {
            f[0][j] = f[0][j - 1] + stations[0][j];
            l[0][j] = 1;
        }
        else if (f[1][j - 1] + TRANSFER_COST + stations[0][j] <=
                 min(f[2][j - 1] + TRANSFER_COST + stations[0][j],
                     f[3][j - 1] + TRANSFER_COST + stations[0][j]))
        {
            f[0][j] = f[1][j - 1] + TRANSFER_COST + stations[0][j];
            l[0][j] = 2;
        }
        else if (f[2][j - 1] + TRANSFER_COST + stations[0][j] <=
                 f[3][j - 1] + TRANSFER_COST + stations[0][j])
        {
            f[0][j] = f[2][j - 1] + TRANSFER_COST + stations[0][j];
            l[0][j] = 3;
        }
        else
        {
            f[0][j] = f[3][j - 1] + TRANSFER_COST + stations[0][j];
            l[0][j] = 4;
        }

        if (f[1][j - 1] + stations[1][j] <=
            min(f[0][j - 1] + TRANSFER_COST + stations[1][j],
                min(f[2][j - 1] + TRANSFER_COST + stations[1][j],
                    f[3][j - 1] + TRANSFER_COST + stations[1][j])))
        {
            f[1][j] = f[1][j - 1] + stations[1][j];
            l[1][j] = 2;
        }
        else if (f[0][j - 1] + TRANSFER_COST + stations[1][j] <=
                 min(f[2][j - 1] + TRANSFER_COST + stations[1][j],
                     f[3][j - 1] + TRANSFER_COST + stations[1][j]))
        {
            f[1][j] = f[0][j - 1] + TRANSFER_COST + stations[1][j];
            l[1][j] = 1;
        }
        else if (f[2][j - 1] + TRANSFER_COST + stations[1][j] <=
                 f[3][j - 1] + TRANSFER_COST + stations[1][j])
        {
            f[1][j] = f[2][j - 1] + TRANSFER_COST + stations[1][j];
            l[1][j] = 3;
        }
        else
        {
            f[1][j] = f[3][j - 1] + TRANSFER_COST + stations[1][j];
            l[1][j] = 4;
        }

        if (f[2][j - 1] + stations[2][j] <=
            min(f[0][j - 1] + TRANSFER_COST + stations[2][j],
                min(f[1][j - 1] + TRANSFER_COST + stations[2][j],
                    f[3][j - 1] + TRANSFER_COST + stations[2][j])))
        {
            f[2][j] = f[2][j - 1] + stations[2][j];
            l[2][j] = 3;
        }
        else if (f[0][j - 1] + TRANSFER_COST + stations[2][j] <=
                 min(f[1][j - 1] + TRANSFER_COST + stations[2][j],
                     f[3][j - 1] + TRANSFER_COST + stations[2][j]))
        {
            f[2][j] = f[0][j - 1] + TRANSFER_COST + stations[2][j];
            l[2][j] = 1;
        }
        else if (f[1][j - 1] + TRANSFER_COST + stations[2][j] <=
                 f[3][j - 1] + TRANSFER_COST + stations[2][j])
        {
            f[2][j] = f[1][j - 1] + TRANSFER_COST + stations[2][j];
            l[2][j] = 2;
        }
        else
        {
            f[2][j] = f[3][j - 1] + TRANSFER_COST + stations[2][j];
            l[2][j] = 4;
        }

        if (f[3][j - 1] + stations[3][j] <=
            min(f[0][j - 1] + TRANSFER_COST + stations[3][j],
                min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
                    f[2][j - 1] + TRANSFER_COST + stations[3][j])))
        {
            f[3][j] = f[3][j - 1] + stations[3][j];
            l[3][j] = 4;
        }
        else if (f[0][j - 1] + TRANSFER_COST + stations[3][j] <=
                 min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
                     f[2][j - 1] + TRANSFER_COST + stations[3][j]))
        {
            f[3][j] = f[0][j - 1] + TRANSFER_COST + stations[3][j];
            l[3][j] = 1;
        }
        else if (f[1][j - 1] + TRANSFER_COST + stations[3][j] <=
                 f[2][j - 1] + TRANSFER_COST + stations[3][j])
        {
            f[3][j] = f[1][j - 1] + TRANSFER_COST + stations[3][j];
            l[3][j] = 2;
        }
        else
        {
            f[3][j] = f[2][j - 1] + TRANSFER_COST + stations[3][j];
            l[3][j] = 3;
        }

        if (f[3][j - 1] + stations[3][j] <=
            min(f[0][j - 1] + TRANSFER_COST + stations[3][j],
                min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
                    f[2][j - 1] + TRANSFER_COST + stations[3][j])))
        {
            f[3][j] = f[3][j - 1] + stations[3][j];
            l[3][j] = 4;
        }
        else if (f[0][j - 1] + TRANSFER_COST + stations[3][j] <=
                 min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
                     f[2][j - 1] + TRANSFER_COST + stations[3][j]))
        {
            f[3][j] = f[0][j - 1] + TRANSFER_COST + stations[3][j];
            l[3][j] = 1;
        }
        else if (f[1][j - 1] + TRANSFER_COST + stations[3][j] <=
                 f[2][j - 1] + TRANSFER_COST + stations[3][j])
        {
            f[3][j] = f[1][j - 1] + TRANSFER_COST + stations[3][j];
            l[3][j] = 2;
        }
        else
        {
            f[3][j] = f[2][j - 1] + TRANSFER_COST + stations[3][j];
            l[3][j] = 3;
        }
    }

    if (f[0][JOBS - 1] + EXITS[0] <=
        min(f[1][JOBS - 1] + EXITS[1],
            min(f[2][JOBS - 1] + EXITS[2],
                f[3][JOBS - 1] + EXITS[3])))
    {
        f[0][JOBS] = f[0][JOBS - 1] + EXITS[0];
        l[0][JOBS] = 1;
    }
    else if (f[1][JOBS - 1] + EXITS[1] <=
             min(f[2][JOBS - 1] + EXITS[2],
                 f[3][JOBS - 1] + EXITS[3]))
    {
        f[1][JOBS] = f[1][JOBS - 1] + EXITS[1];
        l[1][JOBS] = 2;
    }
    else if (f[2][JOBS - 1] + EXITS[2] <=
             f[3][JOBS - 1] + EXITS[3])
    {
        f[2][JOBS] = f[2][JOBS - 1] + EXITS[2];
        l[2][JOBS] = 3;
    }
    else
    {
        f[3][JOBS] = f[3][JOBS - 1] + EXITS[3];
        l[3][JOBS] = 4;
    }

    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j <= JOBS; j++)
        {
            cout << f[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j <= JOBS; j++)
        {
            cout << l[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

int main()
{

    int (*stations)[JOBS] = stationCost();
    cout << stations << " " << *stations << endl;

    fastestWay(stations);

    return 0;
}
