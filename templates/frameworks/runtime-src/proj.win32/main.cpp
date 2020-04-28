/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "main.h"
#include "cocos2d.h"
#include "../Classes/AppDelegate.h"
#include "setup.h"
using namespace std;

USING_NS_CC;

// uncomment below line, open debug console
#define USE_WIN32_CONSOLE

static WNDPROC oldWinProc = nullptr;
static FILE* fpLogFile = nullptr;

LRESULT CALLBACK winProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_COPYDATA: {
        PCOPYDATASTRUCT pMyCDS = (PCOPYDATASTRUCT)lParam;
        if (fpLogFile && pMyCDS->dwData == 1) {
            const char* szBuf = (const char*)(pMyCDS->lpData);
            fputs(szBuf, fpLogFile);
            fflush(fpLogFile);
            break;
        }
    }
    }
    return oldWinProc(hWnd, uMsg, wParam, lParam);
}

static std::string getEngineRoot(void)
{
    TCHAR szAppDir[MAX_PATH] = { 0 };
    if (!GetModuleFileName(NULL, szAppDir, MAX_PATH))
        return "";

    int len = wcslen(szAppDir) + 1;
    char* chAppDir = (char*)malloc(len);
    wcstombs(chAppDir, szAppDir, len);
    chAppDir[len - 1] = '\0';

    // find engine root
    char* p = strstr(chAppDir, "\\win32\\bin");
    if (p) *p = 0; // cut str
    std::string pathTemp(chAppDir);
    free(chAppDir); // free memory
    return pathTemp;
}


static void relaunchSelf(std::string& cmdLine)
{
    // get exe path
    TCHAR szAppDir[MAX_PATH] = { 0 };
    if (!GetModuleFileName(NULL, szAppDir, MAX_PATH))
        return;

    int len = wcslen(szAppDir) + 1;
    char* chAppDir = (char*)malloc(len);
    wcstombs(chAppDir, szAppDir, len);
    chAppDir[len - 1] = '\0';

    std::string exePath(chAppDir);
    free(chAppDir); // free memory

    std::string winCmd = exePath + " " + cmdLine;

    // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
    SECURITY_ATTRIBUTES sa = { 0 };
    sa.nLength = sizeof(sa);

    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFO si = { 0 };
    si.cb = sizeof(STARTUPINFO);

#define MAX_COMMAND 1024 // lenth of commandLine is always beyond MAX_PATH

    WCHAR command[MAX_COMMAND];
    memset(command, 0, sizeof(command));
    MultiByteToWideChar(CP_ACP, 0, winCmd.c_str(), -1, command, MAX_COMMAND);

    BOOL success = CreateProcess(NULL,
        command,   // command line
        NULL,      // process security attributes
        NULL,      // primary thread security attributes
        FALSE,     // handles are inherited
        0,         // creation flags
        NULL,      // use parent's environment
        NULL,      // use parent's current directory
        &si,       // STARTUPINFO pointer
        &pi);      // receives PROCESS_INFORMATION
    if (!success) {
        printf("relaunchSelf fail: %s", winCmd.c_str());
    }
    exit(0);
}

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    AppDelegate app;

    vector<string> args;
    for (int i = 0; i < __argc; ++i)
    {
        wstring ws(__wargv[i]);
        string s;
        s.assign(ws.begin(), ws.end());
        args.push_back(s);
    }
    CommandSetup* cmd = CommandSetup::getInstance();
    cmd->setRelauncher(relaunchSelf);
    cmd->setEngineRootPath(getEngineRoot());
    cmd->parseCommand(args);
    cmd->setupEngine();

    HWND hwnd = Director::getInstance()->getOpenGLView()->getWin32Window();
    oldWinProc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)winProc);
    std::string logPath = cmd->getLogPath();
    if (logPath.size() > 0) {
        fpLogFile = fopen(logPath.c_str(), "w");
        if (!fpLogFile) {
            printf("Open debug file fail:%s", logPath.c_str());
        }
    }

    // create the application instance
    int ret = Application::getInstance()->run();

    if (fpLogFile) {
        fclose(fpLogFile);
    }

#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif

    return ret;
}
