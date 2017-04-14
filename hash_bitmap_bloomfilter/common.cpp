#include "common.hpp"

size_t GetNextPrim(size_t prev)//prev = 10
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

size_t BKDRHash(const char * str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313
	unsigned int hash = 0;
	while (*str )
	{
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}

size_t SDBMHash(const char* str)
{
	register size_t hash = 0;
	while(size_t ch = (size_t)*str++)
	{
		hash = 65599*hash+ch;
		//hash = (size_t)ch+(hash<<6)+ (hash<<16)-hash;
	}

	return hash;
}

size_t RSHash(const char *str)
{
	register size_t hash = 0;
	size_t magic = 63689;
	while(size_t ch = (size_t)*str++)
	{
		hash = hash * magic +ch;
		magic *= 378551;
	}

	return hash;
}

size_t APHash(const char* str)
{
	register size_t hash = 0;
	size_t ch;
	for (long i = 0; ch = (size_t)*str++; i++)
	{
		if (0 == (i&1))
		{
			hash ^= ((hash << 7) ^ (hash >> 3));
		}
		else
		{
			hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
		}
	}

	return hash;
}

size_t JSHash(const char* str)
{
	if (!*str)
		return 0;

	register size_t hash = 1315423911;
	while (size_t ch = (size_t)*str++)
	{
		hash ^= ((hash << 5) + ch + (hash >> 2));
	}

	return hash;
}
