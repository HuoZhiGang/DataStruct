#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_
#include "common.hpp"


enum State{EMPTY, EXIST, DELETE};

template <class K>
class HashFunDefault
{
public:
	size_t operator()(const K& key)
	{
		return key;
	}
};

// 特化
template<>
class HashFunDefault<string>
{
public:
	size_t operator()(const string& key)
	{
		return BKDRHash(key.c_str());
	}
};

// 二次探测
template<class K, class V = K, class FunGetKey = HashFunDefault<K>>
class HashTable
{
private:
	pair<K, V>* _hashTable;
	State* _flag;
	size_t _capacity;
	size_t _size;
public:
	HashTable(size_t capacity = 10)
		:_capacity(capacity)
		,_size(0)
	{
		_capacity = _GetNextPrime(_capacity);
		_hashTable = new pair<K, V>[_capacity];
		_flag = new State[_capacity];
		memset(_flag, EMPTY, sizeof(State)*_capacity);
	}
	HashTable(HashTable< K, V> &ht)
		:_hashTable(NULL)
		,_stat(NULL)
	{
		HashTable<K, V> temp(ht._capacity);
		for (size_t index = 0; index < ht._capacity; ++index)
		{
			if (EXIST == ht._flag[index])
			{
				temp.Insert(ht._hashTable[index].first, ht._hashTable[index].second);
			}
		}
		_Swap(temp);
	}
	bool Find(const K& key, size_t& index)
	{
		index = HashFun1(key);
		size_t i = 1;
		while(_flag[index] != EMPTY)
		{
			if (_flag[index] == EXIST && _hashTable[index].first == key)
				return true;
			index = HashFun2(index, i++);
		}
		index = -1;
		return false;
	}
	bool Remove(const K& key)
	{
		size_t index = 0;
		if(Find(key, index))
		{
			_flag[index] = DELETE;
			--_size;
			return true;
		}
		return false;
	}
	~HashTable()
	{
		delete []_hashTable;
		delete []_flag;
		_hashTable = NULL;
		_flag = NULL;
	}
	bool Insert(const K& key , const V& value)
	{
		_CheckCapacity();
		size_t hashIndex = HashFun1(key);
		size_t i = 1;
		while(_flag[hashIndex] == EXIST)
		{
			if (_hashTable[hashIndex].first == key)
			{
				return false;
			}
			hashIndex = HashFun2(hashIndex, i++);
		}
		_flag[hashIndex] = EXIST;
		_hashTable[hashIndex] = pair<K, V>(key, value);
		_size++;
		return true;
	}
	 
private:
	void _Swap(HashTable<K, V>& ht)
	{
		std::swap(_hashTable, ht._hashTable);
		std::swap(_flag, ht,_flag);
		std::swap(_capacity, ht._capacity);
		std:swap(_size, ht,_size);
	}
	void _CheckCapacity()
	{
		size_t capacity;
		if(_size/_capacity >= 0.7)
		{
			capacity = _GetNextPrime(_capacity);
			HashTable<K, V>ht(capacity);
			for (size_t index = 0; index < _capacity; ++index)
			{
				if (EXIST == _flag[index])
				{
					ht.Insert(_hashTable[index].first, _hashTable[index].second);
				}
			}
		}
	}
	size_t _GetNextPrime(size_t prev)
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList [_PrimeSize] =
		{
			53ul,         97ul,         193ul,       389ul,       769ul,
			1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
			49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
			1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
			50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		for (size_t idx = 0; idx < _PrimeSize; ++idx)
		{
			if(prev < _PrimeList[idx])
				return _PrimeList[idx];
		}
		return -1;
	}
	int HashFun1(const K& key)
	{
		return FunGetKey()(key) % _capacity;
	}
	int HashFun2(size_t prevHashIdx, size_t i)
	{
		return (prevHashIdx + (i<<1) - 1)%_capacity;
	}
};
