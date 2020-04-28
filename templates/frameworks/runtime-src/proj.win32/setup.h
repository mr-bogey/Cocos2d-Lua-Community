#pragma once
#include <string>
#include <vector>
using namespace std;

typedef void (*RELAUNCHER)(string& cmdLine);

class CommandSetup
{
public:
    CommandSetup();
    virtual ~CommandSetup();

    static CommandSetup* getInstance();

    void setRelauncher(RELAUNCHER cb);
    void setEngineRootPath(string root);

    void parseCommand(const vector<string> &args);
    std::string makeCommand(void);

    void setupEngine(void);
    std::string getLogPath();

private:
    void init(void);

    bool _logToFile;
    int _scale;
    int _width;
    int _height;
    string _engineRootPath;
    string _writePath;
    string _workdirPath;
    RELAUNCHER _relauncher;
};