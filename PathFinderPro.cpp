#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <climits>
#include<algorithm>
#include<cctype>
#include<set>
#include<stack>
#define INF INT_MAX
using namespace std;

// Number of cities
const int NUM_CITIES = 16;
int getCityIndex(char city) {
    city = toupper(city); 
    if (city >= 'A' && city <= 'P') {
        return city - 'A';
    }
    return -1; // Invalid input
}

// Non-negative adjacency matrix for Dijkstra's Algorithm
int adjMatrixDijkstra[NUM_CITIES][NUM_CITIES] = {
             //A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P
			  {0, 5, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//A
		      {5, 0, 3, 0, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//B
		      {0, 3, 0, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//C
		      {0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0},//D
		      {6, 5, 0, 0, 0, 3, 0, 4, 3, 5, 0, 0, 0, 0, 0, 0},//E
		      {0, 3, 8, 0, 3, 0, 5, 0, 0, 3, 0, 0, 0, 0, 0, 0},//F
		      {0, 0, 0, 6, 0, 5, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0},//G
		      {0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0},//H
		      {0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 3, 5, 0, 0, 0},//I
		      {0, 0, 0, 0, 5, 3, 0, 0, 4, 0, 4, 0, 5, 4, 6, 0},//J
		      {0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3, 4, 5},//K
		      {0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 2, 0, 0, 0},//L
		      {0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 2, 0, 2, 0, 0},//M
		      {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 0, 2, 0, 0, 0},//N
		      {0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 0, 0, 0, 2},//O
		      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 2, 0}//P
};

// Adjacency matrix representing the graph (distances between cities)
int adjMatrix[NUM_CITIES][NUM_CITIES] = {
           //A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P
            {0, 5, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//A
            {5, 0, 3, 0, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0},//B
            {0, 3, 0, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//C
            {0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0},//D
            {6, 5, 0, 0, 0, 3, 0, 4, 3, 5, 0, 0, 0, 0, 0, 0},//E
            {0, 3, 8, 0, 3, 0, 5, 0, 0, 3, 0, 0, 0, 0, 0, 0},//F
            {0, 0, 0, 6, 0, 5, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0},//G
            {0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0},//H
            {0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 3, 5, 0, 15, 0},//I
            {0, 0, 0, 0, 5, 3, 0, 0, 4, 0, 4, 0, 5, 4, 6, 0},//J
            {0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3, 4, 5},//K
            {0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 2, 0, 0, 0},//L
            {0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 2, 0, 2, 0, 0},//M
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 0, 2, 0, 0, 0},//N
            {0, 17, 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 0, 0, 0, 2},//O
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 2, 0}//P
};

// Negative-weight adjacency matrix for Bellman-Ford Algorithm
int adjMatrixBellmanFord[NUM_CITIES][NUM_CITIES] = {
              //A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P
               {0, 5, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//A
               {5, 0, -3, 0, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//B
               {0, 3, 0, -6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//C
               {0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0},//D
               {6, 5, 0, 0, 0, 0, 0, 4, 3, 5, 0, 0, 0, 0, 0, 0},//E
               {0, 3, 8, 0, 3, 0, -5, 0, 0, 3, 0, 0, 0, 0, 0, 0},//F
               {0, 0, 0, 6, 0, 5, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0},//G
               {0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, -3, 0, 0, 0, 0},//H
               {0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 3, 5, 0, 0, 0},//I
               {0, 0, 0, 0, 5, 3, 0, 0, 4, 0, 4, 0, 5, -4, 6, 0},//J
               {0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3, 4, 5},//K
               {0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 2, 0, 0, 0},//L
               {0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 2, 0, 2, 0, 0},//M
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 0, 2, 0, 0, 0},//N
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 0, 0, 0, 2},//O
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, -2, 0}//P
};

// Function prototypes
void welcome_screen() {
     // Background color
    setbkcolor(CYAN);
    cleardevice();

    // Title Text
    setcolor(WHITE);
    settextstyle(3, 0, 5); // Stylish font
    outtextxy(70, 30, "******** WELCOME ********");

    // Subtitle
    setcolor(RED);
    settextstyle(4, 0, 4);
    outtextxy(80, 100, "Route Optimization");

    setcolor(RED);
    settextstyle(3, 0, 3);
    outtextxy(120, 150, "Using C++ and Graphics");

    // Divider line
    setcolor(MAGENTA);
    line(50, 200, getmaxx() - 50, 200);

    // Creator Info
    setcolor(WHITE);
    settextstyle(2, 0, 5);
    outtextxy(20, 230, "CREATED BY:");

    // Highlighted Names
    setcolor(YELLOW);
    settextstyle(3, 0, 5); // Bigger font for names
    outtextxy(80, 280, "Abhishek Sharma");
    outtextxy(80, 350, "Deepanshu Kumar Jindal");

    // Footer message
    setcolor(RED);
    settextstyle(1, 0, 2);
    outtextxy(80, 420, "Press any key to continue...");
    getch();
}

void city_map() {
    cleardevice();
    setbkcolor(YELLOW);
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(260, 20, "CITY-MAP");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(20, 40, "A");
    outtextxy(220, 40, "B");
    outtextxy(420, 40, "C");
    outtextxy(600, 40, "D");
    outtextxy(100, 170, "E");
    outtextxy(300, 170, "F");
    outtextxy(500, 170, "G");
    outtextxy(20, 310, "H");
    outtextxy(220, 310, "I");
    outtextxy(420, 310, "J");
    outtextxy(600, 310, "K");
    outtextxy(30, 450, "L");
    outtextxy(180, 450, "M");
    outtextxy(320, 450, "N");
    outtextxy(460, 450, "O");
    outtextxy(610, 450, "P");

    // Draw city connections (roads)
    line(30, 50, 610, 50);
    line(110, 180, 510, 180);
    line(30, 50, 110, 180);
    line(230, 50, 110, 180);
    line(230, 50, 310, 180);
    line(430, 50, 310, 180);
    line(610, 50, 510, 180);
    line(230, 320, 610, 320);
    line(110, 180, 30, 320);
    line(110, 180, 230, 320);
    line(110, 180, 430, 320);
    line(310, 180, 430, 320);
    line(510, 180, 610, 320);
    line(40, 460, 330, 460);
    line(190, 460, 430, 320);
    line(30, 320, 40, 460);
    line(230, 320, 40, 460);
    line(230, 320, 190, 460);
    line(430, 320, 330, 460);
    line(610, 320, 330, 460);
    line(610, 320, 470, 460);
    line(430, 320, 470, 460);
    line(470, 460, 620, 460);
    line(600, 310, 610, 450);

    // Draw circles representing cities
    circle(30, 50, 20);
    circle(230, 50, 20);
    circle(430, 50, 20);
    circle(610, 50, 20);
    circle(110, 180, 20);
    circle(310, 180, 20);
    circle(510, 180, 20);
    circle(30, 320, 20);
    circle(230, 320, 20);
    circle(430, 320, 20);
    circle(610, 320, 20);
    circle(40, 460, 20);
    circle(190, 460, 20);
    circle(330, 460, 20);
    circle(470, 460, 20);
    circle(620, 460, 20);
}
void dijkstra(int src, int dest) {
    vector<int> dist(NUM_CITIES, INT_MAX);  // Distance array initialized to infinity
    vector<int> parent(NUM_CITIES, -1);     // Parent array to reconstruct the path
    set<pair<int, int>> activeCities;       // {distance, city}

    dist[src] = 0;  // Distance to the source city is 0
    activeCities.insert({0, src});  // Add the source city to the set

    while (!activeCities.empty()) {
        int u = activeCities.begin()->second;  // Get the city with the smallest distance
        activeCities.erase(activeCities.begin());  // Remove this city from the set

        // Explore all neighboring cities (adjacent cities)
        for (int v = 0; v < NUM_CITIES; v++) {
            // If there's an edge from u to v and the distance can be improved
            if (adjMatrixDijkstra[u][v] && dist[u] + adjMatrixDijkstra[u][v] < dist[v]) {
                activeCities.erase({dist[v], v});  // Remove the old distance if present
                dist[v] = dist[u] + adjMatrixDijkstra[u][v];  // Update the distance
                parent[v] = u;  // Set u as the parent of v
                activeCities.insert({dist[v], v});  // Insert the new distance to the set
            }
        }
    }

    // Print the shortest distance and path
    cout << "Shortest distance from city " << char(src + 'A') << " to city " << char(dest + 'A') << " is " << dist[dest] << endl;
    cout << "Path: ";
    
    vector<int> path;
    // Reconstruct the path using the parent array
    for (int v = dest; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());  // Reverse the path to get the correct order

    for (int i = 0; i < path.size(); i++) {
        cout << char(path[i] + 'A') << (i == path.size() - 1 ? "" : " -> ");
    }
    cout << endl;
}

void bellmanFord(int src, int dest) {
    vector<int> dist(NUM_CITIES, INT_MAX); // Distance array
    vector<int> parent(NUM_CITIES, -1);   // Parent array to reconstruct the path
    dist[src] = 0;

    // Relax edges |V| - 1 times
    for (int i = 0; i < NUM_CITIES - 1; i++) {
        for (int u = 0; u < NUM_CITIES; u++) {
            for (int v = 0; v < NUM_CITIES; v++) {
                if (adjMatrixBellmanFord[u][v] != 0 && dist[u] != INT_MAX && dist[u] + adjMatrixBellmanFord[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrixBellmanFord[u][v];
                    parent[v] = u; // Update the parent of v
                }
            }
        }
    }

    // Verify for negative weight cycles
    bool negativeCycle = false;
    for (int u = 0; u < NUM_CITIES; u++) {
        for (int v = 0; v < NUM_CITIES; v++) {
            if (adjMatrixBellmanFord[u][v] != 0 && dist[u] != INT_MAX &&
                dist[u] + adjMatrixBellmanFord[u][v] < dist[v]) {
                negativeCycle = true;
                break;
            }
        }
        if (negativeCycle) break;
    }

    if (negativeCycle) {
        cout << "Graph contains a negative weight cycle. Shortest path cannot be determined." << endl;
        return;
    }

    // Output the shortest distance
    if (dist[dest] == INT_MAX) {
        cout << "No path exists from city " << char(src + 'A') << " to city " << char(dest + 'A') << "." << endl;
        return;
    }

    cout << "Bellman-Ford Shortest Distance from city " << char(src + 'A') 
         << " to city " << char(dest + 'A') << " is: " << dist[dest] << endl;

    // Reconstruct the path
    vector<int> path;
    for (int v = dest; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    // Print the path
    cout << "Path: ";
    for (int i = 0; i < path.size(); i++) {
        cout << char(path[i] + 'A');
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;
}

int dp[NUM_CITIES][1 << NUM_CITIES];

// TSP dynamic programming function
int tsp(int city, int visitedMask, int endCity) {
if (visitedMask == (1 << NUM_CITIES) - 1) {
    // If there is no direct path (adjMatrix[city][endCity] == 0), return INF
    // Otherwise, return the cost of the direct path
    return (adjMatrix[city][endCity] == 0 && city != endCity) ? INF : adjMatrix[city][endCity];
}

    if (dp[city][visitedMask] != -1) {
        return dp[city][visitedMask];
    }

    int minCost = INF;

    for (int nextCity = 0; nextCity < NUM_CITIES; nextCity++) {
        if (!(visitedMask & (1 << nextCity)) && adjMatrix[city][nextCity] != 0) {
            int costToNextCity = adjMatrix[city][nextCity];
            int newCost = tsp(nextCity, visitedMask | (1 << nextCity), endCity);

            // Avoid integer overflow by skipping paths with INF
            if (newCost < INF && costToNextCity < INF) {
                minCost = min(minCost, costToNextCity + newCost);
            }
        }
    }

    return dp[city][visitedMask] = minCost;
}

// Function to reconstruct the path
void reconstructPath(int city, int visitedMask, int endCity) {
    cout << " " << (char)(city + 'A');
    if (visitedMask == (1 << NUM_CITIES) - 1) {
        cout << " -> " << (char)(endCity + 'A');
        return;
    }

    int nextCity = -1;
    int minCost = INF;

    for (int i = 0; i < NUM_CITIES; i++) {
        if (!(visitedMask & (1 << i)) && adjMatrix[city][i] != 0) {
            int costToNextCity = adjMatrix[city][i];
            int currentCost = tsp(i, visitedMask | (1 << i), endCity);

            if (currentCost < INF && costToNextCity < INF && costToNextCity + currentCost < minCost) {
                minCost = costToNextCity + currentCost;
                nextCity = i;
            }
        }
    }

    cout << " -> " << (char)(nextCity + 'A') << " (Cost: " << adjMatrix[city][nextCity] << ")";
    reconstructPath(nextCity, visitedMask | (1 << nextCity), endCity);
}

int main() {
    // Initialize graphics
    int gdriver = DETECT, gmode;
    initgraph(&gdriver, &gmode, "");

    if (graphresult() != 0) {
        cout << "Graphics initialization failed!" << endl;
        return 1;
    }

    // Show welcome screen and city map
    welcome_screen();
    city_map();

    char continueRide;
    
    do {
        int choice;
        cout << "\nChoose an option:\n";
        cout << "1. Minimum Distance between two cities (Dijkstra's and Bellman-Ford)\n";
        cout << "2. Visit all cities(Travelling Salesman Algorithm using Dynamic Programming)\n";
        cout << "Enter your choice (1 or 2): ";
        cin >> choice;

        if (choice == 1) {
            // Minimum Distance: Run Dijkstra's and Bellman-Ford algorithms
            char sourceChar, destinationChar;
            int source, destination;

            // Input source and destination
            cout << "\nEnter your source city (A-P): ";
            cin >> sourceChar;
            source = getCityIndex(sourceChar);
            if (source == -1) {
                cout << "Invalid source city!" << endl;
                getch();
                closegraph();
                return 1;
            }

            cout << "\nEnter your destination city (A-P): ";
            cin >> destinationChar;
            destination = getCityIndex(destinationChar);
            if (destination == -1) {
                cout << "Invalid destination city!" << endl;
                getch();
                closegraph();
                return 1;
            }

            cout << "\nRunning Dijkstra's Algorithm..." << endl;
            dijkstra(source, destination);

            cout << "\nRunning Bellman-Ford Algorithm..." << endl;
            bellmanFord(source, destination);

        } else if (choice == 2) {
            memset(dp, -1, sizeof(dp));

            char sourceCity, destCity;
            cout << "Enter the source city (A to P): ";
            cin >> sourceCity;
            cout << "Enter the destination city (A to P): ";
            cin >> destCity;

            int startCity = sourceCity - 'A';
            int endCity = destCity - 'A';

            int totalCost = tsp(startCity, 1 << startCity, endCity);

            if (totalCost >= INF) {
                cout << "\nNo valid path exists between " << sourceCity << " and " << destCity << "." << endl;
            } else {
                cout << "\nMinimum Cost: " << totalCost << endl;
                cout << "Path:";
                reconstructPath(startCity, 1 << startCity, endCity);
                cout << endl;
            }
        }

        // Ask the user if they want another ride
        cout << "\nDo you want another ride? (Y/N): ";
        cin >> continueRide;
        
    } 
    while(continueRide == 'Y' || continueRide == 'y');

    // Wait for user to close graphics window
    cout << "\nPress any key to exit the graphics screen." << endl;
    getch();
    closegraph();
    return 0;
}