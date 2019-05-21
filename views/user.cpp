#ifndef __USER_VIEW__
#define __USER_VIEW__ 1

/**
* 用户视图 
*/

#include "../tools/basetool.cpp"
#include "../tools/viewtool.cpp"

#include "../models/model.cpp"
#include "../databases/database.cpp"

#include "account.cpp"
/// 借书动作 
int borrow_action(Book*now) {
	vector<string> path;
	path.pb(SYSTEMNAME);
	path.pb("浏览图书");
	path.pb("借阅");
	path.pb(usernow->username);
	tool_show_path(path);
	if (usernow->is_banned) {
		tool_show_error("您已被禁止借书",1);
		return 1;
	}
	if (records.select(-1,usernow->id,now->id,1).size() > 0) {
		tool_show_info("您已借得此书",1);
		return 1; 
	}
	if (usernow->borrowed == usernow->can_borrow) {
		tool_show_info("您借书数目已满",1);
		return 1; 
	}
	usernow->borrowed ++ ;
	now->lent ++ ;
	records.insert(usernow->id,now->id,1,time(NULL),0,0);
	vector<Record*> item = records.select(-1,usernow->id,now->id,4);
	vector<Record*>::iterator i;
	Each(i,item) (*i)->tp = 5;// 取消预约 
	commit_user();
	commit_book();
	commit_record();
	tool_show_info("借书成功",1);
}
/// 预约动作 
int reserve_action(Book*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("浏览图书");
	path.pb("预约");
	path.pb(usernow->username);
	tool_show_path(path);
	if (usernow->is_banned) {
		tool_show_error("您已被禁止借书",1);
		return 1;
	}
	if (records.select(-1,usernow->id,now->id,4).size() > 0) {
		tool_show_info("您已预约此书",1);
		return 1; 
	}
	records.insert(usernow->id,now->id,4,0,0,0);
	commit_record();
	tool_show_info("预约成功",1);
}
/// 图示细节展示 
int book_detail_view(Book*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("浏览图书");
	path.pb(now->bookname);
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	if (now->allow_lend && records.select(-1,usernow->id,now->id,1).size()==0) {
		if (now->total-now->lent > 0) menu.pb("(1) 借阅");
		else if (now->total > 0
			&& records.select(-1,usernow->id,now->id,4).size()==0)
			menu.pb("(1) 预约");
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
/// 结果清单展示 
int results_view(vector<Book*>item,string key) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("浏览图书");
	path.pb(key);
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	if (item.size() > 0) menu.pb("(1) 选择书籍以查看详细信息");
	while (true) {
		tool_show_path(path);

		tool_show_up();
		if (item.size() == 0) puts(" 无结果！请尝试更改关键字或用其他方式查找。");
		_rep(i,0,item.size()) {
			cout<<" ["<<i<<"] "<<item[i]->bookname<<" 作者："<<item[i]->author<<" 现存："<<item[i]->total-item[i]->lent<<" 库存："<<item[i]->total;
			if (!item[i]->allow_lend) { cout<<" 禁止借出"; }
			puts("");
		}
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("输入图书编号（方括号内数字）");
			rep(i,0,item.size())
				if (tmp == i2str(i)) break;
			if (i < item.size()) book_detail_view(item[i]);
			else tool_show_error("无效输入",1);
		}
	}
}
/// 浏览图书视图 
int search_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("浏览图书");
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	menu.pb("(1) 查看所有图书"); 
	menu.pb("(2) 按书籍名称查找（模糊查找）");
	menu.pb("(3) 按ISBN查找"); // 这里使用了平衡树查找 
	menu.pb("(4) 按作者查找"); 
	menu.pb("(5) 按出版社查找"); 
	vector<Book*>item;
	while (true) {
		tool_show_path(path);
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			item.clear();
			item = books.select(-1,-1,"","","",-1);
			results_view(item,"全部图书");
		} else if (sta == 2) {
			item.clear();
			string bookname = tool_show_require("输入书籍名称");
			item = books.fuzzy_search_bookname(bookname);
			results_view(item,bookname);
		} else if (sta == 3) {
			item.clear();
			string ISBN;
			ISBN = tool_show_require("请输入ISBN（13位数字）");
			if (ISBN.length() != 13) {
				tool_show_error("ISBN长度错误",1);
				continue;
			}
			ll isbn = 0;
			int i;
			rep(i,0,ISBN.length()) {
				if(!isdigit(ISBN[i])) {
					tool_show_error("ISBN必须全为数字",1);
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
			string author = tool_show_require("输入作者");
			item = books.select(-1,-1,"","",author,-1);
			results_view(item,author);
		} else if (sta == 5) {
			item.clear();
			string press = tool_show_require("输入出版社");
			item = books.select(-1,-1,"",press,"",-1);
			results_view(item,press);
		}
	}
    return 1;
}
/// 还书动作 
int return_action(Record*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("已借图书");
	path.pb("还书");
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
	tool_show_info("还书成功",1);
}
/// 已借图书展示 
int borrowed_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("已借图书");
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	menu.pb("(1) 归还图书");
	
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
			if (now > limit) { // 逾期 
				days = ceil(difftime(now,tmpr->start)/DAYSIZE-BORROWTIME);
				printf("[%d] %s，作者：%s。%s借入，已经逾期%d天\n",i,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
			} else {
				days = floor(BORROWTIME-difftime(now,tmpr->start)/DAYSIZE);
				printf("[%d] %s，作者：%s。%s借入，还有%d天到期\n",i,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
			}
		}
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("输入图书编号（方括号内数字）");
			rep(i,0,recs.size())
				if (tmp == i2str(i)) break;
			if (i < recs.size()) return_action(recs[i]);
			else tool_show_error("无效输入",1);
		}
	}
    return 1;
}
/// 我的消息视图 
int message_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("我的消息");
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	menu.pb("(1) 归还图书");
	menu.pb("(2) 借用图书"); 
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
		1 即将到期与逾期书籍 
		2 预约图书有空余
		*/
		
		time_t now = time(NULL), limit;
		int days; 
		
		tool_show_up();
		puts("即将到期与逾期图书："); 
		_rep(i,0,rec1.size()) {
			tmpr = rec1[i];
			tmpb = books.allbook[tmpr->bid];
			limit = tmpr->start+BORROWTIME*DAYSIZE;
			if (now > limit) { // 逾期 
				days = ceil(difftime(now,tmpr->start)/DAYSIZE-BORROWTIME);
				rec3.pb(tmpr);
				printf("[%d] %s，作者：%s。%s借入，已经逾期%d天\n",rec3.size()-1,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
			} else {
				days = floor(BORROWTIME-difftime(now,tmpr->start)/DAYSIZE);
				if (days <= LEFTTIME) {
					rec3.pb(tmpr);
					printf("[%d] %s，作者：%s。%s借入，还有%d天到期\n",rec3.size()-1,tmpb->bookname.c_str(),tmpb->author.c_str(),time2str(tmpr->start).c_str(),days);
				}
			}
		}	
		tool_show_mid();
		puts("预约图书有空余：");
		_rep(i,0,rec2.size()) {
			tmpr = rec2[i];
			tmpb = books.allbook[tmpr->bid];
			if (tmpb->allow_lend && tmpb->lent<tmpb->total) {
				rec4.pb(tmpr);
				printf("[%d] %s，作者：%s。%d本在架上\n",rec4.size()-1,tmpb->bookname.c_str(),tmpb->author.c_str(),tmpb->total-tmpb->lent);
			}
		}	
		tool_show_down();

		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("输入图书编号（方括号内数字）");
			rep(i,0,rec3.size())
				if (tmp == i2str(i)) break;
			if (i < rec3.size()) return_action(rec3[i]);
			else tool_show_error("无效输入",1);
		} else if (sta == 2) {
			string tmp;
			int i;
			tmp = tool_show_require("输入图书编号（方括号内数字）");
			rep(i,0,rec4.size())
				if (tmp == i2str(i)) break;
			if (i < rec4.size()) borrow_action(books.allbook[rec4[i]->bid]);
			else tool_show_error("无效输入",1);
		}
	}
	return 1;
}
/// 用户视图 
int user_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb(usernow->username);
    if (usernow->is_admin) menu.pb("(0) 返回管理界面");
	else menu.pb("(0) 登出");
	menu.pb("(1) 浏览图书"); // 搜索图书以借用
	menu.pb("(2) 已借图书"); // 查看已借图书以归还
	menu.pb("(3) 我的消息"); // 即将到期提醒 + 逾期提醒 + 预约提醒 动态修改 
	menu.pb("(4) 用户设置"); // 修改用户设置
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
