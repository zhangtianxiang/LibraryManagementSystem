#ifndef __LOGIN_VIEW__
#define __LOGIN_VIEW__ 1

/**
* ��¼���� 
*/

#include "../tools/basetool.cpp"

#include "../globals/global.cpp"
#include "../tools/viewtool.cpp"
#include "../models/model.cpp"
#include "../configs/environment_variable.h"

/// �û���¼ 
int login_view() {
	vector<string>path;
	path.pb(SYSTEMNAME);
	path.pb("��¼");
	path.pb("");
	vector<string>menu; 
	menu.pb("(0) ����"); 
	menu.pb("(1) ȷ�ϵ�¼");
	menu.pb("(2) ��������");
	while (true) {
		tool_show_path(path);
		string username;
		string password;
		username = tool_show_require("������ѧ����");
		password = tool_show_require("����������",1);
		while (true) {
			int sta = tool_show_menu(menu);
			if (sta == -1) continue; 
			else if (sta == 0) return 0;
			else if (sta == 1) {
				vector<User*>item = users.select(-1,username,password);
				if (item.size() == 0) {
					tool_show_error("ѧ���Ż��������",1);
					return 0;
				} else if (item.size() > 1) {
					tool_show_error("ϵͳ�����ظ��û������룡",1);
					return 0;
				} else {
					tool_show_info("��½�ɹ���",1);
					usernow = *item.begin();
					return 1;
				}
			} else if (sta == 2) break;
		}
	}
}

/// ���û���¼ 
int root_login_view() {
	vector<string>path;
	path.pb(SYSTEMNAME);
	path.pb("���û���¼");
	path.pb("");
	tool_show_path(path);
	string password = tool_show_require("��������û�����",1);
	if (password == ROOTPASS) {
		tool_show_info("��½�ɹ�",1);
		return 1;
	} else {
		tool_show_error("�������",1);
		return 0;
	}
}

#endif
