#ifndef __CONFIG
#define __CONFIG 1
/**
* ϵͳ�������� 
*/

// �����޸Ŀɲ��ؽ����ݿ� 
#define BORROWTIME 60          /// ÿ�������60�� 
#define LEFTTIME   5           /// ʣ��5��ʱ���� 
#define CANBORROW  10          /// ���û����Խ������� 
const string SYSTEMNAME = "������ѧ�Ž�У��ͼ���";
const string WELCOME = "��ӭ����������ѧ�Ž�У��ͼ��ݣ�"; 
// ���²�������ֻ���״����и��� 
#define BOOKSIZE   5000000LL   /// ͼ��ݲ������ֵ���������Թ�ϣ������С��10^7 
#define HASHKEY    9999991LL   /// С�ڵ��ڶ���BOOKSIZE������ 
#define USER_PASS_LEN_MIN 6    /// �û��������̳��� 
#define USER_PASS_LEN_MAX 16   /// �û����������� 
#define DAYSIZE   86400        /// 86400 ��ÿ��

// �������޸���Ҫ�ؽ����ݿ� 
#define DB_USER "databases/user.txt"
#define DB_BOOK "databases/book.txt"
#define DB_RECORD "databases/record.txt"
 
#endif
