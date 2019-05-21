#ifndef  __BASETOOL__
#define  __BASETOOL__  1

/**
* 引入最基本的头文件，定义基本的宏定义，定义基础输入输出函数与字符处理函数 
*/

/// 头文件 
#include <bits/stdc++.h>
#include <conio.h>

/// 命名空间设置为std 
using namespace std;

/// 基本宏定义 
#define Rep(i,l,r) for(i=(l);i<=(r);i++)
#define Rev(i,r,l) for(i=(r);i>=(l);i--)
#define rep(i,l,r) for(i=(l);i< (r);i++)
#define rev(i,r,l) for(i=(r);i> (l);i--)
#define _Rep(i,l,r)  for(int i=(l);i<=(r);i++)
#define _Rev(i,r,l)  for(int i=(r);i>=(l);i--)
#define _rep(i,l,r)  for(int i=(l);i< (r);i++)
#define _rev(i,l,r)  for(int i=(r);i> (l);i--)
#define Each(i,v)  for(i=v.begin();i!=v.end();i++)
#define pb  push_back
#define mp  make_pair
#define linelen 1024 

/// 类型别名
typedef long long ll;
typedef double lf;
typedef unsigned uint;
typedef pair<int,int> pii;
typedef pair<lf,lf> pff;

/// 输出调试信息 
inline void bug(string a,int b){
    cout<<a<<" = "<<b<<endl;
    getchar();
}

/// 输入cnt个空格 
void whitespace(int cnt) {
    while(cnt-->0)putchar(' ');
}

/// 读入一行返回string 
string mygetline() {
    static char str[linelen];
    while (gets(str), strlen(str) == 0);
    string ret = str;
    return ret;
}

/// 读入多行，以单独一行“#”结束，返回string 
string mygetlines() {
	static char str[linelen];
	while (gets(str), strlen(str) == 0);
	string ret = "", now;
	while (now = str, now != "#") {
		if (ret != "") ret += '\n';
		ret += now;
		gets(str);
	}
	return ret;
}

/// 读入密码，以*号代替字符，利用了控制台输入输出库 
string mygetpass() {
    static char str[linelen];
    static int now;
    string ret = "";
    now = 0;
    char ch;
    while (ch = getch(), true) {
        if (ch == '\n' || ch == '\r') {
            str[now] = '\0';
            putchar('\n');
            break;
        }
        if (ch == '\b') {
            if (now) printf("\b \b"), now -- ;
            continue;
        }
        str[now++] = ch;
        putchar('*');
    }
    ret = str;
    return ret;
}

/// int -> string
string i2str(int x) {
	char s[10];
	sprintf(s,"%d",x);
	string ret = s;
	return ret;
}

char encode_str[linelen];
/// 对字符串进行编码
char* Encode(string s) {
    #define t encode_str
    int len = s.length(), i, j = 0;
    t[j++] = '#';
    rep (i,0,len) {
        if (s[i] == '\\') {
            t[j++] = '\\';
            t[j++] = '\\';
        } else if (s[i] == '\n') {
            t[j++] = '\\';
            t[j++] = 'n';
        } else if (s[i] == '\t') {
            t[j++] = '\\';
            t[j++] = 't';
        } else if (s[i] == '\r') {
            t[j++] = '\\';
            t[j++] = 'r';
        } else if (s[i] == ' ') {
            t[j++] = '\\';
            t[j++] = 's';
        } else if (isprint(s[i]) || s[i] < 0) t[j++] = s[i];
    }
    t[j] = '\0';
    return t;
    #undef t
}
/// 对字符串进行解码
string Decode(const char *s) {
    string t = "";
    int len = strlen(s), i;
    rep (i,1,len) {
        if (s[i] == '\\') {
            i ++ ;
            if (s[i] == '\\') t += '\\';
            else if (s[i] == 'n') t += '\n';
            else if (s[i] == 't') t += '\t';
            else if (s[i] == 'r') t += '\r';
            else if (s[i] == 's') t += ' ';
        } else t += s[i];
    }
    return t;
}

/// 从文件流f读入一个int
void load_int(FILE*f,int&a) {
    fscanf(f,"%d",&a);
}
/// 从文件流f读入一个ll
void load_ll(FILE*f,ll&a) {
    fscanf(f,"%lld",&a);
}
/// 从文件流f读入一个bool
void load_int(FILE*f,bool&a) {
    fscanf(f,"%d",&a);
}
/// 从文件流f读入一个string并解码
void load_str(FILE*f,string&a) {
    static char tmp[256];
    fscanf(f,"%s",tmp);
    a = Decode(tmp);
}
/// 从文件流f输出一个int
void export_int(FILE*f,int a) {
    fprintf(f,"%d ",a);
}
/// 从文件流f输出一个ll
void export_ll(FILE*f,ll a) {
    fprintf(f,"%lld ",a);
}
/// 从文件流f输出一个编码后的string
void export_str(FILE*f,string a) {
    fprintf(f,"%s ",Encode(a));
}

/// time_t -> string 
string time2str(time_t t) {
	char *a = ctime(&t);
	string ret="";
	int len = strlen(a);
	for (int i=0;i<len-1;i++) ret+=a[i];
	return ret;
}

/// 暂停 
void pause() {
    getch();
}

#endif

