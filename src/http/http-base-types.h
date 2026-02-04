#ifndef _BALU_HTTP_BASE_TYPES_H
#define _BALU_HTTP_BASE_TYPES_H

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
} HTTP_REQUEST_TYPE;

typedef struct {
    HTTP_REQUEST_TYPE type;
    const char*       URI;
    const char*       protocol;
    const char*       table;
} HTTPRequestHeader;

typedef struct {
    HTTPRequestHeader header;
    const char*       body;
} HTTPRequest;

#endif
