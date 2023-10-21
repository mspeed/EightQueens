#include<iostream>
using std::cout; using std::endl;

#include<string>
using std::string;

#include<array>
using std::array;

#include<optional>
using std::optional;

#include<bit>
using std::countr_one;


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

class EQ
{
public:
  static optional<uint64_t> AddQueen(uint64_t QueenPattern, uint64_t LegalPositions)
  {
    // Find next legal position and stick a queen in it.

    //pp(QueenPattern, LegalPositions);
    
    //uint64_t const QueenIndex = ((uint64_t)64)-(countr_one(LegalPositions));

    uint64_t const QueenIndex = 36;//((uint64_t)64)-(countr_one(LegalPositions));

    
    cout << "Adding queen at index: " << QueenIndex << endl;
    
    QueenPattern |= (((uint64_t)1) << (63-QueenIndex));

    //pp(QueenPattern, LegalPositions);
    
    // Update legal positions mask.
    // Rank - everything &7
    LegalPositions = EliminateRank(LegalPositions, QueenIndex);
    pp(QueenPattern, LegalPositions);
    // File - All positive values with the same lowest 3 bits
    LegalPositions = EliminateFile(LegalPositions, QueenIndex);
    pp(QueenPattern, LegalPositions);
    // Right-increasing diagonal - add 9 until we have 0x7 in the lowest 3 bits
    LegalPositions = EliminateRightIncreasingDiagonal(LegalPositions, QueenIndex);
    pp(QueenPattern, LegalPositions);
    // Right-decreasing diagonal - subtract 7 until we have 0x7 in the lowest 3 bits
    LegalPositions = EliminateRightDecreasingDiagonal(LegalPositions, QueenIndex);
    pp(QueenPattern, LegalPositions);
    // Left-increasing diagonal - add 7 until we have 0x0 in the lowest 3 bits
    LegalPositions = EliminateLeftIncreasingDiagonal(LegalPositions, QueenIndex);
    pp(QueenPattern, LegalPositions);
    // Left-decreasing diagonal - subtract 9 until we have 0x0 in the lowest 3 bits.
    LegalPositions = EliminateLeftDecreasingDiagonal(LegalPositions, QueenIndex);
    pp(QueenPattern, LegalPositions);
	
    
    // If there's 8 queens we're done.

    return std::nullopt;
  }

  static void pp(uint64_t QueenPattern, uint64_t LegalPositions)
  {
    for(int i = 0; i < 8; i++)
    {
      int Rank = static_cast<uint8_t>(LegalPositions >> (8*i));
      for(int j = 7; j >= 0; j--)
      {
	if((Rank >> j) & 0x1) cout << '-';
	else cout << 'x';
      }
      cout << endl;
    }
    cout << endl;
  }

private:

  // Rank - everything &7
  [[nodiscard]] static uint64_t EliminateRank(uint64_t LegalPositions, int QueenIndex)
  {
    int const Rank = QueenIndex >> 3;
    //cout << "Rank: " << Rank << endl;
    //int StartIndex = QueenIndex & (~0x7);
    //cout << "StartIndex: " << StartIndex << endl;
    uint64_t const Mask = ~(((uint64_t)UINT8_MAX) << ((7-Rank)<<3));
    //cout << std::hex << "Mask: " << Mask << endl;
    //cout << std::hex << "Lega: " << LegalPositions << endl;
    return LegalPositions & Mask;
  }
    // File - All positive values with the same lowest 3 bits
  [[nodiscard]] static uint64_t EliminateFile(uint64_t LegalPositions, int QueenIndex)
  {
    static const uint64_t FILE_MASK = 0x8080808080808080;
    int const File = QueenIndex & 0x7;

    uint64_t const Mask = ~(FILE_MASK >> File);
    //cout << "File mask: " << Mask << endl;
    return LegalPositions & Mask;
  }
    // Right-increasing diagonal - add 9 until we have 0x7 in the lowest 3 bits
  [[nodiscard]] static uint64_t EliminateRightIncreasingDiagonal(uint64_t LegalPositions, int QueenIndex)
  {
    uint64_t Mask = 0;
    while(QueenIndex < 64)
    {
      Mask |= ((uint64_t)1)<<(63-QueenIndex);
      if((QueenIndex & ((uint64_t)0x7)) == (uint64_t)0x7) break;

      QueenIndex += 9;
    }
    return (LegalPositions & (~Mask));
  }
    // Right-decreasing diagonal - subtract 7 until we have 0x7 in the lowest 3 bits
  [[nodiscard]] static uint64_t EliminateRightDecreasingDiagonal(uint64_t LegalPositions, int QueenIndex)
  {
    uint64_t Mask = 0;
    while(QueenIndex >= 0)
    {
      Mask |= ((uint64_t)1)<<(63-QueenIndex);
      if((QueenIndex & ((uint64_t)0x7)) == (uint64_t)0x7) break;

      QueenIndex -= 7;
    }
    return (LegalPositions & (~Mask));
  }
  // Left-increasing diagonal - add 7 until we have 0x0 in the lowest 3 bits
  [[nodiscard]] static uint64_t EliminateLeftIncreasingDiagonal(uint64_t LegalPositions, int QueenIndex)
  {
    uint64_t Mask = 0;
    while(QueenIndex < 64)
    {
      Mask |= ((uint64_t)1)<<(63-QueenIndex);
      if((QueenIndex & ((uint64_t)0x7)) == (uint64_t)0x7) break;

      QueenIndex += 7;
    }
    return (LegalPositions & (~Mask));

  }
  // Left-decreasing diagonal - subtract 9 until we have 0x0 in the lowest 3 bits.
  [[nodiscard]] static uint64_t EliminateLeftDecreasingDiagonal(uint64_t LegalPositions, int QueenIndex)
  {
    uint64_t Mask = 0;
    while(QueenIndex >= 0)
    {
      Mask |= ((uint64_t)1)<<(63-QueenIndex);
      if((QueenIndex & ((uint64_t)0x7)) == (uint64_t)0x7) break;

      QueenIndex -= 9;
    }
    return (LegalPositions & (~Mask));
  }
  
};


int main()
{
  uint64_t QueensMask = 0;
  uint64_t PositionsMask = UINT64_MAX;
  int QueenCount = 0;


  optional<uint64_t> rc = EQ::AddQueen(QueensMask, PositionsMask);

  if(!rc.has_value())
  {
    cout << "False returned." << endl;
  }
  else
  {
    cout << rc.value() << " returned." << endl;
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




int main2(int argc, char* argv[])
{
  int const Iterations = atoi(argv[1]);
  
  cBoard board;
  cEightQueensModel EQM(board);

  for(int i = 0; i < Iterations; i++) EQM.Next();


  return 0;
}
