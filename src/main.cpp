#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const int LINES = 5; // lines
const int JOBS = 50; // processingCosts

int entryCosts[LINES];                     // E(line)
int exitCosts[LINES];                      // X(line)
int processingCosts[LINES][JOBS];          // A(line, station)
int transferCosts[LINES][LINES][JOBS - 1]; // T(lineFrom, lineTo, station-1)

string splitEnd(string str, string delim)
{
    auto start = str.find(delim) + delim.length();
    auto end = str.find("_", start);
    return str.substr(start, end);
}

void getEntryExitCosts()
{
    ifstream inputFile;
    inputFile.open("./data/entry_exit_times.csv");

    string line;
    getline(inputFile, line);
    line = "";
    while (getline(inputFile, line))
    {
        string tempStr, temp;
        int row;
        stringstream inputString(line);

        // LINES
        getline(inputString, tempStr, ',');
        temp = splitEnd(tempStr, "_");
        row = atoi(temp.c_str()) - 1;
        // ENTRIES
        getline(inputString, tempStr, ',');
        entryCosts[row] = atoi(tempStr.c_str());
        // EXITS
        getline(inputString, tempStr, ',');
        exitCosts[row] = atoi(tempStr.c_str());
    }

    cout << "E[LINE]      X[LINE]" << endl;
    for (int i = 0; i < LINES; i++)
    {
        cout << entryCosts[i] << "\t\t" << exitCosts[i] << endl;
    }
}

void getProcessingCosts()
{
    ifstream inputFile;
    inputFile.open("./data/processing_times.csv");

    string line;
    getline(inputFile, line);
    line = "";
    while (getline(inputFile, line))
    {
        string tempStr, temp;
        int job;
        stringstream inputString(line);

        // STATION
        getline(inputString, tempStr, ',');
        temp = splitEnd(tempStr, "_");
        job = atoi(temp.c_str()) - 1;

        // PROCESSING COST
        for (int i = 0; i < LINES; i++)
        {
            getline(inputString, tempStr, ',');
            processingCosts[i][job] = atoi(tempStr.c_str());
        }
    }

    cout << "PROCESSING COST A[LINE][STATION] (in mins)" << endl;
    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j < JOBS; j++)
        {
            cout << processingCosts[i][j] << "  ";
        }
        cout << "," << endl;
    }
}

void getTransferCosts()
{
    ifstream inputFile;
    inputFile.open("./data/transfer_times.csv");

    string line = "";
    getline(inputFile, line);
    line = "";
    while (getline(inputFile, line))
    {
        string tempStr, temp;
        int job, linefrom, lineto, cost;
        stringstream inputString(line);

        // STATION
        getline(inputString, tempStr, ',');
        temp = splitEnd(tempStr, "_");
        job = atoi(temp.c_str()) - 1;
        // LINE_FROM
        getline(inputString, tempStr, ',');
        temp = splitEnd(tempStr, "_");
        linefrom = atoi(temp.c_str()) - 1;
        // LINE_TO
        getline(inputString, tempStr, ',');
        temp = splitEnd(tempStr, "_");
        lineto = atoi(temp.c_str()) - 1;
        // COST
        getline(inputString, tempStr, ',');
        temp = splitEnd(tempStr, "_");
        cost = atoi(temp.c_str());

        // TRANSFER COST
        transferCosts[linefrom][lineto][job] = cost;
    }

    cout << "TRANSFER COST T[LINE_FROM][LINE_TO][STATION-1] (in mins)" << endl;
    for (int i = 0; i < LINES; i++)
    {
        cout << "FROM LINE " << i + 1 << " TO:" << endl;
        for (int j = 0; j < LINES; j++)
        {
            cout << "LINE " << j + 1 << " { ";
            for (int z = 0; z < JOBS - 1; z++)
            {
                cout << transferCosts[i][j][z] << " ";
            }
            cout << "} " << endl;
        }
        cout << endl;
    }
}

void fastestWay()
{
    int f[LINES][JOBS + 1];
    int l[LINES][JOBS + 1];

    for (int i = 0; i < LINES; i++)
    {
        f[i][JOBS] = 0;
        l[i][JOBS] = 0;
    }

    // F[LINES][1] and L[LINES][1]
    for (int i = 0; i < LINES; i++)
    {
        f[i][0] = entryCosts[i] + processingCosts[i][0];
        l[i][0] = i + 1;
    }

    for (int j = 1; j < JOBS; j++)
    {
        // LINE = 1
        if (f[0][j - 1] + processingCosts[0][j] <=
            min(f[1][j - 1] + transferCosts[1][0][j - 1] + processingCosts[0][j],
                min(f[2][j - 1] + transferCosts[2][0][j - 1] + processingCosts[0][j],
                    min(f[3][j - 1] + transferCosts[3][0][j - 1] + processingCosts[0][j],
                        f[4][j - 1] + transferCosts[4][0][j - 1] + processingCosts[0][j]))))
        {
            f[0][j] = f[0][j - 1] + processingCosts[0][j];
            l[0][j] = 1;
        }
        else if (f[1][j - 1] + transferCosts[1][0][j - 1] + processingCosts[0][j] <=
                 min(f[2][j - 1] + transferCosts[2][0][j - 1] + processingCosts[0][j],
                     min(f[3][j - 1] + transferCosts[3][0][j - 1] + processingCosts[0][j],
                         f[4][j - 1] + transferCosts[4][0][j - 1] + processingCosts[0][j])))
        {
            f[0][j] = f[1][j - 1] + transferCosts[1][0][j - 1] + processingCosts[0][j];
            l[0][j] = 2;
        }
        else if (f[2][j - 1] + transferCosts[2][0][j - 1] + processingCosts[0][j] <=
                 min(f[3][j - 1] + transferCosts[3][0][j - 1] + processingCosts[0][j],
                     f[4][j - 1] + transferCosts[4][0][j - 1] + processingCosts[0][j]))
        {
            f[0][j] = f[2][j - 1] + transferCosts[2][0][j - 1] + processingCosts[0][j];
            l[0][j] = 3;
        }
        else if (f[3][j - 1] + transferCosts[3][0][j - 1] + processingCosts[0][j] <=
                 f[4][j - 1] + transferCosts[4][0][j - 1] + processingCosts[0][j])
        {
            f[0][j] = f[3][j - 1] + transferCosts[3][0][j - 1] + processingCosts[0][j];
            l[0][j] = 4;
        }
        else
        {
            f[0][j] = f[4][j - 1] + transferCosts[4][0][j - 1] + processingCosts[0][j];
            l[0][j] = 5;
        }

        // LINE = 2
        if (f[1][j - 1] + processingCosts[1][j] <=
            min(f[0][j - 1] + transferCosts[0][1][j - 1] + processingCosts[1][j],
                min(f[2][j - 1] + transferCosts[2][1][j - 1] + processingCosts[1][j],
                    min(f[3][j - 1] + transferCosts[3][1][j - 1] + processingCosts[1][j],
                        f[4][j - 1] + transferCosts[4][1][j - 1] + processingCosts[1][j]))))
        {
            f[1][j] = f[1][j - 1] + processingCosts[1][j];
            l[1][j] = 2;
        }
        else if (f[0][j - 1] + transferCosts[0][1][j - 1] + processingCosts[1][j] <=
                 min(f[2][j - 1] + transferCosts[2][1][j - 1] + processingCosts[1][j],
                     min(f[3][j - 1] + transferCosts[3][1][j - 1] + processingCosts[1][j],
                         f[4][j - 1] + transferCosts[4][1][j - 1] + processingCosts[1][j])))
        {
            f[1][j] = f[0][j - 1] + transferCosts[0][1][j - 1] + processingCosts[1][j];
            l[1][j] = 1;
        }
        else if (f[2][j - 1] + transferCosts[2][1][j - 1] + processingCosts[1][j] <=
                 min(f[3][j - 1] + transferCosts[3][1][j - 1] + processingCosts[1][j],
                     f[4][j - 1] + transferCosts[4][1][j - 1] + processingCosts[1][j]))
        {
            f[1][j] = f[2][j - 1] + transferCosts[2][1][j - 1] + processingCosts[1][j];
            l[1][j] = 3;
        }
        else if (f[3][j - 1] + transferCosts[3][1][j - 1] + processingCosts[1][j] <=
                 f[4][j - 1] + transferCosts[4][1][j - 1] + processingCosts[1][j])
        {
            f[1][j] = f[3][j - 1] + transferCosts[3][1][j - 1] + processingCosts[1][j];
            l[1][j] = 4;
        }
        else
        {
            f[1][j] = f[4][j - 1] + transferCosts[4][1][j - 1] + processingCosts[1][j];
            l[1][j] = 5;
        }

        // LINE = 3
        if (f[2][j - 1] + processingCosts[2][j] <=
            min(f[0][j - 1] + transferCosts[0][2][j - 1] + processingCosts[2][j],
                min(f[1][j - 1] + transferCosts[1][2][j - 1] + processingCosts[2][j],
                    min(f[3][j - 1] + transferCosts[3][2][j - 1] + processingCosts[2][j],
                        f[4][j - 1] + transferCosts[4][2][j - 1] + processingCosts[2][j]))))
        {
            f[2][j] = f[2][j - 1] + processingCosts[2][j];
            l[2][j] = 3;
        }
        else if (f[0][j - 1] + transferCosts[0][2][j - 1] + processingCosts[2][j] <=
                 min(f[1][j - 1] + transferCosts[1][2][j - 1] + processingCosts[2][j],
                     min(f[3][j - 1] + transferCosts[3][2][j - 1] + processingCosts[2][j],
                         f[4][j - 1] + transferCosts[4][2][j - 1] + processingCosts[2][j])))
        {
            f[2][j] = f[0][j - 1] + transferCosts[0][2][j - 1] + processingCosts[2][j];
            l[2][j] = 1;
        }
        else if (f[1][j - 1] + transferCosts[1][2][j - 1] + processingCosts[2][j] <=
                 min(f[3][j - 1] + transferCosts[3][2][j - 1] + processingCosts[2][j],
                     f[4][j - 1] + transferCosts[4][2][j - 1] + processingCosts[2][j]))
        {
            f[2][j] = f[1][j - 1] + transferCosts[1][2][j - 1] + processingCosts[2][j];
            l[2][j] = 2;
        }
        else if (f[3][j - 1] + transferCosts[3][2][j - 1] + processingCosts[2][j] <=
                 f[4][j - 1] + transferCosts[4][2][j - 1] + processingCosts[2][j])
        {
            f[2][j] = f[3][j - 1] + transferCosts[3][2][j - 1] + processingCosts[2][j];
            l[2][j] = 4;
        }
        else
        {
            f[2][j] = f[4][j - 1] + transferCosts[4][2][j - 1] + processingCosts[2][j];
            l[2][j] = 5;
        }

        // LINE = 4
        if (f[3][j - 1] + processingCosts[3][j] <=
            min(f[0][j - 1] + transferCosts[0][3][j - 1] + processingCosts[3][j],
                min(f[1][j - 1] + transferCosts[1][3][j - 1] + processingCosts[3][j],
                    min(f[2][j - 1] + transferCosts[2][3][j - 1] + processingCosts[3][j],
                        f[4][j - 1] + transferCosts[4][3][j - 1] + processingCosts[3][j]))))
        {
            f[3][j] = f[3][j - 1] + processingCosts[3][j];
            l[3][j] = 4;
        }
        else if (f[0][j - 1] + transferCosts[0][3][j - 1] + processingCosts[3][j] <=
                 min(f[1][j - 1] + transferCosts[1][3][j - 1] + processingCosts[3][j],
                     min(f[2][j - 1] + transferCosts[2][3][j - 1] + processingCosts[3][j],
                         f[4][j - 1] + transferCosts[4][3][j - 1] + processingCosts[3][j])))
        {
            f[3][j] = f[0][j - 1] + transferCosts[0][3][j - 1] + processingCosts[3][j];
            l[3][j] = 1;
        }
        else if (f[1][j - 1] + transferCosts[1][3][j - 1] + processingCosts[3][j] <=
                 min(f[2][j - 1] + transferCosts[2][3][j - 1] + processingCosts[3][j],
                     f[4][j - 1] + transferCosts[4][3][j - 1] + processingCosts[3][j]))
        {
            f[3][j] = f[1][j - 1] + transferCosts[1][3][j - 1] + processingCosts[3][j];
            l[3][j] = 2;
        }
        else if (f[2][j - 1] + transferCosts[2][3][j - 1] + processingCosts[3][j] <=
                 f[4][j - 1] + transferCosts[4][3][j - 1] + processingCosts[3][j])
        {
            f[3][j] = f[2][j - 1] + transferCosts[2][3][j - 1] + processingCosts[3][j];
            l[3][j] = 3;
        }
        else
        {
            f[3][j] = f[4][j - 1] + transferCosts[4][3][j - 1] + processingCosts[3][j];
            l[3][j] = 5;
        }

        // LINE = 5
        if (f[4][j - 1] + processingCosts[4][j] <=
            min(f[0][j - 1] + transferCosts[0][4][j - 1] + processingCosts[4][j],
                min(f[1][j - 1] + transferCosts[1][4][j - 1] + processingCosts[4][j],
                    min(f[2][j - 1] + transferCosts[2][4][j - 1] + processingCosts[4][j],
                        f[3][j - 1] + transferCosts[3][4][j - 1] + processingCosts[4][j]))))
        {
            f[4][j] = f[4][j - 1] + processingCosts[4][j];
            l[4][j] = 5;
        }
        else if (f[0][j - 1] + transferCosts[0][4][j - 1] + processingCosts[4][j] <=
                 min(f[1][j - 1] + transferCosts[1][4][j - 1] + processingCosts[4][j],
                     min(f[2][j - 1] + transferCosts[2][4][j - 1] + processingCosts[4][j],
                         f[3][j - 1] + transferCosts[3][4][j - 1] + processingCosts[4][j])))
        {
            f[4][j] = f[0][j - 1] + transferCosts[0][4][j - 1] + processingCosts[4][j];
            l[4][j] = 1;
        }
        else if (f[1][j - 1] + transferCosts[1][4][j - 1] + processingCosts[4][j] <=
                 min(f[2][j - 1] + transferCosts[2][4][j - 1] + processingCosts[4][j],
                     f[3][j - 1] + transferCosts[3][4][j - 1] + processingCosts[4][j]))
        {
            f[4][j] = f[1][j - 1] + transferCosts[1][4][j - 1] + processingCosts[4][j];
            l[4][j] = 2;
        }
        else if (f[2][j - 1] + transferCosts[2][4][j - 1] + processingCosts[4][j] <=
                 f[3][j - 1] + transferCosts[3][4][j - 1] + processingCosts[4][j])
        {
            f[4][j] = f[2][j - 1] + transferCosts[2][4][j - 1] + processingCosts[4][j];
            l[4][j] = 3;
        }
        else
        {
            f[4][j] = f[3][j - 1] + transferCosts[3][4][j - 1] + processingCosts[4][j];
            l[4][j] = 4;
        }
    }

    if (f[0][JOBS - 1] + exitCosts[0] <=
        min(f[1][JOBS - 1] + exitCosts[1],
            min(f[2][JOBS - 1] + exitCosts[2],
                f[3][JOBS - 1] + exitCosts[3])))
    {
        f[0][JOBS] = f[0][JOBS - 1] + exitCosts[0];
        l[0][JOBS] = 1;
    }
    else if (f[1][JOBS - 1] + exitCosts[1] <=
             min(f[2][JOBS - 1] + exitCosts[2],
                 f[3][JOBS - 1] + exitCosts[3]))
    {
        f[1][JOBS] = f[1][JOBS - 1] + exitCosts[1];
        l[1][JOBS] = 2;
    }
    else if (f[2][JOBS - 1] + exitCosts[2] <=
             f[3][JOBS - 1] + exitCosts[3])
    {
        f[2][JOBS] = f[2][JOBS - 1] + exitCosts[2];
        l[2][JOBS] = 3;
    }
    else
    {
        f[3][JOBS] = f[3][JOBS - 1] + exitCosts[3];
        l[3][JOBS] = 4;
    }

    cout << "F[LINE][STATION]" << endl;
    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j <= JOBS; j++)
        {
            cout << f[i][j] << " ";
            if (j == JOBS && f[i][j] != 0)
            {
                cout << "mins <- TOTAL TIME (" << f[i][j] / 12 << " hrs)";
            }
        }
        cout << endl;
    }
    cout << endl;
    cout << "L[LINE][STATION]" << endl;
    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j <= JOBS; j++)
        {
            cout << l[i][j] << " ";
            if (j == JOBS && l[i][j] != 0)
            {
                cout << "<-";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    getEntryExitCosts();
    cout << endl;
    getProcessingCosts();
    cout << endl;
    getTransferCosts();
    cout << endl;

    fastestWay();

    return 0;
}
