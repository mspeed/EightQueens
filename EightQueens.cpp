#include<iostream>
using std::cout; using std::endl;

#include<string>
using std::string;

#include<array>
using std::array;

#include<optional>
using std::optional;

#include<bitset>
using std::bitset;


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
  enum eQueenCount
  {
    EIGHT_QUEENS = 8,
  };
  
public:
  
  static optional<uint64_t> AddQueen(uint64_t QueenPattern, uint64_t LegalPositions)
  {        
    int QueenIndex = NextLegalPosition(LegalPositions, 0);
              
    while(std::bitset<64>(LegalPositions).count())
    {
      LegalPositions &= ~(((uint64_t)1)<<(63-QueenIndex));
      uint64_t TestQueenPattern = QueenPattern | (((uint64_t)1) << (63-QueenIndex));

      if(EIGHT_QUEENS == std::bitset<64>(TestQueenPattern).count())
      {
	qq(TestQueenPattern);
	return TestQueenPattern;
      }
	
      while(!AddQueen(TestQueenPattern, UpdateLegalPositions(LegalPositions, QueenIndex)).has_value())
      {
	// Find where to try next
	QueenIndex = NextLegalPosition(LegalPositions, QueenIndex);
	if(QueenIndex < 0) return std::nullopt; // Nothing we can do at this level.
	  
	// Set up for this queen position.
	LegalPositions &= ~(((uint64_t)1)<<(63-QueenIndex));
	TestQueenPattern = QueenPattern | (((uint64_t)1) << (63-QueenIndex));
      }

      // Find the next possible queen index to try.
      QueenIndex = NextLegalPosition(LegalPositions, QueenIndex);
      if(QueenIndex < 0) return std::nullopt; // Nothing we can do at this level.	
    }

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

  static void qq(uint64_t QueenPattern)
  {
    static int ConfigCount = 1;
    cout << ConfigCount << endl;
    ConfigCount++;
    for(int i = 0; i < 8; i++)
    {
      int Rank = static_cast<uint8_t>(QueenPattern >> (8*i));
      for(int j = 7; j >= 0; j--)
      {
	if((Rank >> j) & 0x1) cout << "Q ";
	else cout << "- ";
      }
      cout << endl;
    }
    cout << endl;
  }

private:

  static inline uint64_t UpdateLegalPositions(uint64_t LegalPositions, int QueenIndex)
  {
    // Rank - everything &7
    LegalPositions = EliminateRank(LegalPositions, QueenIndex);
    // File - All positive values with the same lowest 3 bits
    LegalPositions = EliminateFile(LegalPositions, QueenIndex);
    // Right-increasing diagonal - add 9 until we have 0x7 in the lowest 3 bits
    LegalPositions = EliminateRightIncreasingDiagonal(LegalPositions, QueenIndex);
    // Right-decreasing diagonal - subtract 7 until we have 0x7 in the lowest 3 bits
    LegalPositions = EliminateRightDecreasingDiagonal(LegalPositions, QueenIndex);
    // Left-increasing diagonal - add 7 until we have 0x0 in the lowest 3 bits
    LegalPositions = EliminateLeftIncreasingDiagonal(LegalPositions, QueenIndex);
    // Left-decreasing diagonal - subtract 9 until we have 0x0 in the lowest 3 bits.
    LegalPositions = EliminateLeftDecreasingDiagonal(LegalPositions, QueenIndex);
    return LegalPositions;
  }
  
  static inline int NextLegalPosition(uint64_t LegalPositions, int QueenIndex)
  {
    bitset<64> const lp(LegalPositions);
    if(lp.none()) return -1;
    while(!lp[63-QueenIndex]){ QueenIndex++; }
    return QueenIndex;
  }  

  // Rank - everything &7
  [[nodiscard]] static uint64_t EliminateRank(uint64_t LegalPositions, int QueenIndex)
  {
    int const Rank = QueenIndex >> 3;
    uint64_t const Mask = ~(((uint64_t)UINT8_MAX) << ((7-Rank)<<3));    
    return (LegalPositions & Mask);
  }
    // File - All positive values with the same lowest 3 bits
  [[nodiscard]] static uint64_t EliminateFile(uint64_t LegalPositions, int QueenIndex)
  {
    static const uint64_t FILE_MASK = 0x8080808080808080;
    int const File = QueenIndex & 0x7;
    uint64_t const Mask = ~(FILE_MASK >> File);
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
      if((QueenIndex & ((uint64_t)0x7)) == (uint64_t)0x0) break;
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
      if((QueenIndex & ((uint64_t)0x7)) == (uint64_t)0x0) break;
      QueenIndex -= 9;
    }
    return (LegalPositions & (~Mask));
  }
  
};


int main()
{
  uint64_t const QueensMask = 0;
  uint64_t const PositionsMask = UINT64_MAX;
  optional<uint64_t> rc = EQ::AddQueen(QueensMask, PositionsMask);  
};
