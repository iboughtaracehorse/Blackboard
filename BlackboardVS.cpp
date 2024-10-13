#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Board.h"

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Board;

class Figure {
protected:
	int X;
	int Y;
	int HEIGHT;
	string COLOR;
	string TYPE;
	string ALIAS;
	Board& grid;

public:

	Figure(Board& board) : grid(board) { X = 0; Y = 0; HEIGHT = 0; COLOR = "none"; ALIAS = ""; }
	virtual ~Figure() {};
	virtual void draw() = 0;

	void getTypeStr() {
		cout << TYPE;
	}

	void getCoordinatesStr() {
		cout << "x: " << X << " y: " << Y << endl;
	}

	string getType() {
		return TYPE;
	}
	int getX() {
		return X;
	}
	int getY() {
		return Y;
	}
	int getHeight() {
		return HEIGHT;
	}
	string getColor() {
		return COLOR;
	}
	string getAlias() {
		return ALIAS;
	}

	char checkColor() {
		string color = this->getColor();
		char fillColor;

		if (color == "none") {
			return '*';
		}
		else {
			fillColor = color[0];
			return fillColor;
		}
	}
};

class Line : public Figure {

	int X2;
	int Y2;

public:
	Line(Board& board, int x, int y, int x2, int y2, string color, string alias) : Figure(board) {
		X = x; Y = y; X2 = x2; Y2 = y2; TYPE = "line", COLOR = color, ALIAS = alias;
	};

	void draw() {
		char fillColor = this->checkColor();
		auto& gridAccess = grid.getGrid();

		if (Y == Y2) {
			int startX = min(X, X2);
			int endX = max(X, X2);
			for (int i = startX; i <= endX; ++i) {
				if (i >= 0 && i < BOARD_WIDTH && Y >= 0 && Y < BOARD_HEIGHT) {
					gridAccess[Y][i] = fillColor;
				}
			}
		}
		else if (X == X2) {
			int startY = min(Y, Y2);
			int endY = max(Y, Y2);
			for (int i = startY; i <= endY; ++i) {
				if (X >= 0 && X < BOARD_WIDTH && i >= 0 && i < BOARD_HEIGHT) {
					gridAccess[i][X] = fillColor;
				}
			}
		}
	}

	int getX2() {
		return X2;
	}
	int getY2() {
		return Y2;
	}
};

class Square : public Figure {

public:
	Square(Board& board, int x, int y, int height, string color, string alias) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "square", COLOR = color, ALIAS = alias;
	};

	void draw() {
		if (HEIGHT <= 0) return;
		string isFilled = this->getColor();
		auto& gridAccess = grid.getGrid();


		if (isFilled == "none") {
			for (int i = 0; i < HEIGHT; ++i) {
				int posY = Y + i;

				if (posY < 0 || posY >= BOARD_HEIGHT) continue;

				for (int j = 0; j < HEIGHT; ++j) {
					int posX = X + j;

					if (posX < 0 || posX >= BOARD_WIDTH) continue;

					if (i == 0 || i == HEIGHT - 1) {
						gridAccess[posY][posX] = '*';
					}

					else if (j == 0 || j == HEIGHT - 1) {
						gridAccess[posY][posX] = '*';
					}
				}
			}
		}
		else {
			char fillColor = this->checkColor();

			for (int i = 0; i < HEIGHT; ++i) {
				int posY = Y + i;

				if (posY < 0 || posY >= BOARD_HEIGHT) continue;

				for (int j = 0; j < HEIGHT; ++j) {
					int posX = X + j;

					if (posX < 0 || posX >= BOARD_WIDTH) continue;

					gridAccess[posY][posX] = fillColor;
				}
			}

		}	
	}
};

class Triangle : public Figure {

public:

	Triangle(Board& board, int x, int y, int height, string color, string alias) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "triangle"; COLOR = color, ALIAS = alias;
	};

	void draw() {
		if (HEIGHT <= 0) return;	

		string isFilled = this->getColor();
		auto& gridAccess = grid.getGrid();


		if (isFilled == "none") {
			for (int i = 0; i < HEIGHT; ++i) {
				int leftMost = X - i;
				int rightMost = X + i;
				int posY = Y + i;
				if (posY < BOARD_HEIGHT && posY >= 0) {
					if (leftMost >= 0 && leftMost < BOARD_WIDTH)
						gridAccess[posY][leftMost] = '*';
					if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
						gridAccess[posY][rightMost] = '*';
				}
			}
			for (int j = 0; j < 2 * HEIGHT - 1; ++j) {
				int baseX = X - HEIGHT + 1 + j;
				int baseY = Y + HEIGHT - 1;
				if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT)
					gridAccess[baseY][baseX] = '*';
			}
		}
		else {
			char fillColor = this->checkColor();

			for (int i = 0; i < HEIGHT; ++i) {
				int numStars = 2 * i + 1;
				int leftMost = X - i;
				int posY = Y + i;

				if (posY < BOARD_HEIGHT && posY >= 0) {
					for (int j = 0; j < numStars; ++j) {
						int position = leftMost + j;
						if (position >= 0 && position < BOARD_WIDTH) {
							gridAccess[posY][position] = fillColor;
						}
					}
				}
			}
		}
	}
};

class Circle : public Figure {

public:
	Circle(Board& board, int x, int y, int height, string color, string alias) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "circle", COLOR = color, ALIAS = alias;
	};

	void draw() {
		if (HEIGHT <= 0) return;

		string isFilled = this->getColor();
		auto& gridAccess = grid.getGrid();

		if (isFilled == "none") {
			int diameter = HEIGHT;
			int radius = diameter / 2;

			for (int i = 0; i <= diameter; ++i) {
				for (int j = 0; j <= diameter; j++) {
					double centreDistance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));

					if (centreDistance >= radius - 0.5 && centreDistance <= radius + 0.5) {
						int posX = X + j;
						int posY = Y + i;
						if (posX >= 0 && posX < BOARD_WIDTH && posY < BOARD_HEIGHT) {
							gridAccess[posX][posY] = '*';
						}
					}
				}
			}
		}
		else {
			char fillColor = this->checkColor();
			int diameter = HEIGHT;
			int radius = diameter / 2;

			for (int i = 0; i <= diameter; ++i) {
				for (int j = 0; j <= diameter; j++) {
					double centreDistance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));

					if (centreDistance <= radius + 0.5) { //chatGPT helped me with this math thing
						int posX = X + j;
						int posY = Y + i;
						if (posX >= 0 && posX < BOARD_WIDTH && posY >= 0 && posY < BOARD_HEIGHT) {
							gridAccess[posX][posY] = fillColor;
						}
					}
				}
			}
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

		cout << "0. line" << endl;
		cout << "   -- has four coordinates and color (x, y, x2, y2, color)" << endl;
		cout << "   -- example: 5, 10, 9, 10, green" << endl;
		cout << "     (draws a green line starting at position (5,10) and ending at (9, 10). can draw only horizontal or vertical lines)" << endl;
		cout << endl;
		
		cout << "1. triangle" << endl;
		cout << "   -- has two coordinates (x, y), height, and color" << endl;
		cout << "   -- example: 5, 10, 9, green" << endl;
		cout << "     (draws a green triangle starting at position (5,10) with a height of 9)" << endl;
		cout << endl;

		cout << "2. square" << endl;
		cout << "   -- has two coordinates (x, y), height, and color" << endl;
		cout << "   -- example: 10, 20, 10, green" << endl;
		cout << "     (draws a green square starting at position (10,20) with a height of 10)" << endl;
		cout << endl;
		
		cout << "3. circle" << endl;
		cout << "   -- has two coordinates (x, y), height (diameter), and color" << endl;
		cout << "   -- example: 10, 20, 10, green" << endl;
		cout << "     (draws a green circle starting at position (10,20) with a diameter of 10)" << endl;
		cout << endl;

		cout << " | available commands:" << endl;
		cout << endl;

		cout << "0. help" << endl;
		cout << "   -- description: prints help. you've just used it bfr" << endl;
		cout << endl;

		cout << "1. add" << endl;
		cout << "   -- description: allows to add a new figure" << endl;
		cout << endl;
		
		cout << "2. print" << endl;
		cout << "   -- description: prints board to console" << endl;
		cout << endl;

		cout << "3. list" << endl;
		cout << "   -- description: lists all added figures in text format with coordinates" << endl;
		cout << endl;

		cout << "4. undo" << endl;
		cout << "   -- description: deletes the last figure from the board" << endl;
		cout << endl;

		cout << "5. clear" << endl;
		cout << "   -- description: clears the board" << endl;
		cout << endl;
		
		cout << "6. save" << endl;
		cout << "   -- description: saves the board to the chosen file" << endl;
		cout << endl;

		cout << "7. exit" << endl;
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
			board.print(cout);
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
		else if (command == "save") {
			board.save();
		}
		else if (command == "load") {
			cout << "NOT IMPLEMENTED" << endl;
			//board.load();
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
		cout << "0. line" << endl;
		cout << "1. triangle" << endl;
		cout << "2. square" << endl;
		cout << "3. circle" << endl;

		int userInput;
		cout << "enter the number corresponding to the figure you want to add: ";
		cin >> userInput;
		cin.ignore();

		int x = 0;
		int y = 0;
		int height = 0;
		int x2 = 0;
		int y2 = 0;
		string color;
		string alias;

		cout << "how do you want to call your new figure: ";
		cin >> alias;

		cout << "enter x--position: ";
		cin >> x;

		cout << "enter y--position: ";
		cin >> y;

		if (userInput == 0) {
			cout << "enter end x--position: ";
			cin >> x2;

			cout << "enter end y--position: ";
			cin >> y2;

			cout << "enter fill color ('none' for transparent): ";
			cin >> color;
			cin.ignore();

			if (x == x2 || y == y2) {
				Line* line = new Line(board, x, y, x2, y2, color, alias);
				board.addFigure(line);
				cout << "new line named " << alias << "was added!" << endl;
			}
			else {
				cout << "only horizontal or vertical lines are available!!" << endl;
			}
		}

		else if (userInput == 1) {
			cout << "enter height: ";
			cin >> height;
			cout << "enter fill color ('none' for transparent): ";
			cin >> color;
			cin.ignore();


			Triangle* triangle = new Triangle(board, x, y, height, color, alias);
			board.addFigure(triangle);
			cout << "new triangle named " << alias << "was added!" << endl;
		}
		else if (userInput == 2) {
			cout << "enter height: ";
			cin >> height;
			cout << "enter fill color ('none' for transparent): ";
			cin >> color;
			cin.ignore();


			Square* square = new Square(board, x, y, height, color, alias);
			board.addFigure(square);
			cout << "new square named " << alias << "was added!" << endl;
		}
		
		else if (userInput == 3) {
			cout << "enter diameter: ";
			cin >> height;
			cout << "enter fill color ('none' for transparent): ";
			cin >> color;
			cin.ignore();


			Circle* circle = new Circle(board, x, y, height, color, alias);
			board.addFigure(circle);
			cout << "new circle named " << alias << "was added!" << endl;
		}
	}


	bool deletionWarning() {
		string input;
		cout << "you are going to loose your current board! you sure you want to do that? (y/n): ";

		getline(cin, input);

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
				if (board.isEmpty()) {
					break;
				}
				else if (deletionWarning()) {
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