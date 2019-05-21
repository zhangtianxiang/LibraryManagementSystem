#ifndef __REGISTER_VIEW__
#define __REGISTER_VIEW__ 1

/**
* 注册界面 
*/

#include "../tools/basetool.cpp"

#include "../globals/global.cpp"
#include "../tools/viewtool.cpp"
#include "../models/model.cpp"
/// 注册视图 
int register_view() {
	vector<string>path;
	path.pb(SYSTEMNAME);
	path.pb("注册");
	path.pb("");
	vector<string>menu; 
	menu.pb("(0) 返回"); 
	menu.pb("(1) 确认注册");
	menu.pb("(2) 重新输入");
	int flag = 0;
	while (true) {
		tool_show_path(path);
		string username;
		string password, pas2;
		string email;
		if (!flag) flag = 1;
		else {
			vector<string>opt;
			opt.pb("(0) 返回"); 
			opt.pb("(1) 继续注册");
			while (true) {
				int sta = tool_show_menu(opt);
				if (sta == -1) continue;
				else if (sta == 0) return 0;
				else if (sta == 1) break;
			}
		}
		username = tool_show_require("请输入学工号");
		if (users.select(-1,username,"","",-1,-1).size() > 0) {
			tool_show_error("已被使用的用户名",1);
			continue;
		}
		password = tool_show_require("请输入密码，8至16位，由字母和数字和字符构成",1);
		int level = tool_check_password(password);
		if (level == 0) continue;
		pas2 = tool_show_require("请再次输入密码",1);
		if (password != pas2) {
			tool_show_error("两次输入密码不一致",1);
			continue;
		}
		email = tool_show_require("请输入邮箱");
		while (true) {
			printf("您输入的新密码安全级别为");
			if (level == 1) puts("低");
			else if (level == 2) puts("中");
			else puts("高");
			int sta = tool_show_menu(menu);
			if (sta == -1) continue; 
			else if (sta == 0) return 0;
			else if (sta == 1) {
				usernow = users.insert(username,password,email,0,0,CANBORROW,0);
				commit_user();
				tool_show_info("注册成功",1);
				return 1;
			} else if (sta == 2) break;
		}
	} 
	return 1;
}

#endif 
