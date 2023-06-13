#include <fstream>
#include <iostream>
#include<stdlib.h>
#include <windows.h>
#include<conio.h>
using namespace std;
const int DIM = 8, Black = 0, White = 1;

#define BLACK 0
#define BROWN 6
#define WHITE 15
#define GREEN 2
#define RED 4
#define LBLUE 9

void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}

void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void PrintBox(int sr, int sc, int rdim, int cdim, char sym)
{
	for (int r = 0; r < rdim; r++) {
		for (int c = 0; c < cdim; c++) {
			if (r == 0 || c == 0 || r == rdim - 1 || c == cdim - 1) {
				gotoRowCol(sr + r, sc + c);
				cout << sym;
			}
		}
	}
}

bool InsideBox(int sr, int sc, int rdim, int rpos, int cpos, int cdim) {
	return (rpos >= sr && rpos <= rdim + sr) && (cpos >= sc && cpos <= cdim + sc);
}

void Load(char B[][8], string fn)
{
	ifstream Rdr(fn);

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Rdr >> B[r][c];
		}
	}
}

void DisplayLoad() {
	PrintBox(3, 100, 5, 10, -37);
	gotoRowCol(5, 103);
	cout << "Load";
}

void DisplaySave() {
	PrintBox(13, 100, 5, 10, -37);
	gotoRowCol(15, 103);
	cout << "Save";
}

void Save(char B[][8], ofstream& Writer)
{

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Writer << B[r][c];
		}
		Writer << endl;
	}
}

void Save(char B[][8], string fn)
{
	ofstream Writer(fn);

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Writer << B[r][c];
		}
		Writer << endl;
	}
}

void Init(char board[][DIM], int turn, string Names[]) {
	srand((time(0)));
	for (int ri = 0; ri < DIM; ri++) {
		for (int ci = 0; ci < DIM; ci++) {
			if (ri >= 2 && ri <= 5) {
				board[ri][ci] = '-';
			}
			else if (ri == 1) {
				board[ri][ci] = 'p';
			}
			else if (ri == 6) {
				board[ri][ci] = 'P';
			}
		}
	}
	board[0][0] = board[0][7] = 'r';
	board[0][1] = board[0][6] = 'h';
	board[0][2] = board[0][5] = 'b';
	board[0][3] = 'q';
	board[0][4] = 'k';

	board[7][0] = board[7][7] = 'R';
	board[7][1] = board[7][6] = 'H';
	board[7][2] = board[7][5] = 'B';
	board[7][3] = 'Q';
	board[7][4] = 'K';

	cout << endl << "Enter name for black: "; cin >> Names[Black];
	cout << endl << "Enter name for white: "; cin >> Names[White];

	turn = rand() % 2;
}

void DisplayBoard(char board[][DIM]) {
	gotoRowCol(0, 0);
	cout << "      A   B   C   D   E   F   G   H";
	cout << "\n     _______________________________\n";
	for (int i = 0; i < 8; i++) {
		cout << i + 1 << "  ";
		for (int j = 0; j < 8; j++) {
			if (j == DIM - 1) {
				cout << " | " << board[i][j] << " | ";
			}
			else {
				cout << " | " << board[i][j];
			}
		}
		cout << endl << "    |___|___|___|___|___|___|___|___| " << endl;
	}
}

void TurnMsg(int turn, string AikNaam) {
	cout << endl << AikNaam << "'s Turn";
}

bool IsBlack(char piece) {
	return (piece >= 'a' && piece <= 'z');
}

bool IsWhite(char piece) {
	return (piece >= 'A' && piece <= 'Z');
}

bool IsMe(char piece, int turn) {
	if (turn == Black) {
		return IsBlack(piece);
	}
	else if (turn == White) {
		return IsWhite(piece);
	}
}

bool IsOpp(char piece, int turn) {
	if (turn == Black) {
		return IsWhite(piece);
	}
	else if (turn == White) {
		return IsBlack(piece);
	}
}

void Upper(char& col) {
	if (col >= 'a' && col <= 'z') {
		col -= 32;
	}
}

void SelectSource(char board[][DIM], int& sci, int& sri, int turn) {
	int r = 0, c = 0;
	getRowColbyLeftClick(r, c);
	r = (r - 2) / 2;
	c = (c - 6) / 4;
	sri = r;
	sci = c;
}

void SelectDest(char board[][DIM], int& dci, int& dri, int turn) {
	int r, c;
	getRowColbyLeftClick(r, c);
	r = (r - 2) / 2;
	c = (c - 6) / 4;
	dri = r;
	dci = c;
}

bool IsValidSource(int sci, int sri, char piece, int turn) {
	return(IsMe(piece, turn) && sci < DIM&& sri < DIM);
}

bool IsValidDest(int dci, int dri, char piece, int turn) {
	return(!IsMe(piece, turn) && dci < DIM&& dri < DIM);
}

void PieceMove(char board[][DIM], int sci, int sri, int dci, int dri) {
	char piece;
	piece = board[sri][sci];
	board[sri][sci] = '-';
	board[dri][dci] = piece;
}

void TurnChange(int& turn) {
	if (turn == White)
		turn = Black;
	else if (turn == Black)
		turn = White;
}

bool IsTopRight(int sri, int sci, int dri, int dci) {
	return dci > sci && dri < sri;
}

bool IsTopLeft(int sri, int sci, int dri, int dci) {
	return dci < sci&& dri < sri;
}

bool IsBottomRight(int sri, int sci, int dri, int dci) {
	return dci > sci && dri > sri;
}

bool IsBottomLeft(int sri, int sci, int dri, int dci) {
	return dci < sci&& dri > sri;
}

bool IsValidHorizontalMove(int sci, int sri, int dci, int dri) {
	return sri == dri;
}

bool IsValidVerticleMove(int sci, int sri, int dci, int dri) {
	return sci == dci;
}

bool IsValidDiagonalMove(int sci, int sri, int dci, int dri) {
	int Dc, Dr;
	Dc = abs(dci - sci);
	Dr = abs(dri - sri);
	return Dc == Dr;
}

bool IsHorizontalPathClear(char Board[][DIM], int sci, int sri, int dci, int dri) {
	int cStart, cEnd;
	if (dci > sci) {
		cStart = sci;
		cEnd = dci;
	}
	else {
		cStart = dci;
		cEnd = sci;
	}
	for (int c = cStart + 1; c < cEnd; c++) {
		if (Board[sri][c] != '-') {
			return false;
		}
	}
	return true;
}

bool IsVerticlePathClear(char Board[][DIM], int sci, int sri, int dci, int dri) {
	int rStart, rEnd;
	if (dri > sri) {
		rStart = sri;
		rEnd = dri;
	}
	else {
		rStart = dri;
		rEnd = sri;
	}
	for (int r = rStart + 1; r < rEnd; r++) {
		if (Board[r][sci] != '-') {
			return false;
		}
	}
	return true;
}

bool IsDiagonalPathClear(char Board[][DIM], int sri, int sci, int dri, int dci) {
	int Dr = abs(sri - dri), cStart, cEnd;
	for (int i = 1; i < Dr; i++) {
		if (IsTopRight(sri, sci, dri, dci)) {
			if (Board[sri - i][sci + i] != '-') {
				return false;
			}
		}
		else if (IsTopLeft(sri, sci, dri, dci)) {
			if (Board[sri - i][sci - i] != '-') {
				return false;
			}
		}
		else if (IsBottomRight(sri, sci, dri, dci)) {
			if (Board[sri + i][sci + i] != '-') {
				return false;
			}
		}
		else if (IsBottomLeft(sri, sci, dri, dci)) {
			if (Board[sri + i][sci - i] != '-') {
				return false;
			}
		}
	}
	return true;
}

bool IsValidRookMove(char board[][DIM], int sci, int sri, int dci, int dri) {
	return (IsValidVerticleMove(sci, sri, dci, dri) && IsVerticlePathClear(board, sci, sri, dci, dri)) || (IsValidHorizontalMove(sci, sri, dci, dri) && IsHorizontalPathClear(board, sci, sri, dci, dri));
}

bool IsValidBishopMove(char board[][DIM], int sci, int sri, int dci, int dri) {
	return (IsValidDiagonalMove(sci, sri, dci, dri) && IsDiagonalPathClear(board, sri, sci, dri, dci));
}

bool IsValidQueenMove(char board[][DIM], int sci, int sri, int dci, int dri) {
	return IsValidRookMove(board, sci, sri, dci, dri) || IsValidBishopMove(board, sci, sri, dci, dri);
}

bool IsValidKingMove(char board[][DIM], int sci, int sri, int dci, int dri) {
	int Dr = abs(sri - dri);
	int Dc = abs(sci - dci);
	return IsValidQueenMove(board, sci, sri, dci, dri) && Dr <= 1 && Dc <= 1;
}

bool IsValidHorseMove(char board[][DIM], int sci, int sri, int dci, int dri) {
	int Dr = abs(sri - dri);
	int Dc = abs(sci - dci);
	return (Dr == 2 && Dc == 1) || (Dr == 1 && Dc == 2);
}

bool IsValidBlackPawnMove(char board[][DIM], int sci, int sri, int dci, int dri, int turn) {
	int Dr = dri - sri;
	int Dc = abs(dci - sci);
	char Destpiece = board[dri][dci];

	if (Dr <= 0) {
		return false;
	}
	if (IsValidVerticleMove(sci, sri, dci, dri) && IsVerticlePathClear(board, sci, sri, dci, dri)) {
		if (sri == 1 && Dr <= 2 && Dc == 0 && Destpiece == '-') {
			return true;
		}
		else if (sri != 1 && Dr == 1 && Dc == 0 && Destpiece == '-') {
			return true;
		}
	}
	return (Dr == 1 && Dc == 1 && IsValidDiagonalMove(sci, sri, dci, dri) && IsOpp(Destpiece, turn));

}

bool IsValidWhitePawnMove(char board[][DIM], int sci, int sri, int dci, int dri, int turn) {
	int Dr = dri - sri;
	int Dc = abs(dci - sci);
	char Destpiece = board[dri][dci];

	if (Dr >= 0) {
		return false;
	}
	if (IsValidVerticleMove(sci, sri, dci, dri) && IsVerticlePathClear(board, sci, sri, dci, dri)) {
		if (sri == 6 && Dr >= -2 && Dc == 0 && Destpiece == '-') {
			return true;
		}
		else if (sri != 6 && Dr == -1 && Dc == 0 && Destpiece == '-') {
			return true;
		}
	}
	return (Dr == -1 && Dc == 1 && IsValidDiagonalMove(sci, sri, dci, dri) && IsOpp(Destpiece, turn));
}

bool IsLegalMove(char Board[][DIM], int sci, int sri, int dci, int dri, int turn) {
	switch (Board[sri][sci]) {
	case 'R':
	case 'r':
		return IsValidRookMove(Board, sci, sri, dci, dri);

	case 'B':
	case 'b':
		return IsValidBishopMove(Board, sci, sri, dci, dri);

	case 'Q':
	case 'q':
		return IsValidQueenMove(Board, sci, sri, dci, dri);

	case 'H':
	case 'h':
		return IsValidHorseMove(Board, sci, sri, dci, dri);

	case 'K':
	case 'k':
		return IsValidKingMove(Board, sci, sri, dci, dri);

	case 'P':
		return IsValidWhitePawnMove(Board, sci, sri, dci, dri, turn);

	case 'p':
		return IsValidBlackPawnMove(Board, sci, sri, dci, dri, turn);
	}
}

void FindOppKing(char Board[][DIM], int& kri, int& kci, int turn) {
	char OppKing;
	if (turn == White) {
		OppKing = 'k';//black king
	}
	else {
		OppKing = 'K';//white king
	}
	for (int r = 0; r < DIM; r++) {
		for (int c = 0; c < DIM; c++) {
			if (Board[r][c] == OppKing) {
				kri = r;
				kci = c;
			}
		}
	}
}

bool Check(char Board[][DIM], int& kci, int& kri, int turn) {
	FindOppKing(Board, kri, kci, turn);
	for (int r = 0; r < DIM; r++) {
		for (int c = 0; c < DIM; c++) {
			if (IsMe(Board[r][c], turn) && IsLegalMove(Board, c, r, kci, kri, turn)) {
				return true;
			}
		}
	}
	return false;
}

bool CheckForGivenCoord(char Board[][DIM], int kci, int kri, int turn) {
	for (int r = 0; r < DIM; r++) {
		for (int c = 0; c < DIM; c++) {
			if (IsMe(Board[r][c], turn) && IsLegalMove(Board, c, r, kci, kri, turn)) {
				return true;
			}
		}
	}
	return false;
}

bool SelfCheck(char Board[][DIM], int sci, int sri, int dci, int dri, int& kci, int& kri, int turn) {
	char Temp[DIM][DIM];
	for (int r = 0; r < DIM; r++) {
		for (int c = 0; c < DIM; c++) {
			Temp[r][c] = Board[r][c];
		}
	}
	PieceMove(Temp, sci, sri, dci, dri);
	TurnChange(turn);
	return Check(Temp, kci, kri, turn);
}

void Highlight(char Board[][DIM], char Temp[][DIM], int sci, int sri, int turn) {
	for (int r = 0; r < DIM; r++) {
		for (int c = 0; c < DIM; c++) {
			if (IsValidDest(c, r, Board[r][c], turn) && IsLegalMove(Board, sci, sri, c, r, turn)) {
				Temp[r][c] = 'X';
				gotoRowCol(2 * r + 2, 4 * c + 6);
				cout << 'X';
			}
			else {
				Temp[r][c] = '-';
			}
		}
	}
}

void UnHighlight(char Board[][DIM], char Temp[][DIM]) {
	for (int r = 0; r < DIM; r++) {
		for (int c = 0; c < DIM; c++) {
			if (Temp[r][c] == 'X') {
				gotoRowCol(2 * r + 2, 4 * c + 6);
				cout << Board[r][c];
			}
		}
	}
}

bool CanIMove(char Board[][DIM], int& kri, int& kci, int turn) {
	for (int sr = 0; sr < DIM; sr++) {
		for (int sc = 0; sc < DIM; sc++) {
			if (IsMe(Board[sr][sc], turn)) {
				for (int dr = 0; dr < DIM; dr++) {
					for (int dc = 0; dc < DIM; dc++) {
						if (IsLegalMove(Board, sc, sr, dc, dr, turn)) {
							if (!SelfCheck(Board, sc, sr, dc, dr, kci, kri, turn)) {
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool CheckMate(char Board[][DIM], int& kci, int& kri, int turn) {
	if (Check(Board, kci, kri, turn)) {
		TurnChange(turn);
		if (CanIMove(Board, kri, kci, turn)) {
			return true;
		}
	}
	return false;

}

bool StaleMate(char Board[][DIM], int kci, int kri, int turn) {
	if (!Check(Board, kci, kri, turn)) {
		TurnChange(turn);
		if (CanIMove(Board, kri, kci, turn)) {
			return true;
		}
	}
	return false;
}

bool IsPawnPromotion(char Piece, int dri, int turn) {
	if (turn == Black) {
		if (dri == 7 && Piece == 'p') {
			return true;
		}
	}
	else if (turn == White) {
		if (dri == 0 && Piece == 'P') {
			return true;
		}
	}
	return false;
}

bool BlackKingMoved(bool k, char Board[][DIM]) {
	return k || Board[0][4] != 'k';  //2nd part of condition is required, when game is loaded.
}

bool WhiteKingMoved(bool K, char Board[][DIM]) {
	return K || Board[7][4] != 'K';  //2nd part of condition is required, when game is loaded.
}

bool BlackLeftRookMoved(bool r1, char Board[][DIM]) {
	return r1 || Board[0][0] != 'r';  //2nd part of condition is required, when game is loaded.;
}

bool BlackRightRookMoved(bool r2, char Board[][DIM]) {
	return r2 || Board[0][7] != 'r';
}

bool WhiteLeftRookMoved(bool R1, char Board[][DIM]) {
	return R1 || Board[7][0] != 'R';
}

bool WhiteRightRookMoved(bool R2, char Board[][DIM]) {
	return R2 || Board[7][7] != 'R';
}

bool IsNothingBetweenBlackKingAndLeftRook(char Board[][DIM]) {
	int Kci = 4;
	int Rci = 0;
	int r = 0;
	for (int c = Rci + 1; c < Kci; c++) {
		if (Board[r][c] != '-') {
			return false;
		}
	}
	return true;
}

bool IsNothingBetweenBlackKingAndRightRook(char Board[][DIM]) {
	int Kci = 4;
	int Rci = 7;
	int r = 0;
	for (int c = Kci + 1; c < Rci; c++) {
		if (Board[r][c] != '-') {
			return false;
		}
	}
	return true;
}

bool IsNothingBetweenWhiteKingAndLeftRook(char Board[][DIM]) {
	int Kci = 4;
	int Rci = 0;
	int r = 7;
	for (int c = Rci + 1; c < Kci; c++) {
		if (Board[r][c] != '-') {
			return false;
		}
	}
	return true;
}

bool IsNothingBetweenWhiteKingAndRightRook(char Board[][DIM]) {
	int Kci = 4;
	int Rci = 7;
	int r = 7;
	for (int c = Kci + 1; c < Rci; c++) {
		if (Board[r][c] != '-') {
			return false;
		}
	}
	return true;
}

bool IsSelfCheckBetweenBlackKingAndLeftRook(char Board[][DIM], int turn) {
	int Kci = 4;
	int Rci = 7;
	int Kri = 0;
	int r = 0;
	int sci = Kci, sri = Kri;

	for (int c = Rci; c <= Kci; c++) {
		if (SelfCheck(Board, sci, sri, c, r, Kci, Kri, turn)) {
			return true;
		}
	}
	return false;
}

bool IsSelfCheckBetweenBlackKingAndRightRook(char Board[][DIM], int turn) {
	int Kci = 4;
	int Rci = 7;
	int Kri = 0;
	int r = 0;
	int sci = Kci, sri = Kri;

	for (int c = Kci; c <= Rci; c++) {
		if (SelfCheck(Board, sci, sri, c, r, Kci, Kri, turn)) {
			return true;
		}
	}
	return false;
}

bool IsSelfCheckBetweenWhiteKingAndLeftRook(char Board[][DIM], int turn) {
	int Kci = 4;
	int Rci = 0;
	int Kri = 7;
	int r = 7;
	int sci = Kci, sri = Kri;

	for (int c = Rci; c <= Kci; c++) {
		if (SelfCheck(Board, sci, sri, c, r, Kci, Kri, turn)) {
			return true;
		}
	}
	return false;
}

bool IsSelfCheckBetweenWhiteKingAndRightRook(char Board[][DIM], int turn) {
	int Kci = 4;
	int Rci = 7;
	int Kri = 7;
	int r = 7;
	int sci = Kci, sri = Kri;

	for (int c = Kci; c <= Rci; c++) {
		if (SelfCheck(Board, sci, sri, c, r, Kci, Kri, turn)) {
			return true;
		}
	}
	return false;
}

bool IsCastlingWithBlackLeft(char Board[][DIM], bool k, bool r1, int turn) {
	if (BlackKingMoved(k, Board) || BlackLeftRookMoved(r1, Board)) {
		return false;
	}
	return (IsNothingBetweenBlackKingAndLeftRook(Board) && !IsSelfCheckBetweenBlackKingAndLeftRook(Board, turn));
}

bool IsCastlingWithWhiteLeft(char Board[][DIM], bool K, bool R1, int turn) {
	if (WhiteKingMoved(K, Board) || WhiteLeftRookMoved(R1, Board)) {
		return false;
	}
	return (IsNothingBetweenWhiteKingAndLeftRook(Board) && !IsSelfCheckBetweenWhiteKingAndLeftRook(Board, turn));
}

bool IsCastlingWithBlackRight(char Board[][DIM], bool k, bool r2, int turn) {
	if (BlackKingMoved(k, Board) || BlackRightRookMoved(r2, Board)) {
		return false;
	}
	return (IsNothingBetweenBlackKingAndRightRook(Board) && !IsSelfCheckBetweenBlackKingAndRightRook(Board, turn));
}

bool IsCastlingWithWhiteRight(char Board[][DIM], bool K, bool R2, int turn) {
	if (WhiteKingMoved(K, Board) || WhiteRightRookMoved(R2, Board)) {
		return false;
	}
	return (IsNothingBetweenWhiteKingAndRightRook(Board) && !IsSelfCheckBetweenWhiteKingAndRightRook(Board, turn));
}

void HighlightForCastling(char Board[][DIM], bool k, bool r1, bool r2, bool K, bool R1, bool R2, char Temp[][DIM], int turn) {
	int r, c;
	if (turn == Black) {
		if (IsCastlingWithBlackLeft(Board, k, r1, turn)) {
			r = 0, c = 0;
			Temp[r][c] = 'C';
			gotoRowCol(2 * r + 2, 4 * c + 6);
			cout << 'C'; //C means Castling

		}

		if (IsCastlingWithBlackRight(Board, k, r2, turn)) {
			r = 0, c = 7;
			Temp[r][c] = 'C';
			gotoRowCol(2 * r + 2, 4 * c + 6);
			cout << 'C';
		}
	}

	else if (turn == White) {
		if (IsCastlingWithWhiteLeft(Board, K, R1, turn)) {
			r = 7, c = 0;
			Temp[r][c] = 'C';
			gotoRowCol(2 * r + 2, 4 * c + 6);
			cout << 'C';
		}

		if (IsCastlingWithWhiteRight(Board, K, R2, turn)) {
			r = 7, c = 7;
			Temp[r][c] = 'C';
			gotoRowCol(2 * r + 2, 4 * c + 6);
			cout << 'C';
		}
	}

}

void UnHighlightForCastling(char Board[][DIM], int dri, int dci, char Temp[][DIM]) {
	for (int r = 0; r < DIM; r++) {
		for (int c = 0; c < DIM; c++) {
			if (Temp[r][c] == 'C' && r != dri && c != dci) {
				gotoRowCol(2 * r + 2, 4 * c + 6);
				cout << Board[r][c];
			}
		}
	}
}

bool IsLegalCastling(char Board[][DIM], bool k, bool r1, bool r2, bool K, bool R1, bool R2, int turn) {
	if (turn == White) {
		return IsCastlingWithWhiteLeft(Board, K, R1, turn) || IsCastlingWithWhiteRight(Board, K, R2, turn);
	}
	else {
		return IsCastlingWithBlackLeft(Board, k, r1, turn) || IsCastlingWithBlackRight(Board, k, r2, turn);
	}
}

bool IsValidCastlingDest(char Board[][DIM], char Temp[][DIM], int dri, int dci, int turn) {
	if (turn == Black) {
		if (Temp[dri][dci] == 'C' && dri == 0 && dci == 0) {
			return true;
		}
		else if (Temp[dri][dci] == 'C' && dri == 0 && dci == 7) {
			return true;
		}
	}
	else {
		if (Temp[dri][dci] == 'C' && dri == 7 && dci == 0) {
			return true;
		}
		else if (Temp[dri][dci] == 'C' && dri == 7 && dci == 7) {
			return true;
		}
	}
	return false;
}

int main() {
	char Board[DIM][DIM] = {}, Temp[DIM][DIM] = {};
	int dci, dri, sci, sri, kci = 0, kri = 0, turn = 0, temp;
	int sr, sc, pr, pc;
	bool r1 = false, r2 = false, R1 = false, R2 = false, k = false, K = false;
	char AtSource;
	char ConvertInto;
	string Names[2];
	string Lfn = "Load.txt", Sfn = "Save.txt";
	char load, save;

	Init(Board, turn, Names);

	system("cls");

	DisplayBoard(Board);
	DisplayLoad();
	DisplaySave();


	if (CheckMate(Board, kci, kri, turn)) {
		gotoRowCol(21, 25);
		cout << endl << "CheckMate!";
		cout << endl << Names[turn] << " won!";
		system("pause");
	}
	else if (CheckMate(Board, kci, kri, turn)) {
		gotoRowCol(22, 25);
		cout << endl << "StaleMate!";
		cout << endl << "Game Drawn!";
		system("pause");

	}
	else if (Check(Board, kci, kri, turn)) {
		gotoRowCol(23, 25);
		cout << "Check!";
		system("pause");
	}

	do {
		TurnMsg(turn, Names[turn]);
		DisplayBoard(Board);
		DisplayLoad();

		do {
			DisplayLoad();
			DisplaySave();

			SelectSource(Board, sci, sri, turn);
			if (InsideBox(3, 100, 5, 2 * sri + 2, 4 * sci + 6, 10)) {
				system("cls");
				Load(Board, Lfn);
				DisplayBoard(Board);
				continue;
			}
			else if (InsideBox(13, 100, 5, 2 * sri + 2, 4 * sci + 6, 10)) {
				Save(Board, Sfn);
				cout << "Game Saved";
				system("cls");
				DisplayBoard(Board);
				continue;
			}
			else if (!IsValidSource(sci, sri, Board[sri][sci], turn)) {
				gotoRowCol(24, 25);
				cout << "Invalid Source!";
				continue;
			}

			Highlight(Board, Temp, sci, sri, turn);
			HighlightForCastling(Board, k, r1, r2, K, R1, R2, Temp, turn);

			SelectDest(Board, dci, dri, turn);
			if (IsValidDest(dci, dri, Board[dri][dci], turn) || IsValidCastlingDest(Board, Temp, dri, dci, turn)) {
				UnHighlight(Board, Temp);
				UnHighlightForCastling(Board, dri, dci, Temp);
				break;
			}
			else {
				UnHighlight(Board, Temp);
				UnHighlightForCastling(Board, dri, dci, Temp);
				gotoRowCol(25, 25);
				cout << "Invalid Destination!";
			}
		} while (1);
		do {
			if (!SelfCheck(Board, sci, sri, dci, dri, kci, kri, turn)) {
				if (IsLegalMove(Board, sci, sri, dci, dri, turn)) {
					break;
				}
				else if (IsLegalCastling(Board, k, r1, r2, K, R1, R2, turn)) {
					if (dri == 0 && dci == 0) {
						Board[0][4] = '-';
						Board[0][0] = '-';
						Board[0][2] = 'k';
						Board[0][3] = 'r';
					}
					else if (dri == 0 && dci == 7) {
						Board[0][4] = '-';
						Board[0][7] = '-';
						Board[0][6] = 'k';
						Board[0][5] = 'r';
					}
					else if (dri == 7 && dci == 0) {
						Board[7][4] = '-';
						Board[7][0] = '-';
						Board[7][2] = 'k';
						Board[7][3] = 'r';
					}
					else if (dri == 7 && dci == 7) {
						Board[7][4] = '-';
						Board[7][7] = '-';
						Board[7][6] = 'k';
						Board[7][5] = 'r';
					}
					break;
				}
				else {
					gotoRowCol(25, 25);
					cout << endl << "Not a legal Move!";
				}
			}

			else {
				gotoRowCol(25, 25);
				cout << endl << "Not a legal Move!";
			}
		} while (1);

		AtSource = Board[sri][sci];    //for castling
		if (AtSource == 'r' && sri == 0 && sci == 0) {
			r1 = true;
		}
		else if (AtSource == 'r' && sri == 0 && sci == 7) {
			r2 = true;
		}
		else if (AtSource == 'R' && sri == 7 && sci == 0) {
			R1 = true;
		}
		else if (AtSource == 'R' && sri == 7 && sci == 7) {
			R2 = true;
		}
		else if (AtSource == 'k' && sri == 0 && sci == 4) {
			k = true;
		}
		else if (AtSource == 'K' && sri == 7 && sci == 4) {
			K = true;
		}

		PieceMove(Board, sci, sri, dci, dri);

		if (IsPawnPromotion(Board[dri][dci], dci, turn)) {
			if (turn == Black) {
				gotoRowCol(22, 25);
				cout << "Pawn Promotion!" << "What do you want your pawn to convert into(q, r, b, h): ";
				cin >> ConvertInto;
				switch (ConvertInto) {
				case 'q':
					Board[dri][dci] = 'q';
					break;

				case 'h':
					Board[dri][dci] = 'h';
					break;

				case 'r':
					Board[dri][dci] = 'r';
					break;

				case 'b':
					Board[dri][dci] = 'b';
					break;

				}
			}
			if (turn == White) {
				gotoRowCol(22, 25);
				cout << "Pawn Promotion!" << "What do you want your pawn to convert into(Q, R, H, B): ";
				cin >> ConvertInto;
				switch (ConvertInto) {
				case 'Q':
					Board[dri][dci] = 'Q';
					break;

				case 'H':
					Board[dri][dci] = 'H';
					break;

				case 'R':
					Board[dri][dci] = 'R';
					break;

				case 'B':
					Board[dri][dci] = 'B';
					break;
				}
			}
		}

		TurnChange(turn);
		system("cls");
	} while (true);
}
