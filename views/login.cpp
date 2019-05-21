#ifndef __LOGIN_VIEW__
#define __LOGIN_VIEW__ 1

/**
* 登录界面 
*/

#include "../tools/basetool.cpp"

#include "../globals/global.cpp"
#include "../tools/viewtool.cpp"
#include "../models/model.cpp"
#include "../configs/environment_variable.h"

/// 用户登录 
int login_view() {
	vector<string>path;
	path.pb(SYSTEMNAME);
	path.pb("登录");
	path.pb("");
	vector<string>menu; 
	menu.pb("(0) 返回"); 
	menu.pb("(1) 确认登录");
	menu.pb("(2) 重新输入");
	while (true) {
		tool_show_path(path);
		string username;
		string password;
		username = tool_show_require("请输入学工号");
		password = tool_show_require("请输入密码",1);
		while (true) {
			int sta = tool_show_menu(menu);
			if (sta == -1) continue; 
			else if (sta == 0) return 0;
			else if (sta == 1) {
				vector<User*>item = users.select(-1,username,password);
				if (item.size() == 0) {
					tool_show_error("学工号或密码错误！",1);
					return 0;
				} else if (item.size() > 1) {
					tool_show_error("系统错误：重复用户与密码！",1);
					return 0;
				} else {
					tool_show_info("登陆成功！",1);
					usernow = *item.begin();
					return 1;
				}
			} else if (sta == 2) break;
		}
	}
}

/// 根用户登录 
int root_login_view() {
	vector<string>path;
	path.pb(SYSTEMNAME);
	path.pb("根用户登录");
	path.pb("");
	tool_show_path(path);
	string password = tool_show_require("请输入根用户密码",1);
	if (password == ROOTPASS) {
		tool_show_info("登陆成功",1);
		return 1;
	} else {
		tool_show_error("密码错误",1);
		return 0;
	}
}

#endif
