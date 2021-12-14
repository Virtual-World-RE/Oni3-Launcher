#pragma once

#include "pch.h"

#ifdef _DEBUG
#define TraceFunc() Trace __log = Trace(__FUNCTION__)
#define TraceParam(ParamName, ParamValue) __log.TraceParameter(ParamName, ParamValue)
#define TraceFlg(Flag) __log.TraceFlag(Flag)
#define TraceMsg(Message) __log.TraceMessage(Message)
#define TraceW(message, windowName) __log.TraceWchar(TEXT(message), windowName)
#define TraceStyle(dwStyle) __log.TraceDwStyle(dwStyle)
#else
#define TraceFunc()
#define TraceParam(ParamName, ParamValue)
#define TraceFlg(Flag)
#define TraceMsg(Message)
#define TraceW(windowName)
#define TraceStyle(style)
#endif

typedef struct S_D3DFLAGS
{
    const CHAR name[200];
    DWORD flag;
} D3DFLAGS;

class Trace
{
private:
    std::string functionName;

public:
    Trace(const std::string &functionName);
    void TraceFlag(DWORD flag);
    void TraceDwStyle(DWORD dwStyle);

    template <typename T>
    void TraceParameter(const std::string &parameterName, const T &parameter)
    {
        std::cout << "\t" << parameterName << ": " << parameter << std::endl;
    }
    void TraceMessage(const std::string message);
    void TraceWchar(LPCTSTR message, LPCTSTR parameter);
    ~Trace();
};
