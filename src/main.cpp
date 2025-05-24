#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const int LINES = 5;
const int JOBS = 50;

int entryCosts[LINES];
int exitCosts[LINES];
int processingCosts[LINES][JOBS];
int transferCosts[LINES][LINES][JOBS];

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

    for (int i = 0; i < LINES; i++)
    {
        cout << entryCosts[i] << " " << exitCosts[i] << endl;
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

        for (int i = 0; i < LINES; i++)
        {
            getline(inputString, tempStr, ',');
            processingCosts[i][job] = atoi(tempStr.c_str());
        }
    }

    cout << "PROCESSING" << endl;
    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j < JOBS; j++)
        {
            cout << processingCosts[i][j] << " ";
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

        transferCosts[linefrom][lineto][job] = cost;
    }

    cout << "TRANSFER" << endl;
    for (int i = 0; i < LINES; i++)
    {
        for (int j = 0; j < LINES; j++)
        {
            cout << " { ";
            for (int z = 0; z < JOBS; z++)
            {
                cout << transferCosts[i][j][z] << " ";
            }
            cout << "} " << endl;
        }
        cout << endl;
    }
}

// int(*stationCost())[JOBS]
//{
//	static int stations[LINES][JOBS];
//	for (int i = 0; i < LINES; i++)
//	{
//		for (int j = 0; j < JOBS; j++)
//		{
//			stations[i][j] = BASE_COST + processingCosts[i][j] + (FAILURE_PENALTY * FAILURE[i][j]);
//			cout << stations[i][j] << " ";
//		}
//		cout << endl;
//	}
//
//	return stations;
// }
//
// int fastestWay(int stations[][JOBS])
//{
//	int f[LINES][JOBS + 1];
//	int l[LINES][JOBS + 1];
//
//	f[0][JOBS] = 0;
//	f[1][JOBS] = 0;
//	f[2][JOBS] = 0;
//	f[3][JOBS] = 0;
//	l[0][JOBS] = 0;
//	l[1][JOBS] = 0;
//	l[2][JOBS] = 0;
//	l[3][JOBS] = 0;
//
//	for (int i = 0; i < LINES; i++)
//	{
//		f[i][0] = entryCosts[i] + stations[i][0];
//		l[i][0] = i + 1;
//	}
//
//	for (int j = 1; j < JOBS; j++)
//	{
//		if (f[0][j - 1] + stations[0][j] <=
//			min(f[1][j - 1] + TRANSFER_COST + stations[0][j],
//				min(f[2][j - 1] + TRANSFER_COST + stations[0][j],
//					f[3][j - 1] + TRANSFER_COST + stations[0][j])))
//		{
//			f[0][j] = f[0][j - 1] + stations[0][j];
//			l[0][j] = 1;
//		}
//		else if (f[1][j - 1] + TRANSFER_COST + stations[0][j] <=
//			min(f[2][j - 1] + TRANSFER_COST + stations[0][j],
//				f[3][j - 1] + TRANSFER_COST + stations[0][j]))
//		{
//			f[0][j] = f[1][j - 1] + TRANSFER_COST + stations[0][j];
//			l[0][j] = 2;
//		}
//		else if (f[2][j - 1] + TRANSFER_COST + stations[0][j] <=
//			f[3][j - 1] + TRANSFER_COST + stations[0][j])
//		{
//			f[0][j] = f[2][j - 1] + TRANSFER_COST + stations[0][j];
//			l[0][j] = 3;
//		}
//		else
//		{
//			f[0][j] = f[3][j - 1] + TRANSFER_COST + stations[0][j];
//			l[0][j] = 4;
//		}
//
//		if (f[1][j - 1] + stations[1][j] <=
//			min(f[0][j - 1] + TRANSFER_COST + stations[1][j],
//				min(f[2][j - 1] + TRANSFER_COST + stations[1][j],
//					f[3][j - 1] + TRANSFER_COST + stations[1][j])))
//		{
//			f[1][j] = f[1][j - 1] + stations[1][j];
//			l[1][j] = 2;
//		}
//		else if (f[0][j - 1] + TRANSFER_COST + stations[1][j] <=
//			min(f[2][j - 1] + TRANSFER_COST + stations[1][j],
//				f[3][j - 1] + TRANSFER_COST + stations[1][j]))
//		{
//			f[1][j] = f[0][j - 1] + TRANSFER_COST + stations[1][j];
//			l[1][j] = 1;
//		}
//		else if (f[2][j - 1] + TRANSFER_COST + stations[1][j] <=
//			f[3][j - 1] + TRANSFER_COST + stations[1][j])
//		{
//			f[1][j] = f[2][j - 1] + TRANSFER_COST + stations[1][j];
//			l[1][j] = 3;
//		}
//		else
//		{
//			f[1][j] = f[3][j - 1] + TRANSFER_COST + stations[1][j];
//			l[1][j] = 4;
//		}
//
//		if (f[2][j - 1] + stations[2][j] <=
//			min(f[0][j - 1] + TRANSFER_COST + stations[2][j],
//				min(f[1][j - 1] + TRANSFER_COST + stations[2][j],
//					f[3][j - 1] + TRANSFER_COST + stations[2][j])))
//		{
//			f[2][j] = f[2][j - 1] + stations[2][j];
//			l[2][j] = 3;
//		}
//		else if (f[0][j - 1] + TRANSFER_COST + stations[2][j] <=
//			min(f[1][j - 1] + TRANSFER_COST + stations[2][j],
//				f[3][j - 1] + TRANSFER_COST + stations[2][j]))
//		{
//			f[2][j] = f[0][j - 1] + TRANSFER_COST + stations[2][j];
//			l[2][j] = 1;
//		}
//		else if (f[1][j - 1] + TRANSFER_COST + stations[2][j] <=
//			f[3][j - 1] + TRANSFER_COST + stations[2][j])
//		{
//			f[2][j] = f[1][j - 1] + TRANSFER_COST + stations[2][j];
//			l[2][j] = 2;
//		}
//		else
//		{
//			f[2][j] = f[3][j - 1] + TRANSFER_COST + stations[2][j];
//			l[2][j] = 4;
//		}
//
//		if (f[3][j - 1] + stations[3][j] <=
//			min(f[0][j - 1] + TRANSFER_COST + stations[3][j],
//				min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
//					f[2][j - 1] + TRANSFER_COST + stations[3][j])))
//		{
//			f[3][j] = f[3][j - 1] + stations[3][j];
//			l[3][j] = 4;
//		}
//		else if (f[0][j - 1] + TRANSFER_COST + stations[3][j] <=
//			min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
//				f[2][j - 1] + TRANSFER_COST + stations[3][j]))
//		{
//			f[3][j] = f[0][j - 1] + TRANSFER_COST + stations[3][j];
//			l[3][j] = 1;
//		}
//		else if (f[1][j - 1] + TRANSFER_COST + stations[3][j] <=
//			f[2][j - 1] + TRANSFER_COST + stations[3][j])
//		{
//			f[3][j] = f[1][j - 1] + TRANSFER_COST + stations[3][j];
//			l[3][j] = 2;
//		}
//		else
//		{
//			f[3][j] = f[2][j - 1] + TRANSFER_COST + stations[3][j];
//			l[3][j] = 3;
//		}
//
//		if (f[3][j - 1] + stations[3][j] <=
//			min(f[0][j - 1] + TRANSFER_COST + stations[3][j],
//				min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
//					f[2][j - 1] + TRANSFER_COST + stations[3][j])))
//		{
//			f[3][j] = f[3][j - 1] + stations[3][j];
//			l[3][j] = 4;
//		}
//		else if (f[0][j - 1] + TRANSFER_COST + stations[3][j] <=
//			min(f[1][j - 1] + TRANSFER_COST + stations[3][j],
//				f[2][j - 1] + TRANSFER_COST + stations[3][j]))
//		{
//			f[3][j] = f[0][j - 1] + TRANSFER_COST + stations[3][j];
//			l[3][j] = 1;
//		}
//		else if (f[1][j - 1] + TRANSFER_COST + stations[3][j] <=
//			f[2][j - 1] + TRANSFER_COST + stations[3][j])
//		{
//			f[3][j] = f[1][j - 1] + TRANSFER_COST + stations[3][j];
//			l[3][j] = 2;
//		}
//		else
//		{
//			f[3][j] = f[2][j - 1] + TRANSFER_COST + stations[3][j];
//			l[3][j] = 3;
//		}
//	}
//
//	if (f[0][JOBS - 1] + exitCosts[0] <=
//		min(f[1][JOBS - 1] + exitCosts[1],
//			min(f[2][JOBS - 1] + exitCosts[2],
//				f[3][JOBS - 1] + exitCosts[3])))
//	{
//		f[0][JOBS] = f[0][JOBS - 1] + exitCosts[0];
//		l[0][JOBS] = 1;
//	}
//	else if (f[1][JOBS - 1] + exitCosts[1] <=
//		min(f[2][JOBS - 1] + exitCosts[2],
//			f[3][JOBS - 1] + exitCosts[3]))
//	{
//		f[1][JOBS] = f[1][JOBS - 1] + exitCosts[1];
//		l[1][JOBS] = 2;
//	}
//	else if (f[2][JOBS - 1] + exitCosts[2] <=
//		f[3][JOBS - 1] + exitCosts[3])
//	{
//		f[2][JOBS] = f[2][JOBS - 1] + exitCosts[2];
//		l[2][JOBS] = 3;
//	}
//	else
//	{
//		f[3][JOBS] = f[3][JOBS - 1] + exitCosts[3];
//		l[3][JOBS] = 4;
//	}
//
//	for (int i = 0; i < LINES; i++)
//	{
//		for (int j = 0; j <= JOBS; j++)
//		{
//			cout << f[i][j] << "\t";
//		}
//		cout << endl;
//	}
//	cout << endl;
//	for (int i = 0; i < LINES; i++)
//	{
//		for (int j = 0; j <= JOBS; j++)
//		{
//			cout << l[i][j] << " ";
//		}
//		cout << endl;
//	}
//
//	return 0;
// }

int main()
{

    /*int(*stations)[JOBS] = stationCost();
    cout << stations << " " << *stations << endl;

    fastestWay(stations);*/

    // csv_parsing();
    getEntryExitCosts();
    getProcessingCosts();
    getTransferCosts();

    cout << "hello";

    return 0;
}
