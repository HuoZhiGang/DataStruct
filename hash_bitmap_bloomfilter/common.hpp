#ifndef _COMMON_H_
#define _COMMON_H_
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// 扩容素数容量
size_t GetNextPrim(size_t prev);
size_t BKDRHash(const char * str);
size_t SDBMHash(const char* str);
size_t RSHash(const char *str);
size_t APHash(const char* str);
size_t JSHash(const char* str);

template<class K>
struct __HashFunc1
{
	size_t operator()(const K& key)
	{
		return BKDRHash(key.c_str());
	}
};

template<class K>
struct __HashFunc2
{
	size_t operator()(const K& key)
	{
		return SDBMHash(key.c_str());
	}
};

template<class K>
struct __HashFunc3
{
	size_t operator()(const K& key)
	{
		return RSHash(key.c_str());
	}
};

template<class K>
struct __HashFunc4
{
	size_t operator()(const K& key)
	{
		return APHash(key.c_str());
	}
};

template<class K>
struct __HashFunc5
{
	size_t operator()(const K& key)
	{
		return JSHash(key.c_str());
	}
};

#endif /*_COMMON_H_*/
