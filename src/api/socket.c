// Code written by Fom477
// Copyright Fom477 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include <git/sock.h>
#include <socket/socket.h>

char *PerformHttp(const char *Url, struct curl_slist *Header)
{
    char *Error;
    CURL *Curl = curl_easy_init();
    if (!Curl)
    {
        Error = "CurlError: Unable to init curl";
        goto OnError;
    }

    CurlMemoryStruct Mem;
    Mem.Memory = malloc(1);
    Mem.Size   = 0;

    if (!Mem.Memory)
    {
        Error = "OOM: No enough memory to alloc";
        goto OnError;
    }

    curl_easy_setopt(Curl, CURLOPT_URL,            Url);

    curl_easy_setopt(Curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(Curl, CURLOPT_HTTPGET,        1L);
    curl_easy_setopt(Curl, CURLOPT_HTTPHEADER,     Header);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION,  WriteCallback);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA,      (void *)&Mem);

    const CURLcode Res = curl_easy_perform(Curl);
    curl_slist_free_all(Header);
    curl_easy_cleanup(Curl);

    if (Res != CURLE_OK)
    {
        fprintf(stderr, "Curl failed with code %d\n", Res);
        Error = "CurlError: Unable to perform curl";
        free(Mem.Memory);
        goto OnError;
    }

    printf("Got response data: %s\n", Mem.Memory);
    return Mem.Memory;
OnError:
    if (!Error)
        Error = "NULL Specified";

    fprintf(stderr, "%s\n", Error);
    return NULL;
}

char *PerformCurl(const char *Url, struct curl_slist *Header, char *Body, const char *Method)
{
    char *Error;
    CURL *Curl = curl_easy_init();
    if (!Curl)
    {
        Error = "CurlError: Unable to init curl";
        goto OnError;
    }

    CurlMemoryStruct Mem;
    Mem.Memory = malloc(1);
    Mem.Size   = 0;

    if (!Mem.Memory)
    {
        Error = "OOM: No enough memory to alloc";
        goto OnError;
    }

    curl_easy_setopt(Curl, CURLOPT_URL,            Url);

    curl_easy_setopt(Curl, CURLOPT_CUSTOMREQUEST,  Method);
    curl_easy_setopt(Curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(Curl, CURLOPT_HTTPHEADER,     Header);
    curl_easy_setopt(Curl, CURLOPT_POSTFIELDS,     Body);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION,  WriteCallback);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA,      (void *)&Mem);

    const CURLcode Res = curl_easy_perform(Curl);
    curl_slist_free_all(Header);
    curl_easy_cleanup(Curl);

    if (Res != CURLE_OK)
    {
        fprintf(stderr, "Curl failed with code %d\n", Res);
        Error = "CurlError: Unable to perform curl";
        free(Mem.Memory);
        goto OnError;
    }

    printf("Got response data: %s\n", Mem.Memory);
    return Mem.Memory;
OnError:
    if (!Error)
        Error = "NULL Specified";

    fprintf(stderr, "%s\n", Error);
    return NULL;
}

size_t WriteCallback(const void *Content, const size_t Size, const size_t N, void *Mem)
{
    const size_t      ActualSize = Size * N;
    CurlMemoryStruct *Memory     = Mem; // Cast to something we can process

    char *CPtr = realloc(Memory->Memory, Memory->Size + ActualSize + 1);
    if (!CPtr)
        return -1; // ... *OOM

    Memory->Memory = CPtr;
    memcpy(&Memory->Memory[Memory->Size], Content, ActualSize);
    Memory->Size += ActualSize;
    Memory->Memory[Memory->Size] = '\0';
    return ActualSize;
}

#if defined(__unix__)
#define WEBLINK_APPLICATION "xdg-open"
#elif defined(_WIN32)
// TODO: complete
#define WEBLINK_APPLICATION
#elif defined(__APPLE__)
// TODO: complete
#define WEBLINK_APPLICATION
#else
#error Unsupported platform
#endif

#ifndef WEBLINK_APPLICATION
#error Incomplete code. Please re-download the source code
#endif

#if defined(__unix__) || defined(__APPLE__)
inline void StartBrowser(const char *Url)
{
    printf("Starting browser of link %s\n", Url);
    const pid_t Pid = fork();
    if (Pid < 0)
    {
        perror("ForkError");
        return;
    }

    if (Pid != 0)
        return;

    printf("Successfully entered child thread of browser, opening %s\n", Url);
    char *Args[] = {WEBLINK_APPLICATION, (char *) Url, NULL};
    execvp(WEBLINK_APPLICATION, Args);
    exit(1);
}
#elif defined(_WIN32)
#include <windows.h>
inline void StartBrowser(const char *Url)
{
    printf("Sorry, but since I'm not advanced in Win32 programming, I don't know how to start a browser. Please open the following link in the browser by your own: %s", Url);
}
#endif