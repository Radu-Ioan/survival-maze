#include "lab_m1/game/Maze.h"

#include <random>
#include <algorithm>

using namespace implemented;
using namespace std;

Maze::Maze(int h, int w) : h(h), w(w), H(2 * h + 1), W(2 * w + 1),
                           grid(H, vector<uint8_t>(W, 1))
{}

Maze::~Maze()
{}

void Maze::generate()
{
	// choose a random starting position
	int current_row = randrange(1, H, 2);
	int current_col = randrange(1, W, 2);

	grid[current_row][current_col] = 0;
	auto neighbors = find_neighbors(current_row, current_col, grid, true);
	int visited = 1;

	while (visited < h * w) {
		auto nn = randrange((int) neighbors.size());
		auto cell = neighbors[nn];
		current_row = cell.first;
		current_col = cell.second;

		visited++;
		grid[current_row][current_col] = 0;

		neighbors.erase(neighbors.begin() + nn);

		cell = find_neighbors(current_row, current_col, grid)[0];
		auto nearest_n0 = cell.first;
		auto nearest_n1 = cell.second;
		grid[(current_row + nearest_n0) / 2]
			[(current_col + nearest_n1) / 2] = 0;

		auto unvisited = find_neighbors(current_row, current_col, grid,
		                                true);

		for (const auto &n : unvisited) {
			if (find(neighbors.begin(), neighbors.end(), n)
			            == neighbors.end()) {
				neighbors.push_back(n);
			}
		}
	}
}


void Maze::generate_entrances()
{
	int start_side = randrange(4);

	if (start_side == 0)
		this->end = {0, randrange(1, W, 2)};
	else if (start_side == 1)
		this->end = {H - 1, randrange(1, W, 2)};
	else if (start_side == 2)
		this->end = {randrange(1, H, 2), 0};
	else
		this->end = {randrange(1, H, 2), W - 1};

	this->start = {randrange(1, H, 2), randrange(1, W, 2)};

	if (abs(this->start.first - this->end.first)
	    + abs(this->start.second - this->end.second) < 2) {
		generate_entrances();
	}
	this->grid[this->start.second][this->start.first] = 0;
	this->grid[this->end.second][this->end.first] = 0;
}

vector<pair<int, int>> Maze::find_neighbors(int r, int c,
                                            vector<vector<uint8_t>> &grid,
                                            bool is_wall)
{
	vector<pair<int, int>> neighbors;

	if (r > 1 && grid[r - 2][c] == (int) is_wall)
		neighbors.push_back({r - 2, c});
	if (r < H - 2 && grid[r + 2][c] == (int) is_wall)
		neighbors.push_back({r + 2, c});
	if (c > 1 && grid[r][c - 2] == (int) is_wall)
		neighbors.push_back({r, c - 2});
	if (c < W - 2 && grid[r][c + 2] == (int) is_wall)
		neighbors.push_back({r, c + 2});

	shuffle(neighbors);
	return neighbors;
}

void Maze::shuffle(vector<pair<int, int>> &v)
{
	if (v.size() <= 1)
		return;
	for (int i = (int) v.size() - 1; i >= 0; i--) {
		using std::swap;
		swap(v[i], v[rand() % (i + 1)]);
	}
}

int Maze::randrange(int start, int stop, int step)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	uniform_int_distribution<> range(start, stop - 1);
	int number = range(gen);
	int pos = number - start;
	int r = pos % step;
	return number - r;
}

int Maze::randrange(int end)
{
	return end >= 2 ? randrange(0, end - 1, 1) : end - 1;
}
