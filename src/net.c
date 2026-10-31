#include "net.h"
#include "head.h"
#include<string.h>


int init_tcp_ser()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket error");
		return -1;
	}
	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SER_PORT);
	seraddr.sin_addr.s_addr = inet_addr(SER_IP);

	int ret = bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
	if (ret < 0)
	{
		perror("bind error");
		return -1;
	}
	
	ret = listen(sockfd, 10);
	if (ret < 0)
	{
		perror("listen error");
		return -1;
	}

	return sockfd;
}

int recv_http_request(int connfd, char *http_req, int maxlen)
{
    memset(http_req, 0, maxlen);
    ssize_t  size = recv(connfd, http_req, maxlen, 0);
    if (size < 0)
    {
        perror("recv error");
        return size;
    }
    return size;
}
int prase_http_request(char *http_req, HTTP_req_t *preq)
{
    memset(preq, 0, sizeof(HTTP_req_t));
    char *ptmp = strstr(http_req, "\r\n\r\n");
    if (NULL  == ptmp)
    {
        printf("strstr error");
        return -1;
    }
    strcpy(preq->content, ptmp+4);

    ptmp = strtok(http_req, " ");
    if (NULL == ptmp)
    {
        perror("strtok error");
        return -1;
    }
    strcpy(preq->method, ptmp);
    ptmp = strtok(NULL, " ");
    if (NULL == ptmp)
    {
        perror("strtok error");
        return -1;
    }
    strcpy(preq->url, ptmp);
    char *ptmp1=ptmp;
    ptmp= strstr(preq->content, "word=");
     if (NULL == ptmp)
    {
         strcpy(preq->word, "NULL");
    }
    else
    {
        strcpy(preq->word, ptmp+5);
    }
    return 0;
}

int send_http_head(int connfd)
{
    char *phead  = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html;charset=utf-8\r\n"
                    "Server: MyServer\r\n"
                    "Connection: closer\r\n"           
                    "\r\n";
    ssize_t size = send(connfd, phead, strlen(phead), 0);
    if (size < 0)
    {
        perror("send error");
        return size;
    }
    return size;
}


int send_file(int connfd, char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("open file error");
        return -1;
    }
    char buff[1024] = {0};
    while (1)
    {
        ssize_t size = read(fd, buff, sizeof(buff));
        if (size <= 0)
        {
            break;
        }
        send(connfd, buff, size, 0);
    }

    close(fd);

    return 0;
}

int send_http_response(int connfd, HTTP_req_t *preq)
{
    char html_usr[512]={0};
    char html_body[512]={0};
    char filename[512] = {0};
    send_http_head(connfd);
    
    if (0 == strcmp(preq->method, "GET"))
    {
        if (!strcmp(preq->url, "/"))
        {
            sprintf(filename, "../source/1.html");
            send_file(connfd, filename);
        }
        else if (strstr(preq->url, ".jpg") || strstr(preq->url, ".png") || strstr(preq->url, ".bmp"))
        {
           //           /images/back.jpg
            // ../source/images/xxx.jpgf
            sprintf(filename, "../source%s", preq->url);
            send_file(connfd, filename);
        }
    }
    else if (0 == strcmp(preq->method, "POST"))
    {
        if (strstr(preq->url, ".html"))
        {
            //  /3.html
            // ../source/2.html
            //检查应户名和密码
            //preq->content

            sprintf(filename, "../source%s", preq->url);
            send_file(connfd, filename);
        }
    }
    else
    {
        if(strcmp(preq->word,"xiaomi")==0)
        {
            strcpy("<h3>xiaomi</h3>",html_usr);
        }
        sprintf(filename,strcat(html_body,html_usr));
        send_file(connfd, filename);

    }

}


