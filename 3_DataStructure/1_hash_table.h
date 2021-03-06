#ifndef DATASTRUCTURE_HASH_TABLE_H
#define DATASTRUCTURE_HASH_TABLE_H 1
//哈希表
//hash table

//实现哈希表

//哈希表
//一种根据键值直接访问内存存储位置的数据结构
//精心设计的哈希表效率非常高
//几乎所有数据结构在查询与插入一个元素时都要与已存储的元素进行至少O(lgn)次比较
//而哈希表通过设置哈希函数直接跳过了这个步骤，将操作效率减小至O(1)
//C++中将哈希表实作为hash_map容器，但实际C++编程中使用这个容器的人并不多
//
//哈希函数
//从值value到键key的映射函数key = f(value)
//存储与查询元素时先用哈希函数计算出值value对应的键key，key即为该值的存储位置
//实际的使用中会出现多个值映射出相同的键key，即键值冲突
//键值冲突问题没有办法真正解决
//但可以通过很多方法来构造不同性能的哈希函数，从而缓解键值冲突
//常见的设计方法有：直接定址法，平方取中法，折叠法，随机数法，除留取余法
//本文给出一个简单的素数取模哈希和一个常用于字符串处理的BKDR哈希
//
//为了方便，本章所有数据结构的实现中均不处理异常


#include <iostream>
using std::cout;
using std::endl;
#ifndef MAX
#define MAX 60
#endif

//1)素数取模哈希
struct prime_hash_node
{
	int _value;
	prime_hash_node *_next;
	prime_hash_node()
	{
		_value = 0;
		_next = (prime_hash_node*)0;
	}
	prime_hash_node(const prime_hash_node& node)
	{
		_value = node._value;
		_next = node._next;
	}
	~prime_hash_node()
	{ }
	prime_hash_node& operator=(const prime_hash_node& node)
	{
		_value = node._value;
		_next = node._next;
		return(*this);
	}
};
struct prime_hash_table
{
	prime_hash_node _table[MAX];
	int _seed;
	prime_hash_table()
	{
		for (int i = 0; i < MAX; ++i) {
			_table[i]._value = 0;
			_table[i]._next = (prime_hash_node*)0;
		}
		_seed = 0;
	}
	prime_hash_table(const prime_hash_table& table)
	{
		for (int i = 0; i < MAX; ++i)
			_table[i] = table._table[i];
		_seed = table._seed;
	}
};

int _prime_hash(int seed, int value)
{
	return(value % seed);
}
void prime_hash_init(prime_hash_table *table, int prime)
{
	for (int i = 0; i < MAX; ++i) {
		table->_table[i]._value = 0;
		table->_table[i]._next = (prime_hash_node*)0;
	}
	table->_seed = prime;
}
void prime_hash_insert(prime_hash_table *table, int value)
{
	//向哈希表中插入值为value的节点
	//用哈希函数计算出在值value在哈希表中对应的节点下标
	int idx = _prime_hash(table->_seed, value);
	prime_hash_node *p = &table->_table[idx];
	while(p->_next != NULL)
		p = p->_next;
	//直接将该点链入最末位置
	p->_next = new prime_hash_node();
	p->_next->_value = value;
}
prime_hash_node* prime_hash_find(prime_hash_table *table, int value)
{
	int idx = _prime_hash(table->_seed, value);
	prime_hash_node *p = &table->_table[idx];
	while(p->_next != NULL){
		if(p->_next->_value == value)
			return(p->_next);
		p = p->_next;
	}
	return((prime_hash_node*)0);
}
void prime_hash_delete(prime_hash_table *table, int value)
{
	//prime_hash_delete函数认为值value一定已经存储于哈希表
	//而不处理值value不存在的异常情况
	int idx = _prime_hash(table->_seed, value);
	prime_hash_node *p = &table->_table[idx];
	while(p->_next->_value != value)
		p = p->_next;
	prime_hash_node *del = p->_next;
	p->_next = p->_next->_next;	//链表中删除p->h_next点
	delete(del);
}
void prime_hash_print(prime_hash_table *table)
{//打印素数取模哈希表
	for(int i = 0; i < table->_seed; ++ i){
		cout << "index " << i << ": ";
		prime_hash_node *p = &table->_table[i];
		while(p->_next != NULL){
			cout << p->_next->_value << ", ";
			p = p->_next;
		}
		cout << endl;
	}
}

//2)BKDR哈希
struct bkdr_hash_table
{
	int _table[MAX];
	int _seed;
	bkdr_hash_table()
	{
		for (int i = 0; i < MAX; ++i)
			_table[i] = 0;
		_seed = 0;
	}
	bkdr_hash_table(const bkdr_hash_table& table)
	{
		for (int i = 0; i < MAX; ++i)
			_table[i] = table._table[i];
		_seed = table._seed;
	}
};

int _bkdr_hash(int seed, const char *s)
{
	int hash = 0;
	while(*s)
		hash = hash * seed + (*s++);
	hash = (hash & 0x7FFFFFFF) % MAX;
	return(hash);
}
void bkdr_hash_init(bkdr_hash_table *table, int seed/*default=31,131,1313*/)
{//种子可以是31 131 1313 13131 ...
	for (int i = 0; i < MAX; ++i)
		table->_table[i] = 0;
	table->_seed = seed;
}
void bkdr_hash_insert(bkdr_hash_table *table, const char *s)
{
	int index = _bkdr_hash(table->_seed, s);
	table->_table[index] = 1;
}
void bkdr_hash_delete(bkdr_hash_table *table, const char *s)
{
	int index = _bkdr_hash(table->_seed, s);
	table->_table[index] = 0;
}
bool bkdr_hash_find(bkdr_hash_table *table, const char *s)
{
	int index = _bkdr_hash(table->_seed, s);
	return(table->_table[index]);
}

#endif
