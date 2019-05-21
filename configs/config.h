#ifndef __CONFIG
#define __CONFIG 1
/**
* 系统参数设置 
*/

// 以下修改可不重建数据库 
#define BORROWTIME 60          /// 每次最长借用60天 
#define LEFTTIME   5           /// 剩余5天时提醒 
#define CANBORROW  10          /// 新用户可以借书数量 
const string SYSTEMNAME = "复旦大学张江校区图书馆";
const string WELCOME = "欢迎来到复旦大学张江校区图书馆！"; 
// 以下参数建议只在首次运行更改 
#define BOOKSIZE   5000000LL   /// 图书馆藏书最大值，二倍用以哈希，建议小于10^7 
#define HASHKEY    9999991LL   /// 小于等于二倍BOOKSIZE的质数 
#define USER_PASS_LEN_MIN 6    /// 用户密码的最短长度 
#define USER_PASS_LEN_MAX 16   /// 用户密码的最长长度 
#define DAYSIZE   86400        /// 86400 秒每天

// 以下下修改需要重建数据库 
#define DB_USER "databases/user.txt"
#define DB_BOOK "databases/book.txt"
#define DB_RECORD "databases/record.txt"
 
#endif
