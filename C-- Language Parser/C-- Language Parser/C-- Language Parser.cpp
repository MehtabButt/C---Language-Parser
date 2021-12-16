#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <list>
#include <algorithm>
#include <stack>
#include <sstream>

using namespace std;

class Token
{
private:
	string lexeme;
	string tokenName;

public:
	Token(string lex, string tName) {

		lexeme = lex;
		tokenName = tName;
	}

	Token() {
		lexeme = "";
		tokenName = "";
	}

	bool isIdentifier() {

		return (tokenName == "ID");
	}

	bool isEmpty() {
		return lexeme == "" && tokenName == "";
	}

	string getLexeme() {
		return lexeme;
	}

	string getTokenName() {
		return tokenName;
	}
};

class Reader {

private:

	string buffer;
	int beginPtr;
	int forwardPtr;
	int lineNumber;
	string filePath;

	bool checkFormat() {

		string fileName = "";
		int i = filePath.rfind("\\", filePath.length());
		if (i != string::npos) {

			fileName = filePath.substr(i + 1, filePath.length() - 1);
			if (regex_match(fileName, regex(".*\.cmm"))) {
				return true;
			}
		}
		return false;
	}

public:

	Reader() {

		ifstream source_file;

		//get file name from user
		while (true)
		{
			cout << "Enter the absolute path of file: ";
			getline(cin, filePath);


			//checking file format
			if (!checkFormat()) {

				cout << "Invalid file name: file format is incorrect\n";
				continue;
			}

			//opening file
			source_file.open(filePath);
			if (source_file.is_open()) {

				//reading file to buffer
				while (!source_file.eof()) {
					string line;
					getline(source_file, line);
					buffer += line + "\n";
				}
				buffer += "$";
				source_file.close();
				break;
			}
			else
			{
				cout << "file not found\n";
			}

		}


		//initializing begin and forward pointer
		beginPtr = 0;
		forwardPtr = 0;
		lineNumber = 1;
	}

	char getNextChar() {

		char ch = buffer[forwardPtr];
		forwardPtr++;
		return ch;
	}

	void retractForwardPointer() {

		if (forwardPtr != 0) {

			forwardPtr--;
		}
	}

	string resetBeginPointer() {

		string lexeme = "";

		while (beginPtr != forwardPtr)
		{
			if (buffer[beginPtr] == '\n')
				lineNumber++;
			lexeme.push_back(buffer[beginPtr]);
			beginPtr++;
		}
		return lexeme;
	}

	int getLineNumber() {

		return lineNumber;
	}

	bool isLastChar() {
		return forwardPtr == buffer.length();
	}

};

//global variables
Reader r;
int errorCount = 0;
int token_no = 0;
int space = 0;
string line_No;
stack<int> countOfProduction;

vector<string> terminals = {
	"INT","CHAR","IF","ELSE","WHILE","RETURN","WRITE","READ","+","-","*",
	"/","<=","<",">",">=","==","/=","ID","NC","VAR_DEC_OP","LC","STRING","OP",
	"CP","OB","CB","OSB","CSB","ASS_OP","INP_OP","SEMI_COLON","COMMA","$"
};

vector<string> nonTerminals{
	"program",
	"funcs",
	"func",
	"dec",
	"type",
	"params",
	"params_prime",
	"stmts",
	"stmt",
	"stmt_prime",
	"stmt_prime2",
	"stmt_prime3",
	"iden",
	"iden_prime",
	"ass",
	"value",
	"value_prime",
	"ari_expr",
	"T",
	"F",
	"ari_expr_prime",
	"T_prime",
	"cond",
	"cond_prime",
	"bool",
	"opr",
	"opr_prime",
	"rel",
	"open_bool",
	"print",
	"str",
	"str_prime",
	"input",
	"loop",
	"return",
	"array",
	"array_prime",
	"array_prime2",
	"seq",
	"num",
	"num_prime",
	"lit",
	"lit_prime"
};

int parseTabel[43][34] =
// INT CHAR IF ELSE WHILE RETURN WRITE READ + - * / <= < > >= == /= ID NC : LC STRING ( ) { } [ ] := >> ; , $
{
	/*"program",*/		  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	/*"funcs",*/		  {2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	/*"func",*/			  {4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"dec",*/			  {18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"type",*/			  {73,74,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"params",*/		  {5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0},
	/*"params'",*/		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,7,0},
	/*"stmts",*/		  {9,9,9,0,9,10,9,9,0,0,0,0,0,0,0,0,0,0,9,9,0,9,0,0,0,0,10,0,0,0,0,0,0,0},
	/*stmt*/			  {84,84,16,0,15,0,13,14,0,0,0,0,0,0,0,0,0,0,11,17,0,77,0,0,0,0,0,0,0,0,0,0,0,0},
	/*stmt'*/			  {76,76,76,0,76,0,76,76,76,76,0,0,76,76,76,76,76,76,76,76,0,76,0,0,0,0,0,0,0,75,0,0,0,0},
	/*stmt_prime2*/		  {0,0,0,0,0,0,0,0,85,85,0,0,85,85,85,85,85,85,0,0,0,0,0,0,8,0,0,12,0,85,0,0,0,0},
	/*stmt_prime3*/		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,86,0,0,0,87,87,0},
	/*"iden",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"iden'",*/		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,20,0},
	/*"ass",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"value",*/		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,24,0,25,0,0,0,0,0,0,0,0,0,0,0,0},
	/*value'*/			  {0,0,0,0,0,0,0,0,26,26,26,26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,78,0,0},
	/*"ari_expr",*/		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27,27,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"T",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"F",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"ari_expr'",*/	  {0,0,0,0,0,0,0,0,28,29,0,0,30,30,30,30,30,30,0,0,0,0,0,0,30,0,0,0,0,0,0,30,0,0},
	/*"T'",*/			  {0,0,0,0,0,0,0,0,34,34,32,33,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,0,0,0,0,34,0,0},
	/*"cond",*/			  {0,0,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"cond'",*/		  {39,39,39,38,39,0,39,39,0,0,0,0,0,0,0,0,0,0,39,39,0,39,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"bool",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40,40,0,40,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"opr",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,41,42,0,43,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"opr'",*/			  {0,0,0,0,0,0,0,0,44,44,0,0,45,45,45,45,45,45,0,0,0,0,0,0,45,0,0,0,0,0,0,45,0,0},
	/*"rel",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,46,48,49,47,50,51,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"open_bool",*/	  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,52,53,0,54,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"print",*/		  {0,0,0,0,0,0,55,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"str",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,58,0,0,57,0,0,0,0,0,0,0,0,0,0,0},
	/*str'*/			  {0,0,0,0,0,0,0,0,60,60,60,60,0,0,0,0,0,0,0,0,0,0,0,0,59,0,0,0,0,0,0,0,0,0},
	/*"input",*/		  {0,0,0,0,0,0,0,61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"loop",*/			  {0,0,0,0,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"return",*/		  {0,0,0,0,0,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"array",*/		  {64,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,79,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*array_prime*/		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,81,0,80,0,0},
	/*array_prime2*/	  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,82,0,0,0,0,0,0,0,0,83,0,0,0,0,0},
	/*"seq",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,66,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"num",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,67,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"num'",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,69,0,0,0,0,0,68,0},
	/*"lit",*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,0,0,0,0,0,0,0,0,0,0,0,0},
	/*"lit'"*/			  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,72,0,0,0,0,0,71,0}
};

bool isLetterOrDigit(char ch) {

	string s;
	s.push_back(ch);
	return regex_match(s, regex("[0-9a-zA-Z]"));
}

bool isLetter(char ch) {
	string s;
	s.push_back(ch);
	return regex_match(s, regex("[a-zA-Z]"));
}

bool isDigit(char ch) {
	string s;
	s.push_back(ch);
	return regex_match(s, regex("[0-9]"));
}

Token getToken() {

	int state = 0;
	bool found = false;
	string error = "";
	int errorLine;
	Token t;
	char ch;

	while (true)
	{
	jump:
		switch (state) {

		case 0:
			ch = r.getNextChar();
			if (ch == ' ' || ch == '\n') {
				if (ch == '\n') {
					if (error != "") {
						errorCount++;
						cout << "line number " << errorLine << ": " << error << " ,Illegal character found\n";
						error = "";
					}
				}
				r.resetBeginPointer();
				goto jump;
			}
			else if (ch == 'i') {
				state = 1;
				goto jump;
			}
			else if (ch == 'e') {
				state = 5;
				goto jump;
			}
			else if (ch == 'w') {
				state = 9;
				goto jump;
			}
			else if (ch == 'r') {
				state = 18;
				goto jump;
			}
			else if (ch == 'c') {
				state = 26;
				goto jump;
			}
			else if (isLetter(ch)) {
				state = 30;
				goto jump;
			}
			else if (ch == '+' || ch == '-') {
				state = 31;
				goto jump;
			}
			else if (isDigit(ch)) {
				state = 39;
				goto jump;
			}
			else if (ch == (char)39) {
				state = 32;
				goto jump;
			}
			else if (ch == (char)34) {
				state = 34;
				goto jump;
			}
			else if (ch == '<') {
				state = 35;
				goto jump;
			}
			else if (ch == '>') {
				state = 35;
				goto jump;
			}
			else if (ch == '=') {
				state = 36;
				goto jump;
			}
			else if (ch == '/') {
				state = 37;
				goto jump;
			}
			else if (ch == '*')
			{
				t = Token(r.resetBeginPointer(), "ARI_OP");  //ARI_OP
				found = true;
			}
			else if (ch == ':') {
				state = 38;
				goto jump;
			}
			else if (ch == ',') {
				t = Token(r.resetBeginPointer(), "COMMA"); //COMMA
				found = true;
			}
			else if (ch == ';')
			{
				t = Token(r.resetBeginPointer(), "SEMI_COLON"); //SEMI COLON
				found = true;
			}
			else if (ch == '(')
			{
				t = Token(r.resetBeginPointer(), "OP"); //OPENING PRENTHESIS
				found = true;
			}
			else if (ch == ')')
			{
				t = Token(r.resetBeginPointer(), "CP"); //CLOSING PRENTHESIS
				found = true;
			}
			else if (ch == '{')
			{
				t = Token(r.resetBeginPointer(), "OB"); //OPENING BRACES
				found = true;
			}
			else if (ch == '}')
			{
				t = Token(r.resetBeginPointer(), "CB"); //CLOSING BRACES
				found = true;
			}
			else if (ch == '[')
			{
				t = Token(r.resetBeginPointer(), "OSB"); //OPENING SQUARE BRACKET
				found = true;
			}
			else if (ch == ']')
			{
				t = Token(r.resetBeginPointer(), "CSB"); //CLOSING SQUARE BRACKET
				found = true;
			}
			else if (ch == '$') {

				if (r.isLastChar()) {
					state = 1000;
					goto jump;
				}
			}
			break;
			//IF
		case 1:
			ch = r.getNextChar();
			if (ch == 'f') {
				state = 2;
				goto jump;
			}
			else if (ch == 'n') {
				state = 3;
				goto jump;
			}
			else if (isLetterOrDigit(ch))
			{
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 2:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "IF");
				found = true;
			}
			break;

			//INT
		case 3:
			ch = r.getNextChar();
			if (ch == 't') {
				state = 4;
				goto jump;
			}
			else if (isLetterOrDigit(ch))
			{
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 4:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "INT");
				found = true;
			}
			break;

			//ELSE
		case 5:
			ch = r.getNextChar();
			if (ch == 'l') {
				state = 6;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 6:
			ch = r.getNextChar();
			if (ch == 's') {
				state = 7;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 7:
			ch = r.getNextChar();
			if (ch == 'e') {
				state = 8;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 8:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "ELSE");
				found = true;
			}
			break;

			//WHILE
		case 9:
			ch = r.getNextChar();
			if (ch == 'h') {
				state = 10;
				goto jump;
			}
			if (ch == 'r') {
				state = 14;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 10:
			ch = r.getNextChar();
			if (ch == 'i') {
				state = 11;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 11:
			ch = r.getNextChar();
			if (ch == 'l') {
				state = 12;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 12:
			ch = r.getNextChar();
			if (ch == 'e') {
				state = 13;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 13:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "WHILE");
				found = true;
			}
			break;

			//WRITE
		case 14:
			ch = r.getNextChar();
			if (ch == 'i') {
				state = 15;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 15:
			ch = r.getNextChar();
			if (ch == 't') {
				state = 16;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 16:
			ch = r.getNextChar();
			if (ch == 'e') {
				state = 17;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 17:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "WRITE");
				found = true;
			}
			break;

			//RETURN
		case 18:
			ch = r.getNextChar();
			if (ch == 'e') {
				state = 19;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 19:
			ch = r.getNextChar();
			if (ch == 't') {
				state = 20;
				goto jump;
			}
			else if (ch == 'a') {
				state = 24;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 20:
			ch = r.getNextChar();
			if (ch == 'u') {
				state = 21;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 21:
			ch = r.getNextChar();
			if (ch == 'r') {
				state = 22;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 22:
			ch = r.getNextChar();
			if (ch == 'n') {
				state = 23;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 23:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "RETURN");
				found = true;
			}
			break;

			//READ
		case 24:
			ch = r.getNextChar();
			if (ch == 'd') {
				state = 25;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 25:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "READ");
				found = true;
			}
			break;

			//CHAR
		case 26:
			ch = r.getNextChar();
			if (ch == 'h') {
				state = 27;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 27:
			ch = r.getNextChar();
			if (ch == 'a') {
				state = 28;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 28:
			ch = r.getNextChar();
			if (ch == 'r') {
				state = 29;
				goto jump;
			}
			else if (isLetterOrDigit(ch)) {
				//modify state 
				state = 30;
				goto jump;
			}
			else
			{
				//modify
				r.retractForwardPointer();
				state = 30;
				goto jump;
			}
			break;
		case 29:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				//modify state
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "CHAR");
				found = true;
			}
			break;

			//ID
		case 30:
			ch = r.getNextChar();
			if (isLetterOrDigit(ch)) {
				state = 30;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "ID");
				found = true;
			}
			break;

			//NUM & ARI_OP
		case 31:
			ch = r.getNextChar();
			if (isDigit(ch)) {
				state = 39;
				goto jump;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "ARI_OP");
				found = true;
			}
			break;

			//LITERAL CONSTANT
		case 32:
			ch = r.getNextChar();
			if (isLetter(ch)) {
				state = 33;
				goto jump;
			}
			break;
		case 33:
			ch = r.getNextChar();
			if (ch == (char)39) {
				t = Token(r.resetBeginPointer(), "LC");
				found = true;
			}
			break;

			//STRING
		case 34:
			ch = r.getNextChar();
			while (ch != (char)34) {
				if (ch == '$' && r.isLastChar()) {
					errorCount++;
					r.retractForwardPointer();
					errorLine = r.getLineNumber();
					error = r.resetBeginPointer();
					error = error.substr(0, 5);
					replace(error.begin(), error.end(), '\n', ' ');
					cout << "line number " << errorLine << ": " << error << "..." << " ,string not closed\n";
					error = "";
					state = 0;
					goto jump;
				}
				ch = r.getNextChar();
			}
			t = Token(r.resetBeginPointer(), "STRING");
			found = true;
			break;

			//REL_OP
		case 35:
			ch = r.getNextChar();
			if (ch == '=') {
				t = Token(r.resetBeginPointer(), "REL_OP");
				found = true;
			}
			else if (ch == '>') {
				t = Token(r.resetBeginPointer(), "INP_OP");
				found = true;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "REL_OP");
				found = true;
			}
			break;
		case 36:
			ch = r.getNextChar();
			if (ch == '=') {
				t = Token(r.resetBeginPointer(), "REL_OP");
				found = true;
			}
			break;

			//COMMENT
		case 37:
			ch = r.getNextChar();
			if (ch == '*') {
				ch = r.getNextChar();
				while (true)
				{
					if (ch == '*') {
						ch = r.getNextChar();
						if (ch == '/') {
							r.resetBeginPointer();
							state = 0;
							goto jump;
						}
					}
					else if (ch == '$' && r.isLastChar()) {
						errorCount++;
						r.retractForwardPointer();
						errorLine = r.getLineNumber();
						error = r.resetBeginPointer();
						error = error.substr(0, 5);
						replace(error.begin(), error.end(), '\n', ' ');
						cout << "line number " << errorLine << ": " << error << "..." << " ,comment not closed\n";
						error = "";
						state = 0;
						goto jump;
					}
					ch = r.getNextChar();
				}

			}
			else if (ch == '=') {
				t = Token(r.resetBeginPointer(), "REL_OP");
				found = true;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "ARI_OP");
				found = true;
			}

			break;
			//ASS_OP & VAR_DEC_OP
		case 38:
			ch = r.getNextChar();
			if (ch == '=') {
				t = Token(r.resetBeginPointer(), "ASS_OP");
				found = true;
			}
			else
			{
				r.retractForwardPointer();
				t = Token(r.resetBeginPointer(), "VAR_DEC_OP");
				found = true;
			}
			break;
		case 39:
			ch = r.getNextChar();
			if (isDigit(ch)) {
				ch = r.getNextChar();
				while (isDigit(ch)) {
					ch = r.getNextChar();
				}
			}
			r.retractForwardPointer();
			t = Token(r.resetBeginPointer(), "NC");
			found = true;
			break;
		default:
			t = Token("", "");
			found = true;
			break;
		}

		if (found) {
			if (error != "") {
				errorCount++;
				cout << "line number " << errorLine << ": " << error << " ,Illegal character found" << "\n";
			}
			return t;
		}
		else
		{
			if (ch == ' ' || ch == '\n')
			{
				r.retractForwardPointer();
			}
			if (error == "") {
				errorLine = r.getLineNumber();
			}
			error += r.resetBeginPointer();
			state = 0;
		}


	}

}

void tokenize() {

	Token t;
	string token;
	list<string> lexemes;
	list<string>::iterator itr;
	ofstream word("words.txt");
	ofstream symb("symboltable.txt");

	if (word.is_open()) {

		/*word << "Lexeme\tToken_Name\n";
		word << "----------------------------------------\n";*/
		symb << "Identifiers\n";
		symb << "--------------------------\n";

		while (true)
		{
			t = getToken();

			if (t.isEmpty()) {
				break;
			}

			word << r.getLineNumber() << " " << t.getLexeme() + "\t\t" + t.getTokenName() + "\n";

			itr = find(lexemes.begin(), lexemes.end(), t.getLexeme());
			if (itr == lexemes.end()) {
				lexemes.push_back(t.getLexeme());

				//addind unique tokens to symbol table

				if (t.getTokenName() == "ID") {
					if (symb.is_open()) {
						symb << t.getLexeme() + "\n";
					}
				}
			}

		}
		if (errorCount > 0)
			cout << "error count: " << errorCount << "\n";

		word << "100 " << "ENDFILE\t\t$\n";
		word.close();
		symb.close();
	}

}

Token getNextToken() {

	string lexeme = "";
	string token = "";
	Token t;
	ifstream tokens("words.txt");

	for (int i = 0; i < token_no; i++) {
		getline(tokens, lexeme);
	}
	lexeme = "";

	//save line number
	if (!tokens.eof()) {
		tokens >> line_No;
	}

	char ch;
	tokens.get(ch);
	auto pos = tokens.tellg();
	if (!tokens.eof()) {

		tokens.get(ch);
	}
	if (ch == (char)34) {
		lexeme = lexeme + ch;
		if (!tokens.eof()) { tokens.get(ch); }
		while (ch != (char)34 && !tokens.eof()) {
			lexeme = lexeme + ch;
			if (!tokens.eof()) { tokens.get(ch); }
		}
		lexeme = lexeme + ch;
	}
	else
	{
		tokens.seekg(pos);
		if (!tokens.eof()) {

			tokens >> lexeme;
		}
	}

	if (!tokens.eof()) {
		tokens >> token;
	}
	t = Token(lexeme, token);
	token_no++;

	return t;
}

void putTokenBack() {
	token_no--;
}

int getIndexOfTerminal(string ter) {

	auto ind = find(terminals.begin(), terminals.end(), ter);
	if (ind != terminals.end()) {
		return ind - terminals.begin();
	}
	return -1;
}

int getIndexOfNonTerminal(string nonTer) {

	auto ind = find(nonTerminals.begin(), nonTerminals.end(), nonTer);
	if (ind != nonTerminals.end()) {
		return ind - nonTerminals.begin();
	}
	return -1;
}

vector<string> getProduction(int index) {
	ifstream grammer("grammer.txt");
	string line, tmp;
	vector<string> words;
	while (index != 0) {
		getline(grammer, line);
		index--;
	}
	stringstream s(line);
	getline(s, tmp, ' ');
	while (getline(s, tmp, ' ')) {
		if (tmp != "") {
			words.push_back(tmp);
		}
	}
	reverse(words.begin(), words.end());

	return words;
}

void drawParseTree(string str) {
	ofstream parsetree;
	parsetree.open("parsetree.txt", ios::app);
	if (parsetree.is_open()) {
		parsetree << str << endl;
		if (str != "$") {
			for (int i = 0; i < space; i++) {
				parsetree << "|  ";
			}
			space++;
			parsetree << "|__";
		}
		parsetree.close();
	}
	else
	{
		cout << "Unable to open parsetree.txt\n";
	}

}

void insertIntoSymbolTable(string identifier, string dataType) {

	ofstream symbolTable;
	symbolTable.open("parser_symboltable.txt", std::ios::app);
	if (symbolTable.is_open()) {

		symbolTable << identifier << "\t" << dataType << endl;
		symbolTable.close();
	}
	else
	{
		cout << "Unable to open parser_symboltable";
	}
}

void generate_parse_tree() {

	stack<string> s;
	Token t;
	list<string> identifiers;
	list<string>::iterator itr;
	string top;
	string dataType;
	int ter_ind;
	int nonTer_ind;
	vector<string> production;
	s.push("$");
	s.push("program");

	// creating new files 
	ofstream f("parser_symboltable.txt");
	f.close();
	f.open("parsetree.txt");
	f.close();

	while (true)
	{
		if (s.empty()) {
			break;
		}

		t = getNextToken();
		top = s.top();
		s.pop();
		if (!countOfProduction.empty()) {
			int i = countOfProduction.top();
			countOfProduction.pop();
			i--;
			countOfProduction.push(i);
		}

		if (t.getTokenName() == top || top == "^") {

			if (top == "INT" || top == "CHAR") {
				dataType = top;
			}
			if (t.getTokenName() == "ID") {
				itr = find(identifiers.begin(), identifiers.end(), t.getLexeme());
				if (itr == identifiers.end()) {
					identifiers.push_back(t.getLexeme());
					insertIntoSymbolTable(t.getLexeme(), dataType);
				}
			}
			space--;

			while (!countOfProduction.empty() && countOfProduction.top() == 0) {
				countOfProduction.pop();
				space--;
			}

			if (top == "^") {
				putTokenBack();
			}
			drawParseTree(top);
		}
		else if (t.getTokenName() == "ARI_OP" && t.getLexeme() == top) {
			space--;

			while (!countOfProduction.empty() && countOfProduction.top() == 0) {
				countOfProduction.pop();
				space--;
			}

			if (top == "^") {
				putTokenBack();
			}
			drawParseTree(top);
		}
		else if (t.getTokenName() == "REL_OP" && t.getLexeme() == top) {
			space--;

			while (!countOfProduction.empty() && countOfProduction.top() == 0) {
				countOfProduction.pop();
				space--;
			}

			if (top == "^") {
				putTokenBack();
			}
			drawParseTree(top);
		}
		else
		{
			drawParseTree(top);
			if (t.getTokenName() == "ARI_OP") { ter_ind = getIndexOfTerminal(t.getLexeme()); }
			else if (t.getTokenName() == "REL_OP") { ter_ind = getIndexOfTerminal(t.getLexeme()); }
			else
			{
				ter_ind = getIndexOfTerminal(t.getTokenName());
			}
			nonTer_ind = getIndexOfNonTerminal(top);
			int i = parseTabel[nonTer_ind][ter_ind];
			if (i == 0) {
				cout << "syntax error at line number: " + line_No + "\n";
				exit(0);
			}
			production = getProduction(i);
			for (string str : production) {
				s.push(str);
			}
			countOfProduction.push(production.size());
			putTokenBack();
		}


	}
}

int main() {

	tokenize();
	cout << endl;
	generate_parse_tree();

	cout << "success";

}