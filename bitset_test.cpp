
#include<bitset>
using std::bitset;

#include<iostream>
using std::cout; using std::endl;


class bs
{
public:

  template<int n>
  static inline void ProcessBitset(bitset<n>& b)
  {
    cout << "Bitset has: " << b.count() << " bits set." << endl;
  };
  

};


int main()
{
  bitset<32> b32(255);
  bitset<64> b64(255);



  bs::ProcessBitset<32>(b32);
  bs::ProcessBitset<64>(b64);
  



}
