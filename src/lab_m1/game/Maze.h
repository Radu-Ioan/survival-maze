#ifndef GFXFRAMEWORK_MAZE_H
#define GFXFRAMEWORK_MAZE_H

#include <cinttypes>
#include <vector>
#include <utility>

#define EMPTY_CELL 0
#define WALL 1
#define ENEMY_CELL 2

namespace implemented
{
	class Maze
	{
	public:
		int h, w;
		int H, W;
		std::vector<std::vector<uint8_t>> grid;
		// first is for row, second is for col
		std::pair<int, int> start, end;
		std::vector<std::vector<std::pair<int, int>>> solutions;

		Maze(int h = 3, int w = 3);
		~Maze();
		void generate();
		void generate_entrances();

		void shuffle(std::vector<std::pair<int, int>> &v);
		int randrange(int start, int stop, int step);
		int randrange(int end);

	private:
		std::vector<std::pair<int, int>> find_neighbors(int r, int c,
		                            std::vector<std::vector<uint8_t>> &grid,
		                            bool is_wall = false);
	};
}

#endif //GFXFRAMEWORK_MAZE_H
