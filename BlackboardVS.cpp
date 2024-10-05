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

	virtual void getType() {
		cout << TYPE;
	}

	virtual void getCoordinates() {
		cout << "x: " << X << " y: " << Y << endl;
	}
};

struct Board {

	vector<vector<char>> grid;
	vector<Figure*> figures;

	Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

	void clear() {
		for (int i = 0; i < BOARD_HEIGHT; ++i) {
			fill(grid[i].begin(), grid[i].end(), ' ');
		}
	}

	void print() {

		cout << "^";  
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			cout << "-";
		}
		cout << "^" << endl;

		for (int i = 0; i < BOARD_HEIGHT; ++i) {
			cout << "|";  
			for (int j = 0; j < BOARD_WIDTH; ++j) {
				cout << grid[i][j];
			}
			cout << "|" << endl; 
		}

		cout << "^";
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			cout << "-";
		}
		cout << "^" << endl;

	}

	vector<vector<char>>& getGrid() {
		return grid;
	}

	void addFigure(Figure* figure) {
		figures.push_back(figure);
	}

	void drawFigures() {
		clear();
		for (Figure* figure : figures) {
			figure->draw();
		}
	}

	void list() {
		if (figures.empty()) {
			cout << endl << "oops! your board must be empty! try 'add' to add new figures!" << endl << endl;
		}
		for (auto& figure : figures) {
			figure->getType();
			cout << " with coordinates ";
			figure->getCoordinates();
		}
	}

	void undo() {
		if (figures.empty()) {
			cout << "no figures on the board!" << endl;
		}
		else {
			figures.pop_back();
		}
	}

	void deleteFigures() {
		while (!figures.empty()) {
			figures.pop_back();
		}
		cout << endl << "all figures were deleted!!" << endl << endl;
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

class Help {

public:

	Help() = default;

	// this awesome text was generated by chatgpt since i cannot stand help methods. upd: edited heavily by me

	void help() {
		cout << endl << endl << "here are all available shapes and commands: " << endl << endl;
		cout << endl;

		cout << " | available shapes:" << endl;
		cout << endl;

		cout << "1. triangle" << endl;
		cout << "   -- has two coordinates (x, y) and height" << endl;
		cout << "   -- example: 5, 10, 9" << endl;
		cout << "     (draws a triangle starting at position (5,10) with a height of 9)" << endl;
		cout << endl;

		cout << "2. square" << endl;
		cout << "   -- has two coordinates (x, y) and height" << endl;
		cout << "   -- example: 10, 20, 10" << endl;
		cout << "     (draws a square starting at position (10,20) with a height of 10)" << endl;
		cout << endl;

		cout << " | available commands:" << endl;
		cout << endl;

		cout << "0. help" << endl;
		cout << "   -- description: prints help. you've just used it bfr" << endl;
		cout << endl;

		cout << "1. print" << endl;
		cout << "   -- description: prints board to console" << endl;
		cout << endl;

		cout << "2. list" << endl;
		cout << "   -- description: lists all added figures in text format with coordinates" << endl;
		cout << endl;
		
		cout << "3. undo" << endl;
		cout << "   -- description: deletes the last figure from the board" << endl;
		cout << endl;
		
		cout << "4. clear" << endl;
		cout << "   -- description: clears the board" << endl;
		cout << endl;
		
		cout << "5. exit" << endl;
		cout << "   -- description: exit the program, yu will lost all the progress" << endl;
		cout << endl;
		
		
		cout << "!!! ANYTHING ELSE IS NOT SUPPORTED !!!" << endl;
	}
};

class InputParser {

	Board& board;
	Help help;

public:

	InputParser(Board& board) : board(board) { cout << "welcome to the BLACKBOARD!!" << endl << endl; };

	void commands(string command) {
		if (command == "print") {
			board.drawFigures();
			board.print();
		}
		else if (command == "list") {
			board.list();
		}
		else if (command == "add") {
			addFigure();
		}
		else if (command == "undo") {
			board.undo();
		}
		else if (command == "clear") {
			board.deleteFigures();
			board.clear();
		}
		else if (command == "help") {
			help.help();
		}
		else {
			cout << "unknown command! please try again" << endl;
		}
	}

	void addFigure() {
		cout << "so you want to add a new figure... great! here are your options: " << endl;
		cout << "1. triangle" << endl;
		cout << "2. square" << endl;

		int userInput;
		cout << "enter the number corresponding to the figure you want to add: ";
		cin >> userInput;

		int x;
		int y;
		int height;

		cout << "enter x--position: ";
		cin >> x;

		cout << "enter y--position: ";
		cin >> y;

		cout << "enter height (works as diametr for circle): ";
		cin >> height;

		if (userInput == 1) {
			Triangle* triangle = new Triangle(board, x, y, height);
			board.addFigure(triangle);
			cout << "new triangle was added!" << endl;
		}
		else if (userInput == 2) {
			Square* square = new Square(board, x, y, height);
			board.addFigure(square);
			cout << "new square was added!" << endl;
		}
		else {
			cout << "we don't carry THIS here..." << endl;
		}
	}

	bool deletionWarning() {
		string input;
		cout << "you are going to loose your current board! you sure you want to do that? (y/n): ";

		if (input == "y" || input == "Y") {
			return true;
		}
		else {
			return false;
		}
	}

	void start() {
		string userInput;
		while (true) {
			cout << "enter your command (or 'help' to see all commands): ";

			getline(cin, userInput);

			if (userInput == "exit") {
				deletionWarning();
				getline(cin, userInput);

				if (userInput == "y" || userInput == "Y") {
					break;
				}
				else {
					continue;
				}
			}
			commands(userInput);
		}
	}
};


int main() {

	Board board;
	Help help;

	InputParser parser(board);
	parser.start();

	return 0;

};



