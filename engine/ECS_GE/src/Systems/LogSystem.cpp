#include "LogSystem.h"

#include <dinput.h>


bool Loger::init(const std::string& execPath)
{
    try {
        if (execPath == "" || execPath.empty()) {
            std::cout << "Exec file path is empty\n";
            return false;
        }
        if (m_out.is_open()) m_out.close();
        const size_t found = execPath.find_last_of("/\\");
        auto logFilePath = execPath.substr(0, found);
        logFilePath += "/\\logFile.log";
        

        if (logFileIsValid(logFilePath)) {
        }
        else {
            if (createLogFile(logFilePath)) {
            }
            else {
                std::cout << "Fail to create log file!\n";
                return m_isInit = false;
            }
        }
        m_out.open(logFilePath, std::ios::app);
        return m_isInit = true;
    }
    catch (const std::exception& exp) {
        std::cout << exp.what();
        return m_isInit = false;
    }
}

Loger& Loger::getInstanse()
{
    static Loger log;
    return log;
}

bool Loger::writeMessage(const char* mes, LOG_TYPE type)
{
    return writeMessage(std::string(mes), type);
}

bool Loger::writeMessage(const std::string& mes, LOG_TYPE type)
{
    try {
        std::string log_msg = "";
        time_t now = time(0);
        std::string dt = "[" + std::string(ctime(&now)); ///текуща€ дата и врем€
        dt.at(dt.size() - 1) = ']';
        log_msg = dt + get_type(type) + ("\t") + mes + "\n";

        ///«аполенени€ данными
        std::lock_guard<std::mutex>  guard(m_mutex);
        OutputDebugStringA(log_msg.c_str());
        if(isInit())
			m_out << log_msg << std::endl;
        return true;
    }
    catch (const std::exception& exp) {
        std::cout << exp.what();
        return false;
    }
}

bool Loger::isInit()
{
    return m_isInit;
}

bool Loger::createLogFile(const std::string& logFilePath)
{
    std::ofstream file(logFilePath, std::ios_base::out | std::ios_base::trunc);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}

bool Loger::logFileIsValid(const std::string& logFilePath)
{
    std::ifstream file;
    file.open(logFilePath);
    if (!file) {
        return false;
    }
	file.close();
	return true;
}

std::string Loger::get_type(const LOG_TYPE& type)
{
    std::string ret_type;
    switch (type) {
    case (LOG_TYPE::DEB):
        ret_type = "<DEBUG> ";
        break;
    case (LOG_TYPE::WAR):
        ret_type = "<WARNING> ";
        break;
    case (LOG_TYPE::CRIT):
        ret_type = "<!CRITICAL!> ";
        break;
    case (LOG_TYPE::FAT):
        ret_type = "<!!!FATAL!!!> ";
        break;
    default:
        ret_type = "<DEBUG> ";
        break;
    }
    return ret_type;
}

Loger::~Loger()
{
    if (m_out.is_open()) m_out.close();
}

Loger::Loger()
{}

void APIENTRY Loger::glDebugOutput(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    static std::map<GLenum, const GLchar*> Sources =
    {
        {GL_DEBUG_SOURCE_API, "API"},
        {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM"},
        {GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
        {GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY"},
        {GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
        {GL_DEBUG_SOURCE_OTHER, "OTHER"}
    };

    static std::map<GLenum, const GLchar*> Severities =
    {
        {GL_DEBUG_SEVERITY_HIGH, "HIGH"},
        {GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
        {GL_DEBUG_SEVERITY_LOW, "LOW"},
        {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}
    };

    static std::map<GLenum, const GLchar*> Types =
    {
        {GL_DEBUG_TYPE_ERROR, "ERROR"},
        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
        {GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
        {GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
        {GL_DEBUG_TYPE_MARKER, "MARKER"},
        {GL_DEBUG_TYPE_PUSH_GROUP, "PUSH_GROUP"},
        {GL_DEBUG_TYPE_POP_GROUP, "POP_GROUP"},
        {GL_DEBUG_TYPE_OTHER, "OTHER"}
    };
    char ErrorString[512];
    sprintf_s(ErrorString, "[OpenGL %s] - SEVERITY: %s, SOURCE: %s, ID: %d: %s\n", Types[type], Severities[severity], Sources[source], id, message);
    LOGER.writeMessage(ErrorString);
}