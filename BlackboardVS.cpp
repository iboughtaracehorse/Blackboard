#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

vector<vector<char>> grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' '));

class Figure {

protected:
	int X;
	int Y;
	int HEIGHT;
	string COLOR;
	string TYPE;
	string ALIAS;

public:

	Figure() { X = 0; Y = 0; HEIGHT = 0; COLOR = "none"; ALIAS = ""; }

	virtual void draw(vector<vector<char>>& grid) = 0;

	virtual ~Figure() {};

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
	virtual string getAlias() {
		return ALIAS;
	}
	virtual void listSpecific() {}

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
	
	void setColor(string color) {
		this->COLOR = color;
	}

	void editHeight(int coefficient) {
		this->HEIGHT *= coefficient;
	}
	virtual void editCoordinates(int newX, int newY, int newX2, int newY2) = 0;
};

class Line : public Figure {

	int X2;
	int Y2;

public:
	Line(int x, int y, int x2, int y2, string color, string alias) : Figure() {
		X = x; Y = y; X2 = x2; Y2 = y2; TYPE = "Line", COLOR = color, ALIAS = alias;
	};

	void draw(vector<vector<char>>& grid) {
		char fillColor = this->checkColor();

		if (Y == Y2) {
			int startX = min(X, X2);
			int endX = max(X, X2);
			for (int i = startX; i <= endX; ++i) {
				if (i >= 0 && i < BOARD_WIDTH && Y >= 0 && Y < BOARD_HEIGHT) {
					grid[Y][i] = fillColor;
				}
			}
		}
		else if (X == X2) {
			int startY = min(Y, Y2);
			int endY = max(Y, Y2);
			for (int i = startY; i <= endY; ++i) {
				if (X >= 0 && X < BOARD_WIDTH && i >= 0 && i < BOARD_HEIGHT) {
					grid[i][X] = fillColor;
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

	void listSpecific() {
		cout << ALIAS << " " 
			<< TYPE << " "
			<< COLOR << " "
			<< X << " " << Y << " " << X2 << " " << Y2 << " "
			<< HEIGHT << endl;
	}

	void editCoordinates(int newX, int newY, int newX2, int newY2) {
		if (X == X2 || Y == Y2) {
			this->X = newX;
			this->X2 = newX2;
			this->Y = newY;
			this->Y2 = newY2;
		}
		else {
			cout << "invalid coordinates. check 'help' and try again" << endl;
		}
	}
	
};

class Square : public Figure {

public:
	Square(int x, int y, int height, string color, string alias) : Figure() {
		X = x; Y = y; HEIGHT = height, TYPE = "Square", COLOR = color, ALIAS = alias;
	};

	void draw(vector<vector<char>>& grid) {
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
						grid[posY][posX] = '*';
					}

					else if (j == 0 || j == HEIGHT - 1) {
						grid[posY][posX] = '*';
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

					grid[posY][posX] = fillColor;
				}
			}

		}
	}

	void listSpecific() {
		cout << ALIAS << " "
			<< TYPE << " "
			<< COLOR << " "
			<< X << " " << Y << " "
			<< HEIGHT << endl;
	}

	void editCoordinates(int newX, int newY, int newX2, int newY2) {
		this->X = newX;
		this->Y = newY;
	}
};

class Triangle : public Figure {

public:

	Triangle(int x, int y, int height, string color, string alias) : Figure() {
		X = x; Y = y; HEIGHT = height, TYPE = "Triangle"; COLOR = color, ALIAS = alias;
	};

	void draw(vector<vector<char>>& grid) {
		if (HEIGHT <= 0) return;

		string isFilled = this->getColor();

		if (isFilled == "none") {
			for (int i = 0; i < HEIGHT; ++i) {
				int leftMost = X - i;
				int rightMost = X + i;
				int posY = Y + i;
				if (posY < BOARD_HEIGHT && posY >= 0) {
					if (leftMost >= 0 && leftMost < BOARD_WIDTH)
						grid[posY][leftMost] = '*';
					if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
						grid[posY][rightMost] = '*';
				}
			}
			for (int j = 0; j < 2 * HEIGHT - 1; ++j) {
				int baseX = X - HEIGHT + 1 + j;
				int baseY = Y + HEIGHT - 1;
				if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT)
					grid[baseY][baseX] = '*';
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
							grid[posY][position] = fillColor;
						}
					}
				}
			}
		}
	}

	void listSpecific() {
		cout << ALIAS << " "
			<< TYPE << " "
			<< COLOR << " "
			<< X << " " << Y << " "
			<< HEIGHT << endl;
	}

	void editCoordinates(int newX, int newY, int newX2, int newY2) {
		this->X = newX;
		this->Y = newY;
	}
};

class Circle : public Figure {

public:
	Circle(int x, int y, int height, string color, string alias) : Figure() {
		X = x; Y = y; HEIGHT = height, TYPE = "circle", COLOR = color, ALIAS = alias;
	};

	void draw(vector<vector<char>>& grid) {
		if (HEIGHT <= 0) return;

		string isFilled = this->getColor();

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
							grid[posX][posY] = '*';
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
							grid[posX][posY] = fillColor;
						}
					}
				}
			}
		}
	}

	void listSpecific() {
		cout << ALIAS << " "
			<< TYPE << " "
			<< COLOR << " "
			<< X << " " << Y << " "
			<< HEIGHT << endl;
	}

	void editCoordinates(int newX, int newY, int newX2, int newY2) {
		this->X = newX;
		this->Y = newY;
	}
};

struct Board {

	vector<vector<char>> grid;
	vector<Figure*> figures;
	Figure* selectedFigure = nullptr;

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
			figure->draw(grid);
		}
	}

	void list(ostream& target) {
		if (isEmpty()) {
			cout << endl << "oops! your board must be empty! try 'add' to add new figures!" << endl << endl;
			return;
		}

		for (auto& figure : figures) {
			if (Line* line = dynamic_cast<Line*>(figure)) {
				target << "Line " << line->getAlias() << " " << line->getX() << " " << line->getY()
					<< " " << line->getX2() << " " << line->getY2() << " " << line->getColor() << endl;
			}
			else if (Triangle* triangle = dynamic_cast<Triangle*>(figure)) {
				target << "Triangle " << triangle->getAlias() << " " << triangle->getX() << " " << triangle->getY()
					<< " " << triangle->getHeight() << " " << triangle->getColor() << endl;
			}
			else if (Square* square = dynamic_cast<Square*>(figure)) {
				target << "Square " << square->getAlias() << " " << square->getX() << " " << square->getY()
					<< " " << square->getHeight() << " " << square->getColor() << endl;
			}
			else if (Circle* circle = dynamic_cast<Circle*>(figure)) {
				target << "Circle " << circle->getAlias() << " " << circle->getX() << " " << circle->getY()
					<< " " << circle->getHeight() << " " << circle->getColor() << endl;
			}
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
			print(file);
			file.close();
		}
		else {
			cout << "cannot open the file!!!!!!!!!!!" << endl;
			return;
		}
		cout << "saved successfully!!" << endl;
	}*/

	void save() {
		string directory;
		string filename;
		ofstream file;

		if (isEmpty()) {
			cout << endl << "oops! your board must be empty! try 'add' to add new figures!" << endl << endl;
			return;
		}

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
			list(file);
		}

		else {
			cout << "cannot open the file!!!!!!!!!!!" << endl;
			return;
		}

		file.close();
		cout << "saved successfully!!" << endl;
	} // DO WITH LIST

	void load() {
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
			string type, alias;
			stringstream ss(currentLine);
			ss >> type >> alias;

			if (type == "line") {
				int x, y, x2, y2;
				string color;
				ss >> x >> y >> x2 >> y2 >> color;
				Line* line = new Line(x, y, x2, y2, color, alias);
				addFigure(line);
			}
			else if (type == "triangle") {
				int x, y, height;
				string color;
				ss >> x >> y >> height >> color;
				Triangle* triangle = new Triangle(x, y, height, color, alias);
				addFigure(triangle);
			}
			else if (type == "square") {
				int x, y, height;
				string color;
				ss >> x >> y >> height >> color;
				Square* square = new Square(x, y, height, color, alias);
				addFigure(square);
			}
		}
	}

	Figure* select(stringstream& ss) {

		if (figures.empty()) {
			cout << "nothing on the board yet!" << endl;
			return nullptr;
		}

		string alias;
		ss >> alias;

		for (auto& figure : figures) {
			if (figure->getAlias() == alias) {
				cout << alias << " selected successfully!" << endl;
				selectedFigure = figure;
				return figure;
			}
		}
		cout << "no such figure!" << endl;
		return nullptr;
	}

	Figure* getSelected() {
		return selectedFigure;
	}

	void showSelected() {
		cout << "currently selected: ";
		selectedFigure->listSpecific();
	}

	void remove(Figure* figure) {
		selectedFigure = nullptr;
	
		auto i = find(figures.begin(), figures.end(), figure); //int didnt workout

		if (i != figures.end()) {
			figures.erase(i);
			cout << "selected figure removed successfully! nothing is selected now" << endl;
		}
	}

	void repaint(Figure* figure, string color) {
		figure->setColor(color);
		cout << "color changed to: " << color << endl;
	}

	void edit(Figure* figure, int coefficient) {
		figure->editHeight(coefficient);
		cout << "height multiplied by: " << coefficient << endl;
	}

	void move(Figure* figure, stringstream& ss) {
		if (Line* line = dynamic_cast<Line*>(figure)) {
			int newX;
			int newY;
			int newX2;
			int newY2;
			ss >> newX >> newY >> newX2 >> newY2;

			if (ss.fail() || !ss.eof()) {
				cout << "error occured. invalid coordinates" << endl;
				return;
			}

			figure->editCoordinates(newX, newY, newX2, newY2);
			cout << "moved successfully" << endl;
		}
		else {
			int newX;
			int newY;
			ss >> newX >> newY;

			if (ss.fail() || !ss.eof()) {
				cout << "error occured. invalid coordinates" << endl;
				return;
			}

			figure->editCoordinates(newX, newY, 0, 0);
			cout << "moved successfully" << endl;
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
		
		cout << "6. select" << endl;
		cout << "   -- description: select the figure by its alias" << endl;
		cout << endl;
		
		cout << "7. select" << endl;
		cout << "   -- description: select the figure by its alias" << endl;
		cout << endl;
		
		cout << "6. repaint" << endl;
		cout << "   -- description: changes color of the figure. only if selected" << endl;
		cout << endl;

		cout << "7. edit" << endl;
		cout << "   -- description: multiplies height/diameter by coefficient in order to scale the figure. only if selected" << endl;
		cout << endl;
		
		cout << "8. edit" << endl;
		cout << "   -- description: changes coordinates. only if selected" << endl;
		cout << endl;

		cout << "9. exit" << endl;
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

	void commands(string& input) {
		string command;
		stringstream ss(input);
		ss >> command;
		
		if (command == "print") {
			board.drawFigures();
			board.print(cout);
		}
		else if (command == "list") {
			board.list(cout);
		}
		else if (command == "add") {
			addFigure(ss);
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
		else if (command == "select") {
			board.select(ss);
		}
		else if (command == "remove") {
			board.remove(board.getSelected());
		}
		else if (command == "repaint") {
			if (isSelected()) {
				board.repaint(board.getSelected(), askForColor(ss));
			}
		}
		else if (command == "edit") {
			if (isSelected()) {
				board.edit(board.getSelected(), askForCoefficient(ss));
			}
		}
		else if (command == "move") {
			if (isSelected()) {
				board.move(board.getSelected(), ss);
			}
		}
		else if (command == "load") {
			board.load();
		}
		else if (command == "help") {
			help.help();
		}
		else {
			cout << "unknown command! please try again" << endl;
		}
	}

	void addFigure(stringstream& ss) {
		string figure;
		int x = 0;
		int y = 0;
		int height = 0;
		int x2 = 0;
		int y2 = 0;
		string color;
		string alias;

		ss >> figure >> alias >> x >> y;

		if (figure == "line") {
			ss >> x2 >> y2 >> color;

			if (x == x2 || y == y2) {
				Line* line = new Line(x, y, x2, y2, color, alias);
				board.addFigure(line);
				cout << "new line named " << alias << " was added!" << endl;
			}
			else {
				cout << "only horizontal or vertical lines are available!!" << endl;
			}
		}

		else if (figure == "triangle") {
			ss >> height >> color;
			Triangle* triangle = new Triangle(x, y, height, color, alias);
			board.addFigure(triangle);
			cout << "new triangle named " << alias << " was added!" << endl;
		}

		else if (figure == "square") {
			ss >> height >> color;
			Square* square = new Square(x, y, height, color, alias);
			board.addFigure(square);
			cout << "new square named " << alias << " was added!" << endl;
		}

		else if (figure == "circle") {
			ss >> height >> color;
			Circle* circle = new Circle(x, y, height, color, alias);
			board.addFigure(circle);
			cout << "new circle named " << alias << " was added!" << endl;
		}
	}

	bool isSelected() {
		if (!board.selectedFigure) {
			cout << "oops! looks like you havent selected anything yet!" << endl;
			return false;
		}
		return true;
	}

	string askForColor(stringstream& ss) {
		board.showSelected();

		string color;
		ss >> color;

		return color;
	}

	int askForCoefficient(stringstream& ss) {
		board.showSelected();

		int input;
		ss >> input;

		return input;
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