#ifndef __HASH_MODEL__ 
#define __HASH_MODEL__  1

/**
* 关键字为ll，存储数据为TP的哈希表模板。 
*/

#include "../tools/basetool.cpp"
#include "../configs/config.h"

/// 链表节点 
template<class TP> struct mynode {
	ll key;
	TP data;
	mynode<TP> *nxt;
	mynode(){nxt=NULL;}
};
/// 单链表 
template<class TP> struct mylist {
	int size;
	mynode<TP> *head, *tail;// tail is the latest
	mylist() {head=tail=NULL;size=0;}
	~mylist() {
		mynode<TP>*p;
		while (head != NULL) {
			p = head, head = head->nxt;
			delete p;
		}
	}
	mynode<TP>* insert(const ll&x,const TP&y) {
		mynode<TP>* now = new mynode<TP>;
		now->key = x, now->data = y;
		if (size == 0) {
			size = 1, head = tail = now; return now;
		}
		size ++ , tail->nxt = now, tail = now;
		return now;
	}
	bool empty() { return size == 0; }
	mynode<TP>* search(const ll&x) {
		if (size == 0) return NULL;
		mynode<TP>* now = NULL;
		now = head;
		while (now != NULL) {
			if (now->key == x) return now;
			now = now->nxt;
		}
		return NULL;
	}
};

/// 对ll关键字hash，存储TP，闭散列处理冲突，只有插入与查询，不处理删除 
template<class TP>
class Hash {
private:
	int key;       // 哈希函数值 
	int capacity;  // 容积 
	int size;      // 体积 
	mylist<TP>* a; // 存储TP mylist<TP> a[...]
	int hash(const ll&k) {
		int pos = k % key;
		return pos;
	}
public:
	Hash(int d=HASHKEY,int sz=BOOKSIZE*2) {
		a = new mylist<TP>[sz];
		capacity = sz;
		size = 0;
		key = d;
	}
	~Hash() {
		delete[]a;
	} 
	bool search(const ll&k,TP&ret) {
		int pos = hash(k);
		mynode<TP>* node = a[pos].search(k);
		if (node == NULL) return false;
		ret = node->data;
		return true;
	}
	void insert(const ll&k,const TP&data) {
		int pos = hash(k);
		a[pos].insert(k,data);
	}
};

#endif
