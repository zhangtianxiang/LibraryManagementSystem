#ifndef __RECORD_MODEL__
#define __RECORD_MODEL__

/**
* ������־�Ĵ洢�ṹ����ز��� 
*/

#include "../tools/basetool.cpp"
/// ������¼ 
class Record {
public:
	int id; // record id
	int uid;// user id
	int bid;// book id
	int tp;
	time_t start, end;// start-end time
	int num; 
	// tp=1 user borrow if returned change to tp=2    start
	// tp=2 user borrowed and returned              start+end
	// tp=3 user return                                end
	// tp=4 user reserved if borrowed change to tp=5  
	// tp=5 user reserved and borrowed                
	// tp=6 admin add a new book                      start num
	// tp=7 admin update store of book                start num
	
	/*
	time_t time(time_t *time);
	time_t --> struct tm : struct tm *localtime(const time_t *time);
	struct tm --> time_t : time_t mktime(struct tm *time);
	difftime             : double difftime ( time_t time2, time_t time1 );
	time_t           �����char *ctime(const time_t *time);
	struct tm        �����char * asctime ( const struct tm * time ); 
	���Դ洢time_t������ʱ���õ����� 
	*/
	Record(){id=-1;}
	Record(const int&id,const int&uid,const int&bid,const int&tp,const time_t&start,const time_t&end,const int&num):
		id(id),uid(uid),bid(bid),tp(tp),start(start),end(end),num(num){};
};
/// ���м�¼ 
class Records {
public:
	int idx;
	vector<Record*> allrecord;
	
	Records(){idx=0;allrecord.clear();}
	Record* insert(const int&uid,const int&bid,const int&tp,const time_t&start,const time_t&end,const int&num) {
		Record* newrecord = new Record(idx++,uid,bid,tp,start,end,num);
		allrecord.pb(newrecord);
		return newrecord;
	}
	vector<Record*> select(const int&id=-1,const int&uid=-1,const int&bid=-1,const int&tp=-1) {
		vector<Record*> item;
		vector<Record*>::iterator i;
		Each(i,allrecord) {
			if (id!=-1 && (*i)->id!=id) continue;
			if (uid!=-1 && (*i)->uid!=uid) continue;
			if (bid!=-1 && (*i)->bid!=bid) continue;
			if (tp!=-1 && (*i)->tp!=tp) continue;
			item.pb(*i);
		}
		return item;
	}
};

#endif
