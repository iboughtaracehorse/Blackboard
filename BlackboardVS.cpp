#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

struct Board {

	vector<vector<char>> grid;

	Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, '-')) {}

	void print() {
		for (auto& row : grid) {
			for (char c : row) {
				cout << c;
			}
			cout << "\n";
		}
	}

	vector<vector<char>>& getGrid() {
		return grid;
	}
};

class Figure {

protected: 
	int x;
	int y;
	int height;
	vector<vector<char>>* grid;

public:

	Figure(Board& board) { x = 0; y = 0; height = 0; grid = &board.getGrid(); }

};

class Square: public Figure {

public:
	Square(Board& board) : Figure(board) {};
};

class Triangle: public Figure {

public:

	Triangle(Board& board) : Figure(board) {};

	void draw() {
		if (height <= 0) return;
		for (int i = 0; i < height; ++i) {
			int leftMost = x - i;
			int rightMost = x + i;
			int posY = y + i;
			if (posY < BOARD_HEIGHT) {
				if (leftMost >= 0 && leftMost < BOARD_WIDTH)
					(*grid)[posY][leftMost] = '*';
				if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
					(*grid)[posY][rightMost] = '*';
			}
		}
		for (int j = 0; j < 2 * height - 1; ++j) {
			int baseX = x - height + 1 + j;
			int baseY = y + height - 1;
			if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT)
				(*grid)[baseY][baseX] = '*';
		}
	}
};



int main() {
	Board board;
	
	board.print();
	return 0;
};



