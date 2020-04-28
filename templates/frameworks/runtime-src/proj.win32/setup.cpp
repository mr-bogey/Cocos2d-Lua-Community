#include "setup.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;


// singleton
static CommandSetup* s_instance = nullptr;

CommandSetup::CommandSetup()
    : _logToFile(false)
    , _scale(100)
    , _width(450)
    , _height(800)
    , _relauncher(nullptr)
{
}

CommandSetup::~CommandSetup()
{
}

CommandSetup* CommandSetup::getInstance()
{
    if (!s_instance) {
        s_instance = new (std::nothrow) CommandSetup();
        s_instance->init();
    }
    return s_instance;
}

void CommandSetup::init(void)
{
    // setup hotkeys
    auto keyEvent = cocos2d::EventListenerKeyboard::create();
    keyEvent->onKeyReleased = [&](EventKeyboard::KeyCode key, Event*) {
        if (key == EventKeyboard::KeyCode::KEY_F5) {
            std::string cmdLine = makeCommand();
            _relauncher(cmdLine);
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyEvent, 1);
}

void CommandSetup::setRelauncher(RELAUNCHER cb)
{
    _relauncher = cb;
}

void CommandSetup::setEngineRootPath(std::string root)
{
    _engineRootPath = root;
}

void CommandSetup::parseCommand(const vector<string> &args)
{
    if (args.size() >= 2)
    {
        auto it = args.begin();
        while (it != args.end())
        {
            string arg = *it;
            if (arg.compare("-workdir") == 0)
            {
                ++it;
                if (it == args.end()) break;
                _workdirPath = *it;
            }
            else if (arg.compare("-writable") == 0)
            {
                ++it;
                if (it == args.end()) break;
                _writePath = *it;
            }
            else if (arg.compare("-size") == 0)
            {
                ++it;
                if (it == args.end()) break;
                const string sizeStr = *it;
                size_t pos = sizeStr.find("x");
                if (pos != sizeStr.npos && pos > 0)
                {
                    string widthStr, heightStr;
                    widthStr.assign(sizeStr, 0, pos);
                    heightStr.assign(sizeStr, pos + 1, sizeStr.length() - pos);
                    _width = atoi(widthStr.c_str());
                    _height = atoi(heightStr.c_str());
                }
            }
            else if (arg.compare("-scale") == 0)
            {
                ++it;
                if (it == args.end()) break;
                _scale = atoi((*it).c_str());
            }
            else if (arg.compare("-log") == 0)
            {
                _logToFile = true;
            }
            ++it;
        }

    }
    if (_writePath.length() == 0)
    {
        if (_workdirPath.length() > 0)
        {
            _writePath = _workdirPath;
        }
        else
        {
            _writePath = _engineRootPath;
        }
    }
}

std::string CommandSetup::makeCommand(void)
{
    stringstream stream;
    if (_workdirPath.length() > 0)
    {
        stream << "-workdir ";
        stream << _workdirPath;
    }

    stream << " -size ";
    stream << _width;
    stream << "x";
    stream << _height;

    stream << " -scale ";
    stream << _scale;

    if (_writePath.length() > 0)
    {
        stream << " -writable ";
        stream << _writePath;
    }

    if (_logToFile) {
        stream << " -log";
    }

    return stream.str();
}

void CommandSetup::setupEngine(void)
{
    printf("=== begin setup configs ===\n");
    printf("logToFile:%s\n", _logToFile ? "true" : "false");
    printf("scale:%f\n", _scale / 100.0);
    printf("size:%dx%d\n", _width, _height);
    printf("writePath:%s\n", _writePath.c_str());
    printf("workDirPath:%s\nisDirectoryExist: %s\n", _workdirPath.c_str(), (_workdirPath.size() > 0 && FileUtils::getInstance()->isDirectoryExist(_workdirPath)) ? "TRUE" : "FALSE");
    printf("engineRootPath:%s\nisDirectoryExist: %s\n", _engineRootPath.c_str(), FileUtils::getInstance()->isDirectoryExist(_engineRootPath) ? "TRUE" : "FALSE");
    printf("=== end setup configs ===\n");

    if (_workdirPath.size() > 0) {
        // load game
        std::vector<std::string> paths;
        paths.push_back(_workdirPath);
        FileUtils::getInstance()->setDefaultResourceRootPath(_workdirPath);
        FileUtils::getInstance()->setSearchPaths(paths);
    }
    else {
        // load engine default path.
        std::vector<std::string> paths;
        paths.push_back(_engineRootPath);
        FileUtils::getInstance()->setDefaultResourceRootPath(_engineRootPath);
        FileUtils::getInstance()->setSearchPaths(paths);
    }

    FileUtils::getInstance()->setWritablePath(_writePath);
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = cocos2d::GLViewImpl::createWithRect("__PROJECT_COCOS_NAME__", Rect(0, 0, _width, _height), _scale / 100.0f);
        director->setOpenGLView(glview);
        director->startAnimation();
    }
}

std::string CommandSetup::getLogPath()
{
    if (!_logToFile) {
        return "";
    }

    std::string logFileName = "debug.log";
    char lastChar = _writePath[_writePath.length() - 1];
    if (lastChar == '/' || lastChar == '\\') {
        return _writePath + logFileName;
    }

    return _writePath + "/" + logFileName;
}
