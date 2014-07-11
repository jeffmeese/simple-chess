#ifndef PIECE_TYPE_H
#define PIECE_TYPE_H

//! Describes the piece types
//! If this defintion changes check the Move class for changes
enum PieceType
{
	InvalidSquare = 0xf0,
	NoPiece = 0,
	/*WhitePawn = 0x05,
	WhiteKnight = 0x09,
	WhiteBishop = 0x11,
	WhiteRook = 0x21,
	WhiteQueen = 0x41,
	WhiteKing = 0x81,
	BlackPawn = 0x06,
	BlackKnight = 0x0A,
	BlackBishop = 0x12,
	BlackRook = 0x22,
	BlackQueen = 0x42,
	BlackKing = 0x82*/
	WhitePawn = 0x01,
	WhiteKnight = 0x02,
	WhiteBishop = 0x03,
	WhiteRook = 0x04,
	WhiteQueen = 0x05,
	WhiteKing = 0x06,
	BlackPawn = 0x08,
	BlackKnight = 0x09,
	BlackBishop = 0x0A,
	BlackRook = 0x0B,
	BlackQueen = 0x0C,
	BlackKing = 0x0D
};

bool isPiece(PieceType pieceType);
bool isWhitePiece(PieceType pieceType);
bool isBlackPiece(PieceType pieceType);
bool isFriendlyPiece(PieceType pieceType, PieceType otherPieceType);
bool isEnemyPiece(PieceType pieceType, PieceType otherPieceType);

#endif // #ifndef PIECE_TYPE_H
