#include "scheduler.h"
// 调度器任务结构体定义
typedef struct
{
    void (*task_func)(void);    // 任务函数
    unsigned long int rate_ms;  // 任务执行周期（毫秒）
    unsigned long int last_run; // 任务上次运行时间
} task_t;
// 调度器任务列表
task_t Scheduler_Task[] = {
	{task_led0_proc,	5,	0},		  // LED端口扫描，每20毫秒执行一次
	{task_led1_proc,	5,	0},
	{task_beep_proc,	5,	0},
//	{task_key_gets, 	5,	0},
	{task_smg_proc,		100,0},
	{task_smg_scan,		2,	0},
	{task_adc_key_proc,10,	0}
};

u8 task_num;//任务数量
//调度器初始化
void Scheduler_Init(void)
{
    task_num = sizeof(Scheduler_Task) / sizeof(task_t); // 计算任务数量
}
//调度器运行
void Scheduler_Run(void)
{
    u8 i;
    for (i = 0; i < task_num; i++)
    {
        unsigned long int now_time = uwTick;// 获取当前时间 
        if (now_time >= (Scheduler_Task[i].last_run + Scheduler_Task[i].rate_ms))// 检查任务是否需要执行
        {
            Scheduler_Task[i].last_run = now_time; // 更新任务上次运行时间
            Scheduler_Task[i].task_func();// 执行任务         
        }
    }
}