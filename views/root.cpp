#ifndef __ROOT_VIEW__
#define __ROOT_VIEW__ 1

/**
* 根用户管理界面 
*/

#include "../tools/basetool.cpp"
#include "../tools/viewtool.cpp"

/// 根用户对用户权限进行修改 
int root_user_detail_view(User*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
    path.pb("后台");
    path.pb("查看与修改用户");
	path.pb(now->username);
	path.pb("root");
	menu.pb("(0) 返回");
	menu.pb("(1) 更改借用权限");
	menu.pb("(2) 更改管理权限"); 
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
			tool_show_info("修改用户借书权限成功",1);
		} else if (sta == 2) {
			now->is_admin ^= 1;
			tool_show_info("修改用户管理权限成功",1);
		}
	}
}
/// 根用户视图 
int root_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("后台");
    path.pb("root");
    menu.pb("(0) 退出");
    menu.pb("(1) 更改用户权限");
    User*tmpu;
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
			if (i < users.alluser.size()) root_user_detail_view(users.alluser[i]);
			else tool_show_error("无效输入",1);
		}
	}
	return 1;
}

#endif
