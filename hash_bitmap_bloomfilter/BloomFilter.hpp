#ifndef _BOLLMFILTER_H_
#define _BOLLMFILTER_H_

#include "common.hpp"
#include "BitMap.hpp"

template <class K , 
          class Hash1 = __HashFunc1<K>,
          class Hash2 = __HashFunc2<K>,
          class Hash3 = __HashFunc3<K>,
          class Hash4 = __HashFunc4<K>,
          class Hash5 = __HashFunc5<K>>
class BloomFilter
{
public:
    BloomFilter()
    {}
    BloomFilter(int size)
    {
        _capacity = GetNextPrim(size);
        _bitmap = new BitMap(_capacity);
    }

    void Set(const K& key)
    {
        _bitmap->Set( Hash1()(key)%_capacity );
        _bitmap->Set( Hash2()(key)%_capacity );
        _bitmap->Set( Hash3()(key)%_capacity );
        _bitmap->Set( Hash4()(key)%_capacity );
        _bitmap->Set( Hash5()(key)%_capacity );
    }

    bool Find(const K& key)
    {
        if(!_bitmap->Test( Hash1()(key)%_capacity) )
            return false;       
        if(!_bitmap->Test( Hash2()(key)%_capacity) )
            return false;       
        if(!_bitmap->Test( Hash3()(key)%_capacity) )
            return false;       
        if(!_bitmap->Test( Hash4()(key)%_capacity) )
            return false;       
        if(!_bitmap->Test( Hash5()(key)%_capacity) )
            return false;
        return true;
    }
private:
    BitMap* _bitmap;
    int _capacity;
};

#endif  /*_BOLLMFILTER_H_*/
