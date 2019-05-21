#ifndef __DATABASE__
#define __DATABASE__ 1

/**
* �������ݣ��洢���� 
*/

#include "../configs/config.h"
#include "../tools/basetool.cpp"
#include "../tools/viewtool.cpp"

FILE *fu, *fb, *fr;

/// �ؽ��������ݿ�
int build_all() {
    #define ope(f,s) if((f=fopen(s,"w")) == NULL) { tool_show_error("Build database failed!"); return 0; } else fclose(f)
    ope(fu,DB_USER);
    ope(fb,DB_BOOK);
    ope(fr,DB_RECORD);
    #undef ope
    tool_show_info("�������ݿ�ɹ�",1);
    return 1;
}

/// �����������ݿ�
int link_all() {
    int cnt = 0;
    #define ope(f,s) if((f=fopen(s,"r"))!=NULL) cnt ++ 
    ope(fu,DB_USER);
    ope(fb,DB_BOOK);
    ope(fr,DB_RECORD);
    #undef ope
    if (cnt < 3) return 0;
    return 1;
}

/// �ر����ݿ�����
int close_all() {
    #define  ope(f) if(f!=NULL) fclose(f);
    ope(fu);
    ope(fb);
    ope(fr);
    #undef ope
    return 1;
}

/// �����û�
void load_user() {
    int tot;
    if (fscanf(fu,"%d", &tot) == EOF) tot = 0;
    if (tot == 0) return;
    users.idx = tot;
    User* now;
    _rep(i,0,tot) {
        now = new User;
        now->id = i;
        load_str(fu,now->username);
        load_str(fu,now->password);
        load_str(fu,now->email);
        load_int(fu,now->is_admin);
        load_int(fu,now->is_banned);
        load_int(fu,now->can_borrow);
        load_int(fu,now->borrowed);
        users.alluser.pb(now);
    }
}

/// �����鼮
void load_book() {
    int tot;
    if (fscanf(fb,"%d", &tot) == EOF) tot = 0;
    if (tot == 0) return;
    books.idx = tot;
    Book* now;
    _rep(i,0,tot) {
        now = new Book;
        now->id = i;
        load_ll(fb,now->ISBN);
        load_str(fb,now->bookname);
        load_str(fb,now->press);
        load_str(fb,now->author);
        load_str(fb,now->about_author);
        load_str(fb,now->intro);
        load_str(fb,now->catalog);
        load_int(fb,now->page);
        load_int(fb,now->total);
        load_int(fb,now->lent);
        load_int(fb,now->allow_lend);
        books.allbook.pb(now);
        books.isbn.insert(now->ISBN,now);
    }
}

/// ������־ 
void load_record() {
	int tot;
    if (fscanf(fr,"%d", &tot) == EOF) tot = 0;
    if (tot == 0) return;
    records.idx = tot;
    Record* now;
    _rep(i,0,tot) {
    	now = new Record;
    	now->id = i;
    	load_int(fr,now->uid);
    	load_int(fr,now->bid);
    	load_int(fr,now->tp);
    	load_ll(fr,now->start);
    	load_ll(fr,now->end);
    	load_int(fr,now->num);
    	records.allrecord.pb(now);
    }
}

/// �������ӵ��������ݿ�ʱ�������ݿ��г�ʼ�����ݣ�������ʾ�ؽ����ݿ⡣
int load_data() {
    if (link_all()) {
        load_user();
        load_book();
        load_record();
        close_all();
        return 1;
    } else {
        vector<string> path, menu;
        path.pb("������ѧ�Ž�У��ͼ���");
        path.pb("�������ݿ�");
        path.pb("");
        menu.pb("(0) �˳�");
        menu.pb("(1) �ؽ����ݿ�");
        tool_show_path(path);
        tool_show_info("�������ݿ�ʧ�ܣ��Ƿ��ؽ����ݿ⣿");
        while(1) {
            int sta = tool_show_menu(menu);
            if (sta == -1) continue;
            if (sta == 0) return 0;
            if (sta == 1) return build_all();
        }
    }
}

/// �����û��ĸ���
void commit_user() {
    if ((fu=fopen(DB_USER,"w")) == NULL) {
        tool_show_error("Cannot find data file\"user.txt\"",1);
        return ;
    }
    export_int(fu,users.idx);
    _rep(i,0,users.idx) {
        #define now (users.alluser[i])
        export_str(fu,now->username);
        export_str(fu,now->password);
        export_str(fu,now->email);
        export_int(fu,now->is_admin);
        export_int(fu,now->is_banned);
        export_int(fu,now->can_borrow);
        export_int(fu,now->borrowed);
        #undef now
    }
    fclose(fu);
}

/// �����鼮�ĸ���
void commit_book() {
    if ((fb=fopen(DB_BOOK,"w")) == NULL) {
        tool_show_error("Cannot find data file\"book.txt\"",1);
        return ;
    }
    export_int(fb,books.idx);
    _rep(i,0,books.idx) {
        #define now (books.allbook[i])
        export_ll(fb,now->ISBN);
        export_str(fb,now->bookname);
        export_str(fb,now->press);
        export_str(fb,now->author);
        export_str(fb,now->about_author);
        export_str(fb,now->intro);
        export_str(fb,now->catalog);
        export_int(fb,now->page);
        export_int(fb,now->total);
        export_int(fb,now->lent);
        export_int(fb,now->allow_lend);
        #undef now
    }
    fclose(fb);
}
/// ������־ 
void commit_record() {
    if ((fr=fopen(DB_RECORD,"w")) == NULL) {
        tool_show_error("Cannot find data file\"record.txt\"",1);
        return ;
    }
    export_int(fr,records.idx);
    _rep(i,0,records.idx) {
        #define now (records.allrecord[i])
        export_int(fr,now->uid);
        export_int(fr,now->bid);
        export_int(fr,now->tp);
        export_ll(fr,now->start);
        export_ll(fr,now->end);
        export_int(fr,now->num);
        #undef now
    }
    fclose(fr);
}

#endif
