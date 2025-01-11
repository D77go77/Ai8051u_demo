#include "scheduler.h"
// ����������ṹ�嶨��
typedef struct
{
    void (*task_func)(void);    // ������
    unsigned long int rate_ms;  // ����ִ�����ڣ����룩
    unsigned long int last_run; // �����ϴ�����ʱ��
} task_t;
// �����������б�
task_t Scheduler_Task[] = {
	{task_led0_proc,	5,	0},		  // LED�˿�ɨ�裬ÿ20����ִ��һ��
	{task_led1_proc,	5,	0},
	{task_beep_proc,	5,	0},
//	{task_key_gets, 	5,	0},
	{task_smg_proc,		100,0},
	{task_smg_scan,		2,	0},
	{task_adc_key_proc,10,	0}
};

u8 task_num;//��������
//��������ʼ��
void Scheduler_Init(void)
{
    task_num = sizeof(Scheduler_Task) / sizeof(task_t); // ������������
}
//����������
void Scheduler_Run(void)
{
    u8 i;
    for (i = 0; i < task_num; i++)
    {
        unsigned long int now_time = uwTick;// ��ȡ��ǰʱ�� 
        if (now_time >= (Scheduler_Task[i].last_run + Scheduler_Task[i].rate_ms))// ��������Ƿ���Ҫִ��
        {
            Scheduler_Task[i].last_run = now_time; // ���������ϴ�����ʱ��
            Scheduler_Task[i].task_func();// ִ������         
        }
    }
}