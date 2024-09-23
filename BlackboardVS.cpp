#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Figure {
	
};

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
		if (height <= 0) return; 
		for (int i = 0; i < height; ++i) {
			int leftMost = x - i; 
			int rightMost = x + i; 
			int posY = y + i; 
			if (posY < BOARD_HEIGHT) {
				if (leftMost >= 0 && leftMost < BOARD_WIDTH) 
					grid[posY][leftMost] = '*';
				if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
					grid[posY][rightMost] = '*';
			}
		}
		for (int j = 0; j < 2 * height - 1; ++j) {
			int baseX = x - height + 1 + j;
			int baseY = y + height - 1;
			if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) 
				grid[baseY][baseX] = '*';
		}
	}
};

int main() {
	Board board;
	board.drawTriangle(10, 1, 5);
	board.drawTriangle(13, 1, 7);
	board.print();
	return 0;
};



