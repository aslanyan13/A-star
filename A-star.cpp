#include <iostream>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

class Cell;
float distance(Cell start, Cell finish);

const int HEIGHT = 20, WIDTH = 60;
const float D = 1, D2 = 1.4;

// MAP
// # - let
// . - free cell

/* BIG MAP 40x100
const char map[HEIGHT][WIDTH] = {
	"...############################################################################################....",
	"...#...............................#..........................#...#......#....................#....",
	"...################### #############..........#...............#...#....................#......#....",
	"...#...............................################ #######...#####################.###########....",
	"...#..............#................#.............#............#......#...............#........#....",
	"..................#................#.............#............#......................#........#....",
	"...#..............##################..........................#......#........................#....",
	"...#..............#......#.........#.............#............#############.###################....",
	"...####### ########......#.......................#.############.........#.....................#....",
	"...#.....#.....#.........#.........###############............#.........#............##########....",
	"...#...........#.........#.........#.............#............#.........#..............#...........",
	"...### ##### ###.........#.........#.............#............######.############......#......#....",
	"...#...........#.........#.############## #################.###........#........#......#......#....",
	"...#.......................#................#.................#........#......................#....",
	"...#...........#...........#................#.......#..................#........#......#####.##....",
	"...################### #####................#########.........#.................#.............#....",
	"...#......#................######### #........................#######################.#########....",
	"...#......#..........#####.#.........#### ###.................#.....................#.........#....",
	"...#......#..........#.....#.......#.#......#.................#...........#.........#.........#...",
	"...#......#..........#.....#.......#.#......#.................#...........#.........#.........#....",
	"...#......##########...............#..........#...............#...........###########.........#....",
	"...#.................#.....#.......#..........................#...........#.........#.........#....",
	"...################### #############..........#...............#...........#...................#....",
	"...#...............................################ #######...#.....................#.........#....",
	"...#..............#................#............#.............##########################.######....",
	"..................#................#............#...........#...........#...........#.........#....",
	"...#..............###########################.###.............#.....................#.........#....",
	"...#..............#......#.........#............#.#############.........#.....................#....",
	"...####### ########......#......................#.......#.....#.........#...........#.........#....",
	"...#.....#.....#.........#.........#....................#.....#.........#...........#.........#....",
	"...#...........#.........#.........#............#.............###########...........#.........#....",
	"...### ##### ###.........#.........#............#.......#.....#.....................#.........#....",
	"...#...........#.........#.############## ###################.#.........#...........#.........#....",
	"...#.......................#................#........#........#.........#...........######.####....",
	"...#...........#...........#................#........#........#.........#...........#.........#....",
	"...################### #####................#######.##........#.........#...........#.........#....",
	"...#......#................######### #...............#........#.........#...........#.........#....",
	"...#.................#####.#.........#### ###........#........#.........#.....................#....",
	"...#......#..........#.....#.........#......#.................#.........#...........#.........#....",
	"...############################################################################################...."
};
*/

// SMALL MAP 5x12
/*
const char map[HEIGHT][WIDTH] = {
	"......#.....",
	"......#.....",
	"............",
	"............",
	"............"
};
*/


// MEDIUM MAP 20x60
const char map[HEIGHT][WIDTH] = {
	"...#...............................#..........#............",
	"...#...............................#.......................",
	"...################### #############..........#............",
	"...#...............................################ #######",
	"...#..............#................#.......................",
	"..................#................#.......................",
	"...#..............##################.......................",
	"...#..............#................#.......................",
	"...####### ##############.##...............................",
	"...#...........#...........#.#######.......................",
	"...#...........#...........#.......#.......................",
	"...#...........#...........#.......#.......................",
	"...#...........#...........############## #################",
	"...#.......................#................#..............",
	"...#...........#...........#................#..............",
	"...################### #####................#..............",
	"...#......#................######### #.....................",
	"...#......#..........#####.#.........#### ###..............",
	"...#.................#.....#.........#......#..............",
	"...#......#..........#.....#.........#......#.............."
};

// Colors codes
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

// Cursor Position
void gotoxy(int x, int y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Seting Cursor Color
void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

class Cell {
public:
	int x, y;
	char sym;
	bool isLet;
	int cameFrom;
	float f; // Sum of g and h
	float g; // Cost of step
	float h; // Heuristic (Aproximate distance to goal)

	void calcF() { f = g + h; }
	Cell() {};

	Cell(int X, int Y, bool state = true) :x(X), y(Y), isLet(state) { };

	void setPos(int X, int Y) {
		x = X;
		y = Y;
	};

	bool operator==(Cell obj) {
		return this->x == obj.x && this->y == obj.y;
	}

	// Returns neighbors
	vector <Cell> getNeighbors(Cell area[][WIDTH]) {
		vector <Cell> neighbors;

		for (int i = 0; i < HEIGHT; i++)
			for (int j = 0; j < WIDTH; j++) {
				if (abs(area[i][j].x - this->x) <= 1 && abs(area[i][j].y - this->y) <= 1 && !(area[i][j] == *this))
				{
					neighbors.push_back(area[i][j]);
				}
			}
		return neighbors;
	}
};

// Heuristic Euclidean function
/*float distance(Cell start, Cell finish) {
	return sqrt(pow(finish.x - start.x, 2) + pow(finish.y - start.y, 2));
}*/

// Heuristic Manhattan function 
float distance(Cell start, Cell finish) {
	float dx = abs(start.x - finish.x);
	float dy = abs(start.y - finish.y);
	return D * (dx + dy) + (D2 - 2 * D) * min(dx, dy);
}

// Pathfinding function
vector <Cell> aStar(Cell start, Cell target, Cell area[HEIGHT][WIDTH]) {
	vector <Cell> returnPath;
	vector <Cell> openSet;
	int i, j;

	// Area initialization from map
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			area[i][j].setPos(j, i);
			area[i][j].g = (float)INT_MAX;
			area[i][j].f = (float)INT_MAX;
			if (map[i][j] == '#') {
				area[i][j].isLet = false;
				area[i][j].sym = '#';
			}
			else {
				area[i][j].isLet = true;
				area[i][j].sym = '.';
			}
		}
	}

	openSet.push_back(start);
	openSet[0].g = 0;
	openSet[0].f = distance(openSet[0], area[target.y][target.x]);
	openSet[0].cameFrom = -1;

	vector <Cell> path; // Path Cells
	int k = 0; // Path cell index

	while (openSet.size() != 0) {
		Cell current;
		int minF = 0;

		for (i = 0; i < openSet.size(); i++)
			if (openSet[minF].f > openSet[i].f) minF = i;

		current = openSet[minF];
		path.push_back(current);

		if (current == target) {
			cout << "Path found!" << endl;
			break;
		}
		// Removing current cell
		vector <Cell>::iterator iter = find(openSet.begin(), openSet.end(), current);
		openSet.erase(iter);

		vector <Cell> neighbors = current.getNeighbors(area);

		for (i = 0; i < neighbors.size(); i++)
		{
			float g_tmp;
			if (abs(current.x - neighbors[i].x) == 1 && abs(current.y - neighbors[i].y) == 1)
				g_tmp = current.g + D2;
			else
				g_tmp = current.g + D;
			if (g_tmp < neighbors[i].g && neighbors[i].isLet == true) {

				neighbors[i].cameFrom = k;
				neighbors[i].g = g_tmp;
				neighbors[i].h = ceill(distance(neighbors[i], target));
				// neighbors[i].h = distance(neighbors[i], area[targetY][targetX]);
				neighbors[i].calcF();

				if (find(openSet.begin(), openSet.end(), neighbors[i]) == openSet.end())
					openSet.push_back(neighbors[i]);
			}
		}
		k++;
	}

	Cell current = openSet[openSet.size() - 1];

	do {
		current = path[current.cameFrom];
		returnPath.push_back(current);
	} while (current.cameFrom != -1);

	return returnPath;
}

int main()
{
	system("cls");
	srand(clock());
	int i, j;

	Cell area[HEIGHT][WIDTH];

	do {
		system("cls");

		int startX = 0, startY = 1;
		do {
			startX = rand() % WIDTH;
			startY = rand() % HEIGHT;
		} while (area[startY][startX].isLet == false);
		
		int targetX = 43, targetY = 1;
		do {
			targetX = rand() % WIDTH;
			targetY = rand() % HEIGHT;
		} while (area[targetY][targetX].isLet == false);

		clock_t start = clock(); // Starting timer

		cout << "Finding best path..." << endl;

		vector<Cell> path = aStar(Cell(startX, startY), Cell(targetX, targetY), area);

		// path visualisation
		for (i = 0; i < path.size(); i++)
			area[path[i].y][path[i].x].sym = '+';

		area[startY][startX].sym = 'S';
		area[targetY][targetX].sym = 'F';

		cout << endl;

		// Drawing area
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (area[i][j].sym == '+') SetColor(Yellow, Black);
				if (area[i][j].sym == 'S') SetColor(LightGreen, Black);
				if (area[i][j].sym == 'F') SetColor(LightRed, Black);
				if (area[i][j].sym == '.') SetColor(DarkGray, Black);
				if (area[i][j].sym == '#') SetColor(Red, Black);
				cout << area[i][j].sym;
			}
			cout << endl;
		}

		SetColor(7, 0);
		char ans;
		cout << "Path finded in " << clock() - start << "ms" << endl << endl; // Timer end
		
		do {
			cout << "Try again? (Y/n): ";
			ans = (char)_getch();
		} while (ans != 'y' || ans == 'Y' || ans == 'n' || ans == 'N');

		if (ans == 'n' || ans == 'N') break;

	} while (true);

	return 0;
}