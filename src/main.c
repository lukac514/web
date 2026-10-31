
#include "head.h"
#include "net.h"
#include"sqlite.h"
int main(void)
{
    char http_req[1024] = {0};
    ssize_t size = 0;
    HTTP_req_t req;
    int ret = 0;

    int sockfd = init_tcp_ser();
    if (sockfd < 0)
    {
        return -1;
    }

    while (1)
    {
        int connfd = accept(sockfd, NULL, NULL);
        if (connfd < 0)
        {
            perror("accept error");
            return -1;
        }
        
        size = recv_http_request(connfd, http_req, sizeof(http_req));
        if (size < 0)
        {
            return -1;
        }
        printf("------------------------\n");
        printf("%s\n", http_req);
        printf("------------------------\n");

        ret = prase_http_request(http_req, &req);
        if (ret < 0)
        {
            return -1;
        }
        char buff[1024];
        printf("method : %s\n", req.method);
        printf("url : %s\n", req.url);
         printf("word : %s\n", req.word);
        db_open("../phone.db");
        db_query_by_brand(req.word,buff, strlen(req.word));
        send_http_response(connfd, &req);
        printf("%s\n",buff);
        close(connfd);
        db_close("../phone.db");
    }
    close(sockfd);
    return 0;
}