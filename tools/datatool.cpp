#ifndef __DATATOOL__
#define __DATATOOL__ 1

/**
* 数据处理工具 
*/

#include "../configs/config.h"
#include "basetool.cpp"
#include "viewtool.cpp"

/// dp求两个string的匹配度，返回pff
pff LCS(string a,string b) {
	static int f[256][256];
	memset(f,0,sizeof f);
	int n = a.length(), m = b.length();
	a = "#"+a;
	b = "#"+b;
	if (!n || !m) return mp(0,0);
	_Rep(i,0,n) _Rep(j,0,m) {
		if (!i || !j) { f[i][j] = 0; continue; }
		f[i][j] = max(f[i-1][j],f[i][j-1]);
		int delta = (a[i]==b[j]);
		if (!delta && isalpha(a[i]) && isalpha(b[j])) delta = (tolower(a[i])==tolower(b[j]));
		if (i && j) f[i][j] = max(f[i][j],f[i-1][j-1]+delta);
	}
	return mp(1.0-(lf)f[n][m]/n,1.0-(lf)f[n][m]/m);
}

/// 是否为合法的密码，且返回密码的安全级别
int tool_check_password(string s) {
	int len = s.length(), i, sta=0, rt=0;
	if (len < USER_PASS_LEN_MIN || len > USER_PASS_LEN_MAX) {
		tool_show_error("密码长度不合法");
		return 0;
	}
	rep (i,0,len) {
		if (isalpha(s[i])) sta |= 1;
		else if (isdigit(s[i])) sta |= 2;
		else if (ispunct(s[i])) sta |= 4;
		else {
			tool_show_error("密码中存在非法字符");
			return 0;
		}
	}
	if (sta&1) rt++;
	if (sta&2) rt++;
	if (sta&4) rt++;
	return rt;
}

#endif
