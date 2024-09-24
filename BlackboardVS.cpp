#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

struct Board;

class Figure {

protected:
	int X;
	int Y;
	int HEIGHT;
	string TYPE;

public:

	Figure(Board& board) { X = 0; Y = 0; HEIGHT = 0; }

	virtual void draw() = 0;

	string getType() {
		return TYPE;
	}
};

struct Board {

	vector<vector<char>> grid;
	vector<Figure*> figures;

	Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

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
		for (Figure* figure : figures) {
			figure->draw();
		}
	}

	void list() {
		for (auto& figure : figures) {
			cout << figure->getType() << "  " << endl;
		}
	}
};

class Square: public Figure {

	vector<vector<char>>* grid;

public:
	Square(Board& board, int x, int y, int height) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "square", grid = &board.getGrid();;
	};

	void draw() {
		if (HEIGHT <= 0) return;
		for (int i = 0; i < HEIGHT; ++i) {
			int posY = Y + i;

			if (posY < 0 || posY >= BOARD_HEIGHT) continue;

			for (int j = 0; j < HEIGHT; ++j) {
				int posX = X + j;

				if (posX < 0 || posX >= BOARD_WIDTH) continue;

				if (i == 0 || i == HEIGHT - 1) {
					(*grid)[posY][posX] = '*';
				}

				else if (j == 0 || j == HEIGHT - 1) {
					(*grid)[posY][posX] = '*'; // well previous method was absolutely stupid i apologize to every square i tried to create with it 
				}
			}
		}
	}
};

class Triangle: public Figure {

	vector<vector<char>>* grid;

public:

	Triangle(Board& board, int x, int y, int height) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "triangle"; grid = &board.getGrid();
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

	Triangle* triangle = new Triangle(board, 20, 10, 5);
	Square* square = new Square(board, 15, 2, 11);

	board.addFigure(triangle);
	board.addFigure(square);

	board.drawFigures();
	
	board.print();
	
	board.list();

	delete triangle;

	return 0;

};



