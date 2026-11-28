// Code written by Fom477
// Copyright Fom477 2026

#include <logging.h>
#include <stddef.h>
#include <stdio.h>

#define GENERAL "GENERAL"
#define GUNMU   "\033[0m"

void Log(const LoggerCollection *Loggers, const int LoggerId, const char *Message)
{
    if (Loggers->Size <= LoggerId)
        return; // Error

    const Logger *Logger = Loggers->Logger[LoggerId];

    int BgColorHex = Logger->BackgroundColor;
    int FtColorHex = Logger->FrontColor;

    // Process (color):
    // int color --- &color ---> int *color --- (LoggerColor *) color ---> LoggerColor *color --- *color ---> LoggerColor color
    // Directly read the int hex as the LoggerColor, which will directly fill each channel with the correct binary data at the address
    const LoggerColor BgColor = *(LoggerColor *) &BgColorHex;
    const LoggerColor FtColor = *(LoggerColor *) &FtColorHex;

    char Bg[64]; // Background
    char Ft[64]; // Front

    snprintf(Bg, sizeof(Bg), "\033[48;2;%c;%c;%cm", BgColor.ChannelR, BgColor.ChannelG, BgColor.ChannelB);
    snprintf(Ft, sizeof(Ft), "\033[38;2;%c;%c;%cm", FtColor.ChannelR, FtColor.ChannelG, FtColor.ChannelB);

    printf("%s%s[%s] [%s / %s] %s%s\n", Bg, Ft, Logger->HasPrefix? Logger->CurrentPrefix: GENERAL, Loggers->WorkerGroup, Logger->LevelName, Message, GUNMU);
}

void RemovePrefix(const LoggerCollection *Loggers)
{
    for (int I = 0; I < Loggers->Size; I++)
    {
        Loggers->Logger[I]->CurrentPrefix = NULL;
        Loggers->Logger[I]->HasPrefix     = 0;
    }
}

void SwitchPrefix(const LoggerCollection *Loggers, const char *Prefix)
{
    for (int I = 0; I < Loggers->Size; I++)
    {
        Loggers->Logger[I]->CurrentPrefix = (char *) Prefix;
        Loggers->Logger[I]->HasPrefix     = 1;
    }
}