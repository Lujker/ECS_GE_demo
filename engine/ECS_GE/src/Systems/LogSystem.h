#pragma once
#ifndef LOGER_H
#define LOGER_H

#include <memory>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>
#include <map>
#include "gl_Include.h"

/*!
    \brief ������������ ����� ��� ���������
*/
enum LOG_TYPE {
    DEB,
    WAR,
    CRIT,
    FAT
};

/*!
    \brief ����� ��� �����������
    \author Zelenskiy V.P.
    \version 1.1
    \date ���� 2021 ����
    \warning ������ ����� �� ���������, � ������������ ����� ��������

    ����� ��� ����������� � ���������, ���������� �� ������� singleton
    � �� ������ ����������� � ���������.
    ����� �������������� ���������� ������� ����� init
    ������� ���� � ��� ����� ����������.
*/
#define LOGER Loger::getInstanse()
#define LOG LOGER.writeMessage
class Loger
{
public:
    static Loger& getInstanse();
    virtual bool init(const std::string& exePath);
    virtual bool writeMessage(const char* mes, LOG_TYPE type = DEB);
    virtual bool writeMessage(const std::string& mes, LOG_TYPE type = DEB);
    virtual bool isInit();
    static void APIENTRY glDebugOutput(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);

    Loger(Loger&) = delete;
    Loger& operator= (const Loger&) = delete;
protected:
    virtual bool createLogFile(const std::string& logFilePath);
    virtual bool logFileIsValid(const std::string& logFilePath);

    virtual ~Loger();
    Loger();

private:
    std::string get_type(const LOG_TYPE& type);
    std::fstream m_out;
    bool m_isInit;
    std::mutex m_mutex;
};
#endif // LOGER_H