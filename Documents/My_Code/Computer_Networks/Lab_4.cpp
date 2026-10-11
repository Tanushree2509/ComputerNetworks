#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

const int INF = 9999;

// Function to display the network cost matrix
void displayCostMatrix(const vector<vector<int>>& cost, int n)
{
    cout << "\n========== INITIAL NETWORK COST MATRIX ==========\n";

    cout << setw(10) << " ";

    for (int i = 0; i < n; i++)
    {
        cout << setw(10) << i;
    }

    cout << endl;

    for (int i = 0; i < n; i++)
    {
        cout << setw(10) << i;

        for (int j = 0; j < n; j++)
        {
            if (cost[i][j] == INF)
                cout << setw(10) << "INF";
            else
                cout << setw(10) << cost[i][j];
        }

        cout << endl;
    }
}

// Function to display routing tables
void displayRoutingTables(
    const vector<vector<int>>& distance,
    const vector<vector<int>>& nextHop,
    int n,
    string title)
{
    cout << "\n========== " << title << " ==========\n";

    for (int source = 0; source < n; source++)
    {
        cout << "\nRouting Table for Router " << source << endl;

        cout << setw(15) << "Destination"
             << setw(15) << "Cost"
             << setw(15) << "Next Hop" << endl;

        cout << "---------------------------------------------\n";

        for (int destination = 0; destination < n; destination++)
        {
            cout << setw(15) << destination;

            if (distance[source][destination] == INF)
                cout << setw(15) << "INF";
            else
                cout << setw(15) << distance[source][destination];

            if (nextHop[source][destination] == -1)
                cout << setw(15) << "-";
            else
                cout << setw(15) << nextHop[source][destination];

            cout << endl;
        }
    }
}

// Function to display shortest path and minimum cost
void displayShortestPath(
    const vector<vector<int>>& distance,
    const vector<vector<int>>& nextHop,
    int source,
    int destination)
{
    cout << "\n========== SHORTEST PATH ==========\n";

    if (distance[source][destination] == INF)
    {
        cout << "No path exists between Router "
             << source << " and Router "
             << destination << endl;

        return;
    }

    cout << "Source Router      : " << source << endl;
    cout << "Destination Router : " << destination << endl;
    cout << "Minimum Cost       : "
         << distance[source][destination] << endl;

    cout << "Shortest Path      : ";

    int current = source;

    cout << current;

    while (current != destination)
    {
        current = nextHop[current][destination];

        if (current == -1)
        {
            cout << "\nPath cannot be determined.";
            return;
        }

        cout << " -> " << current;
    }

    cout << endl;
}

int main()
{
    int N;

    cout << "=============================================\n";
    cout << "       DISTANCE VECTOR ROUTING ALGORITHM\n";
    cout << "=============================================\n";

    cout << "\nEnter number of routers: ";
    cin >> N;

    vector<vector<int>> cost(N, vector<int>(N));
    vector<vector<int>> distance(N, vector<int>(N));
    vector<vector<int>> nextHop(N, vector<int>(N));

    cout << "\nEnter the network cost matrix.\n";
    cout << "Enter " << INF
         << " for INF (no direct connection).\n\n";

    // Input cost matrix
    for (int i = 0; i < N; i++)
    {
        cout << "Enter costs for Router " << i << ": ";

        for (int j = 0; j < N; j++)
        {
            cin >> cost[i][j];

            distance[i][j] = cost[i][j];

            // Distance from router to itself is always 0
            if (i == j)
            {
                distance[i][j] = 0;
                nextHop[i][j] = i;
            }

            // Direct connection exists
            else if (cost[i][j] != INF)
            {
                nextHop[i][j] = j;
            }

            // No direct connection
            else
            {
                nextHop[i][j] = -1;
            }
        }
    }

    // Display initial cost matrix
    displayCostMatrix(cost, N);

    // Display initial routing tables
    displayRoutingTables(
        distance,
        nextHop,
        N,
        "INITIAL ROUTING TABLES"
    );

    // Distance Vector Routing Algorithm
    bool updated;

    do
    {
        updated = false;

        for (int source = 0; source < N; source++)
        {
            for (int destination = 0;
                 destination < N;
                 destination++)
            {
                for (int neighbour = 0;
                     neighbour < N;
                     neighbour++)
                {
                    // Check if source is directly connected
                    // to neighbour and neighbour has a path
                    // to destination
                    if (cost[source][neighbour] != INF &&
                        distance[neighbour][destination] != INF)
                    {
                        int newCost =
                            cost[source][neighbour] +
                            distance[neighbour][destination];

                        // Update if a shorter path is found
                        if (newCost < distance[source][destination])
                        {
                            distance[source][destination] =
                                newCost;

                            nextHop[source][destination] =
                                neighbour;

                            updated = true;
                        }
                    }
                }
            }
        }

    } while (updated);

    // Display final routing tables
    displayRoutingTables(
        distance,
        nextHop,
        N,
        "FINAL ROUTING TABLES"
    );

    // Ask user for source and destination
    int selectedSource;
    int selectedDestination;

    cout << "\nEnter source router: ";
    cin >> selectedSource;

    cout << "Enter destination router: ";
    cin >> selectedDestination;

    // Display shortest path
    displayShortestPath(
        distance,
        nextHop,
        selectedSource,
        selectedDestination
    );

    cout << "\n=============================================\n";
    cout << "          PROGRAM EXECUTION COMPLETE\n";
    cout << "=============================================\n";

    return 0;
}