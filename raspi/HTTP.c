#include "HTTP.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static bool splitRequestStartLine(struct request_start * target, char * str);

int splitRequest(struct request * target, char * str){
    enum splitErrorCode errCode = success;

    char * temp = strtok(str, "\n");

    if (!splitRequestStartLine(&(target->requestStartLine), temp)){
        printf("fail splitRequestStartLine\n");
        errCode = fail_splitStartLine;

        goto RETURN_CODE;
    }

RETURN_CODE:
    return errCode;
}

static bool splitRequestStartLine(struct request_start * target, char * str){
    target->HTTP_method = strtok(str, " ");
    target->request_target = strtok(NULL, " ");
    target->HTTP_ver = strtok(NULL, " ");

    return target->HTTP_method && target->request_target && target->HTTP_ver && !strtok(NULL, " ");
}