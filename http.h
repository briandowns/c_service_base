#ifdef __cplusplus
extern "C" {
#endif

#ifndef __HTTP_H
#define __HTTP_H

#include <arpa/inet.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <ulfius.h>

#include "logger.h"

#define HTTP_METHOD_GET     "GET"
#define HTTP_METHOD_POST    "POST"
#define HTTP_METHOD_PUT     "PUT"
#define HTTP_METHOD_DELETE  "DELETE"
#define HTTP_METHOD_HEAD    "HEAD"
#define HTTP_METHOD_CONNECT "CONNECT"
#define HTTP_METHOD_OPTIONS "OPTIONS"
#define HTTP_METHOD_TRACE   "TRACE"
#define HTTP_METHOD_PATCH   "PATCH"

#define HTTP_STATUS_CODE_CONTINUE            100 // RFC 7231, 6.2.1
#define HTTP_STATUS_CODE_SWITCHING_PROTOCOLS 101 // RFC 7231, 6.2.2
#define HTTP_STATUS_CODE_PROCESSING          102 // RFC 2518, 10.1
#define HTTP_STATUS_CODE_EARLY_HINTS         103 // RFC 8297

#define HTTP_STATUS_CODE_OK                    200 // RFC 7231, 6.3.1
#define HTTP_STATUS_CODE_CREATED               201 // RFC 7231, 6.3.2
#define HTTP_STATUS_CODE_ACCEPTED              202 // RFC 7231, 6.3.3
#define HTTP_STATUS_CODE_NONAUTHORITATIVE_INFO 203 // RFC 7231, 6.3.4
#define HTTP_STATUS_CODE_NO_CONTENT            204 // RFC 7231, 6.3.5
#define HTTP_STATUS_CODE_RESET_CONTENT         205 // RFC 7231, 6.3.6
#define HTTP_STATUS_CODE_PARTIAL_CONTENT       206 // RFC 7233, 4.1
#define HTTP_STATUS_CODE_MULTI_STATUS          207 // RFC 4918, 11.1
#define HTTP_STATUS_CODE_ALREADY_REPORTED      208 // RFC 5842, 7.1
#define HTTP_STATUS_CODE_IM_USED               226 // RFC 3229, 10.4.1

#define HTTP_STATUS_CODE_MULTIPLE_CHOICES   300 // RFC 7231, 6.4.1
#define HTTP_STATUS_CODE_MOVED_PERMANENTLY  301 // RFC 7231, 6.4.2
#define HTTP_STATUS_CODE_FOUND              302 // RFC 7231, 6.4.3
#define HTTP_STATUS_CODE_SEE_OTHER          303 // RFC 7231, 6.4.4
#define HTTP_STATUS_CODE_NOT_MODIFIED       304 // RFC 7232, 4.1
#define HTTP_STATUS_CODE_USE_PROXY          305 // RFC 7231, 6.4.5
#define HTTP_STATUS_CODE_TEMPORARY_REDIRECT 307 // RFC 7231, 6.4.7
#define HTTP_STATUS_CODE_PERMANENT_REDIRECT 308 // RFC 7538, 3

#define HTTP_STATUS_CODE_BAD_REQUEST                     400 // RFC 7231, 6.5.1
#define HTTP_STATUS_CODE_UNAUTHORIZED                    401 // RFC 7235, 3.1
#define HTTP_STATUS_CODE_PAYMENT_REQUIRED                402 // RFC 7231, 6.5.2
#define HTTP_STATUS_CODE_FORBIDDEN                       403 // RFC 7231, 6.5.3
#define HTTP_STATUS_CODE_NOT_FOUND                       404 // RFC 7231, 6.5.4
#define HTTP_STATUS_CODE_METHOD_NOT_ALLOWED              405 // RFC 7231, 6.5.5
#define HTTP_STATUS_CODE_NOT_ACCEPTABLE                  406 // RFC 7231, 6.5.6
#define HTTP_STATUS_CODE_PROXY_AUTH_REQUIRED             407 // RFC 7235, 3.2
#define HTTP_STATUS_CODE_REQUEST_TIMEOUT                 408 // RFC 7231, 6.5.7
#define HTTP_STATUS_CODE_CONFLICT                        409 // RFC 7231, 6.5.8
#define HTTP_STATUS_CODE_GONE                            410 // RFC 7231, 6.5.9
#define HTTP_STATUS_CODE_LENGTH_REQUIRED                 411 // RFC 7231, 6.5.10
#define HTTP_STATUS_CODE_PRECONDITION_FAILED             412 // RFC 7232, 4.2
#define HTTP_STATUS_CODE_REQUEST_ENTITY_TOO_LARGE        413 // RFC 7231, 6.5.11
#define HTTP_STATUS_CODE_REQUEST_URI_TOO_LONG            414 // RFC 7231, 6.5.12
#define HTTP_STATUS_CODE_UNSUPPORTED_MEDIA_TYPE          415 // RFC 7231, 6.5.13
#define HTTP_STATUS_CODE_REQUESTED_RANGE_NOT_SATISFIABLE 416 // RFC 7233, 4.4
#define HTTP_STATUS_CODE_EXPECTATION_FAILED              417 // RFC 7231, 6.5.14
#define HTTP_STATUS_CODE_TEAPOT                          418 // RFC 7168, 2.3.3
#define HTTP_STATUS_CODE_MISDIRECTED_REQUEST             421 // RFC 7540, 9.1.2
#define HTTP_STATUS_CODE_UNPROCESSABLE_ENTITY            422 // RFC 4918, 11.2
#define HTTP_STATUS_CODE_LOCKED                          423 // RFC 4918, 11.3
#define HTTP_STATUS_CODE_FAILED_DEPENDENCY               424 // RFC 4918, 11.4
#define HTTP_STATUS_CODE_TOO_EARLY                       425 // RFC 8470, 5.2.
#define HTTP_STATUS_CODE_UPGRADE_REQUIRED                426 // RFC 7231, 6.5.15
#define HTTP_STATUS_CODE_PRECONDITION_REQUIRED           428 // RFC 6585, 3
#define HTTP_STATUS_CODE_TOO_MANY_REQUESTS               429 // RFC 6585, 4
#define HTTP_STATUS_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE 431 // RFC 6585, 5
#define HTTP_STATUS_CODE_UNAVAILABLE_FOR_LEGAL_REASONS   451 // RFC 7725, 3

#define HTTP_STATUS_CODE_INTERNAL_SERVER_ERROR           500 // RFC 7231, 6.6.1
#define HTTP_STATUS_CODE_NOT_IMPLEMENTED                 501 // RFC 7231, 6.6.2
#define HTTP_STATUS_CODE_BAD_GATEWAY                     502 // RFC 7231, 6.6.3
#define HTTP_STATUS_CODE_SERVICE_UNAVAILABLE             503 // RFC 7231, 6.6.4
#define HTTP_STATUS_CODE_GATEWAY_TIMEOUT                 504 // RFC 7231, 6.6.5
#define HTTP_STATUS_CODE_HTTP_VERSION_NOT_SUPPORTED      505 // RFC 7231, 6.6.6
#define HTTP_STATUS_CODE_VARIANT_ALSO_NEGOTIATES         506 // RFC 2295, 8.1
#define HTTP_STATUS_CODE_INSUFFICIENT_STORAGE            507 // RFC 4918, 11.5
#define HTTP_STATUS_CODE_LOOP_DETECTED                   508 // RFC 5842, 7.2
#define HTTP_STATUS_CODE_NOT_EXTENDED                    510 // RFC 2774, 7
#define HTTP_STATUS_CODE_NETWORK_AUTHENTICATION_REQUIRED 511 // RFC 6585, 6

#define HTTP_STATUS_MESSAGE_CONTINUE                        "Continue"
#define HTTP_STATUS_MESSAGE_SWITCHING_PROTOCOLS             "Switching Protocols"
#define HTTP_STATUS_MESSAGE_PROCESS                         "Processing"
#define HTTP_STATUS_MESSAGE_EARLY_HINTS                     "Early Hints"
#define HTTP_STATUS_MESSAGE_OK                              "OK"
#define HTTP_STATUS_MESSAGE_CREATED                         "Created"
#define HTTP_STATUS_MESSAGE_ACCEPTED                        "Accepted"
#define HTTP_STATUS_MESSAGE_NONAUTHORITATIVE_INFO           "Non-Authoritative Information"
#define HTTP_STATUS_MESSAGE_NO_CONTENT                      "No Content"
#define HTTP_STATUS_MESSAGE_RESET_CONTENT                   "Reset Content"
#define HTTP_STATUS_MESSAGE_PARTIAL_CONTENT                 "Partial Content"
#define HTTP_STATUS_MESSAGE_MULTI_STATUS                    "Multi-Status"
#define HTTP_STATUS_MESSAGE_ALREADY_REPORTED                "Already Reported"
#define HTTP_STATUS_MESSAGE_IM_USED                         "IM Used"
#define HTTP_STATUS_MESSAGE_MULTIPLE_CHOICES                "Multiple Choices"
#define HTTP_STATUS_MESSAGE_MOVED_PERMANENTLY               "Moved Permanently"
#define HTTP_STATUS_MESSAGE_FOUND                           "Found"
#define HTTP_STATUS_MESSAGE_SEE_OTHER                       "See Other"
#define HTTP_STATUS_MESSAGE_NOT_MODIFIED                    "Not Modified"
#define HTTP_STATUS_MESSAGE_USE_PROXY                       "Use Proxy"
#define HTTP_STATUS_MESSAGE_TEMPORARY_REDIRECT              "Temporary Redirect"
#define HTTP_STATUS_MESSAGE_PERMANENT_REDIRECT              "Permanent Redirect"
#define HTTP_STATUS_MESSAGE_BAD_REQUEST                     "Bad Request"
#define HTTP_STATUS_MESSAGE_UNAUTHORIZED                    "Unauthorized"
#define HTTP_STATUS_MESSAGE_PAYMENT_REQUIRED                "Payment Required"
#define HTTP_STATUS_MESSAGE_FORBIDDEN                       "Forbidden"
#define HTTP_STATUS_MESSAGE_NOT_FOUND                       "Not Found"
#define HTTP_STATUS_MESSAGE_METHOD_NOT_ALLOWED              "Method Not Allowed"
#define HTTP_STATUS_MESSAGE_NOT_ACCEPTABLE                  "Not Acceptable"
#define HTTP_STATUS_MESSAGE_PROXY_AUTH_REQUIRED             "Proxy Authentication Required"
#define HTTP_STATUS_MESSAGE_REQUEST_TIMEOUT                 "Request Timeout"
#define HTTP_STATUS_MESSAGE_CONFLICT                        "Conflict"
#define HTTP_STATUS_MESSAGE_GONE                            "Gone"
#define HTTP_STATUS_MESSAGE_LENGTH_REQUIRED                 "Length Required"
#define HTTP_STATUS_MESSAGE_PRECONDITION_FAILED             "Precondition Failed"
#define HTTP_STATUS_MESSAGE_REQUEST_ENTITY_TOO_LARGE        "Request Entity Too Large"
#define HTTP_STATUS_MESSAGE_REQUEST_URI_TOO_LONG            "Request URI Too Long"
#define HTTP_STATUS_MESSAGE_UNSUPPORTED_MEDIA_TYPE          "Unsupported Media Type"
#define HTTP_STATUS_MESSAGE_REQUESTED_RANGE_NOT_SATISFIABLE "Requested Range Not Satisfiable"
#define HTTP_STATUS_MESSAGE_EXPECTATION_FAILED              "Expectation Failed"
#define HTTP_STATUS_MESSAGE_TEAPOT                          "I'm a teapot"
#define HTTP_STATUS_MESSAGE_MISDIRECTED_REQUEST             "Misdirected Request"
#define HTTP_STATUS_MESSAGE_UNPROCESSABLE_ENTITY            "Unprocessable Entity"
#define HTTP_STATUS_MESSAGE_LOCKED                          "Locked"
#define HTTP_STATUS_MESSAGE_FAILED_DEPENDENCY               "Failed Dependency"
#define HTTP_STATUS_MESSAGE_TOO_EARLY                       "Too Early"
#define HTTP_STATUS_MESSAGE_UPGRADE_REQIUIRED               "Upgrade Required"
#define HTTP_STATUS_MESSAGE_PRECONDITION_REQUIRED           "Precondition Required"
#define HTTP_STATUS_MESSAGE_TOO_MANY_REQUESTS               "Too Many Requests"
#define HTTP_STATUS_MESSAGE_REQUEST_HEADER_FIELDS_TOO_LARGE "Request Header Fields Too Large"
#define HTTP_STATUS_MESSAGE_UNAVAILABLE_FOR_LEGAL_REASONS   "Unavailable For Legal Reasons"
#define HTTP_STATUS_MESSAGE_INTERNAL_SERVER_ERROR           "Internal Server Error"
#define HTTP_STATUS_MESSAGE_NOT_IMPLEMENTED                 "Not Implemented"
#define HTTP_STATUS_MESSAGE_BAD_GATEWAY                     "Bad Gateway"
#define HTTP_STATUS_MESSAGE_UNAVAILABLE                     "Service Unavailable"
#define HTTP_STATUS_MESSAGE_GATEWAY_TIMEOUT                 "Gateway Timeout"
#define HTTP_STATUS_MESSAGE_HTTP_VERSION_NOT_SUPPORTED      "HTTP Version Not Supported"
#define HTTP_STATUS_MESSAGE_VARIAN_ALSO_NEGOTIATES          "Variant Also Negotiates"
#define HTTP_STATUS_MESSAGE_INSUFFICIENT_STORAGE            "Insufficient Storage"
#define HTTP_STATUS_MESSAGE_LOOP_DETECTED                   "Loop Detected"
#define HTTP_STATUS_MESSAGE_NOT_EXTENDED                    "Not Extended"
#define HTTP_STATUS_MESSAGE_NETWORK_AUTHENTICATION_REQUIRED "Network Authentication Required"

#define HTTP_REQUEST_HEADER_AIM                            "A-IM"
#define HTTP_REQUEST_HEADER_ACCEPT                         "Accept"
#define HTTP_REQUEST_HEADER_ACCEPT_CHARSET                 "Accept-Charset"
#define HTTP_REQUEST_HEADER_ACCEPT_DATETIME                "Accept-Datetime"
#define HTTP_REQUEST_HEADER_ACCEPT_ENCODING                "Accept-Encoding"
#define HTTP_REQUEST_HEADER_ACCEPT_LANGUAGE                "Accept-Language"
#define HTTP_REQUEST_HEADER_ACCEPT_CONTROL_REQUEST_METHOD  "Access-Control-Request-Method"
#define HTTP_REQUEST_HEADER_ACCEPT_CONTROL_REQUEST_HEADERS "Access-Control-Request-Headers"
#define HTTP_REQUEST_HEADER_AUTHORIZATION                  "Authorization"
#define HTTP_REQUEST_HEADER_CACHE_CONTROL                  "Cache-Control"
#define HTTP_REQUEST_HEADER_CONNECTION                     "Connection"
#define HTTP_REQUEST_HEADER_CONTENT_ENCODING               "Content-Encoding"
#define HTTP_REQUEST_HEADER_CONTENT_LENGTH                 "Content-Length"
#define HTTP_REQUEST_HEADER_CONTENT_MD5                    "Content-MD5"
#define HTTP_REQUEST_HEADER_CONTENT_TYPE                   "Content-Type"
#define HTTP_REQUEST_HEADER_COOKIE                         "Cookie"
#define HTTP_REQUEST_HEADER_DATE                           "Date"
#define HTTP_REQUEST_HEADER_EXPECT                         "Expect"
#define HTTP_REQUEST_HEADER_FORWARDED                      "Forwarded"
#define HTTP_REQUEST_HEADER_FROM                           "From"
#define HTTP_REQUEST_HEADER_HOST                           "Host"	
#define HTTP_REQUEST_HEADER_HTTP2_SETTINGS                 "HTTP2-Settings"
#define HTTP_REQUEST_HEADER_IF_MATCH                       "If-Match"
#define HTTP_REQUEST_HEADER_IF_MODIFIED_SINCE              "If-Modified-Since"
#define HTTP_REQUEST_HEADER_IF_NONE_MATCH                  "If-None-Match"
#define HTTP_REQUEST_HEADER_IF_RANGE                       "If-Range"
#define HTTP_REQUEST_HEADER_IF_UNMODIFIED_SINCE            "If-Unmodified-Since"
#define HTTP_REQUEST_HEADER_MAX_FORWARDS                   "Max-Forwards"
#define HTTP_REQUEST_HEADER_PRAGMA                         "Pragma"
#define HTTP_REQUEST_HEADER_PROXY_AUTHORIZATION            "Proxy-Authorization"
#define HTTP_REQUEST_HEADER_RANGE                          "Range"
#define HTTP_REQUEST_HEADER_REFERRER                       "Referer"
#define HTTP_REQUEST_HEADER_TE                             "TE"
#define HTTP_REQUEST_HEADER_TRAILER                        "Trailer"	
#define HTTP_REQUEST_HEADER_TRANSFER_ENCODING              "Transfer-Encoding"
#define HTTP_REQUEST_HEADER_USER_AGENT                     "User-Agent"
#define HTTP_REQUEST_HEADER_UPGRADE                        "Upgrade"
#define HTTP_REQUEST_HEADER_WARNING                        "Warning"

#define HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN      "Access-Control-Allow-Origin"
#define HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_CREDENTIALS HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN
#define HTTP_RESPONSE_HEADER_ACCESS_CONTROL_EXPOSE_HEADERS    HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN
#define HTTP_RESPONSE_HEADER_ACCESS_CONTROL_MAX_AGE           HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN
#define HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_METHODS     HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN
#define HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_HEADERS     HTTP_RESPONSE_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN
#define HTTP_RESPONSE_HEADER_ACCEPT_PATCH                     "Accept-Patch"
#define HTTP_RESPONSE_HEADER_ACCEPT_RANGES                    "Accept-Ranges"
#define HTTP_RESPONSE_HEADER_AGE                              "Age"
#define HTTP_RESPONSE_HEADER_ALLOW                            "Allow"	
#define HTTP_RESPONSE_HEADER_ALT_SVC                          "Alt-Svc"
#define HTTP_RESPONSE_HEADER_CACHE_CONTROL                    "Cache-Control"
#define HTTP_RESPONSE_HEADER_CONNECTION                       "Connection"
#define HTTP_RESPONSE_HEADER_CONTENT_DISPOSITION              "Content-Disposition"
#define HTTP_RESPONSE_HEADER_CONTENT_ENCODING                 "Content-Encoding"
#define HTTP_RESPONSE_HEADER_CONTENT_LANGUAGE                 "Content-Language"
#define HTTP_RESPONSE_HEADER_CONTENT_LENGTH                   "Content-Length"
#define HTTP_RESPONSE_HEADER_CONTENT_LOCATION                 "Content-Location"
#define HTTP_RESPONSE_HEADER_CONTENT_MD5                      "Content-MD5"
#define HTTP_RESPONSE_HEADER_CONTENT_RANGE                    "Content-Range"
#define HTTP_RESPONSE_HEADER_CONTENT_TYPE                     "Content-Type"
#define HTTP_RESPONSE_HEADER_DATE                             "Date"
#define HTTP_RESPONSE_HEADER_DELTA_BASE                       "Delta-Base"
#define HTTP_RESPONSE_HEADER_ETAG                             "ETag"
#define HTTP_RESPONSE_HEADER_EXPIRES                          "Expires"
#define HTTP_RESPONSE_HEADER_IM                               "IM"
#define HTTP_RESPONSE_HEADER_LAST_MODIFIED                    "Last-Modified"
#define HTTP_RESPONSE_HEADER_LINK                             "Link"
#define HTTP_RESPONSE_HEADER_LOCATION                         "Location"
#define HTTP_RESPONSE_HEADER_P3P                              "P3P"
#define HTTP_RESPONSE_HEADER_PRAGMA                           "Pragma"
#define HTTP_RESPONSE_HEADER_PROXY_AUTHENTICATE               "Proxy-Authenticate"
#define HTTP_RESPONSE_HEADER_PUBLIC_KEY_PINS                  "Public-Key-Pins"
#define HTTP_RESPONSE_HEADER_RETRY_AFTER                      "Retry-After"
#define HTTP_RESPONSE_HEADER_SET_COOKIE                       "Set-Cookie"
#define HTTP_RESPONSE_HEADER_STRICT_TRANSPORT_SECURITY        "Strict-Transport-Security"
#define HTTP_RESPONSE_HEADER_TRAILER                          "Trailer"
#define HTTP_RESPONSE_HEADER_TRANSFER_ENCODING                "Transfer-Encoding"
#define HTTP_RESPONSE_HEADER_TK                               "Tk"
#define HTTP_RESPONSE_HEADER_UPGRADE                          "Upgrade"
#define HTTP_RESPONSE_HEADER_VARY                             "Vary"
#define HTTP_RESPONSE_HEADER_VIA                              "Via"
#define HTTP_RESPONSE_HEADER_WARNING                          "Warning"
#define HTTP_RESPONSE_HEADER_WWW_AUTHENTICATE                 "WWW-Authenticate"
#define HTTP_RESPONSE_HEADER_X_FRAME_OPTIONS                  "X-Frame-Options"

#define HTTP_MAX_HEADER_BYTES 1 << 20 // 1 MB

/**
 * time_spent takes the start time of a route handler and calculates how long
 * it ran for. It then returns that value to be logged. This is intended to be 
 * used with the log_request function.
 */
#define TIME_SPENT(x)                        \
{                                            \
    clock_t diff = clock() - x;              \
    int msec = diff * 1000 / CLOCKS_PER_SEC; \
    msec % 1000;                             \
}

/**
 * log_request
 */
void
log_request(const struct _u_request *request, struct _u_response *response, clock_t start);

/**
 * callback_default is used to handled calls that don't have a matching route.
 * Returns an expected 404.
 */
int
callback_default(const struct _u_request *request, struct _u_response *response, void *user_data);

/**
 * callback_health_check handles all health check requests to the thinq service.
 */
int
callback_health_check(const struct _u_request *request, struct _u_response *response, void *user_data);

int
callback_auth_token(const struct _u_request *request, struct _u_response *response, void *user_data);

/**
 * get_header_value returns the value for the given header by key. The caller
 * is responsible for freeing the returned value. if the value isn't found, an
 * empty string is returned.
 */
char*
get_header_value(const char *header, char *key);

#endif /* __HTTP_H */
#ifdef __cplusplus
}
#endif
