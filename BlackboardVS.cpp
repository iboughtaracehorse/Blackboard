#include <iostream>
#include <vector>

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

struct Board {
	std::vector<std::vector<char>> grid;
	Board() : grid(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH, ' ')) {}
	void print() {
		for (auto& row : grid) {
			for (char c : row) {
				std::cout << c;
			}
			std::cout << "\n";
		}
	}
	void drawTriangle(int x, int y, int height) {
		for (int i = 0; i < height; ++i) {
			int numStars = 2 * i + 1;
			int leftMost = x - i;
			for (int j = 0; j < numStars; ++j) {
				int position = leftMost + j;
				if (position >= 0 && position < BOARD_WIDTH && (y + i) <
					BOARD_HEIGHT && (y + i) >= 0)
					grid[y + i][position] = '*';
			}
		}
	}
};
int main() {
	Board board;
	board.drawTriangle(80, 1, 5);
	board.print();
	return 0;
}
