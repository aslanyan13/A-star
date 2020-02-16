from colorama import init
from colorama import Fore, Back, Style

import time;
import os;
import random;
import math;

init()

D = 1;
D2 = 1.4;

HEIGHT = 20;
WIDTH = 59;

map = [
	"...#...............................#..........#............",
	"...#...............................#.......................",
	"...##.##############################..........#............",
	"...#...............................#######.################",
	"...#..............#................#.......................",
	"..................#................#.......................",
	"...#..............##################.......................",
	"...#..............#................#.......................",
	"...####.#################.##...............................",
	"...#...........#...........#.#######.......................",
	"...#...........#...................#.......................",
	"...#...........#...........#.......#.......................",
	"...#...........#...........################################",
	"...#........................................#..............",
	"...#...........#...........#................#..............",
	"...####.##########.#########................#..............",
	"...#......#................###########.....................",
	"...#......#..........#####.#.........#### ###..............",
	"...#.................#.....#.........#......#..............",
	"...#......#..........#.....#.........#......#.............."
];

# Cell class
class Cell:
	sym = ''
	cameFrom = -1;
	isLet = None;
	f = float('inf');
	g = float('inf');

	def __init__ (self, x, y):
		self.x = x;
		self.y = y;

	def getNeighbors (self, area):
		neighbors = [];

		for i in range(0, HEIGHT):
			for j in range(0, WIDTH):
				if abs(area[i][j].x - self.x) <= 1 and abs(area[i][j].y - self.y) <=1 and area[i][j] != self:
					neighbors.append(area[i][j]);

		return neighbors;


# Manhattan heruistic function
def heruistic (start, finish):
	dx = abs(start.x - finish.x);
	dy = abs(start.y - finish.y);

	return D * (dx + dy) + (D2 - 2 * D) * min(dx, dy);


def aStar (start, target, area):
	path = [];
	openSet = [];

	for i in range(0, HEIGHT):
		area.append([]);
		for j in range(0, WIDTH):
			# print(i , ' ' , j);
			area[i].append( Cell(j ,i) );

			if map[i][j] == '#':
				area[i][j].isLet = True;
				area[i][j].sym = '#';
			else:
				area[i][j].isLet = False;
				area[i][j].sym = '.';

	openSet.append(start);
	openSet[0].g = 0;
	openSet[0].f = heruistic(openSet[0], target);

	while len(openSet) != 0:
		current = openSet[0];

		for x in openSet:
			if current.f > x.f:
				current = x;

		if current.x == target.x and current.y == target.y:
			print("Path founded!");
			current = openSet [ len(openSet) - 1].cameFrom;

			while current.cameFrom != -1:
				#print(current.x,  " ", current.y);
				path.append(current);
				current = current.cameFrom;

			return path;

		openSet.remove(current);

		neighbors = current.getNeighbors(area);
		#print(len(neighbors));

		for neighbor in neighbors:
			
			gTemp = current.g + D;

			if gTemp < neighbor.g and neighbor.isLet == False:
				neighbor.cameFrom = current;
				neighbor.g = gTemp;
				neighbor.f = math.ceil( gTemp + heruistic(neighbor, target) );

				if openSet.count(neighbor) == 0:
					openSet.append( neighbor );

	print("Path not found!");

os.system('cls');
area = [];

while True:
	startX = random.randint(0, WIDTH - 1);
	startY = random.randint(0, HEIGHT - 1);

	if map[startY][startX] == '.':
		break;
start = Cell(startX, startY);

while True:
	targetX = random.randint(0, WIDTH - 1);
	targetY = random.randint(0, HEIGHT - 1);

	if map[targetY][targetX] == '.':
		break;

target = Cell(targetX, targetY);

startTime = int(round(time.time() * 1000))
path = aStar(start, target, area);

if path != None:
	endTime = int(round(time.time() * 1000)) - startTime;

	for cell in path:
		area[cell.y][cell.x].sym = '+';

	area[start.y][start.x].sym = 'S';
	area[target.y][target.x].sym = 'F';

	for i in range(0, HEIGHT):
		for j in range(0, WIDTH):
			if area[i][j].sym == '+':
				print(Fore.YELLOW + Style.BRIGHT, end='');
			if area[i][j].sym == '#':
				print(Fore.RED + Style.NORMAL, end='');
			if area[i][j].sym == '.':
				print(Fore.BLACK + Style.BRIGHT, end='');
			if area[i][j].sym == 'S':
				print(Fore.GREEN + Style.BRIGHT, end='');
			if area[i][j].sym == 'F':
				print(Fore.RED + Style.BRIGHT, end='');
			print(area[i][j].sym, end='');

			print(Fore.RESET + Style.RESET_ALL, end='')
		print('\n', end='');

	print('Time elasped:', endTime, 'ms');
else:
	print('Start (', startX, ',', startY, ')');
	print('Target (', targetX, ',', targetY, ')');