#ifndef GFXFRAMEWORK_MAZE_H
#define GFXFRAMEWORK_MAZE_H

#include <cinttypes>
#include <vector>
#include <utility>

namespace implemented
{
	class Maze
	{
	public:
		int h, w;
		int H, W;
		std::vector<std::vector<uint8_t>> grid;
		std::pair<int, int> start, end;
		std::vector<std::vector<std::pair<int, int>>> solutions;

		Maze(int h = 3, int w = 3);
		~Maze();
		void generate();
		void generate_entrances();

	private:
		std::vector<std::pair<int, int>> find_neighbors(int r, int c,
		                                                std::vector<std::vector<uint8_t>> &grid,
		                                                bool is_wall = false);
		void shuffle(std::vector<std::pair<int, int>> &v);
		int randrange(int start, int stop, int step);
		int randrange(int end);
	};
}

#endif //GFXFRAMEWORK_MAZE_H
