/*
1.TURN THE CLIENT INTO A LIBRARY(SHARED LIBRARY).SO
2. TURN MAIN() INTO OUR OWN CONNECT()
3. REPLACE REGULAR CONNECT() WITH OUR OWN CONNECT()
4. GRAB THE IP AND PORT FROM THE ORIG CONNECT()

*/



#include "toralize.h"

Req *request(struct sockaddr_in *sock2)
{
    Req *req;

    req = malloc(reqsize); 
    if (req == NULL) {
        perror("malloc");
        return NULL;
    }
    
    req->vn = 4;
    req->cd = 1;
    req->dsport = sock2->sin_port;
    req->dstip = sock2->sin_addr.s_addr;
    
    strncpy((char *)req->userid, USERNAME, 8);
    
    return req;
}


int connect(int s2, const struct sockaddr *sock2,socklen_t addrlen){
    int s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buf[ressize];
    int success;
    char tmp[512]; 
    int (*p)(int , const struct sockaddr*,socklen_t);


    p = dlsym(RTLD_NEXT, "connect");

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);
    
    if (p(s, (struct sockaddr *)&sock, sizeof(sock)) < 0) {
        perror("connect");
        return -1;
    }
    
    printf("Connected\n");

    req = request((struct sockaddr_in *)sock2);
    if (req == NULL) {
        fprintf(stderr, "Failed to create request\n");
        close(s);
        return -1;
    }

    write(s, req, reqsize);
    memset(buf, 0, ressize);
        
    if (read(s, buf, ressize) < 1) {
        perror("read");
        free(req);
        close(s);
        return -1;
    }

    res = (Res *)buf;
    success = (res->cd == 90);
    if (!success) {
        fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);
        close(s);
        return -1;
    }

    print("Successfully connected to the proxy\n","%s:%d");
    dup2(s,s2);
    free(req);
    
    
    return 0;
}