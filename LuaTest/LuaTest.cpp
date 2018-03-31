// LuaTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
extern "C" {
#include "src/lualib.h"
#include "src/lauxlib.h"
#include <time.h>
}

using namespace std;

int main()
{
    cout << "abc:" << time(NULL) << endl;

    char *file = "my.lua";
    lua_State *L = luaL_newstate(); // r1
    luaL_openlibs(L);
    luaL_dofile(L, file);

    time_t timep;
    tm p;
    p.tm_year = 70;
    p.tm_mon = 1;
    p.tm_mday = 1;
    p.tm_hour = 1;
    p.tm_min = 1;
    p.tm_sec = 1;
    //time(&timep);
    //printf("time():%d\n", timep);
    //localtime_s(&p, &timep);
    timep = mktime(&p);
    printf("time()->localtime()->mktime():%d\n", timep);

    cin.get();
    return 0;
}

