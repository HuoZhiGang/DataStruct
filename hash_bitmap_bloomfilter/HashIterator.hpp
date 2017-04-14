#ifndef _HASHITERATOR_H_
#define _HASHITERATOR_H_

#include "common.hpp"
#include <vector>
#include <assert.h>
#include <string>


// <key, value> 结构的HashNode
template<class K, 
	     class V>
struct HashTableNode
{
	HashTableNode(const pair<K, V>& kv)
		: _next(NULL)
		,_kv(kv)
		{}

	HashTableNode<K, V>* _next;  // 指向下一个结点的指针
	pair<K, V> _kv;				 // 结点中的data 
};

// 前置声明 哈希表
template<typename K, 
		 typename V, 
	     class _HashFun = HashFunDefault<K>>
class HashTableBucket;

// 哈希迭代器
template<typename K, 
		 typename V, 
	     class Ref,  // pair 的引用
		 class Ptr,  // pair 的指针
		 class _HashFun>
struct _HashIterator
{
    typedef HashTableNode<K, V> Node;
    typedef _HashIterator<K, V, Ref, Ptr, _HashFun> Self;
    typedef HashTableBucket<K, V, _HashFun> HashTable;
 
    Node* _pCur;
    HashTable* _ht;

    // 构造函数
    _HashIterator(Node* pCur, HashTable* ht)
        :_pCur(pCur)
        ,_ht(ht)
    {}

    // 返回节点pair<k, v> 的引用
    Ref operator*()
    {
        return _pCur->_kv;
    }

    // 返回节点pair<k,v>的地址
    Ptr operator->()
    {
        return &(operator*());
    }

    // 前置++
    Self& operator++()
    {
        assert(_pCur);
        if(_pCur->_next != NULL)
            _pCur = _pCur->_next;
        else
        {
            size_t buctet = _ht->HashFun(_pCur->_kv.first);
            for (;buctet < _ht->_table.size(); ++buctet)
            {
                _pCur = _ht->_table[buctet];
                if (_pCur != NULL)
                {
                    return _pCur;
                }
            }
        }
        return _pCur;
    }

    bool operator==(const Self& self)
    {
        return _ht == self._ht && _pCur == self._pCur;
    }

    bool operator!=(const Self& self)
    {
        return !(*this == self);
    }
};

template<class K, 
		 class V, 
         class _HashFun = HashFunDefault<K>>
class HashTableBucket
{
public:// 迭代器部分

    typedef _HashIterator<K, V, 
        pair<K, V>&, 
        pair<K, V>*, 
        _HashFun> Iterator;

    typedef _HashIterator<K, V, 
        const pair<K, V>&, 
        const pair<K, V>* ConstIterator;

    friend Iterator;  // 设置迭代器类为哈希类的友元类

    // 迭代器开始
    Iterator Begin()
    {
        for (size_t index = 0; index < _table.size(); ++index)
        {
            if(_table[index])
                return Iterator(_table[index], this);
        }

        return Iterator(NULL, this);
    }

    // 迭代器结束
    Iterator End()
    {
        return (NULL, this);
    }

    

public: // 基本操作
   
    // 构造函数
    HashTableBucket(size_t size = 10)
        :_size(0)
    {
        _table.resize(GetNextPrim(size));
    }

    // 往哈希表中插入数据
    pair<bool, Iterator>  Insert (const pair<K, V>& kv)
    {
        _CheckCapacity();
        size_t index = HashFun(kv.first);
        HashTableNode<K, V>* pCur = _table[index];

        while(NULL != pCur)
        {
            if(pCur->_kv.first)
                return make_pair(false, Iterator(pCur, this));

            pCur = pCur->_next
        }
        // 头插
        HashTableNode<K, V>* pNewNode = new HashTableNode<K, V>(kv);
        pNewNode->_next = _table[idnex];
        _table[index] = pNewNode;
        _size++;
        return make_pair(true, Iterator(pNewNode, this));
    }

    // 查找数据
    bool Find(const K& key)
    {
        size_t index = HashFun(key);
        HashTableNode<K, V>* pCur = _table[index];

        while(pCur != NULL)
        {
            if(pCur->_kv.first == key)
                return true;
            pCur = pCur->_next;
        }

        return false;
    }

    // 删除
    bool Remove(const K& key)
    {
        size_t index = HashFun(key);
        HashTableNode<K, V>* pCur = _table[index];
        HashTableNode<K, V>* pPre = pCur;        
        while(pCur != NULL)
        {
            if(pCur->_kv.first == key)
            {
                // 相当于在没有头结点的单链表中删除结点，需要考虑，
                // 1.是否是头结点，如果是直接更新哈希表中的值，去释放pCur
                // 2.如果不是，前一个节点直接连到当前结点的下一个结点，然后去删除

                if(pCur == _table[index]) // 1.
                {
                    _table[index] = pCur->_next;
                }
                else // 2
                {
                    pPre->_next = pCur->_next;
                }

                // 释放当前结点
                delete pCur;
                --_size;
                return true;

            }
            pPre = pCur;
            pCur = pCur->_next;
        }


        return false;
    }

    // 清空哈希表
    void Clear()
    {
        for (size_t index = 0 ; index < _table.size(); ++index)
        {
            HashTableNode<K, V>* pCur = _table[index];
            while(pCur != NULL);
            {
                HashTableNode<K, V>* pNext = pCur->_next;
                delete pCur;
                pCur = pNext;
            }
            _table[index] = NULL;
        }
    }

    // 析构函数
    ~HashTableBucket()
    {
        Clear();
    }

    // 重载下标运算符
    V& operator[](const K& key)
    {
        size_t bucket = HashFun(key);
        HashTableNode<K, V>* pCur = _table[bucket];
        while(pCur)
        {
            if(pCur->_kv.first == key)
                return pCur->_kv.second;
            pCur = pCur->_next;
        }

        assert(false);
    }

private: // 私有成员函数
    // 检查容量并扩容
    void _CheckCapacity()
    {
        if(_size == _table.size())
        {
            HashTableBucket<K, V> temp(GetNextPrim(_size));
            // 扩容 
            for (size_t index = 0; index < _size; ++index)
            {
                HashTableNode<k, V>* pCur = _table[index];

                while(pCur != NULL)
                {
                    HashTableNode<K, V>* pNext = pCur->_next;

                    // 获取当前结点在扩容后的哈希表中的位置
                    size_t temp_index = temp.HashFun(pCur->_kv.first);
                    // 将当前结点，头插到新哈希中对应的位置
                    pCur->_next = temp._table[temp_index];
                    temp._table[temp_index] = pCur;

                    pCur = pNext;
                    temp._size++;
                }
                _table[index] = NULL;

            }

            std::swap(_size, temp.size);
            std::swap(_table, temp._table);
        }
    }
    
    // 哈希函数
	size_t HashFun(const K& key)// 哈希函数
	{
		return _HashFun()(key) % _table.capacity(); // 对key 除留取余
	}


private: // 私有成员数据

	vector<HashTableNode<K, V>*> _table; // 哈希表中保存链表的头结点
	
    size_t _size; // 当前哈希表中的元素个数
};
#endif  /*_HASHITERATOR_H_*/
