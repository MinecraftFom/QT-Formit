// Code written by Fom477
// Copyright Fom477 2026

#ifndef FORMIT_SOCKET
#define FORMIT_SOCKET 1

typedef struct CurlMemoryStruct CurlMemoryStruct;

struct CurlMemoryStruct
{
    char  *Memory;
    size_t Size;
};

size_t WriteCallback(const void *Content, size_t Size, size_t N, void *Mem);

#ifdef __cplusplus
extern "C" char *PerformCurl (const char *Url, struct curl_slist *Header, char *Body, const char *Method);
extern "C" char *PerformHttp (const char *Url, struct curl_slist *Header);
extern "C" void  StartBrowser(const char *Url);
#else
char *PerformCurl (const char *Url, struct curl_slist *Header, char *Body, const char *Method);
char *PerformHttp (const char *Url, struct curl_slist *Header);
void  StartBrowser(const char *Url);
#endif
#endif