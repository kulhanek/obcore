// return suffix relative to OB installation directory
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

// _WIN32_WINNT is because GetModuleHandleEx
// see  http://msdn.microsoft.com/en-us/library/ms683200%28v=vs.85%29.aspx (last remark)
#define _WIN32_WINNT 0x0501
#include <windows.h>

using namespace std;

// -----------------------------------------------------------------------------
// this is not probably the best solution - but it should work
// -----------------------------------------------------------------------------

extern "C" const char* obprefix(const char* suffix)
{
    static map<string,string>       paths;
    map<string,string>::iterator    it;
    map<string,string>::iterator    ie = paths.end();
    string                          key(suffix);

    it = paths.find(key);
    if( it != ie ){
        return(it->second.c_str());
    }

    // get the path to libobcore.dll module
    HMODULE lib_handle = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                      reinterpret_cast <LPCTSTR>(&obprefix),&lib_handle);
    char buffer[2048];
    buffer[0] = '\0';
    if( lib_handle ){
        GetModuleFileName(lib_handle,buffer,2048);
        buffer[2047] = '\0';
        FreeLibrary(lib_handle);
    }
    std::string obcore_dll_name(buffer);

    // get app directory
    int last = 0;
    int count = 0;
    for(int i=(int)obcore_dll_name.size()-1; i >=0; i--){
        if( obcore_dll_name[i] == '\\' ){
            count++;
            if(count == 2){
                last = i;
            }
        }
    }
    std::string obcore_dir = obcore_dll_name.substr(0,last);

    // complete path
    string final_path = obcore_dir + key;
    paths[key] = final_path;

    return(final_path.c_str());
}

// -----------------------------------------------------------------------------
