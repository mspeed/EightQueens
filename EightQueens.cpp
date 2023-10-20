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
    EliminateRightIncreasingDiagonal(Index);
    // Right-decreasing diagonal - subtract 7 until we have 0x7 in the lowest 3 bits
    EliminateRightDecreasingDiagonal(Index);
    // Left-increasing diagonal - add 7 until we have 0x0 in the lowest 3 bits
    EliminateLeftIncreasingDiagonal(Index);
    // Left-decreasing diagonal - subtract 9 until we have 0x0 in the lowest 3 bits.
    EliminateLeftDecreasingDiagonal(Index);
    
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

  void EliminateRightIncreasingDiagonal(int Index)
  {
    // Add 9 until we have 7 in the lower 3 bits, or we exceed 64.
    while(Index < 64)
    {
      if(mLegal[Index])
      {
	mLegal[Index] = false;
	mBoard.PlacePiece('x', Index);
      }
      
      if((Index & 0x7) == 0x7) return;
      Index += 9;
    }
  }

  void EliminateRightDecreasingDiagonal(int Index)
  {
    // Subtract 7 until we have 7 in the lower 3 bits, or we drop below 0.
    while(Index >= 0)
    {
      if(mLegal[Index])
      {
	mLegal[Index] = false;
	mBoard.PlacePiece('x', Index);
      }
      
      if((Index & 0x7) == 0x7) return;
      Index -= 7;
    }
  }

  void EliminateLeftIncreasingDiagonal(int Index)
  {
    // Add 7 until we have 0 in the lower 3 bits, or we exceed 64.
    while(Index < 64)
    {
      if(mLegal[Index])
      {
	mLegal[Index] = false;
	mBoard.PlacePiece('x', Index);
      }
      
      if((Index & 0x7) == 0x0) return;
      Index += 7;
    }
  }

  void EliminateLeftDecreasingDiagonal(int Index)
  {
    // Subtract 9 until we have 0 in the lower 3 bits, or we drop below 0.
    while(Index >= 0)
    {
      if(mLegal[Index])
      {
	mLegal[Index] = false;
	mBoard.PlacePiece('x', Index);
      }
      
      if((Index & 0x7) == 0x0) return;
      Index -= 9;
    }
  }
  
  int NextLegalMove() const
  {
    for(array<bool,64>::const_iterator b = mLegal.begin(); b < mLegal.end(); b++){ if (*b) return b-mLegal.begin(); }
    return -1;
  }

};




int main(int argc, char* argv[])
{
  int const Iterations = atoi(argv[1]);
  
  cBoard board;
  cEightQueensModel EQM(board);

  for(int i = 0; i < Iterations; i++) EQM.Next();

}
