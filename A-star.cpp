#include <iostream>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

const int HEIGHT = 20, WIDTH = 60;

// MAP
// # - let
// . - free cell
const char map[HEIGHT][WIDTH] = {
	"...#...............................#..........#............",
	"...#...............................#.......................",
	"...################### #############..........#............",
	"...#...............................################ #######",
	"...#..............#................#.......................",
	"..................#................#.......................",
	"...#..............##################.......................",
	"...#..............#................#.......................",
	"...####### ########........................................",
	"...#...........#...................#.......................",
	"...#...........#...................#.......................",
	"...#...........#...................#.......................",
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

float distance(Cell start, Cell finish) {
	return sqrt(pow(finish.x - start.x, 2) + pow(finish.y - start.y, 2));
}

int main()
{
	system("cls");
	srand(time(NULL));
	int i, j;

	Cell area[HEIGHT][WIDTH];

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			area[i][j].setPos(j, i);
			area[i][j].g = 1000;
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

	vector <Cell> openSet;

	int startX, startY;
	do {
		startX = rand() % WIDTH;
		startY = rand() % HEIGHT;
	} while (area[startY][startX].isLet == false);

	int targetX, targetY;
	do {
		targetX = rand() % WIDTH;
		targetY = rand() % HEIGHT;
	} while (area[targetY][targetX].isLet == false);
	
	openSet.push_back(Cell(startX, startY));
	openSet[0].g = 0;
	openSet[0].h = distance(openSet[0], area[targetY][targetX]);
	openSet[0].calcF();
	openSet[0].cameFrom = -1;

	vector <Cell> path; // Path Cells
	int k = 0; // Path cell index

	clock_t start = clock();
	cout << "Finding best path..." << endl;

	while (openSet.size() != 0) {
		Cell current;
		int minF = 0;

		for (i = 0; i < openSet.size(); i++)
			if (openSet[minF].f > openSet[i].f) minF = i;

		current = openSet[minF];
		path.push_back(current);

		// cout << "Current Pos. X: " << current.x << " Y: " << current.y << endl;

		if (current == area[targetY][targetX]) {
			cout << "Path found!" << endl;
			break;
		}

		// Removing current cell
		vector <Cell>::iterator iter = find(openSet.begin(), openSet.end(), current);
		openSet.erase(iter);

		vector <Cell> neighbors = current.getNeighbors(area);

		for (i = 0; i < neighbors.size(); i++)
		{
			//float g_tmp = current.g + distance(current, neighbors[i]);
			float g_tmp = current.g + 1;
			if (g_tmp < neighbors[i].g && neighbors[i].isLet == true) {
				
				neighbors[i].cameFrom = k;
				neighbors[i].g = g_tmp;
				neighbors[i].h = distance(neighbors[i], area[targetY][targetX]);
				neighbors[i].calcF();

				if ( find(openSet.begin(), openSet.end(), neighbors[i]) == openSet.end())
					openSet.push_back(neighbors[i]);
			}
		}
		k++;
	}

	Cell current = openSet[openSet.size() - 1];
	do {
		current = path[current.cameFrom];
		area[current.y][current.x].sym = '+';
	} while (current.cameFrom != -1);

	area[startY][startX].sym = 'S';
	area[targetY][targetX].sym = 'F';

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
	cout << "Path finded in " << clock() - start << "ms" << endl << endl;
	do {
		cout << "Try again? (Y/n): ";
		ans = (char)_getch();

		if (ans == 'y' || ans == 'Y') main();
		if (ans == 'n' || ans == 'N') exit(0);
		cout << endl;
	} while (true);
	
	return 0;
}