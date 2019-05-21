#ifndef __ADMIN_VIEW__
#define __ADMIN_VIEW__ 1

/**
* ������� 
*/

#include "../tools/basetool.cpp"

#include "user.cpp"
#include "account.cpp"

/// ����Ա�鿴ͼ��ϸ�ڣ�������������仯��ͼ 
int admin_book_detail_view(Book*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb("ͼ��");
    path.pb("�鿴���޸�");
	path.pb(now->bookname);
	path.pb(usernow->username);
	menu.pb("(0) ����");
	menu.pb("(1) �޸Ŀ��");
	
	menu.pb("(2) �޸�����");
	menu.pb("(3) �޸ĳ�����");
	menu.pb("(4) �޸�����");
	menu.pb("(5) �޸Ĺ�������");
	menu.pb("(6) �޸ļ��");
	menu.pb("(7) �޸�Ŀ¼");
	menu.pb("(8) �޸�ҳ��");
	menu.pb("(9) �޸Ľ���Ȩ��");
	vector<Record*>recs;
	Record*tmpr;
	while (true) {
		tool_show_path(path);
		tool_show_up();
		now->show_detail();
		tool_show_mid();
		recs = records.select(-1,-1,now->id,-1);
		puts("���⣺");
		_rep(i,0,recs.size()) {
			tmpr = recs[i];
			if (tmpr->tp == 1)
				printf("[%s]: %s ���1������δ�黹��\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str());
		}
		tool_show_mid();
		puts("�������¼��");
		_rep(i,0,recs.size()) {
			tmpr = recs[i];
			if (tmpr->tp == 6)
				printf("[%s]: %s ������%d���ɱ���⡣\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str(),tmpr->num);
			if (tmpr->tp == 7)
				if (tmpr->num >= 0)
					printf("[%s]: %s ����%d����\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str(),tmpr->num);
				else
					printf("[%s]: %s �Ƴ�%d����\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str(),-tmpr->num);
		}
		tool_show_down();
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string delta = tool_show_require("��������䶯�����������������ӣ���������");
			int del = 0, i;
			if (delta[0] == '-') {
				rep(i,1,delta.length()) {
					if (!isdigit(delta[i])) {
						tool_show_error("�Ƿ�����",1);
						break;
					}
					del = del*10+delta[i]-'0';
				}
				del = -del;
			} else
				rep(i,0,delta.length()) {
					if (!isdigit(delta[i])) {
						tool_show_error("�Ƿ�����",1);
						break;
					}
					del = del*10+delta[i]-'0';
				}
			if (i != delta.length()) continue;
			if (del == 0 || now->total+del<now->lent) {
				tool_show_error("�Ƿ�����",1);
				continue;
			}
			now->total += del;
			records.insert(usernow->id,now->id,7,time(NULL),0,del);
			commit_book();
			commit_record();
			tool_show_info("�޸Ŀ��ɹ�",1);
		} else if (sta == 2) {
			now->bookname = tool_show_require("����������");
			commit_book();
		} else if (sta == 3) {
			now->press = tool_show_require("�����������");
			commit_book();
		} else if (sta == 4) {
			now->author = tool_show_require("����������");
			commit_book();
		} else if (sta == 5) {
			now->about_author = tool_show_requires("�������������");
			commit_book();
		} else if (sta == 6) {
			now->intro = tool_show_requires("��������");
			commit_book();
		} else if (sta == 7) {
			now->catalog = tool_show_requires("������Ŀ¼");
			commit_book();
		} else if (sta == 8) {
			string page = tool_show_require("������ҳ��");
			int pag = 0, i;
			rep(i,0,page.length()) {
				if (!isdigit(page[i])) {
					tool_show_error("�Ƿ�����",1);
					break;
				}
				pag = pag*10+page[i]-'0';
			}
			if (i != page.length()) continue;
			now->page = pag;
			commit_book();
		} else if (sta == 9) {
			now->allow_lend ^= 1;
			commit_book();
		}
	}
}
/// ����Ա�鿴����ͼ����ͼ 
int all_book_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb("ͼ��");
    path.pb("�鿴���޸�");
    path.pb(usernow->username); 
	menu.pb("(0) ����");
	menu.pb("(1) �鿴��ϸ��Ϣ�����ü�¼���޸�ͼ����Ϣ�����");
	vector<Book*> item;
	item = books.allbook;
	while (true) {
		tool_show_path(path);
		tool_show_up();
		if (item.size() == 0) puts(" ��ͼ�顣");
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
			if (i < item.size()) admin_book_detail_view(item[i]);
			else tool_show_error("��Ч����",1);
		}
	} 
}
/// ����Ա���ͼ����ͼ 
int add_book_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb("ͼ��");
    path.pb("���");
    path.pb(usernow->username); 
	menu.pb("(0) ȡ�����Ĳ�����");
	menu.pb("(1) ����ͼ�鲢����");

	menu.pb("(2) �޸�����");
	menu.pb("(3) �޸�ISBN");
	menu.pb("(4) �޸ĳ�����");
	menu.pb("(5) �޸�����");
	menu.pb("(6) �޸Ĺ�������");
	menu.pb("(7) �޸ļ��");
	menu.pb("(8) �޸�Ŀ¼");
	menu.pb("(9) �޸�ҳ��");
	menu.pb("(10)�޸Ŀ��");
	menu.pb("(11)�޸Ľ���Ȩ��");
	Book* now = new Book;
	while (true) {
		tool_show_path(path);
		tool_show_up();
		now->show_detail();
		tool_show_down();
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			char s[15];
			sprintf(s,"%lld",now->ISBN);
			if (strlen(s) != 13) {
				tool_show_error("ISBN���ȴ���",1);
				continue; 
			}
			if (books.hash_find_ISBN(now->ISBN) != NULL) {
				tool_show_error("�Ѵ��ڵ�ISBN����˶�",1);
				continue;
			}
			books.insert(now->ISBN,now->bookname,now->press,now->author,now->about_author,now->intro,now->catalog,now->page,now->total,0,now->allow_lend);
			records.insert(usernow->id,now->id,6,time(NULL),0,now->total);
			commit_book();
			commit_user();
			tool_show_info("���ͼ��ɹ�",1);
			return 1;
		} else if (sta == 2) {
			now->bookname = tool_show_require("����������");
		} else if (sta == 3) {
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
			now->ISBN = isbn;
		} else if (sta == 4) {
			now->press = tool_show_require("�����������");
		} else if (sta == 5) {
			now->author = tool_show_require("����������");
		} else if (sta == 6) {
			now->about_author = tool_show_requires("�������������");
		} else if (sta == 7) {
			now->intro = tool_show_requires("��������");
		} else if (sta == 8) {
			now->catalog = tool_show_requires("������Ŀ¼");
		} else if (sta == 9) {
			string page = tool_show_require("������ҳ��");
			int pag = 0, i;
			rep(i,0,page.length()) {
				if (!isdigit(page[i])) {
					tool_show_error("�Ƿ�����",1);
					break;
				}
				pag = pag*10+page[i]-'0';
			}
			if (i != page.length()) continue;
			now->page = pag;
		} else if (sta == 10) {
			string total = tool_show_require("����������");
			int tot = 0, i;
			rep(i,0,total.length()) {
				if (!isdigit(total[i])) {
					tool_show_error("�Ƿ�����",1);
					break; 
				}
				tot = tot*10+total[i]-'0';
			} 
			if (i != total.length()) continue;
			now->total = tot;
		} else if (sta == 11) {
			now->allow_lend ^= 1;
		}
	}
}
/// ����Ա����ͼ����ͼ 
int managebook_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb("ͼ��");
    path.pb(usernow->username); 
	menu.pb("(0) ����");
	menu.pb("(1) �鿴ͼ�����޸���Ϣ�Ϳ��");
	menu.pb("(2) ���ͼ��");
	while (true) {
		tool_show_path(path);
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) all_book_view();
		else if (sta == 2) add_book_view();
	}
	return 1;
}
/// ����Ա�鿴�û���Ϣ���޸���Ȩ����ͼ 
int admin_user_detail_view(User*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb("�û�");
    path.pb("�鿴���޸�");
	path.pb(now->username);
	path.pb(usernow->username);
	menu.pb("(0) ����");
	if (now!=usernow && !now->is_admin)
		menu.pb("(1) ���Ľ���Ȩ��");
	while (true) {
		tool_show_path(path);
		tool_show_up();
		now->show_detail();
		tool_show_down();
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			now->is_banned ^= 1;
			tool_show_info("�޸��û�Ȩ�޳ɹ�",1);
		}
	}
}
/// ����Ա�鿴�����û���ͼ 
int manageuser_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb("�û�");
    path.pb(usernow->username); 
	menu.pb("(0) ����");
	menu.pb("(1) �鿴�û���ϸ��Ϣ���޸��û�Ȩ��");
	User* tmpu;
	while (true) {
		tool_show_path(path);
		tool_show_up();
		
		_rep(i,0,users.alluser.size()) {
			tmpu = users.alluser[i];
			printf("[%d] %s�����䣺%s %s %s\n",i,tmpu->username.c_str(),tmpu->email.c_str(),(tmpu->is_admin?"<����Ա>":"        "),(tmpu->is_banned?"<�����>":"        "));
		}
		
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("�����û���ţ������������֣�");
			rep(i,0,users.alluser.size())
				if (tmp == i2str(i)) break;
			if (i < users.alluser.size()) admin_user_detail_view(users.alluser[i]);
			else tool_show_error("��Ч����",1);
		} 
	}
	return 1;
}
/// ����Ա�鿴ͼ���ͳ����Ϣ��ͼ 
int statistics_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb("ͳ��");
    path.pb(usernow->username);
	menu.pb("(0) ����");
	
	int totu(0),totadm(0),totban(0),totb(0),tott(0),totl(0),totd(0),totr(0);
	User* tmpu;
	Book* tmpb;
	Record* tmpr;
	totu = users.alluser.size();
	_rep(i,0,totu) {
		tmpu = users.alluser[i];
		if (tmpu->is_admin) totadm ++ ;
		if (tmpu->is_banned) totban ++ ;
	}
	totb = books.allbook.size();
	_rep(i,0,totb) {
		tmpb = books.allbook[i];
		tott += tmpb->total;
		totl += tmpb->lent;
		if (tmpb->total == 0) totd ++ ;
	}
	_rep(i,0,records.allrecord.size()) {
		tmpr = records.allrecord[i];
		if (tmpr->tp == 1 || tmpr->tp == 2) totr ++ ;
	}
	
	while (true) {
		tool_show_path(path);
		
		tool_show_up();
		time_t timenow = time(NULL);
		printf("��ֹ��%s��\n",time2str(time(NULL)).c_str());
		printf("����    ������%d������Ա��%d���������%d��\n",totu,totadm,totban);
		printf("��ͼ    �顿��%d���ܿ�棺%d���ѽ����%d���ڼ��ϣ�%d���޿�棺%d��\n",totb,tott,totl,tott-totl,totd);
		printf("�����ü�¼����%d��\n",totr);
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
	}
	return 1;
}
/// ����Ա��ͼ 
int admin_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("����");
    path.pb(usernow->username); 
	menu.pb("(0) �ǳ�");
	menu.pb("(1) ����ʹ�ý���");
	menu.pb("(2) ����ͼ��");
	menu.pb("(3) �����û�");
	menu.pb("(4) ��Ϣͳ��");
	menu.pb("(5) �û�����");
    while (true) {
        tool_show_path(path);
        int sta = tool_show_menu(menu);
        if (sta == -1) continue;
        else if (sta == 0) {
        	usernow = NULL;
        	return 1;
		} else if (sta == 1) user_view();
        else if (sta == 2) managebook_view();
        else if (sta == 3) manageuser_view();
        else if (sta == 4) statistics_view(); 
        else if (sta == 5) account_view();
    }
    return 1;
}

#endif
