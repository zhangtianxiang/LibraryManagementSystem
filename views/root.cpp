#ifndef __ROOT_VIEW__
#define __ROOT_VIEW__ 1

/**
* ���û�������� 
*/

#include "../tools/basetool.cpp"
#include "../tools/viewtool.cpp"

/// ���û����û�Ȩ�޽����޸� 
int root_user_detail_view(User*now) {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
    path.pb("��̨");
    path.pb("�鿴���޸��û�");
	path.pb(now->username);
	path.pb("root");
	menu.pb("(0) ����");
	menu.pb("(1) ���Ľ���Ȩ��");
	menu.pb("(2) ���Ĺ���Ȩ��"); 
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
			tool_show_info("�޸��û�����Ȩ�޳ɹ�",1);
		} else if (sta == 2) {
			now->is_admin ^= 1;
			tool_show_info("�޸��û�����Ȩ�޳ɹ�",1);
		}
	}
}
/// ���û���ͼ 
int root_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("��̨");
    path.pb("root");
    menu.pb("(0) �˳�");
    menu.pb("(1) �����û�Ȩ��");
    User*tmpu;
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
			if (i < users.alluser.size()) root_user_detail_view(users.alluser[i]);
			else tool_show_error("��Ч����",1);
		}
	}
	return 1;
}

#endif
