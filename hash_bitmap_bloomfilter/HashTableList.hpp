#ifndef _HASHTABLELIST_H_
#define _HASHTABLELIST_H_

/************************************************************************/
/*  2017年4月9日18:18:06 
/*  哈希链地址法 
/************************************************************************/
#include "common.hpp"


// 哈希表链上节点
template <class K, class V>
struct HashTableNode
{
	HashTableNode(const K& key, const V& value)
		: _key(key)
		, _value(value)
		, _next(NULL)
	{}

	HashTableNode<K, V>* _next; // 指向下一个结点
	K _key; // 当前结点的key
	V _value; // 当前结点的value
};

// 哈希函数默认获取key函数 仿函数
template<class K>
class HashFunDef
{
public:
	size_t operator()(const K& key)
	{
		return key;
	}
};

//对key是string特化
template<>
class HashFunDef<string>
{
public:
	size_t operator()(const string& key)
	{
		return BKDRHash(key.c_str());
	}
};


// 哈希表
template<class K, class V, class GetHashKey = HashFunDef<K>>
class HashTableBucket
{
public:
	typedef HashTableNode<K, V> Node;
public:
	HashTableBucket(size_t capacity = 10)
		:_size(0)
	{// 构造函数 1.根据传进来的参数，哈希表的大小，并初始化
		capacity = GetNextPrim(capacity);// 选择合适的容量
		_table.resize(capacity);// 设置capacity个大小
		for (size_t idx = 0; idx < capacity; ++idx)//初始化为空
		{
			_table[idx] = NULL;
		}
	}
	// 插入
	bool Insert(const K&key, const V& value)
	{
		// 1.检测容量与扩容， 2. 获取key 3.检测是否已经存在 
		CheckCapacity(); // 插入之前检查容量，满了则扩充
		size_t index = HashFun(key);
		HashTableNode<K, V>* pCur = _table[index]; //获取当前key对应的指针
		while(pCur != NULL) // 当前位置不空
		{
			if(key == pCur->_key) // 如果当前结点刚好和要插入的结点则返回失败
				return false;
			// 否则往后遍历 ，确定该key是否已经插入
			pCur = pCur->_next;
		}
		// 程序走到这里说明该key不存在
		HashTableNode<K, V>* newNode = new HashTableNode<K, V>(key, value);
		// 头插，插入该向量
		newNode->_next = _table[index];
		_table[index] = newNode;
		_size++;
		return true;
	}
	bool Find(const K& key)
	{
		// 查找 1.获取索引 2.看是否能找到该值
		size_t index = HashFun(key);
		HashTableNode<K, V>* pCur = _table[index];
		while(NULL!=pCur)
		{
			if(key == pCur->_key)
				return true;
			pCur = pCur->_next;
		}
		return false;
	}
	// 删除 采用头删
	bool Remoove(const K& key)
	{
		size_t index = HashFun(key);
		Node* pCur = _table[index];
		Node* pPre = pCur;
		while(pCur!=NULL)
		{
			if(key == pCur->_key)
			{
				if(pCur = _table[index]) // 说明是头结点
				{
					_table[index] = pCur->_next; // 改变头结点
				}
				else
				{
					pPre->_next = pCur->_next; //断开与当前结点的连接
				}
				delete pCur;
				pCur = NULL;
				--_size;
				return true;
			}
			pPre = pCur;
			pCur = pCur->_next;
		}

		return false;
	}
	// 析构函数
	~HashTableBucket()
	{
		Clear();
	}
	void Clear()
	{
		for(size_t index = 0; index < _table.size(); ++index)
		{
			Node* pCur = _table[index];
			while(pCur)
			{
				Node* pDelNode = pCur;
				pCur = pCur->_next;
				delete pDelNode;
			}
			_table[index] = NULL;
		}
		_size = 0;
	}
private:
	// 扩容
	void CheckCapacity()
	{
		if(_size == _table.size())
		{
			HashTableBucket<K, V> temp(GetNextPrim(_size));// 扩容
			for(size_t index = 0; index < _size; ++index)
			{
				HashTableNode<K, V>* pCur = _table[index];
				HashTableNode<K, V>* pPre = NULL;
				while(pCur)
				{
					pPre = pCur;
					pCur = pCur->_next;
					// 取得当前元素的key
					size_t newkey = temp.HashFun(pPre->_key);
					// 放到桶中
					pPre->_next = temp._table[index];
					temp._table[index] = pPre;
					temp._size++;
				}
				_table[index] = NULL;
			}
			std::swap(_size, temp._size);
			std::swap(_table, temp._table);
		}
	
	}
private:
	size_t HashFun(const K& key)
	{
		return GetHashKey()(key) % _table.capacity();
	}
private:
	size_t _size; // 存储当前哈希表中有效元素个数
	vector<Node*> _table; // 保存各个链表的头结点
};  
#endif /*_HastTableList_H_*/
