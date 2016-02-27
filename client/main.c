#include "main.h"


#define DECL(res, name, ...) \
	extern res name(__VA_ARGS__); \
	res (* real_ ## name)(__VA_ARGS__)  __attribute__((section(".magicptr"))); \
	res my_ ## name(__VA_ARGS__)


DECL(int, FSAInit, void) {
    // test 
    char msg[5];
    msg[0] = 'T';
    msg[1] = 'e';
    msg[2] = 's';
    msg[3] = 't';
    msg[4] = '\0';
    cafiine_OSFatal(msg);
    
    if ((int)bss_ptr == 0x0a000000) {
        bss_ptr = memalign(sizeof(struct bss_t), 0x40);
        memset(bss_ptr, 0, sizeof(struct bss_t));
    }
    return real_FSAInit();
}
DECL(int, FSAShutdown, void) {
    return real_FSAShutdown();
}
DECL(int, FSAAddClient, void *r3) {
    int res = real_FSAAddClient(r3);

    if ((int)bss_ptr != 0x0a000000 && res < MAX_CLIENT && res >= 0) {
        cafiine_connect(&bss.socket_fsa[res]);
    }

    return res;
}
DECL(int, FSADelClient, int client) {
    if ((int)bss_ptr != 0x0a000000 && client < MAX_CLIENT && client >= 0) {
        cafiine_disconnect(bss.socket_fsa[client]);
    }

    return real_FSADelClient(client);
}
DECL(int, FSAOpenFile, int client, const char *path, const char *mode, int *handle) {
    if ((int)bss_ptr != 0x0a000000 && client < MAX_CLIENT && client >= 0) {
        int ret;
        if (cafiine_fopen(bss.socket_fsa[client], &ret, path, mode, handle) == 0)
            return ret;
    }

    return real_FSAOpenFile(client, path, mode, handle);
}

static int client_num_alloc(void *pClient) {
    int i;

    for (i = 0; i < MAX_CLIENT; i++)
        if (bss.pClient_fs[i] == 0) {
            bss.pClient_fs[i] = pClient;
            return i;
        }
    return -1;
}
static void clietn_num_free(int client) {
    bss.pClient_fs[client] = 0;
}
static int client_num(void *pClient) {
    int i;

    for (i = 0; i < MAX_CLIENT; i++)
        if (bss.pClient_fs[i] == pClient)
            return i;
    return -1;
}

struct fs_async_t {
    void (*callback)(int status, int command, void *request, void *response, void *context);
    void *context;
    void *queue;
};


DECL(int, curl_easy_init, void) {
    char msg[5];
    msg[0] = 'E';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'y';
    msg[4] = '\0';
    cafiine_OSFatal(msg);
    return real_curl_easy_init();
}
DECL(int, curl_global_init, long flags) {
    char msg[7];
    msg[0] = 'G';
    msg[1] = 'l';
    msg[2] = 'o';
    msg[3] = 'b';
    msg[4] = 'a';
    msg[5] = 'l';
    msg[6] = '\0';
    cafiine_OSFatal(msg);
    return real_curl_global_init(flags);
}
/*DECL(CURLcode, curl_easy_send, CURL * curl , const void * buffer , size_t buflen , size_t * n ) {
    cafiine_OSFatal();
    return real_curl_easy_send(curl, buffer, buflen, n);
}*/
#define MAKE_MAGIC(x) { x, my_ ## x, &real_ ## x }

struct magic_t {
    const void *real;
    const void *replacement;
    const void *call;
} methods[] __attribute__((section(".magic"))) = {
    //MAKE_MAGIC(FSAInit),
    MAKE_MAGIC(curl_easy_init),
    MAKE_MAGIC(curl_global_init),
};
