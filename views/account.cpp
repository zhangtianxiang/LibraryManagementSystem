#ifndef __ACCOUNT_VIEW__
#define __ACCOUNT_VIEW__ 1

/**
* �˻���ؽ��� 
*/

#include "../tools/basetool.cpp"
#include "../tools/datatool.cpp" 

/// �˻�������ͼ 
int account_view() {
	vector<string> path, menu;
	path.pb(SYSTEMNAME);
	path.pb("�û�����");
	path.pb(usernow->username);
	menu.pb("(0) ����");
	menu.pb("(1) �޸�����");
	menu.pb("(2) �޸�����"); 
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
			pas1 = tool_show_require("������ԭ��������",1);
			if (pas1 != usernow->password) {
				tool_show_error("�������",1);
				continue;
			}
			pas2 = tool_show_require("�����������룬8��16λ������ĸ�����ֺ��ַ�����",1);
			int level = tool_check_password(pas2);
			if (level == 0) continue;
			pas3 = tool_show_require("���ٴ�����������",1);
			if (pas3 != pas2) {
				tool_show_error("�����������벻һ��",1);
				continue;
			}
			while (true) {
				printf("������������밲ȫ����Ϊ");
				if (level == 1) puts("��");
				else if (level == 2) puts("��");
				else puts("��");
				puts("�Ƿ�ȷ���޸ģ�");
				vector<string>opt;
				opt.pb("(0) ȡ��");
				opt.pb("(1) ȷ��");
				int sta2 = tool_show_menu(opt);
				if (sta2 == -1) continue; 
				else if (sta2 == 0) break;
				else if (sta2 == 1) {
					usernow->password = pas2;
					commit_user();
					tool_show_info("�޸�����ɹ�",1);
					break; 
				}
			}
		} else if (sta == 2) {
			string email = tool_show_require("������������");
			usernow->email = email;
			commit_user();
			tool_show_info("�޸�����ɹ�",1);
		}
	}
    return 1;
}

#endif
