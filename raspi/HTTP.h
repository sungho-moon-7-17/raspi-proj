#ifndef HTTP_H
#define HTTP_H

enum splitErrorCode{
success = 0,
fail_splitStartLine,
};

struct request_start{
char * HTTP_method;
char * request_target;
char * HTTP_ver;
};

struct request{
struct request_start requestStartLine;
char *  requestHeader;
char * requestBody;

} ;

int splitRequest(struct request * target, char * str);

#endif