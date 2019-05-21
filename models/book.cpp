#ifndef __BOOK_MODEL__
#define __BOOK_MODEL__ 1

/**
* 定义图书存储结构及相关操作 
*/

#include "../tools/basetool.cpp"
#include "../tools/datatool.cpp"
#include "../configs/config.h"

#include "hash.cpp"

/// 单本图书 
class Book {
public:
    int id;          // unique primary key
    ll ISBN;         // 书号 13位 可检索 已检索 
    string bookname; // 题名   可搜索 
    string press;    // 出版社 可搜索 
    string author;   // 作者   可搜索 
    string about_author;// 关于作者
    string intro;    // 简介
    string catalog;  // 目录
    int page;        // 页数
    int total;       // 库存 库存为0表示删除 
    int lent;        // 借出
    bool allow_lend; // 是否可借 库存不为0，但是禁止借出，例如小说等不许外借。 
    Book(){id=-1;page=total=lent=ISBN=0;allow_lend=true;bookname=press=author=intro=catalog=about_author="";}
    Book(const int&id,const ll&ISBN,const string&bookname,const string&press,const string&author,const string&about_author,const string&intro,const string&catalog,const int&page,const int&total,const int&lent,const bool&allow_lend):
        id(id),ISBN(ISBN),bookname(bookname),press(press),author(author),about_author(about_author),intro(intro),catalog(catalog),page(page),total(total),lent(lent),allow_lend(allow_lend){}
	void show_detail() {
		/// 展示图书细节 
		printf("《%s》\n", bookname.c_str());
		printf("【系统号】  ：%d\n", id);
		printf("【ISBN】    ：%lld\n", ISBN);
		printf("【作者】    ：%s\n", author.c_str());
		printf("【出版社】  ：%s\n",press.c_str());
		printf("【页数】    ：%d\n",page);
		printf("【关于作者】：\n\n%s\n\n",about_author.c_str());
		printf("【简介】    ：\n\n%s\n\n", intro.c_str());
		printf("【目录】    ：\n\n%s\n\n",catalog.c_str());
		printf("【馆藏】    ：%d本，已借出%d本\n",total,lent);
		if (!allow_lend) puts("【当前本书禁止借出图书馆！】");
	}
};
/// 所有图书 
class Books {
public:
    int idx;
    vector<Book*> allbook;
    Hash<Book*> isbn;
    Books(){idx=0;allbook.clear();}
    void insert_to_hash(Book* now) {
    	/// 存储入哈希表 
    	isbn.insert(now->ISBN,now);
	}
    Book* insert(const ll&ISBN,const string&bookname,const string&press,const string&author,const string&about_author,const string&intro,const string&catalog,const int&page,const int&total,const int&lent,const bool&allow_lend) {
        Book* newbook = new Book(idx++,ISBN,bookname,press,author,about_author,intro,catalog,page,total,lent,allow_lend);
        allbook.pb(newbook);
		insert_to_hash(newbook);
        return newbook;
    }
    Book* hash_find_ISBN(const ll&ISBN) {
    	/// 用哈希表查找ISBN对应的图书 
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
    	// 模糊搜索 
    	/*** 
		* 这里求最长公共子序列长度占模式串百分比为第一关键字，
		* 子序列占被搜索串百分比为第二关键字对结果排序。
		* 取第一关键字 50% 以上，且第二关键字 10% 以上。 
		*/
		vector<Book*> item;
		vector<Book*>::iterator i;
		vector<pair<pff,int> > bks;
		Each(i,allbook) {
			if ((*i)->total < 1) continue;// 不考虑删掉的书 
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
