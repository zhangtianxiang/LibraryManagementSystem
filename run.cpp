#ifndef ___SYSTEM___
#define ___SYSTEM___ 1
/**
* Æô¶¯ÏµÍ³ 
*/

#include "tools/basetool.cpp"
#include "globals/global.cpp"

#include "databases/database.cpp"
#include "views/view.cpp"

int main() {
    int sta;
    if (!load_data()) return 0;
    welcome_view();
    return 0;
}
#endif
