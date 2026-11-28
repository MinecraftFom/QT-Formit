// Code written by Fom477
// Copyright Fom477 2026

#ifndef FORMIT_LOGGING
#define FORMIT_LOGGING 1

#include <formit.h>

typedef struct Logger           Logger;
typedef struct LoggerCollection LoggerCollection;
typedef struct __RGB            LoggerColor;

struct Logger
{
    const char *LevelName;

    char       *CurrentPrefix;
    int         HasPrefix;

    int         BackgroundColor, FrontColor;
};

struct LoggerCollection
{
    const char *WorkerGroup;

    int     Size;
    Logger *Logger[];
};

// Memory layout based
#if IS_LITTLE_ENDIAN
volatile struct __RGB
{
    unsigned char ChannelB, ChannelG, ChannelR, ChannelA;
};
#elif IS_BIG_ENDIAN
volatile struct __RGB
{
    unsigned char ChannelA, ChannelR, ChannelG, ChannelB;
};
#else
#error What in hell is going on??!!
#endif

void Log         (const LoggerCollection *Loggers, int LoggerId, const char *Message);
void RemovePrefix(const LoggerCollection *Loggers);
void SwitchPrefix(const LoggerCollection *Loggers, const char *Prefix);

#endif