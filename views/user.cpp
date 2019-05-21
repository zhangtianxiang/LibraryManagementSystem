#ifndef __USER_VIEW__
#define __USER_VIEW__ 1

/**
* �û���ͼ 
*/

#include "../tools/basetool.cpp"
#include "../tools/viewtool.cpp"

#include "../models/model.cpp"
#include "../databases/database.cpp"

#include "account.cpp"
/// ���鶯�� 
int borrow_action(Book*now) {
	vector<string> path;
	path.pb(SYSTEMNAME);
	path.pb("���ͼ��");
	path.pb("����");
	path.pb(usernow->username);
	tool_show_path(path);
	if (usernow->is_banned) {
		tool_show_error("���ѱ���ֹ����",1);
		return 1;
	}
	if (records.select(-1,usernow->id,now->id,1).size() > 0) {
		tool_show_info("���ѽ�ô���",1);
		return 1; 
	}
	if (usernow->borrowed == usernow->can_borrow) {
		tool_show_info("��������Ŀ����",1);
		return 1; 
	}
	usernow->borrowed ++ ;
	now->lent ++ ;
	records.insert(usernow->id,now->id,1,time(NULL),0,0);
	vector<Record*> item = records.select(-1,usernow->id,now->id,4);
	vector<Record*>::iterator i;
	Each(i,item) (*i)->tp = 5;// ȡ��ԤԼ 
	commit_user();
	commit_book();
	commit_record();
	tool_show_info("����ɹ�",1);
}
/// ԤԼ���� 
int reserve_action(Book*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("���ͼ��");
	path.pb("ԤԼ");
	path.pb(usernow->username);
	tool_show_path(path);
	if (usernow->is_banned) {
		tool_show_error("���ѱ���ֹ����",1);
		return 1;
	}
	if (records.select(-1,usernow->id,now->id,4).size() > 0) {
		tool_show_info("����ԤԼ����",1);
		return 1; 
	}
	records.insert(usernow->id,now->id,4,0,0,0);
	commit_record();
	tool_show_info("ԤԼ�ɹ�",1);
}
/// ͼʾϸ��չʾ 
int book_detail_view(Book*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("���ͼ��");
	path.pb(now->bookname);
	path.pb(usernow->username);
	menu.pb("(0) ����");
	if (now->allow_lend && records.select(-1,usernow->id,now->id,1).size()==0) {
		if (now->total-now->lent > 0) menu.pb("(1) ����");
		else if (now->total > 0
			&& records.select(-1,usernow->id,now->id,4).size()==0)
			menu.pb("(1) ԤԼ");
	}
	while (true) {
		tool_show_path(path);
		tool_show_up();
		now->show_detail();
		tool_show_down();
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			if (now->total-now->lent > 0) borrow_action(now);
			else if (now->total > 0) reserve_action(now);
		}
	}
}
/// ����嵥չʾ 
int results_view(vector<Book*>item,string key) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("���ͼ��");
	path.pb(key);
	path.pb(usernow->username);
	menu.pb("(0) ����");
	if (item.size() > 0) menu.pb("(1) ѡ���鼮�Բ鿴��ϸ��Ϣ");
	while (true) {
		tool_show_path(path);

		tool_show_up();
		if (item.size() == 0) puts(" �޽�����볢�Ը��Ĺؼ��ֻ���������ʽ���ҡ�");
		_rep(i,0,item.size()) {
			cout<<" ["<<i<<"] "<<item[i]->bookname<<" ���ߣ�"<<item[i]->author<<" �ִ棺"<<item[i]->total-item[i]->lent<<" ��棺"<<item[i]->total;
			if (!item[i]->allow_lend) { cout<<" ��ֹ���"; }
			puts("");
		}
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("����ͼ���ţ������������֣�");
			rep(i,0,item.size())
				if (tmp == i2str(i)) break;
			if (i < item.size()) book_detail_view(item[i]);
			else tool_show_error("��Ч����",1);
		}
	}
}
/// ���ͼ����ͼ 
int search_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("���ͼ��");
	path.pb(usernow->username);
	menu.pb("(0) ����");
	menu.pb("(1) �鿴����ͼ��"); 
	menu.pb("(2) ���鼮���Ʋ��ң�ģ�����ң�");
	menu.pb("(3) ��ISBN����"); // ����ʹ����ƽ�������� 
	menu.pb("(4) �����߲���"); 
	menu.pb("(5) �����������"); 
	vector<Book*>item;
	while (true) {
		tool_show_path(path);
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			item.clear();
			item = books.select(-1,-1,"","","",-1);
			results_view(item,"ȫ��ͼ��");
		} else if (sta == 2) {
			item.clear();
			string bookname = tool_show_require("�����鼮����");
			item = books.fuzzy_search_bookname(bookname);
			results_view(item,bookname);
		} else if (sta == 3) {
			item.clear();
			string ISBN;
			ISBN = tool_show_require("������ISBN��13λ���֣�");
			if (ISBN.length() != 13) {
				tool_show_error("ISBN���ȴ���",1);
				continue;
			}
			ll isbn = 0;
			int i;
			rep(i,0,ISBN.length()) {
				if(!isdigit(ISBN[i])) {
					tool_show_error("ISBN����ȫΪ����",1);
					break;
				}
				isbn = isbn*10+ISBN[i]-'0';
			}
			if (i != ISBN.length()) continue;
			Book* now = books.hash_find_ISBN(isbn);
			if (now != NULL) item.pb(now);
			results_view(item,ISBN);
		} else if (sta == 4) {
			item.clear();
			string author = tool_show_require("��������");
			item = books.select(-1,-1,"","",author,-1);
			results_view(item,author);
		} else if (sta == 5) {
			item.clear();
			string press = tool_show_require("���������");
			item = books.select(-1,-1,"",press,"",-1);
			results_view(item,press);
		}
	}
    return 1;
}
/// ���鶯�� 
int return_action(Record*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("�ѽ�ͼ��");
	path.pb("����");
	path.pb(usernow->username);
	tool_show_path(path);
	
	time_t tnow = time(NULL);
	now->tp = 2;
	now->end = tnow;
	records.insert(now->uid,now->bid,3,tnow,0,0);
	books.allbook[now->bid]->lent -- ;
	users.alluser[now->uid]->borrowed -- ;
	commit_book();
	commit_user();
	commit_record();
	tool_show_info("����ɹ�",1);
}
/// �ѽ�ͼ��չʾ 
int borrowed_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("�ѽ�ͼ��");
	path.pb(usernow->username);
	menu.pb("(0) ����");
	menu.pb("(1) �黹ͼ��");
	
	vector<Record*>recs;
	Book*tmpb;
	Record*tmpr;
	while (true) {
		tool_show_path(path);
		
		recs.clear();
		recs = records.select(-1,usernow->id,-1,1);
		
		tool_show_up();
		time_t now = time(NULL), limit;
		int days; 
		_rep(i,0,recs.size()) {
			tmpr = recs[i];
			tmpb = books.allbook[tmpr->bid];
			limit = tmpr->start+BORROWTIME*DAYSIZE;
			if (now > limit) { // ���� 
				days = ceil(difftime(now,tmpr->start)/DAYSIZE-BORROWTIME);
				printf("[%d] %s�����ߣ�%s��%s���룬�Ѿ�����%d��\n",i,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
			} else {
				days = floor(BORROWTIME-difftime(now,tmpr->start)/DAYSIZE);
				printf("[%d] %s�����ߣ�%s��%s���룬����%d�쵽��\n",i,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
			}
		}
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("����ͼ���ţ������������֣�");
			rep(i,0,recs.size())
				if (tmp == i2str(i)) break;
			if (i < recs.size()) return_action(recs[i]);
			else tool_show_error("��Ч����",1);
		}
	}
    return 1;
}
/// �ҵ���Ϣ��ͼ 
int message_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("�ҵ���Ϣ");
	path.pb(usernow->username);
	menu.pb("(0) ����");
	menu.pb("(1) �黹ͼ��");
	menu.pb("(2) ����ͼ��"); 
	vector<Record*>rec1,rec2,rec3,rec4;
	Record* tmpr;
	Book* tmpb;
	while (true) {
		tool_show_path(path);
		
		rec1.clear();
		rec2.clear();
		rec3.clear();
		rec4.clear();
		rec1 = records.select(-1,usernow->id,-1,1);
		rec2 = records.select(-1,usernow->id,-1,4);
		/*
		1 ���������������鼮 
		2 ԤԼͼ���п���
		*/
		
		time_t now = time(NULL), limit;
		int days; 
		
		tool_show_up();
		puts("��������������ͼ�飺"); 
		_rep(i,0,rec1.size()) {
			tmpr = rec1[i];
			tmpb = books.allbook[tmpr->bid];
			limit = tmpr->start+BORROWTIME*DAYSIZE;
			if (now > limit) { // ���� 
				days = ceil(difftime(now,tmpr->start)/DAYSIZE-BORROWTIME);
				rec3.pb(tmpr);
				printf("[%d] %s�����ߣ�%s��%s���룬�Ѿ�����%d��\n",rec3.size()-1,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
			} else {
				days = floor(BORROWTIME-difftime(now,tmpr->start)/DAYSIZE);
				if (days <= LEFTTIME) {
					rec3.pb(tmpr);
					printf("[%d] %s�����ߣ�%s��%s���룬����%d�쵽��\n",rec3.size()-1,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
				}
			}
		}	
		tool_show_mid();
		puts("ԤԼͼ���п��ࣺ");
		_rep(i,0,rec2.size()) {
			tmpr = rec2[i];
			tmpb = books.allbook[tmpr->bid];
			if (tmpb->allow_lend && tmpb->lent<tmpb->total) {
				rec4.pb(tmpr);
				printf("[%d] %s�����ߣ�%s��%d���ڼ���\n",rec4.size()-1,tmpb->bookname.c_str(),tmpb->author.c_str(),tmpb->total-tmpb->lent);
			}
		}	
		tool_show_down();

		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("����ͼ���ţ������������֣�");
			rep(i,0,rec3.size())
				if (tmp == i2str(i)) break;
			if (i < rec3.size()) return_action(rec3[i]);
			else tool_show_error("��Ч����",1);
		} else if (sta == 2) {
			string tmp;
			int i;
			tmp = tool_show_require("����ͼ���ţ������������֣�");
			rep(i,0,rec4.size())
				if (tmp == i2str(i)) break;
			if (i < rec4.size()) borrow_action(books.allbook[rec4[i]->bid]);
			else tool_show_error("��Ч����",1);
		}
	}
	return 1;
}
/// �û���ͼ 
int user_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb(usernow->username);
    if (usernow->is_admin) menu.pb("(0) ���ع������");
	else menu.pb("(0) �ǳ�");
	menu.pb("(1) ���ͼ��"); // ����ͼ���Խ���
	menu.pb("(2) �ѽ�ͼ��"); // �鿴�ѽ�ͼ���Թ黹
	menu.pb("(3) �ҵ���Ϣ"); // ������������ + �������� + ԤԼ���� ��̬�޸� 
	menu.pb("(4) �û�����"); // �޸��û�����
    while (true) {
        tool_show_path(path);
        int sta = tool_show_menu(menu);
        if (sta == -1) continue;
        else if (sta == 0) {
        	if (!usernow->is_admin) usernow = NULL;
        	return 1;
		} else if (sta == 1) search_view();
        else if (sta == 2) borrowed_view();
        else if (sta == 3) message_view();
        else if (sta == 4) account_view();
    }
    return 1;
}

#endif
