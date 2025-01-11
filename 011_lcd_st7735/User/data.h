#ifndef DATA_H
#define DATA_H





// 定义一个结构体用于存储测量数据
struct _cj//注意结构体内存对齐问题，大的数据类型放上面
{
    u8 state; // 总状态
    u8 h; // 时
    u8 m; // 分
    u8 s; // 秒
};
extern struct  _cj cj;



#endif


