#ifndef __BOOK_MODEL__
#define __BOOK_MODEL__ 1

/**
* ����ͼ��洢�ṹ����ز��� 
*/

#include "../tools/basetool.cpp"
#include "../tools/datatool.cpp"
#include "../configs/config.h"

#include "hash.cpp"

/// ����ͼ�� 
class Book {
public:
    int id;          // unique primary key
    ll ISBN;         // ��� 13λ �ɼ��� �Ѽ��� 
    string bookname; // ����   ������ 
    string press;    // ������ ������ 
    string author;   // ����   ������ 
    string about_author;// ��������
    string intro;    // ���
    string catalog;  // Ŀ¼
    int page;        // ҳ��
    int total;       // ��� ���Ϊ0��ʾɾ�� 
    int lent;        // ���
    bool allow_lend; // �Ƿ�ɽ� ��治Ϊ0�����ǽ�ֹ���������С˵�Ȳ�����衣 
    Book(){id=-1;page=total=lent=ISBN=0;allow_lend=true;bookname=press=author=intro=catalog=about_author="";}
    Book(const int&id,const ll&ISBN,const string&bookname,const string&press,const string&author,const string&about_author,const string&intro,const string&catalog,const int&page,const int&total,const int&lent,const bool&allow_lend):
        id(id),ISBN(ISBN),bookname(bookname),press(press),author(author),about_author(about_author),intro(intro),catalog(catalog),page(page),total(total),lent(lent),allow_lend(allow_lend){}
	void show_detail() {
		/// չʾͼ��ϸ�� 
		printf("��%s��\n", bookname.c_str());
		printf("��ϵͳ�š�  ��%d\n", id);
		printf("��ISBN��    ��%lld\n", ISBN);
		printf("�����ߡ�    ��%s\n", author.c_str());
		printf("�������硿  ��%s\n",press.c_str());
		printf("��ҳ����    ��%d\n",page);
		printf("���������ߡ���\n\n%s\n\n",about_author.c_str());
		printf("����顿    ��\n\n%s\n\n", intro.c_str());
		printf("��Ŀ¼��    ��\n\n%s\n\n",catalog.c_str());
		printf("���ݲء�    ��%d�����ѽ��%d��\n",total,lent);
		if (!allow_lend) puts("����ǰ�����ֹ���ͼ��ݣ���");
	}
};
/// ����ͼ�� 
class Books {
public:
    int idx;
    vector<Book*> allbook;
    Hash<Book*> isbn;
    Books(){idx=0;allbook.clear();}
    void insert_to_hash(Book* now) {
    	/// �洢���ϣ�� 
    	isbn.insert(now->ISBN,now);
	}
    Book* insert(const ll&ISBN,const string&bookname,const string&press,const string&author,const string&about_author,const string&intro,const string&catalog,const int&page,const int&total,const int&lent,const bool&allow_lend) {
        Book* newbook = new Book(idx++,ISBN,bookname,press,author,about_author,intro,catalog,page,total,lent,allow_lend);
        allbook.pb(newbook);
		insert_to_hash(newbook);
        return newbook;
    }
    Book* hash_find_ISBN(const ll&ISBN) {
    	/// �ù�ϣ�����ISBN��Ӧ��ͼ�� 
		Book* ret = NULL;
		isbn.search(ISBN,ret);
		return ret;
	}
    vector<Book*> select(const int&id=-1,const ll&ISBN=-1,const string&bookname="",const string&press="",const string&author="",const int&allow_lend=-1) {
        vector<Book*> item;
        vector<Book*>::iterator i;
        Each(i,allbook) {
            if ((*i)->total < 1) continue;
            if (id!=-1 && (*i)->id!=id) continue;
            if (ISBN!=-1 && (*i)->ISBN!=ISBN) continue;
            if (bookname!="" && (*i)->bookname!=bookname) continue;
            if (press!="" && (*i)->press!=press) continue;
            if (author!="" && (*i)->author!=author) continue;
            if (allow_lend!=-1 && (*i)->allow_lend!=allow_lend) continue;
            item.pb(*i);
        }
        return item;
    }
    vector<Book*> fuzzy_search_bookname(string bookname) {
    	// ģ������ 
    	/*** 
		* ����������������г���ռģʽ���ٷֱ�Ϊ��һ�ؼ��֣�
		* ������ռ���������ٷֱ�Ϊ�ڶ��ؼ��ֶԽ������
		* ȡ��һ�ؼ��� 50% ���ϣ��ҵڶ��ؼ��� 10% ���ϡ� 
		*/
		vector<Book*> item;
		vector<Book*>::iterator i;
		vector<pair<pff,int> > bks;
		Each(i,allbook) {
			if ((*i)->total < 1) continue;// ������ɾ������ 
			pff a = LCS(bookname,(*i)->bookname);
			if (a.first<=0.5 && a.second<=0.9) bks.pb(mp(a,(*i)->id));
		}
		if (bks.size() == 0) return item;
		sort(bks.begin(),bks.end());
		_rep(j,0,bks.size()) item.pb(allbook[bks[j].second]);
		return item;
	}
};

#endif
