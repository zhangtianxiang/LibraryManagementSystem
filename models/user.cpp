#ifndef __USER_MODEL__
#define __USER_MODEL__ 1

/**
* �����û��ṹ����ز��� 
*/

#include "../tools/basetool.cpp"

/// �����û� 
class User {
public:
    int id;          // unique primary key
    // user main-information
    string username; // unique also school job number
    string password;
    string email;
    bool is_admin;
    bool is_banned;
    // user other-information
    int can_borrow;
    int borrowed;
    User(){id=-1;username=password=email="";is_banned=is_admin=0;can_borrow=borrowed=0;}
    User(const int&id,const string&username,const string&password,const string&email,const bool&is_admin,const bool&is_banned,const int&can_borrow,const int&borrowed):
        id(id),username(username),password(password),email(email),is_admin(is_admin),is_banned(is_banned),can_borrow(can_borrow),borrowed(borrowed){}
	void show_detail() {
		/// �û�չʾϸ�� 
		printf("���û�����%s %s %s\n",username.c_str(),(is_admin?"<����Ա>":"        "),(is_banned?"<�����>":"        "));
		printf("�����䡿��%s\n",email.c_str());
	}
};
/// �����û� 
class Users {
public:
    int idx;
    vector<User*> alluser;
    
    Users(){idx=0;alluser.clear();}
    User* insert(const string&username,const string&password,const string&email,const bool&is_admin,const bool&is_banned,const int&can_borrow,const int&borrowed) {
        User* newuser = new User(idx++,username,password,email,is_admin,is_banned,can_borrow,borrowed);
        alluser.pb(newuser);
        return newuser;
    }
    vector<User*> select(const int&id=-1,const string&username="",const string&password="",const string&email="",const int&is_admin=-1,const int&is_banned=-1) {
        vector<User*> item;
        vector<User*>::iterator i;
        Each(i,alluser) {
            if (id!=-1 && (*i)->id!=id) continue;
            if (username!="" && (*i)->username!=username) continue;
            if (password!="" && (*i)->password!=password) continue;
            if (email!="" && (*i)->email!=email) continue;
            if (is_admin!=-1 && is_admin!=(*i)->is_admin) continue;
            if (is_banned!=-1 && is_banned!=(*i)->is_banned) continue;
            item.pb(*i);
        }
        return item;
    }
};

#endif

