#ifndef __ADMIN_VIEW__
#define __ADMIN_VIEW__ 1

/**
* 管理界面 
*/

#include "../tools/basetool.cpp"

#include "user.cpp"
#include "account.cpp"

/// 管理员查看图书细节，借阅情况及库存变化视图 
int admin_book_detail_view(Book*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb("图书");
    path.pb("查看与修改");
	path.pb(now->bookname);
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	menu.pb("(1) 修改库存");
	
	menu.pb("(2) 修改书名");
	menu.pb("(3) 修改出版社");
	menu.pb("(4) 修改作者");
	menu.pb("(5) 修改关于作者");
	menu.pb("(6) 修改简介");
	menu.pb("(7) 修改目录");
	menu.pb("(8) 修改页数");
	menu.pb("(9) 修改借用权限");
	vector<Record*>recs;
	Record*tmpr;
	while (true) {
		tool_show_path(path);
		tool_show_up();
		now->show_detail();
		tool_show_mid();
		recs = records.select(-1,-1,now->id,-1);
		puts("馆外：");
		_rep(i,0,recs.size()) {
			tmpr = recs[i];
			if (tmpr->tp == 1)
				printf("[%s]: %s 借出1本，尚未归还。\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str());
		}
		tool_show_mid();
		puts("库存变更记录：");
		_rep(i,0,recs.size()) {
			tmpr = recs[i];
			if (tmpr->tp == 6)
				printf("[%s]: %s 将本书%d本采编入库。\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str(),tmpr->num);
			if (tmpr->tp == 7)
				if (tmpr->num >= 0)
					printf("[%s]: %s 购入%d本。\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str(),tmpr->num);
				else
					printf("[%s]: %s 移出%d本。\n",time2str(tmpr->start).c_str(),users.alluser[tmpr->uid]->username.c_str(),-tmpr->num);
		}
		tool_show_down();
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string delta = tool_show_require("请输入库存变动非零整数，正数增加，负数减少");
			int del = 0, i;
			if (delta[0] == '-') {
				rep(i,1,delta.length()) {
					if (!isdigit(delta[i])) {
						tool_show_error("非法输入",1);
						break;
					}
					del = del*10+delta[i]-'0';
				}
				del = -del;
			} else
				rep(i,0,delta.length()) {
					if (!isdigit(delta[i])) {
						tool_show_error("非法输入",1);
						break;
					}
					del = del*10+delta[i]-'0';
				}
			if (i != delta.length()) continue;
			if (del == 0 || now->total+del<now->lent) {
				tool_show_error("非法输入",1);
				continue;
			}
			now->total += del;
			records.insert(usernow->id,now->id,7,time(NULL),0,del);
			commit_book();
			commit_record();
			tool_show_info("修改库存成功",1);
		} else if (sta == 2) {
			now->bookname = tool_show_require("请输入书名");
			commit_book();
		} else if (sta == 3) {
			now->press = tool_show_require("请输入出版社");
			commit_book();
		} else if (sta == 4) {
			now->author = tool_show_require("请输入作者");
			commit_book();
		} else if (sta == 5) {
			now->about_author = tool_show_requires("请输入关于作者");
			commit_book();
		} else if (sta == 6) {
			now->intro = tool_show_requires("请输入简介");
			commit_book();
		} else if (sta == 7) {
			now->catalog = tool_show_requires("请输入目录");
			commit_book();
		} else if (sta == 8) {
			string page = tool_show_require("请输入页数");
			int pag = 0, i;
			rep(i,0,page.length()) {
				if (!isdigit(page[i])) {
					tool_show_error("非法输入",1);
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
/// 管理员查看所有图书视图 
int all_book_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb("图书");
    path.pb("查看与修改");
    path.pb(usernow->username); 
	menu.pb("(0) 返回");
	menu.pb("(1) 查看详细信息及借用记录与修改图书信息及库存");
	vector<Book*> item;
	item = books.allbook;
	while (true) {
		tool_show_path(path);
		tool_show_up();
		if (item.size() == 0) puts(" 无图书。");
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
			if (i < item.size()) admin_book_detail_view(item[i]);
			else tool_show_error("无效输入",1);
		}
	} 
}
/// 管理员添加图书视图 
int add_book_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb("图书");
    path.pb("添加");
    path.pb(usernow->username); 
	menu.pb("(0) 取消更改并返回");
	menu.pb("(1) 保存图书并返回");

	menu.pb("(2) 修改书名");
	menu.pb("(3) 修改ISBN");
	menu.pb("(4) 修改出版社");
	menu.pb("(5) 修改作者");
	menu.pb("(6) 修改关于作者");
	menu.pb("(7) 修改简介");
	menu.pb("(8) 修改目录");
	menu.pb("(9) 修改页数");
	menu.pb("(10)修改库存");
	menu.pb("(11)修改借用权限");
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
				tool_show_error("ISBN长度错误",1);
				continue; 
			}
			if (books.hash_find_ISBN(now->ISBN) != NULL) {
				tool_show_error("已存在的ISBN，请核对",1);
				continue;
			}
			books.insert(now->ISBN,now->bookname,now->press,now->author,now->about_author,now->intro,now->catalog,now->page,now->total,0,now->allow_lend);
			records.insert(usernow->id,now->id,6,time(NULL),0,now->total);
			commit_book();
			commit_user();
			tool_show_info("添加图书成功",1);
			return 1;
		} else if (sta == 2) {
			now->bookname = tool_show_require("请输入书名");
		} else if (sta == 3) {
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
			now->ISBN = isbn;
		} else if (sta == 4) {
			now->press = tool_show_require("请输入出版社");
		} else if (sta == 5) {
			now->author = tool_show_require("请输入作者");
		} else if (sta == 6) {
			now->about_author = tool_show_requires("请输入关于作者");
		} else if (sta == 7) {
			now->intro = tool_show_requires("请输入简介");
		} else if (sta == 8) {
			now->catalog = tool_show_requires("请输入目录");
		} else if (sta == 9) {
			string page = tool_show_require("请输入页数");
			int pag = 0, i;
			rep(i,0,page.length()) {
				if (!isdigit(page[i])) {
					tool_show_error("非法输入",1);
					break;
				}
				pag = pag*10+page[i]-'0';
			}
			if (i != page.length()) continue;
			now->page = pag;
		} else if (sta == 10) {
			string total = tool_show_require("请输入库存量");
			int tot = 0, i;
			rep(i,0,total.length()) {
				if (!isdigit(total[i])) {
					tool_show_error("非法输入",1);
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
/// 管理员管理图书视图 
int managebook_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb("图书");
    path.pb(usernow->username); 
	menu.pb("(0) 返回");
	menu.pb("(1) 查看图书与修改信息和库存");
	menu.pb("(2) 添加图书");
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
/// 管理员查看用户信息及修改其权限视图 
int admin_user_detail_view(User*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb("用户");
    path.pb("查看与修改");
	path.pb(now->username);
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	if (now!=usernow && !now->is_admin)
		menu.pb("(1) 更改借用权限");
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
			tool_show_info("修改用户权限成功",1);
		}
	}
}
/// 管理员查看所有用户视图 
int manageuser_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb("用户");
    path.pb(usernow->username); 
	menu.pb("(0) 返回");
	menu.pb("(1) 查看用户详细信息与修改用户权限");
	User* tmpu;
	while (true) {
		tool_show_path(path);
		tool_show_up();
		
		_rep(i,0,users.alluser.size()) {
			tmpu = users.alluser[i];
			printf("[%d] %s，邮箱：%s %s %s\n",i,tmpu->username.c_str(),tmpu->email.c_str(),(tmpu->is_admin?"<管理员>":"        "),(tmpu->is_banned?"<被封禁>":"        "));
		}
		
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string tmp;
			int i;
			tmp = tool_show_require("输入用户编号（方括号内数字）");
			rep(i,0,users.alluser.size())
				if (tmp == i2str(i)) break;
			if (i < users.alluser.size()) admin_user_detail_view(users.alluser[i]);
			else tool_show_error("无效输入",1);
		} 
	}
	return 1;
}
/// 管理员查看图书馆统计信息视图 
int statistics_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb("统计");
    path.pb(usernow->username);
	menu.pb("(0) 返回");
	
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
		printf("截止到%s：\n",time2str(time(NULL)).c_str());
		printf("【用    户】：%d（管理员：%d，被封禁：%d）\n",totu,totadm,totban);
		printf("【图    书】：%d（总库存：%d，已借出：%d，在架上：%d，无库存：%d）\n",totb,tott,totl,tott-totl,totd);
		printf("【借用记录】：%d条\n",totr);
		tool_show_down();
		
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
	}
	return 1;
}
/// 管理员视图 
int admin_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("管理");
    path.pb(usernow->username); 
	menu.pb("(0) 登出");
	menu.pb("(1) 进入使用界面");
	menu.pb("(2) 管理图书");
	menu.pb("(3) 管理用户");
	menu.pb("(4) 信息统计");
	menu.pb("(5) 用户设置");
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
