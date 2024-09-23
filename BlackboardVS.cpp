#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Figure;

struct Board {

	vector<vector<char>> grid;
	vector<Figure*> figures;

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

	void addFigure(Figure* figure) {
		figures.push_back(figure);
	}

	void drawFigures() {
		for (auto& figure : figures) {
			// draw figure 
		}
	}

	void list() {
		for (auto& figure : figures) {
			//cout << figure->getType();
		}
	}
};

class Figure {

protected: 
	int X;
	int Y;
	int HEIGHT;
	string TYPE;
	vector<vector<char>>* grid;

public:

	Figure(Board& board) { X = 0; Y = 0; HEIGHT = 0; grid = &board.getGrid(); }

	string getType() {
		return TYPE;
	}

};

class Square: public Figure {

public:
	Square(Board& board, int x, int y, int height) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "square";
	};

	void draw() {
		if (HEIGHT <= 0) return;
		for (int i = 0; i < HEIGHT; ++i) {
			for (int j = 0; j < HEIGHT; ++j) {
				if (j == 0 || j == Y + HEIGHT) {
					(*grid)[i][j] = '*';
				}
				else {
					(*grid)[i][0] = '*';
					(*grid)[i][HEIGHT] = '*';
				}
			}
		}
	}
};

class Triangle: public Figure {

public:

	Triangle(Board& board, int x, int y, int height) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "triangle";
	};

	void draw() {
		if (HEIGHT <= 0) return;
		for (int i = 0; i < HEIGHT; ++i) {
			int leftMost = X - i;
			int rightMost = X + i;
			int posY = Y + i;
			if (posY < BOARD_HEIGHT) {
				if (leftMost >= 0 && leftMost < BOARD_WIDTH)
					(*grid)[posY][leftMost] = '*';
				if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
					(*grid)[posY][rightMost] = '*';
			}
		}
		for (int j = 0; j < 2 * HEIGHT - 1; ++j) {
			int baseX = X - HEIGHT + 1 + j;
			int baseY = Y + HEIGHT - 1;
			if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT)
				(*grid)[baseY][baseX] = '*';
		}
	}
};



int main() {

	Board board;

	Triangle* triangle = new Triangle(board, 10, 10, 5);
	Square* square = new Square(board, 20, 20, 3);

	board.addFigure(triangle);
	board.addFigure(square);
	
	board.print();
	board.list();

	return 0;

};



