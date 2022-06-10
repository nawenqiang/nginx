#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static char *ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void* conf);
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);
static char* ngx_conf_set_myconfig(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static char *ngx_http_mytest_merge_loc_conf(ngx_conf_t* cf, void* parent, void *child);
static void* ngx_http_mytest_create_loc_conf(ngx_conf_t* cf);

static ngx_uint_t testRadixValue1 = 0x20000000;
static ngx_uint_t testRadixValue2 = 0x40000000;
static ngx_uint_t testRadixValue3 = 0x80000000;
static ngx_uint_t testRadixValue4 = 0xa0000000;



#if 0
typedef struct {
    // 用于散列表中的关键字
    ngx_str_t servername;
    // 这个成员仅是为了方便区别而已
    ngx_int_t seq;
} TestWildcardHashNode;

void ngx_http_hash_test() {
    /* ngx_hash_keys_arrays_t用于预先向散列表中添加元素，这里的元素支持带通配符*/
    ngx_hash_keys_arrays_t ha;
    // 支持通配符的散列表
    ngx_hash_combined_t combinedHash;
    ngx_memzero(&ha, sizeof(ngx_hash_keys_arrays_t));

    ha.temp_pool = ngx_create_pool(16384, cf->log);
    if (ha.temp_pool == NULL) {
        return NGX_ERROR;
    }
    ha.pool = cf->pool;
    if (ngx_hash_keys_array_init(&ha, NGX_HASH_LARGE) != NGX_OK) {
        return NGX_ERROR;
    }

    TestWildcardHashNode testHashNode[3];
    testHashNode[0].servername.len = ngx_strlen(".test.com");
    testHashNode[0].servername.data = ngx_pcalloc(cf->pool, ngx_strlen(".test.com"));
    ngx_memcpy(testHashNode[0].servername.data,".test.com",ngx_strlen(".test.com"));

    testHashNode[1].servername.len = ngx_strlen("www.test.*");
    testHashNode[1].servername.data = ngx_pcalloc(cf->pool, ngx_strlen("www.test.*"));
    ngx_memcpy(testHashNode[1].servername.data,"www.test.*",ngx_strlen("www.test.*"));

    testHashNode[2].servername.len = ngx_strlen("www.test.com");
    testHashNode[2].servername.data = ngx_pcalloc(cf->pool, ngx_strlen("www.test.com"));
    ngx_memcpy(testHashNode[2].servername.data,"www.test.com",ngx_strlen("www.test.com"));

    for (i = 0; i < 3; i++)
    {
        testHashNode[i].seq = i;
        ngx_hash_add_key(&ha, &testHashNode[i].servername,&testHashNode[i],NGX_HASH_WILDCARD_KEY);
    }
}

ngx_hash_init_t hash;
hash.key = ngx_hash_key_lc;
hash.max_size = 100;
hash.bucket_size = 48;
hash.name = "test_server_name_hash";
hash.pool = cf->pool;

if (ha.keys.nelts) {
    /*需要显式地把
     * ngx_hash_init_t中的
     * hash指针指向
     * combinedHash中的完全匹配散列表
     * */
    hash.hash = &combinedHash.hash;
    // 初始化完全匹配散列表时不会使用到临时内存池
    hash.temp_pool = NULL;
     /*将
     keys动态数组直接传给
     ngx_hash_init方法即可，
     ngx_hash_init_t中的
     hash指针就是初始化成功的散列表
     */
    if (ngx_hash_init(&hash, ha.keys.elts, ha.keys.nelts) != NGX_OK)
    {
    return NGX_ERROR;
    }
}

if (ha.dns_wc_head.nelts) {
    hash.hash = NULL;
    // 注意，
    // ngx_hash_wildcard_init方法需要使用临时内存池
    hash.temp_pool = ha.temp_pool;
    if (ngx_hash_wildcard_init(&hash, ha.dns_wc_head.elts,
    ha.dns_wc_head.nelts)!= NGX_OK)
    {
    return NGX_ERROR;
    }
    /* ngx_hash_init_t中的
    // hash指针是
    // ngx_hash_wildcard_init初始化成功的散列表，需要将它赋到
    // combinedHash.wc_head前置通配符散列表指针中
    // */
    combinedHash.wc_head = (ngx_hash_wildcard_t ) hash.hash;
    }

if (ha.dns_wc_tail.nelts) {
    hash.hash = NULL;
    // 注意，
    // ngx_hash_wildcard_init方法需要使用临时内存池
    hash.temp_pool = ha.temp_pool;
    if (ngx_hash_wildcard_init(&hash, ha.dns_wc_tail.elts,
    ha.dns_wc_tail.nelts)!= NGX_OK)
    {
    return NGX_ERROR;
    }
    /* ngx_hash_init_t中的
    hash指针是
    ngx_hash_wildcard_init初始化成功的散列表，需要将它赋到
    combinedHash.wc_tail后置通配符散列表指针中
    */
    combinedHash.wc_tail = (ngx_hash_wildcard_t ) hash.hash;
    }
    
ngx_destroy_pool(ha.temp_pool);


findServer
ngx_str_t findServer;
findServer.len = ngx_strlen("www.test.org");
/*为什么必须要在内存池中分配空间以保存关键字呢？因为我们使用的散列方法是
 * ngx_hash_key_lc，它会试着把关键字全小写
 */
    findServer.data = ngx_pcalloc(cf->pool, ngx_strlen("www.test.org"));
 ngx_memcpy(findServer.data,"www.test.org",ngx_strlen("www.test.org"));
 /*
 * ngx_hash_find_combined方法会查找出
 * www.test.*对应的散列表元素，返回其指向的用户数据
 * ngx_hash_find_combined，也就是
 * testHashNode[1]*/
TestWildcardHashNode* findHashNode = ngx_hash_find_combined(&combinedHash,
        ngx_hash_key_lc(findServer.data, findServer.len),
        findServer.data, findServer.len);

#endif
    

typedef struct {
    ngx_str_t my_str;
    ngx_int_t my_num;
    ngx_flag_t my_flag;
    size_t my_size;
    ngx_array_t* my_str_array;
    ngx_array_t* my_keyval;
    off_t my_off;
    ngx_msec_t my_msec; //ngx_uint_t m
    time_t my_sec;
    ngx_bufs_t my_bufs;
    ngx_uint_t my_enum_seq;
    ngx_uint_t my_bitmask;
    ngx_uint_t my_access;
    ngx_path_t* my_path;
} ngx_http_mytest_conf_t;

typedef struct {
    ngx_uint_t my_step;
} ngx_http_mytest_ctx_t;

static ngx_http_module_t ngx_http_mytest_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */
    NULL, /* create main configuration */
    NULL, /* init main configuration */
    NULL, /* create server configuration */
    NULL, /* merge server configuration */
    ngx_http_mytest_create_loc_conf, /* create location configuration */
    ngx_http_mytest_merge_loc_conf /* merge location configuration */
};



static ngx_command_t ngx_http_mytest_commands[] = {
    { 
        ngx_string("mytest"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
        ngx_http_mytest,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL },

    { ngx_string("test_flag"),
        NGX_HTTP_LOC_CONF| NGX_CONF_FLAG,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t, my_flag),
        NULL },

    { ngx_string("test_myconfig"),
        NGX_HTTP_SRV_CONF |NGX_HTTP_LOC_CONF | NGX_CONF_TAKE12,
        ngx_conf_set_myconfig,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL },

    ngx_null_command
};


ngx_module_t ngx_http_mytest_module = {
    NGX_MODULE_V1,
    &ngx_http_mytest_module_ctx, /* module context */
    ngx_http_mytest_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

static char* ngx_conf_set_myconfig(ngx_conf_t* cf, ngx_command_t* cmd, void* conf)
{
    /*注意，参数conf就是HTTP框架传给用户的在
    ngx_http_mytest_create_loc_conf回调方法中分配的结构体
    ngx_http_mytest_conf_t*/

    ngx_http_mytest_conf_t* mycf = (ngx_http_mytest_conf_t*)conf;

    /* cf->args是1个ngx_array_t队列，它的成员都是
    ngx_str_t结构。我们用value指向ngx_array_t的
    elts内容，其中value[1]就是第1个参数，同理，
    value[2]是第2个参数
    */
    ngx_str_t* value = cf->args->elts;
    if (cf->args->nelts > 1)
    {
        // 直接赋值即可，ngx_str_t结构只是指针的传递
        mycf->my_str = value[1];
    }

    if (cf->args->nelts > 2)
    {
        // 将字符串形式的第2个参数转为整型
        mycf->my_num = ngx_atoi(value[2].data, value[2].len);
        //如果字符串转化整型失败，将报“invalid number”错误，Nginx启动失败
        
        if (mycf->my_num == NGX_ERROR) {
            return "invalid number";
        }
    }
    // 返回成功
    return NGX_CONF_OK;
}
    
/*
 *    server {
        listen       8000;
        server_name  localhost;


        test_myconfig 1 2;
        location /a {
            mytest;
        }

        location /{
            root   html;
            index  index.html index.htm;
        }

        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
        }
        ngx_http_mytest_merge_loc_conf被触发4次，http和server合并一次，共三个location，每个与server合并一次
 */
static char *
ngx_http_mytest_merge_loc_conf(ngx_conf_t* cf, void* parent, void *child)
{
    ngx_http_mytest_conf_t* prev = (ngx_http_mytest_conf_t* )parent;
    ngx_http_mytest_conf_t* conf = (ngx_http_mytest_conf_t* )child;
    ngx_conf_merge_str_value(conf->my_str, prev->my_str, "defaultstr");
    //ngx_conf_merge_sec_value(conf->my_num, prev->my_num, 99);

    conf->my_num = prev->my_num;
    return NGX_CONF_OK;
}

static void* ngx_http_mytest_create_loc_conf(ngx_conf_t* cf)
{
    ngx_http_mytest_conf_t* mycf;
    mycf = (ngx_http_mytest_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_mytest_conf_t));
    if (mycf == NULL) {
        return NULL;
    }
    mycf->my_flag = NGX_CONF_UNSET;
    mycf->my_num = NGX_CONF_UNSET;
    mycf->my_str_array = NGX_CONF_UNSET_PTR;
    mycf->my_keyval = NULL;
    mycf->my_off = NGX_CONF_UNSET;
    mycf->my_msec = NGX_CONF_UNSET_MSEC;
    mycf->my_sec = NGX_CONF_UNSET;
    mycf->my_size = NGX_CONF_UNSET_SIZE;
    return mycf;
}

static char *ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void* conf) {
    ngx_http_core_loc_conf_t *clcf;
    /*首先找到
     * mytest配置项所属的配置块，
     * clcf看上去像是
     * location块内的数据结构，其实不然，它可以是
     * main、
     * srv或者
     * loc级别配置项，也就是说，在每个
     * http{}和
     * server{}内也都有一个
     * ngx_http_core_loc_conf_t结构体
     * */
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module); 
    /*HTTP框架在处理用户请求进行到
      NGX_HTTP_CONTENT_PHASE阶段时，如果请求的主机域名、
      URI与mytest配置项所在的配置块相匹配，就将调用我们实现的
      ngx_http_mytest_handler方法处理这个请求
      */
                                                                          
    clcf->handler = ngx_http_mytest_handler; 
    return NGX_CONF_OK;
}

void ngx_http_radix_tree_test(ngx_http_request_t *r) {
    ngx_radix_tree_t * radixTree = ngx_radix_tree_create(r->pool, -1);

    int rc;
    rc = ngx_radix32tree_insert(radixTree,0x20000000, 0xe0000000, (uintptr_t)&testRadixValue1);
    rc = ngx_radix32tree_insert(radixTree,0x40000000, 0xe0000000, (uintptr_t)&testRadixValue2);
    rc = ngx_radix32tree_insert(radixTree,0x80000000, 0xe0000000, (uintptr_t)&testRadixValue3);
    rc = ngx_radix32tree_insert(radixTree,0x200000, 0xe0000000, (uintptr_t)&testRadixValue3);
    //rc = ngx_radix32tree_insert(radixTree,0xa0000000, 0xe0000000, (uintptr_t)&testRadixValue4);

    ngx_uint_t* v1 = (ngx_uint_t *) ngx_radix32tree_find(radixTree, 0x200000);
    ngx_uint_t* v2 = (ngx_uint_t *) ngx_radix32tree_find(radixTree, 0x400000);
    ngx_uint_t* v3 = (ngx_uint_t *) ngx_radix32tree_find(radixTree, 0xa0000000);

    rc = ngx_radix32tree_delete(radixTree, 0x80000000, 0xe0000000);

    ngx_uint_t* v4 = (ngx_uint_t *) ngx_radix32tree_find(radixTree, 0x80000000);
    return;
}

static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r) {
    ngx_http_radix_tree_test(r);

    void* mcf = ngx_http_get_module_main_conf(r, ngx_http_mytest_module);
    void* scf = ngx_http_get_module_srv_conf(r, ngx_http_mytest_module);
    void* lcf = ngx_http_get_module_loc_conf(r, ngx_http_mytest_module);
    void* myctx = ngx_http_get_module_ctx(r, ngx_http_mytest_module);

    if (myctx == NULL)
    {
        /*必须在当前请求的内存池
         * r->pool中分配上下文结构体，这样请求结束时结构体占用的内存才会释放
         * */
        myctx = ngx_palloc(r->pool, sizeof(ngx_http_mytest_ctx_t));
        if (myctx == NULL)
        {
            return NGX_ERROR;
        }
        // 将刚分配的结构体设置到当前请求的上下文中
        ngx_http_set_ctx(r, myctx, ngx_http_mytest_module);
    }
        

    // 必须是GET或者HEAD方法，否则返回405 Not Allowed
    if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }
    //ngx_http_read_client_request_body
    // 丢弃请求中的包体
    ngx_int_t rc = ngx_http_discard_request_body(r); 
    if (rc != NGX_OK) {
        return rc;
    }
    /*设置返回的Content-Type。注意，
    ngx_str_t有一个很方便的初始化宏
    ngx_string，它可以把
    ngx_str_t的data和len成员都设置好
    */
    ngx_str_t type = ngx_string("text/plain"); // 返回的包体内容
    ngx_str_t response = ngx_string("Hello World!"); // 设置返回状态码

    r->headers_out.status = NGX_HTTP_OK; // 响应包是有包体内容的，需要设置Content-Length长度
    r->headers_out.content_length_n = response.len; // 设置Content-Type
    r->headers_out.content_type = type; // 发送HTTP头部
    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }
    // 构造ngx_buf_t结构体准备发送包体
    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, response.len); 
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR; 
    }
    // 将Hello World复制到ngx_buf_t指向的内存中
    ngx_memcpy(b->pos, response.data, response.len); // 注意，一定要设置好last指针
    b->last = b->pos + response.len; // 声明这是最后一块缓冲区
    b->last_buf = 1;
    // 构造发送时的ngx_chain_t结构体
    ngx_chain_t out;
    // 赋值ngx_buf_t
    out.buf = b;
    // 设置next为NULL
    out.next = NULL;
    /*最后一步为发送包体，发送结束后
    HTTP框架会调用
    ngx_http_finalize_request方法结束请求
    */

    return ngx_http_output_filter(r, &out); 
    /*
    ngx_buf_t* b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t))
    b->start = (u_char*)ngx_pcalloc(r->pool, 128); 
    b->pos = b->start;
    b->last = b->start;
    b->end = b->last + 128;
    b->temporary = 1;
    */
}

