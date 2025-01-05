#include "myKey.h"

u8 key_code=0;

u8 getkeybtn()
{
	u8 tmp=0;
	if(!P50)tmp+=7;
	if(!P51)tmp+=6;
	if(!P32)tmp+=5;
	if(!P33)tmp+=4;
	return tmp;
}

u8 getkey22()
{
	u8 tmp=0;
	P33=1;P32=0;
	P37=1;P36=1;
	if(!P37)tmp+=5;
	if(!P36)tmp+=7;
	P33=0;P32=1;
	P37=1;P36=1;
	if(!P37)tmp+=4;
	if(!P36)tmp+=6;
	
	return tmp;
}

void key_scan()
{
	static u8 k_state = 0;
	static u8 _key=0;
	static u8 _cnt=0;
	u8 k_tmp;
	
	k_tmp=getkey22();
	switch(k_state)
	{
		case 0:
			if(k_tmp)k_state=1; // �����⵽�м����£������״̬1
		break;
		case 1:
			if(k_tmp)// ���������⵽�����£����һ���ж����ĸ���
			{
				_key=k_tmp;
				if(_key==17)// ������ض��ļ�����������17���������״̬3
				{
					k_state=3;
					_cnt=0;
				}
				else if(_key==8 || _key== 5)// ����������ض��ļ�����������8��5���������״̬4
				{
					k_state=4;
					_cnt=0;
				}else
				k_state=2; // ����������������״̬2
			}else k_state=0; // ������ͷţ���ص���ʼ״̬0
			break;
		case 2:
			if(!k_tmp)// ������ͷţ����¼��ֵ���ص���ʼ״̬0
			{
				key_code=_key;
				k_state=0;
			}
		break;
		case 3:
			if(k_tmp) // ����ǳ����ض�������������17�����ڴﵽһ��ʱ�䳤�������״̬30
			{
				if(++_cnt>=50)
				{
					key_code=17;//�ۼ�
					k_state=30;
				}
			}else k_state=0;
		break;
		case 30:
			if(!k_tmp)
			{
				k_state=0;
			}
		break;
			
		case 4:
			if(k_tmp)// ���������ض�������������8��5�����ڳ���һ��ʱ����ͼ�ֵ
			{
				if(++_cnt>=50)
				{
					_cnt-=5;//��������-�����ۼ�
					key_code=_key;
				}
			}else // ������ͷţ����ͼ�ֵ���ص���ʼ״̬0
			{
				key_code=_key;
				k_state=0;
			}
		break;
	}
}

// ��������¼��ĺ���
void key_proc()
{
    // ִ�м���ɨ��
    key_scan();
    // ���ݰ������봦��ͬ���߼�
    switch(key_code)
    {
        case 4:
			switch(cj.state)
			{
				case 10:cj.state=20;break;
				case 20:cj.state=30;break;
				case 30:cj.state=40;break;
				case 40:cj.state=10;break;
			}
        break;
        case 5:
			cj.state=20;
        break;
        case 6:
			cj.state=40;
        break;
        case 7:
			cj.state=10;
        break;
    }
    key_code=0;// ���ð���������׼����һ�μ����¼�
}











