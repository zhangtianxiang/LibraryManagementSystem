#ifndef __HASH_MODEL__ 
#define __HASH_MODEL__  1

/**
* �ؼ���Ϊll���洢����ΪTP�Ĺ�ϣ��ģ�塣 
*/

#include "../tools/basetool.cpp"
#include "../configs/config.h"

/// ����ڵ� 
template<class TP> struct mynode {
	ll key;
	TP data;
	mynode<TP> *nxt;
	mynode(){nxt=NULL;}
};
/// ������ 
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

/// ��ll�ؼ���hash���洢TP����ɢ�д����ͻ��ֻ�в������ѯ��������ɾ�� 
template<class TP>
class Hash {
private:
	int key;       // ��ϣ����ֵ 
	int capacity;  // �ݻ� 
	int size;      // ��� 
	mylist<TP>* a; // �洢TP mylist<TP> a[...]
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
