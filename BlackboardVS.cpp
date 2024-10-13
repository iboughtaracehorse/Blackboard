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
	string COLOR;
	string TYPE;

public:

	Figure(Board& board) { X = 0; Y = 0; HEIGHT = 0; COLOR = "none"; }

	virtual void draw() = 0;

	virtual void getTypeStr() {
		cout << TYPE;
	}

	virtual void getCoordinatesStr() {
		cout << "x: " << X << " y: " << Y << endl;
	}

	virtual string getType() {
		return TYPE;
	}
	virtual int getX() {
		return X;
	}
	virtual int getY() {
		return Y;
	}
	virtual int getHeight() {
		return HEIGHT;
	}
	virtual string getColor() {
		return COLOR;
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

struct Board {

	vector<vector<char>> grid;
	vector<Figure*> figures;

	Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

	void clear() {
		for (int i = 0; i < BOARD_HEIGHT; ++i) {
			fill(grid[i].begin(), grid[i].end(), ' ');
		}
	}

	void print(ostream& target) {

		target << "^";
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			target << "-";
		}
		target << "^" << endl;

		for (int i = 0; i < BOARD_HEIGHT; ++i) {
			target << "|";
			for (int j = 0; j < BOARD_WIDTH; ++j) {
				target << grid[i][j];
			}
			target << "|" << endl;
		}

		target << "^";
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			target << "-";
		}
		target << "^" << endl;

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
			figure->getTypeStr();
			cout << " with coordinates ";
			figure->getCoordinatesStr();
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

	bool isEmpty() {
		return figures.empty();
	}

	void save() {
		string directory;
		string filename;
		ofstream file;

		cout << "enter a full directory (with filename) to save the file in (ot 'this' to save in the current directory): ";
		getline(cin, directory);

		if (directory == "this") {
			cout << "how do you want to call your file?: ";
			getline(cin, filename);

			file.open(filename);
		}
		else {
			file.open(directory);
		}

		if (file.is_open()) {
			print(file);
			file.close();
		}
		else {
			cout << "cannot open the file!!!!!!!!!!!" << endl;
			return;
		}
			cout << "saved successfully!!" << endl;
	}

	/*void save() {
		string directory;
		string filename;
		ofstream file;

		cout << "enter a full directory (with filename) to save the file in (ot 'this' to save in the current directory): ";
		getline(cin, directory);

		if (directory == "this") {
			cout << "how do you want to call your file?: ";
			getline(cin, filename);

			file.open(filename);
		}
		else {
			file.open(directory);
		}

		if (file.is_open()) {
			for (auto figure : figures) {
				file << figure->getType() << " "
					<< figure->getX() << " "
					<< figure->getY() << " "
					<< figure->getHeight() << endl;
			}
		}

		else {
			cout << "cannot open the file!!!!!!!!!!!" << endl;
			return;
		}

		file.close();
		cout << "saved successfully!!" << endl;
	}*/


	/*void load() {
		string directory;
		string filename;

		cout << "enter a full directory (with filename) to load the file in (ot 'this' to load from the current directory): ";
		getline(cin, directory);

		if (directory == "this") {
			cout << "enter filename you want to load: ";
			getline(cin, filename);

			ifstream file(filename);

			if (file.is_open()) {
				readFromFile(file);

				file.close();
			}
			else {
				cout << "cannot open the file!!!!!!!!!!!" << endl;
				return;
			}

		}
		else {

			ifstream file(directory);

			if (file.is_open()) {
				readFromFile(file);
				drawFigures();
				file.close();
			}
			else {
				cout << "cannot open the file!!!!!!!!!!!" << endl;
				return;
			}
		}

	}

	void readFromFile(ifstream& file) {
		figures.clear();
		clear();

		string currentLine;

		while (getline(file, currentLine)) {
			string type = "";
			stringstream ss(currentLine);
			ss >> type;

			if (type == "line") {
				int x, y, x2, y2;
				Line* line = new Line(*this, x, y, x2, y2);
				addFigure(line);
			}
			else if (type == "triangle") {
				int x, y, height;
				Triangle* triangle = new Triangle(*this, x, y, height);
				addFigure(triangle);
			}
			if (type == "square") {
				int x, y, height;
				Square* square = new Square(*this, x, y, height);
				addFigure(square);
			}
		}
	}*/
};

class Line : public Figure {

	vector<vector<char>>* grid;
	int X2;
	int Y2;

public:
	Line(Board& board, int x, int y, int x2, int y2, string color) : Figure(board) {
		X = x; Y = y; X2 = x2; Y2 = y2; TYPE = "line", COLOR = color; grid = &board.getGrid();
	};

	void draw() {
		char fillColor = this->checkColor();

		if (Y == Y2) {
			int startX = min(X, X2);
			int endX = max(X, X2);
			for (int i = startX; i <= endX; ++i) {
				if (i >= 0 && i < BOARD_WIDTH && Y >= 0 && Y < BOARD_HEIGHT) {
					(*grid)[Y][i] = fillColor;
				}
			}
		}
		else if (X == X2) {
			int startY = min(Y, Y2);
			int endY = max(Y, Y2);
			for (int i = startY; i <= endY; ++i) {
				if (X >= 0 && X < BOARD_WIDTH && i >= 0 && i < BOARD_HEIGHT) {
					(*grid)[i][X] = fillColor;
				}
			}
		}
	}
};

class Square : public Figure {

	vector<vector<char>>* grid;

public:
	Square(Board& board, int x, int y, int height, string color) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "square", COLOR = color; grid = &board.getGrid();;
	};

	void draw() {
		if (HEIGHT <= 0) return;
		string isFilled = this->getColor();

		if (isFilled == "none") {
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
						(*grid)[posY][posX] = '*';
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

					(*grid)[posY][posX] = fillColor;
				}
			}

		}	
	}
};

class Triangle : public Figure {

	vector<vector<char>>* grid;

public:

	Triangle(Board& board, int x, int y, int height, string color) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "triangle"; COLOR = color; grid = &board.getGrid();
	};

	void draw() {
		if (HEIGHT <= 0) return;		
		char fillColor = this->checkColor();

		for (int i = 0; i < HEIGHT; ++i) {
			int leftMost = X - i;
			int rightMost = X + i;
			int posY = Y + i;
			if (posY < BOARD_HEIGHT) {
				if (leftMost >= 0 && leftMost < BOARD_WIDTH)
					(*grid)[posY][leftMost] = fillColor;
				if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
					(*grid)[posY][rightMost] = fillColor;
			}
		}
		for (int j = 0; j < 2 * HEIGHT - 1; ++j) {
			int baseX = X - HEIGHT + 1 + j;
			int baseY = Y + HEIGHT - 1;
			if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT)
				(*grid)[baseY][baseX] = fillColor;
		}
	}
};

class Circle : public Figure {

	vector<vector<char>>* grid;

public:
	Circle(Board& board, int x, int y, int height, string color) : Figure(board) {
		X = x; Y = y; HEIGHT = height, TYPE = "circle", COLOR = color; grid = &board.getGrid();;
	};

	void draw() {
		if (HEIGHT <= 0) return;

		int diameter = HEIGHT;
		int radius = diameter / 2;

		for (int i = 0; i <= diameter; ++i) {
			for (int j = 0; j <= diameter; j++) {
				double centreDistance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));

				if (centreDistance >= radius - 0.5 && centreDistance <= radius + 0.5) {
					int posX = X + j;
					int posY = Y + i;
					if (posX >= 0 && posX < BOARD_WIDTH && posY < BOARD_HEIGHT) {
						(*grid)[posX][posY] = '*';
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
		cout << "   -- has four coordinates (x, y, x2, y2)" << endl;
		cout << "   -- example: 5, 10, 9, 10" << endl;
		cout << "     (draws a line starting at position (5,10) and ending at (9, 10). can draw only horizontal or vertical lines)" << endl;
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
		
		cout << "3. circle" << endl;
		cout << "   -- has two coordinates (x, y) and height (diameter)" << endl;
		cout << "   -- example: 10, 20, 10" << endl;
		cout << "     (draws a circle starting at position (10,20) with a diameter of 10)" << endl;
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
		
		cout << "5. save" << endl;
		cout << "   -- description: saves the board to the chosen file" << endl;
		cout << endl;

		cout << "6. exit" << endl;
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
				Line* line = new Line(board, x, y, x2, y2, color);
				board.addFigure(line);
				cout << "new line was added!" << endl;
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


			Triangle* triangle = new Triangle(board, x, y, height, color);
			board.addFigure(triangle);
			cout << "new triangle was added!" << endl;
		}
		else if (userInput == 2) {
			cout << "enter height: ";
			cin >> height;
			cout << "enter fill color ('none' for transparent): ";
			cin >> color;
			cin.ignore();


			Square* square = new Square(board, x, y, height, color);
			board.addFigure(square);
			cout << "new square was added!" << endl;
		}
		
		else if (userInput == 3) {
			cout << "enter diameter: ";
			cin >> height;
			cout << "enter fill color ('none' for transparent): ";
			cin >> color;
			cin.ignore();


			Circle* circle = new Circle(board, x, y, height, color);
			board.addFigure(circle);
			cout << "new circle was added!" << endl;
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