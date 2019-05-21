#ifndef __ACCOUNT_VIEW__
#define __ACCOUNT_VIEW__ 1

/**
* 账户相关界面 
*/

#include "../tools/basetool.cpp"
#include "../tools/datatool.cpp" 

/// 账户设置视图 
int account_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("用户设置");
	path.pb(usernow->username);
	menu.pb("(0) 返回");
	menu.pb("(1) 修改密码");
	menu.pb("(2) 修改邮箱"); 
	while (true) {
		tool_show_path(path);
		tool_show_up();
		usernow->show_detail();
		tool_show_down();
		int sta = tool_show_menu(menu);
		if (sta == -1) continue;
		else if (sta == 0) return 1;
		else if (sta == 1) {
			string pas1, pas2, pas3;
			pas1 = tool_show_require("请输入原来的密码",1);
			if (pas1 != usernow->password) {
				tool_show_error("密码错误",1);
				continue;
			}
			pas2 = tool_show_require("请输入新密码，8至16位，由字母和数字和字符构成",1);
			int level = tool_check_password(pas2);
			if (level == 0) continue;
			pas3 = tool_show_require("请再次输入新密码",1);
			if (pas3 != pas2) {
				tool_show_error("两次输入密码不一致",1);
				continue;
			}
			while (true) {
				printf("您输入的新密码安全级别为");
				if (level == 1) puts("低");
				else if (level == 2) puts("中");
				else puts("高");
				puts("是否确认修改？");
				vector<string>opt;
				opt.pb("(0) 取消");
				opt.pb("(1) 确认");
				int sta2 = tool_show_menu(opt);
				if (sta2 == -1) continue; 
				else if (sta2 == 0) break;
				else if (sta2 == 1) {
					usernow->password = pas2;
					commit_user();
					tool_show_info("修改密码成功",1);
					break; 
				}
			}
		} else if (sta == 2) {
			string email = tool_show_require("请输入新邮箱");
			usernow->email = email;
			commit_user();
			tool_show_info("修改邮箱成功",1);
		}
	}
    return 1;
}

#endif
