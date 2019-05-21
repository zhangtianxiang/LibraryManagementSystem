#ifndef __REGISTER_VIEW__
#define __REGISTER_VIEW__ 1

/**
* ע����� 
*/

#include "../tools/basetool.cpp"

#include "../globals/global.cpp"
#include "../tools/viewtool.cpp"
#include "../models/model.cpp"
/// ע����ͼ 
int register_view() {
	vector<string>path;
	path.pb(SYSTEMNAME);
	path.pb("ע��");
	path.pb("");
	vector<string>menu; 
	menu.pb("(0) ����"); 
	menu.pb("(1) ȷ��ע��");
	menu.pb("(2) ��������");
	int flag = 0;
	while (true) {
		tool_show_path(path);
		string username;
		string password, pas2;
		string email;
		if (!flag) flag = 1;
		else {
			vector<string>opt;
			opt.pb("(0) ����"); 
			opt.pb("(1) ����ע��");
			while (true) {
				int sta = tool_show_menu(opt);
				if (sta == -1) continue;
				else if (sta == 0) return 0;
				else if (sta == 1) break;
			}
		}
		username = tool_show_require("������ѧ����");
		if (users.select(-1,username,"","",-1,-1).size() > 0) {
			tool_show_error("�ѱ�ʹ�õ��û���",1);
			continue;
		}
		password = tool_show_require("���������룬8��16λ������ĸ�����ֺ��ַ�����",1);
		int level = tool_check_password(password);
		if (level == 0) continue;
		pas2 = tool_show_require("���ٴ���������",1);
		if (password != pas2) {
			tool_show_error("�����������벻һ��",1);
			continue;
		}
		email = tool_show_require("����������");
		while (true) {
			printf("������������밲ȫ����Ϊ");
			if (level == 1) puts("��");
			else if (level == 2) puts("��");
			else puts("��");
			int sta = tool_show_menu(menu);
			if (sta == -1) continue; 
			else if (sta == 0) return 0;
			else if (sta == 1) {
				usernow = users.insert(username,password,email,0,0,CANBORROW,0);
				commit_user();
				tool_show_info("ע��ɹ�",1);
				return 1;
			} else if (sta == 2) break;
		}
	} 
	return 1;
}

#endif 
