#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <vector>
#include <iostream>
using namespace std;  
class BitMap
{
public:
	BitMap()
	{}
	BitMap(size_t size)
	{
		_table.resize((size>>5) + 1);
	}
	void Set(int data)
	{
		 size_t byteNo = data >> 5;
		 size_t bitNo = data % 32;

		 _table[byteNo] |= (1 << bitNo);
	}
	void ReSet(int data)
	{
		size_t byteNo = data >> 5;
		size_t bitNo = data % 32;
		_table[byteNo] &= ~(1<<bitNo);
	}
	bool Test(int data)
	{
		size_t byteNo = data >> 5;
		size_t bitNo = data % 32;
		if(_table[byteNo] & (1<<bitNo))
			return true;

		return false;
	}
private:
	vector<unsigned int> _table;
};


#endif /* _BITMAP_H_ */
