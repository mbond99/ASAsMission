// AStarExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//This game is strategy game exploring an implementation of the A* algorithm

#include <iostream>
#include <vector>
using namespace std;

//types of land avaliable in game and time needed to travel across
enum landType { grass = 1, forest = 5, water = 15, lava = 25 };

//the land square struct represents each area on the grid 
struct landSquare {
    int land_x;
    int land_y;
    int parent_x;
    int parent_y;
    bool isGoal;
    int heuristic;
    landType type; //will be used to calculate g
    int g; //type value + parent g
    int f; //sum of heruistic and movement cost
    
    //default constructor
    landSquare() {
        land_x = 0;
        land_y = 0;
        parent_x = 0;
        parent_y = 0;
        isGoal = false;
        heuristic = 0;
        type = grass;
        g = 0;
        f = 0;
    }

    //constructor
    landSquare(int x, int y, int px, int py, bool goal, int h, landType t, int gValue, int fValue) {
        land_x = x;
        land_y = y;
        parent_x = px;
        parent_y = py;
        isGoal = goal;
        heuristic = h;
        type = t;
        g = gValue;
        f = fValue;
    }
};

//function declarations
int calculatef(landSquare);
landSquare findCurrentLand(vector<landSquare>);
bool checkGoal(landSquare);
landSquare addLand(landSquare, int grid[][5], int);
int calculateh(int, int);
bool checkClosedList(vector<landSquare>, int, int);
bool checkOpenList(vector<landSquare>, int, int);
void compareGs(landSquare, landSquare, vector<landSquare> &open);
void reviewPath(vector<landSquare>, int, int, int, int);
int applyDamage(int, landSquare);




int main()
{
    int grid[5][5];
    int gameMode;
    int ASATimeLimit = 90;
    int ASADamage = 0;
    char userInput;
    bool validUserInput = false;
    bool tryAgain = true;
    bool notValid = true;
    bool goalFound = false;
    bool inClosed = false;
    bool inOpen = false;
    vector<landSquare> openLands;
    vector<landSquare> closedLands;
    vector<landSquare>::iterator it;
    landSquare neighbor;
    cout << "Welcome to ASA Mission\n\n";
    cout << "You have been tasked with a mission to choose the area to release a robot delivering top secret information to your alies across enemy land.\n";
    cout << "The robot's name is ASA. ASA has been programmed to find the optimal path between the starting point and the destination using the A* algorithm.\n";
    cout << "ASA will be following a grid system like the one below with S as the starting location and D as the destination\n";
    cout << "S _ _ _ _\n";
    cout << "_ _ _ _ _\n";
    cout << "_ _ _ _ _\n";
    cout << "_ _ _ _ _\n";
    cout << "_ _ _ _ D\n\n";
    cout << "ASA is able to travel over 4 types of terrain: grass, forest, water, and lava. However each type of terrain takes a different amount of time to navigate.\n";
    cout << "The amount of time required to travel through each terrain square of the grid is as follows:\n";
    cout << "grass = 1 minute\nforest = 5 minutes\nwater = 15 minutes\nlava = 25 minutes\n\n";
    cout << "ASA's program will find the quickest route through the terrain grid you choose; however since ASA will be traveling through enemy territory you must make sure ASA is not destroyed while traveling.\n";
    cout << "Each territory type has a percentage chance of ASA being hit by enemy fire. The percentages for each territory are as follows:\n";
    cout << "grass = 80%\nforest = 70%\nwater = 30%\nlava = 5%\n\n";
    cout << "ASA's battery will last for 90 minutes and her defense can take 5 hits before being destroyed.\n";
    cout << "You must choose the best combination of terrain tiles for the grid to have ASA arrive safely and before her battery dies!\n\n";


    //have user build grid
    while (tryAgain) {
        cout << "Select the terrain you would like for each square of the grid.\nBoth the start and destinatiion will be grass.\nASA is not able to be hit by enemy fire on the start and destination squares.\n\n";
        cout << "The grid spots are as follows:\n";
        cout << " 1   2   3   4   5\n";
        cout << " 6   7   8   9  10\n";
        cout << "11  12  13  14  15\n";
        cout << "16  17  18  19  20\n";
        cout << "21  22  23  24  25\n\n";

        cout << "____Terrain List ____\n";
        cout << "  1. Grass\n  2. Forest\n  3.Water\n  4. Lava\n\n";
        int spaceNum = 1;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int terrain = 0;
                notValid = true;
                if (spaceNum == 1 || spaceNum == 25) {
                    grid[i][j] = 1;
                }
                else {
                    while (notValid) {
                        cout << "Enter the number of the terrain you would like for spot " << spaceNum << endl;
                        cin >> terrain;
                        if (terrain >= 1 && terrain <= 4) {
                            grid[i][j] = terrain;
                            notValid = false;
                        }
                        else {
                            cout << "Input not recognized. You must enter the number beside the terrain you would like to assign(1-4).\n\n";
                            notValid = true;
                        }
                    }
                }
                spaceNum++;
            }
        }
        //display grid to user
        cout << "\n\nThis is the map you have created for ASA!\nG = grass\nF = forest\nW = water\nL = lava\n\n";
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int terrainType = 0;
                terrainType = grid[i][j];
                switch (terrainType) {
                case 1:
                    cout << "G  ";
                    break;
                case 2:
                    cout << "F  ";
                    break;
                case 3:
                    cout << "W  ";
                    break;
                case 4:
                    cout << "L  ";
                    break;
                }
            }
            cout << endl;
        }
        cout << "\n\nThis is a representation of the movement cost for ASA to travel over each square:\n\n";
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int terrainType;
                terrainType = grid[i][j];
                switch (terrainType) {
                case 1:
                    cout << "1  ";
                    break;
                case 2:
                    cout << "5  ";
                    break;
                case 3:
                    cout << "15 ";
                    break;
                case 4:
                    cout << "30 ";
                    break;
                }
            }
            cout << endl<<endl;
        }
        bool validUserInput = false;
        while (!validUserInput) {
            cout << "Would you like to use this map? Enter y to continue with this map. Enter n to create another map.";
            cin >> userInput;
            if (userInput == 'y' || userInput == 'Y' || userInput == 'n' || userInput == 'N') {
                validUserInput = true;
                if (userInput == 'y' || userInput == 'Y') {
                    tryAgain = false;
                    cout << "Very good! ASA's journey will be beginning shortly...\n\n\n";
                    cout << "These are the map coordinates:\n";
                    cout << "(0,0) (0,1) (0,2) (0,3) (0,4)\n";
                    cout << "(1,0) (1,1) (1,2) (1,3) (1,4)\n";
                    cout << "(2,0) (2,1) (2,2) (2,3) (2,4)\n";
                    cout << "(3,0) (3,1) (3,2) (3,3) (3,4)\n";
                    cout << "(4,0) (4,1) (4,2) (4,3) (4,4)\n\n";
                }
                else {
                    tryAgain = true;
                }
            }
            else {
                cout << "Input not recognized. You must enter y or n to continue.";
                validUserInput = false;
            }
        }

    }


    //initialize open list with starting square probably make this a function
    landSquare currentLand = landSquare(0, 0, 0, 0, false, 0, grass, 1, 0);
    currentLand.heuristic = calculateh(currentLand.land_x, currentLand.land_y);
    currentLand.f = calculatef(currentLand);
    openLands.push_back(currentLand);

    //this loop will execute as long as the open set is not empty; if the open set becomes empty and the solution has not been found the solution does not exist.
    //in this game a solution will always exist however the robot may be eliminated or run out of time before that solution can be found resulting in game over
    while (!openLands.empty()) {
        int lowestf = 0;
        int pos = 0;
        landSquare currentLand;
        currentLand = openLands.front();
        lowestf = currentLand.f;  //set the lowest f to the first land in openLands becaue this is currently the lowest found f
        //iterating through openLands searching for lowestf value
        for (int i = 0; i < openLands.size(); i++) {
            currentLand = openLands.at(i);
            //if a lower f value is found in a land this becomes the new lowestf value; the position of this land in the openLands vector is stored in pos
            if (currentLand.f < lowestf) {
                lowestf = currentLand.f;
                pos = i;
            }
        }
        //set the currentLand to the land found to have the lowestf above using its position in the openLands vector
        currentLand = openLands.at(pos);
        it = openLands.begin() + pos;
        openLands.erase(it); //remove this land from the openLands vector it is now the q node in the A* algorithm
        
        //check to see if the currentLand is the goal; this is a bool value assigned to every landSquare
        if (currentLand.isGoal) {
            int pathTotal = 0;
            reviewPath(closedLands, currentLand.parent_x, currentLand.parent_y, ASADamage, ASATimeLimit);
            return 0; //end of program when goal is found
        }

       //the goal has not been found yet
       else {
            closedLands.push_back(currentLand); //the q land has been removed from the openLands vector and is now added to the closedLands vector

            //the following set of if statements are used to discover the neighboring lands to the q land
            //as long as the q land is not on the top edge of the grid explore north neighbor
            if (currentLand.land_x != 0) {
                inClosed = checkClosedList(closedLands, currentLand.land_x - 1, currentLand.land_y); //check to see if the north neighbor is in closedLands vector
                inOpen = checkOpenList(openLands, currentLand.land_x - 1, currentLand.land_y); //check to see if the north neighbor is alread in openLands vector
                //if the neighbor is in the closed list do not explore further
                if (!inClosed) {
                    neighbor = addLand(currentLand, grid, 1); //create a new landSquare structure for the neighbor, 1 represents north neighbor
                    //if the neighbor is already in the open list check to see if newly discovered neighbor has lower g value meaning a more efficient path to this neighbor
                    if (inOpen) {
                        compareGs(neighbor, currentLand, openLands); 
                    }
                    //if the neighbor makes it this far add to the openLands vector
                    else {
                        openLands.push_back(neighbor);
                    }
                }
             }
            //as long as the q land is not on the right edge of the grid explore east neighbor
            if (currentLand.land_y != 4) {
                inClosed = checkClosedList(closedLands, currentLand.land_x, currentLand.land_y + 1);
                if (!inClosed) {
                    neighbor = addLand(currentLand, grid, 2); //2 represents east neighbor
                    if (inOpen) {
                        compareGs(neighbor, currentLand, openLands);
                    }
                    else {
                        openLands.push_back(neighbor);
                    }
                }
            }
            //as long as the q land is not on the bottom edge of the grid explore south neighbor
            if (currentLand.land_x != 4) {
                inClosed = checkClosedList(closedLands, currentLand.land_x + 1, currentLand.land_y);
                if (!inClosed) {
                    neighbor = addLand(currentLand, grid, 3); //3 represents south neighbor
                    if (inOpen) {
                        compareGs(neighbor, currentLand, openLands);
                    }
                    else {
                        openLands.push_back(neighbor);
                        }
                }
            }
            //as long as the q land is not on the left edge of the grid explore west neighbor
            if (currentLand.land_y != 0) {
                inClosed = checkClosedList(closedLands, currentLand.land_x, currentLand.land_y - 1);
                if (!inClosed) {
                    neighbor = addLand(currentLand, grid, 4); //4 represents west neighbor
                    if (inOpen) {
                        compareGs(neighbor, currentLand, openLands);
                    }
                    else {
                        openLands.push_back(neighbor);
                        }
                }
            }
        }

    }
}

int calculatef(landSquare land) {
    int f;
    f = land.heuristic + land.g;
    return f;
}

//1 = north, 2 = east, 3 = south, 4 = west
landSquare addLand(landSquare current, int grid[][5], int dir) {
    landSquare newLand;
    int type;

    switch (dir) {
        //north neighbor x-1
    case 1:
        newLand.land_x = current.land_x - 1;
        newLand.land_y = current.land_y;
        break;
        //east neighbor y+1
    case 2:
        newLand.land_x = current.land_x;
        newLand.land_y = current.land_y + 1;
        break;
        //south neighbor x+1
    case 3:
        newLand.land_x = current.land_x + 1;
        newLand.land_y = current.land_y;
        break;
        //west neighbor y-1
    case 4:
        newLand.land_x = current.land_x;
        newLand.land_y = current.land_y - 1;
        break;
    }

    newLand.parent_x = current.land_x;
    newLand.parent_y = current.land_y;
    if ((newLand.land_x == 4) && (newLand.land_y == 4)) {
        newLand.isGoal = true;
    }
    else {
        newLand.isGoal = false;
    }
    type = grid[newLand.land_x][newLand.land_y];
    switch (type) {
    case 1:
        newLand.type = grass;
        break;
    case 2:
        newLand.type = forest;
        break;
    case 3:
        newLand.type = water;
        break;
    case 4:
        newLand.type = lava;
        break;
    }
    newLand.heuristic = calculateh(newLand.land_x, newLand.land_y);
    newLand.g = current.g + newLand.type;
    newLand.f = calculatef(newLand);


    return newLand;
}

int calculateh(int xPos, int yPos) {
    int h = 0;
    for (int i = xPos; i < 4; i++) {
        h++;
    }
    for (int i = yPos; i < 4; i++) {
        h++;
    }
    return h;
}

bool checkClosedList(vector<landSquare> closed, int newX, int newY) {
    bool isClosed = false;
    for (int i = 0; i < closed.size(); i++) {
        int x, y;
        x = closed[i].land_x;
        y = closed[i].land_y;
        if ((x == newX) && (y == newY)) {
            isClosed = true;
        }
    }
    return isClosed;
}

bool checkOpenList(vector<landSquare> open, int newX, int newY) {
    bool isOpen = false;
    for (int i = 0; i < open.size(); i++) {
        int x, y;
        x = open[i].land_x;
        y = open[i].land_y;
        if ((x == newX) && (y == newY)) {
            isOpen = true;
        }
        return isOpen;
    }
}

void compareGs(landSquare newSquare, landSquare oldSquare, vector<landSquare> &open) {
    if (newSquare.g < oldSquare.g) {
        //remove oldSquare and add new square
        int x, y;
        x = oldSquare.land_x;
        y = oldSquare.land_y;
        for (int i = 0; i < open.size(); i++) {
            if ((open[i].land_x == x) && (open[i].land_y == y)){
                //open.erase[i];
                open.erase(open.begin() + i);
                open.push_back(newSquare);
            }
        }
    }
}

void reviewPath(vector<landSquare> closed, int x, int y, int damage, int timeLimit) {
    int start_x = 0;
    int start_y = 0;
    int pathTotal = 0;
    bool startFound = false;
    bool done = false;
    vector<landSquare> finalList;
    int pos = 0;
    while (!startFound) {
        for (int i = 0; i < closed.size(); i++) {
            if ((closed[i].land_x == x) && (closed[i].land_y == y)) {
                pathTotal++;
                x = closed[i].parent_x;
                y = closed[i].parent_y;
                finalList.push_back(closed[i]);
                pos++;
                if (x == start_x && y == start_y) {
                    startFound = true;
                }
            }
        }
    }
    cout << "ASA's travel coordinates:\n (0, 0) \n";
    for (int i = finalList.size(); i > 0; i--) {
        //determine if ASA is hit by enemy fire when crossing currentLand
        damage = applyDamage(damage, finalList[i-1]);
        if (done) {
            //do nothing
        }
        else if (finalList[i-1].g > timeLimit) {
            cout << "\nGame Over!\nASA's battery died at position (" << finalList[i - 1].land_x << ", " << finalList[i - 1].land_y << ")\n";
            done = true;
        }
        else if (damage == 6) {
            cout << "\nGame Over!\nASA was destroyed by enemies at position (" << finalList[i - 1].land_x << ", " << finalList[i - 1].land_y << ")\n";
            done = true;
        }
        else {
            cout << "(" << finalList[i - 1].land_x << ", " << finalList[i - 1].land_y << ")\n";
        }
    }
    if (!done) {
        cout << "(4, 4)\n\nCongratulations!\nASA delivered the top secret information!";
    }
}

//apply damage to ASA based on the percentage chance of being hit by enemy fire for each terrain type
int applyDamage(int damage, landSquare current) {
    int randVal = rand() % 100;
    switch (current.type) {
     case grass:
        //grass has 80% chance of damage
        if (randVal < 80) {
            damage++;
            if (damage <= 6) {
                cout << "ASA has taken " << damage << " damage!\n";
            }
        }
        break;
    case forest:
        //forest has 70% chance of damage
        if (randVal < 70) {
            damage++;
            if (damage <= 6) {
                cout << "ASA has taken " << damage << " damage!\n";
            }
        }
        break;
    case water:
        //water has 30% chance of damage
        if (randVal < 30) {
            damage++;
            if (damage <= 6) {
                cout << "ASA has taken " << damage << " damage!\n";
            }
        }
        break;
    case lava:
        //lava has 5% chance of damage
        if (randVal < 5) {
            damage++;
            if (damage <= 6) {
                cout << "ASA has taken " << damage << " damage!\n";
            }
        }
        break;
    }
    return damage;
}
