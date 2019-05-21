#ifndef __VIEWTOOL__
#define __VIEWTOOL__ 1

/**
* 视图工具
*/

#include "basetool.cpp"
#include <windows.h>

/// 输出上横线 
void tool_show_up() {
	puts("──────────────────────────────────────────────────");
}
/// 输出间隔线 
void tool_show_mid() {
	puts("┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄");
}
/// 输出下横线 
void tool_show_down() {
	puts("──────────────────────────────────────────────────");
}
/// 输出信息或提示，slp=1时延时 ，依赖于windows-api 
void tool_show_info(const string&info,int slp=0) {
	// info or hint
	puts("┌────────────────────────────────────────────────┐");
	printf("│Info: %s",info.c_str());
	whitespace(90-info.length());
	puts("│");
	puts("└────────────────────────────────────────────────┘");
	if (slp) Sleep(500);
}
/// 输出错误信息，slp=1时延时，依赖于windows-api 
void tool_show_error(const string&err,int slp=0) {
	puts("┌┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┐");
	printf("┆Error: %s",err.c_str());
	whitespace(89-err.length());
	puts("┆");
	puts("└┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┘");	
	if (slp) Sleep(500);
} 
/// 输出路径条 
void tool_show_path(const vector<string>&path) {
	system("cls"); 
	int size = path.size();
	if (path[size-1] == "") puts("╭────────────────────────────────────────────────╮");
	else puts("╭─────────────────────────────────────────┬──────╮");
	printf("│");
	int cnt = 2;
	_rep (i,0,size-1) {
		printf("%s > ",path[i].c_str());
		cnt += path[i].length()+3;
	}
	if (path[size-1] == "") {
		whitespace(98-cnt);
		puts("│");
	} else {
		whitespace(84-cnt);
		printf("│");
		whitespace(12-path[size-1].length());
		printf("%s│\n",path[size-1].c_str());
	}
	if (path[size-1] == "") puts("╰────────────────────────────────────────────────╯");
	else puts("╰─────────────────────────────────────────┴──────╯");
}
/// 输出菜单并返回选择项，-1表示无效输入。 
int tool_show_menu(const vector<string>&menu) {
//	puts("├────────────────────────────────────────────────┤");
	_rep(i,0,menu.size()) {
		printf("│%s",menu[i].c_str());
		whitespace(96 - menu[i].length());
		puts("│");
	}
	puts("╰────────────────────────────────────────────────╯");
	string mode = mygetline();
	string num;
	char tmp[10];
	_rep (i,0,menu.size()) {
		sprintf(tmp,"%d",i);
		num = tmp;
		if (mode == num) return i;
	}
	tool_show_error("未知命令");
	return -1;
}
/// 输出带提示的输入框，pass=1时以密文方式输入。 
string tool_show_require(const string&req,bool pass=false) {
	puts("┌────────────────────────────────────────────────┐");
	printf("│> %s",req.c_str());
	whitespace(94-req.length());
	puts("│");
	puts("├────────────────────────────────────────────────┤");
	printf("│");
	string ret = pass ? mygetpass() : mygetline();
	puts("└────────────────────────────────────────────────┘");
	return ret;
}
/// 输出带提示的多行输入框 
string tool_show_requires(const string&req) {
	// 输入多行以单独一行'#'结束 
	puts("┌────────────────────────────────────────────────┐");
	printf("│> %s（多行输入，以单独一行\"#\"结束输入）",req.c_str());
	whitespace(94-req.length());
	puts("│");
	puts("├────────────────────────────────────────────────┤");
	string ret = mygetlines();
	puts("└────────────────────────────────────────────────┘");
	return ret;
}
#endif
