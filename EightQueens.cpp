#include<iostream>
using std::cout; using std::endl;

#include<string>
using std::string;

#include<array>
using std::array;


class cBoard
{
  string const mDefaultRank = ". . . . . . . .";
  array<string, 8> mRanks;
  
public:
  cBoard()
  {
    for(string& i : mRanks) i = mDefaultRank; 
    for(string& i : mRanks) cout << i << "\r\n";
  }
  
  void Draw() 
  {
      for(int i = 0; i < 8; i++) cout << "\e[A";
      for(string& i : mRanks) cout << i << "\r\n";
      cout << std::flush;
  }

  void inline PlacePiece(char Piece, char File, uint_fast8_t Rank)
  {
    mRanks[8-Rank][(File-'a')<<1] = Piece;
  }

  void inline PlacePiece(char Piece, int Index)
  {
    mRanks[7-(Index>>3)][(Index&0x7)<<1] = Piece;
  }

};

class cEightQueensModel
{
  array<bool, 64> mLegal;
  cBoard & mBoard;
  
public:

  cEightQueensModel(cBoard & Board) : mBoard(Board)
  {
    for(bool& b : mLegal) b = true;
  }

  void Next()
  {
    // Find next legal move & make it.
    PlaceQueen(NextLegalMove());
  }

private:

  void PlaceQueen(int Index)
  {
    uint_fast8_t const Rank = 1 + (Index>>3);
    char const File = 'a' + (Index&0x7);

    UpdateLegalMoves(Index);
    
    PlaceQueen(File, Rank);
  }
  
  void PlaceQueen(char File, uint_fast8_t Rank)
  {
    // Update board.    
    mBoard.PlacePiece('Q', File, Rank);
    mBoard.Draw();    
  };
  
  void UpdateLegalMoves(int Index)
  {
    // Illegal moves now:

    // Rank - everything &7
    EliminateRank(Index);
    // File - All positive values with the same lowest 3 bits
    EliminateFile(Index);
    // Right-increasing diagonal - add 9 until we have 0x7 in the lowest 3 bits 
    // Right-decreasing diagonal - subtract 7 until we have 0x7 in the lowest 3 bits
    // Left-increasing diagonal - add 7 until we have 0x0 in the lowest 3 bits
    // Left-decreasing diagonal - subtract 0 until we have 0x0 in the lowest 3 bits.
    
  };

  void EliminateRank(int Index)
  {
    int const StartIndex = Index & (~0x7);
    for(int i = 0; i < 8; i++)
    {
      int const TestIndex = StartIndex+i;
      if(mLegal[TestIndex])
      {
	mLegal[StartIndex+i] = false;
	mBoard.PlacePiece('x', StartIndex+i);
      }
    }
  }

  void EliminateFile(int Index)
  {
    int StartIndex = Index & 0x7;
    for(int i = 0; i < 8; i++)
    {
      int const TestIndex = StartIndex + (i << 3);
      if(mLegal[TestIndex])
      {
	mLegal[StartIndex + (i << 3)] = false;
	mBoard.PlacePiece('x', StartIndex+(i<<3));
      }
    }
  }
  
  int NextLegalMove() const
  {
    for(array<bool,64>::const_iterator b = mLegal.begin(); b < mLegal.end(); b++){ if (*b) return b-mLegal.begin(); }
    return -1;
  }


};




int main()
{
  cBoard board;

  //board.Draw();

  //  board.PlacePiece('Q', 'a', 1);
  //  board.PlacePiece('Q', 'b', 2);
  
  //board.Draw();

  cEightQueensModel EQM(board);

  EQM.Next();
  EQM.Next();


}
