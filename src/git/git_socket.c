// Code written by Fom477
// Copyright Fom477 2026

#include <arpa/inet.h>
#include <curl/curl.h>
#include <json_object.h>
#include <json_tokener.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <build_check.h>
#include <git.h>
#include <git/auth.h>
#include <git/sock.h>
#include <socket/socket.h>

#if !BUILD_CHECK(GIT_SOCK)
#   error PreprocessError: Modules not found
#endif

#ifndef SECRET_QUOTE
#define SECRET_QUOTE \
    "HTTP/1.1 200 OK\r\n" \
    "Content-Type: text/html\r\n\r\n" \
    "<html>\n" \
    "<head>\n" \
    "    <meta http-equiv=\"refresh\" content=\"3;url=https://fom477.net/success.html\">\n" \
    "</head>\n" \
    "<body>\n" \
    "    <h3>Magic Foxoltl Fom Has Helped You Login To GitHUB!!!</h3>\n" \
    "    <p>Redirecting you in 3 seconds...</p>\n" \
    "    <br><a href='https://fom477.net'>Click _HERE_ if you are not redirected automatically!</a>\n" \
    "</body>\n" \
    "</html>" // Good to know that fom477 actually knows nothing about javascript, so I only used pure html
#define SECRET_QUOTE_LENGTH strlen(SECRET_QUOTE)
#endif

GitUserAuth *ExchangeToken(const int Port, char *AuthCode, const char *ClientID, const char *ClientSecret)
{
    printf("Exchanging token from auth code: %s\n", AuthCode);
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

    char Post[256];
    snprintf(Post, sizeof(Post), "client_id=%s&client_secret=%s&code=%s&redirect_uri=http://127.0.0.1:%d", ClientID, ClientSecret, AuthCode, Port);
    struct curl_slist *Headers = NULL;
    Headers = curl_slist_append(Headers, "Accept: application/json");
    Headers = curl_slist_append(Headers, "Content-Type: application/x-www-form-urlencoded");
    Headers = curl_slist_append(Headers, GIT_AGENT);

    curl_easy_setopt(Curl, CURLOPT_URL,           "https://github.com/login/oauth/access_token");
    curl_easy_setopt(Curl, CURLOPT_POSTFIELDS,    Post);
    curl_easy_setopt(Curl, CURLOPT_HTTPHEADER,    Headers);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA,     (void *)&Mem);

    const CURLcode Res = curl_easy_perform(Curl);
    curl_slist_free_all(Headers);
    curl_easy_cleanup(Curl);

    if (Res != CURLE_OK)
    {
        free(Mem.Memory);
        printf("Invalid curl status: %d (not %d)\n", Res, CURLE_OK);
        Error = "CurlError: Failed to perform curl";
        goto OnError;
    }

    printf("Successfully got response: %s\n", Mem.Memory);
    json_object *Response = json_tokener_parse(Mem.Memory);
    free(Mem.Memory); // Memory leak is annoying

    if (!Response)
    {
        Error = "JsonError: Failed to parse json"; 
        goto OnError;
    }

    json_object *TokenJson;
    char *AccessTokenResponse = NULL;
    if (json_object_object_get_ex(Response, "access_token", &TokenJson))
        AccessTokenResponse = strdup(json_object_get_string(TokenJson));

    json_object_put(Response);

    const GitUser *UserInfo = GetUserProperty(AccessTokenResponse);
    printf("Got access token: %s\n", AccessTokenResponse);

    const GitUser UserInfoCopied = {.UserDisplayName = UserInfo->UserDisplayName, .UserMail = UserInfo->UserMail, .UserName = UserInfo->UserName};

    GitUserAuth *UserAuthCopied = malloc(sizeof(GitUserAuth));
    UserAuthCopied->User        = UserInfoCopied;
    UserAuthCopied->UserToken   = AccessTokenResponse;

    return UserAuthCopied;
OnError:
    if (!Error)
        Error = "NULL Specified";

    fprintf(stderr, "%s\n", Error);
    return NULL;
}

GitUser *GetUserProperty(const char *AccessToken)
{
    struct curl_slist *Headers  = NULL;
    struct curl_slist *Headers2 = NULL;
    char Header[256];
    snprintf(Header, sizeof(Header), "Authorization: Bearer %s", AccessToken);

    Headers  = curl_slist_append(Headers, Header);
    Headers  = curl_slist_append(Headers, "Accept: application/vnd.github+json");
    Headers  = curl_slist_append(Headers, "Content-Type: application/json");
    Headers  = curl_slist_append(Headers, GIT_AGENT);

    Headers2 = curl_slist_append(Headers2, Header);
    Headers2 = curl_slist_append(Headers2, "Accept: application/vnd.github+json");
    Headers2 = curl_slist_append(Headers2, "Content-Type: application/json");
    Headers2 = curl_slist_append(Headers2, GIT_AGENT);

    char *Raw  = PerformHttp("https://api.github.com/user", Headers);
    char *Raw2 = PerformHttp("https://api.github.com/user/emails", Headers2);

    if (!Raw || !Raw2)
    {
        fprintf(stderr, "CurlError: Unable to perform curl\n");
        if (Raw) free(Raw);
        if (Raw2) free(Raw2);

        return NULL;
    }

    char *UserProperty  = strdup(Raw);
    char *EmailProperty = strdup(Raw2);
    free(Raw);
    free(Raw2);

    printf("Successfully got response: \n1 -> %s\n2 -> %s\n", UserProperty, EmailProperty);

    json_object *UserJsonObject = json_tokener_parse(UserProperty);
    json_object *EmailJsonObject = json_tokener_parse(EmailProperty);
    free(UserProperty);
    free(EmailProperty);

    if (!UserJsonObject || !EmailJsonObject)
    {
        fprintf(stderr, "JsonError: Failed to parse response\n");
        return NULL;
    }

    json_object *UserDisplayNameObject;
    json_object *UserMailObject;
    json_object *UserNameObject;
    json_object_object_get_ex(UserJsonObject, "name", &UserDisplayNameObject);
    json_object_object_get_ex(json_object_array_get_idx(EmailJsonObject, 0), "email", &UserMailObject);
    json_object_object_get_ex(UserJsonObject, "login", &UserNameObject);

    const char *UserDisplayName = json_object_get_string(UserDisplayNameObject);
    const char *UserMail = json_object_get_string(UserMailObject);
    const char *UserName = json_object_get_string(UserNameObject);

    const GitUser User = CreateGitUser(!UserDisplayName? "(empty)": UserDisplayName, !UserMail? "null@fom477.net": UserMail, !UserName? "null": UserName); // *Actually fom477.net hasn't been assigned with a email server

    GitUser *UserCopied = malloc(sizeof(GitUser));
    UserCopied->UserDisplayName = User.UserDisplayName;
    UserCopied->UserMail        = User.UserMail;
    UserCopied->UserName        = User.UserName;
    printf("Got result: \n\tUser Display Name -> %s\n\tUser Mail -> %s\n\tUser Name -> %s\n", UserCopied->UserDisplayName, UserCopied->UserMail, UserCopied->UserName);
    return UserCopied;
}

char *StartLoopback(const int Port, const char *ClientID) // Starts a server of ip 127.0.0.1:${Port}
{
    int ClientFD, ServerFD;

    struct sockaddr_in SocketAddr;
    socklen_t          SocketLength = sizeof(SocketAddr);

    if ((ServerFD = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        goto SocketIOError;
    }

    const int Option = 1;
    setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR, &Option, sizeof(Option));

    SocketAddr.sin_family = AF_INET;
    SocketAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    SocketAddr.sin_port = htons(Port);

    if (bind(ServerFD, (struct sockaddr *)&SocketAddr, SocketLength) < 0)
    {
        close(ServerFD);
        goto SocketIOError;
    }

    if (listen(ServerFD, 1) < 0)
    {
        close(ServerFD);
        goto SocketIOError;
    }

    char AuthUrl[512];
    snprintf(AuthUrl, sizeof(AuthUrl), "https://github.com/login/oauth/authorize?client_id=%s&redirect_uri=http://127.0.0.1:%d"
                                       "&scope="
                                       "delete_repo%%20"
                                       "repo%%20"
                                       "read:user%%20"
                                       "user", ClientID, Port);
    StartBrowser(AuthUrl);

    if ((ClientFD = accept(ServerFD, (struct sockaddr *)&SocketAddr, &SocketLength)) < 0)
    {
        close(ServerFD);
        goto SocketIOError;
    }

    char Buffer[8192] = {0}; // Alloc on stack
    read(ClientFD, Buffer, 8191); // Buffer Size (8192) - 1
    write(ClientFD, SECRET_QUOTE, SECRET_QUOTE_LENGTH);
    close(ClientFD);
    close(ServerFD);
    char *Code = strstr(Buffer, "code=");
    if (!Code)
        goto ResponseFormatError;

    Code += 5;
    char AuthCode[128] = {0};
    int  I = 0;
    while (Code[I] /*Code[I] is a valid byte*/ && Code[I] != ' ' && Code[I] != '&' && Code[I] != '\0' && I < sizeof(AuthCode) - 1)
    {
        AuthCode[I] = Code[I];
        I++;
    }

    printf("Successfully got auth code: %s\n", AuthCode);

    return strdup(AuthCode);
ResponseFormatError:
    perror("ResponseFormatError");
    return NULL;
SocketIOError:
    perror("SocketIOError");
    return NULL;
}