#ifndef __NET_H__
#define __NET_H__


#define SER_PORT 8080
#define SER_IP   "192.168.101.133"


typedef struct _http_req
{
    char method[16];
    char url[256];
    char word[1024];
    char content[512];
}HTTP_req_t;



int init_tcp_ser();
int recv_http_request(int connfd, char *http_req, int maxlen);
int send_http_response(int connfd, HTTP_req_t *preq);
int prase_http_request(char *http_req, HTTP_req_t *preq);
#endif