#include "lua/lua.hpp"
#include <iostream>
using namespace std;

//��ʾջ�����
static void stackDump(lua_State* L);

int main()
{
    //����һ��state
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    //��lua�ļ�
    int fret = luaL_dofile(L,"luac.lua");
    if(fret)
    {
        std::cout<<"read lua file error!"<<std::endl;
    }

    //��ȡ����
    lua_getglobal(L,"name");   //string to be indexed
    std::cout<<"name = "<<lua_tostring(L,-1)<<std::endl;

    //��ȡ����
    lua_getglobal(L,"version"); //number to be indexed
    std::cout<<"version = "<<lua_tonumber(L,-1)<<std::endl;

    //��ȡ��
    lua_getglobal(L, "me");  //table to be indexed
    if(!lua_istable(L,-1))
    {
        std::cout<<"error:it is not a table"<<std::endl;
    }
    //ȡ����Ԫ��
    lua_getfield(L, -1 ,"name");
    std::cout<<"student name = "<<lua_tostring(L,-1)<<std::endl;
    lua_getfield(L,-2,"gender");
    std::cout<<"student gender = "<<lua_tostring(L,-1)<<std::endl;

    //�޸ı���Ԫ��
    lua_pushstring(L, "007");
    lua_setfield(L,-4, "name");
    lua_getfield(L, -3 ,"name");
    std::cout<<"student newName = "<<lua_tostring(L,-1)<<std::endl;

    //ȡ����
    lua_getglobal(L,"add");
    lua_pushnumber(L,15);
    lua_pushnumber(L,5);
    lua_pcall(L,2,1,0);//2-������ʽ��1-����ֵ���������ú���������ִ���꣬�Ὣ����ֵѹ��ջ��
    std::cout<<"5 + 15 = "<<lua_tonumber(L,-1)<<std::endl;

    //�鿴ջ
    stackDump(L);

    //�ر�state
    lua_close(L);
    return 0;
}

static void stackDump(lua_State* L) {
    cout << "\nbegin dump lua stack" << endl;
    int i = 0;
    int top = lua_gettop(L);
    for (i = 1; i <= top; ++i) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING: {
                printf("'%s' ", lua_tostring(L, i));
            }
                break;
            case LUA_TBOOLEAN: {
                printf(lua_toboolean(L, i) ? "true " : "false ");
            }
                break;
            case LUA_TNUMBER: {
                printf("%g ", lua_tonumber(L, i));
            }
                break;
            default: {
                printf("%s ", lua_typename(L, t));
            }
                break;
        }
    }
    cout << "\nend dump lua stack" << endl;
}
