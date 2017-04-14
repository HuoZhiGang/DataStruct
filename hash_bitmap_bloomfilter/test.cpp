#include "HashTableList.hpp"
#include "BitMap.hpp"
#include "HashTable.hpp"
#include "BloomFilter.hpp"
// 测试 哈希表线性探测
/*
void test_2017_04_02_hashTable_1()
{
	HashTable<int> hashable1;
	hashable1.Insert(10);
	hashable1.Insert(20);
	hashable1.Insert(30);
	hashable1.Remove(30);
	hashable1.Insert(40);
	hashable1.Insert(50);
	size_t index;
	hashable1.Find(20, index);
}*/

// 测试哈希表线性探测值string 特化 
void test_2017_04_02_hastTable_2()
{
	HashTable<string, string, HashFunDefault<string>> ht;
	ht.Insert("ss", "你好");
	ht.Insert("ss", "gege");
	size_t index;
	ht.Find("a",index);
	ht.Remove("ss");
}

// 测试哈希表之链地址法
void test_2017_04_02_hashTable_list()
{
	HashTableBucket<string, string> ht;
	ht.Insert("张三", "陕西省");
	ht.Insert("李四", "福建省");
	ht.Insert("王五", "河南省");
	ht.Insert("赵六", "陕西省");
	ht.Insert("赵六", "陕西省");
	cout << boolalpha <<ht.Find("张三")<<endl;
	ht.Remoove("张三");
	cout << boolalpha <<ht.Find("张三")<<endl;
}

//测试位图
void test_bitmap()
{
	BitMap bm(50);
	bm.Set(5);
	bm.Set(16);
	bm.Set(26);
	bm.Set(36);
	bm.Set(46);
	cout << boolalpha << bm.Test(16) << endl;
	cout << boolalpha << bm.Test(36) << endl;
	cout << boolalpha << bm.Test(6) << endl;
	cout << boolalpha << bm.Test(46) << endl;
	bm.ReSet(46);
	cout << boolalpha << bm.Test(46) << endl;


}

// 测试iterator
void test_iterator()
{
   
}

// 测试布隆过滤器 BloomFilter
void test_bloomfilter()
{
    BloomFilter<string> bf(20);
    bf.Set("你好啊");
    bf.Set("你好");
    bf.Set("hellowrold");
    bf.Set("www.baidu.com");

    cout << bf.Find("你好啊") << endl;
    cout << bf.Find("你好") << endl;
    cout << bf.Find("hellowrold") << endl;
    cout << bf.Find("www.baidu.com") << endl;
    cout << bf.Find("www") << endl;


}
int main()
{
    test_bloomfilter();
	//test_bitmap();
	//test_iterator();
	//test_2017_04_02_hashTable_list();
	//test_2017_04_02_hastTable_2();
	return 0;
}
