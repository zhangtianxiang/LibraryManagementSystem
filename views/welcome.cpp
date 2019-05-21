#ifndef __WELCOME_VIEW__
#define __WELCOME_VIEW__ 1

/**
* ����ӭ����  ��ʼ���� 
*/

#include "../tools/basetool.cpp"
#include "../tools/viewtool.cpp"
#include "../configs/config.h"

#include "login.cpp"
#include "user.cpp"
#include "admin.cpp"
#include "register.cpp"
#include "root.cpp" 
/// ��ʼ���� 
int welcome_view() {
    vector<string> path, menu;
    path.pb(SYSTEMNAME);
    path.pb("");
    menu.pb("(0) �˳�");
    menu.pb("(1) ��¼");
    menu.pb("(2) ע��"); 
    menu.pb("(3) ����");
    while (true) {
        tool_show_path(path);
        if (WELCOME != "") tool_show_info(WELCOME); 
        int sta = tool_show_menu(menu);
        if (sta == -1) continue;
        else if (sta == 0) return 1;
        else if (sta == 1) {
            if (login_view()) {
                if (usernow->is_admin) admin_view();
                else user_view();
            }
        } else if (sta == 2) {
            if (register_view())
            	user_view();
        } else if (sta == 3) {
			if (root_login_view()) 
            	root_view();
        }
    }
}

#endif
