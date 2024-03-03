#include "HTTP.h"

#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static enum HTTPErrorCode lastErrCode;
enum HTTPErrorCode getLastErrCode(){
    return lastErrCode;
}

#pragma region request
static bool splitRequestStartLine(struct request_start * target, char * str);

enum HTTPErrorCode splitRequest(struct request * target, char * str)
{
    enum HTTPErrorCode errCode = DEFAULT;
    char * startLine = strtok(str, "\n");

    if (!splitRequestStartLine(&(target->startLine), startLine))
    {
        printf("fail splitRequestStartLine\n");
        errCode = fail_splitRequestStartLine;

        goto SplitRequestStartLine_RETURN;
    }

SplitRequestStartLine_RETURN:
    return lastErrCode = errCode;
}

static bool splitRequestStartLine(struct request_start * target, char * str)
{
    char * tempFileName;

    target->HTTP_method = strtok(str, " ");
    target->request_target = strtok(NULL, " ");
    target->HTTP_ver = strtok(NULL, "\r");


    return target->HTTP_method && target->request_target && target->HTTP_ver && !strtok(NULL, " ");
}
#pragma endregion



#pragma region response
static void initResponse(struct response * respData);
static void getResponseMessage(struct response respData, char * message);
static enum HTTPErrorCode getContentType(char * fileName, struct response * respData);

int processGetRequest(int client_socket, struct request reqData)
{
    struct response respData;
    char responseBuf[BUFSIZ];
    int readSize;
    enum HTTPErrorCode errCode = DEFAULT;
    char fileName[100];

    initResponse(&respData);    // respData 초기화
    // 파일 이름 설정
    sprintf(fileName, "%s%s", ABSOLUTE_PATH, (strtok(reqData.startLine.request_target, "/"))?reqData.startLine.request_target:"/index.html");

    respData.startLine.HTTP_ver = reqData.startLine.HTTP_ver;
    if (access(fileName, F_OK) == 0)
    {
        respData.startLine.stat_code = "200";
        respData.startLine.stat_text = "OK";
        
        errCode = getContentType(fileName, &respData);
    }
    else
    {
        respData.startLine.stat_code = "404";
        respData.startLine.stat_text = "Not Found";

        errCode = fail_notFindFile;
    }


    getResponseMessage(respData, responseBuf);
    write(client_socket, responseBuf, strlen(responseBuf));

    if (errCode != DEFAULT)
        goto ProcessGetRequest_RETURN;

    int fd = open(fileName, O_RDONLY);  // file open
    while((readSize = read(fd, responseBuf, BUFSIZ)) > 0)  // send file content                       
    {
        write(client_socket, responseBuf, readSize);
    }
    close(fd);

    write(client_socket, "\r\n", 2);    // response의 끝을 알리는 개행문자

ProcessGetRequest_RETURN:
    return lastErrCode = errCode;
}

static void initResponse(struct response * respData)
{
    respData->header.content_type = NULL;
}

static enum HTTPErrorCode getContentType(char * fileName, struct response * respData)
{
    enum HTTPErrorCode errCode = DEFAULT;
    char * requestTargetType[] = {
        ".html",
        ".css",
        ".js",
        ".ico",
        ".png",
    };
    char * contentType[] = {
        "text/html",                // HTML
        "text/css",                 // CSS
        "application/javascript",   // JS
        "image/x-icon",             // ICO
        "image/png",                // PNG
    };
    int len = sizeof(contentType) / sizeof(char *);

    for (int i = 0; i < len; i++)
    {
        if (strstr(fileName, requestTargetType[i]))
        {
            respData->header.content_type = contentType[i];
            goto GetContentType_RETURN;
        }
    }
    errCode = fail_noMatchContentType;

GetContentType_RETURN:
    return lastErrCode = errCode;
}

static void getResponseMessage(struct response respData, char * message)
{
    // status line
    sprintf(message, "%s %s %s\r\n", respData.startLine.HTTP_ver, respData.startLine.stat_code, respData.startLine.stat_text);
    // header
    if (respData.header.content_type != NULL)
        sprintf(message + strlen(message), "Content-Type: %s\r\n", respData.header.content_type);

    strcat(message, "\r\n");    // header 끝은 "\r\n\r\n"로 표기됨
}

#pragma endregion