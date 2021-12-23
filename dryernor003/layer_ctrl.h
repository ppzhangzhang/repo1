#include "projutils.h"

/*
#include "layer_ctrl.h"
*/




ITULayer* ctrlLayer;
ITULayer* logoLayer;

ITULayer* mainLayer;
ITULayer* startLayer;
ITULayer* prodmodLayer;
ITULayer* settingWiFiSsidLayer;

extern bool busComOK;
extern bool bluetoothOK;
extern u16 ctrl_timer;
//

bool(*ButtonUpOnPress)();
bool(*ButtonDownOnPress)();
bool(*ProcessButtomDown)();

///*
//Nc ReturnIdle
//*/
extern void outQsetType(u8 type);
///*
//Cycle StartPause
//*/
extern void outQsetTypeValue(u8 type, u8 value);
extern void boardsetCycleCMD(u8 ct);
//extern void outQsetOther(u8 type, u8 location);
extern void outQquery(u16 qt);
extern void outQsetBuffer(u16 address, u8 datalen, u8* data);
//
extern void out_write_buffer_struct_to_board(void*data, u8 len);

typedef struct{
	u8 CycleName;
	u8 RunMode;
	u8 DryingStage;
	u16 RmainTime;
	u16 MoneyCost;
	bool PayShow;
}StartLayerType;

typedef struct{
	u8 CycleName;
}CtrlLayerType;

typedef enum{//�·���
	DRY_Nc = 0,				//0 ���¼�
	DRY_Reset,				//1 ��λ
	DRY_PowerButton = 3,	//3	0=�ػ�/1=���� ��Դ��
	DRY_Cycle,				//4	������ ����ѡ��
	DRY_CycleStart,			//5 ������ ����ѡ��+����
	DRY_StartPause,			//6	0=��ͣ/1=���� ��ͣ
	DRY_Invalid = 10,		//10 ��Ч����
	DRY_FactoryReset,		//11 0=��ͨ�û�/1=�ۺ� 0=��ͨ�û�ʱ,ֻ���������ָ�����ֵ.1 = �ۺ�ʱ, �����������ָ�����ֵ��, ����������ͺ������������������������.
	DRY_ReturnIdle,			//12 ���ش���״̬
	DRY_MessageReply,		//13 ��ʾ��Ϣ���(��WP�ϴ����ͬ) �Ի����ֵ0=��/1=ȷ��/2=ȡ��3=��֪����/4=��������  ��ʾ��ϢӦ�� ע:WP��������ʾ��Ϣ,�û�������Ӧ�������е�ȷ�ϲ���.
	DRY_SetCycleParamters,	//14 0e ������ ���ó���۸�
	DRY_SetHappyHourTime = 16,//16 0x10	�ۿ����
	DRY_SetRtc,				//17 0x11 ����RTCʱ��
	DRY_SetMachineRunParameters,//18 0x12 ���û������в���
	DRY_UiTouch,			//UiTouch Ͳ�� �� ���ϵ���������
	DRY_ModuleSet = 200,	//200 0=�����ͺ�����/1=��ż�/2=��ż�3=����/4=����
	DRY_TestOptionsSwitch,	//201 0=�ӿ�ʮ��ѭ������1=��������ѭ������2=washerģʽѭ������3=������ʾ(B_TST)ѭ������4=�ֿظ���ѭ������5=demo6=Diagnostic Swtich ����ģʽ����ע:���ڿ����͹ر�һЩ����Ĺ���ģʽ
	DRY_TstSpecialNum,		//202 1=��1/ 2=��1/ 3=��10/ 4=��10 ������ʾ���
	DRY_UniversalDialogBox,	//203 0=��һ��/1=��һ��2=��ͣ/3=���� ͨ�öԻ��� ע:���ڲ����Լ�/�ֿظ���
	DRY_SetMotoPara,		//204 0=ת��1=���ٶ�2=���ٶ� ֵ,˫�ֽڸ�λ��ǰת��:��Χ0-1600rpm,���ٶ�:��Χ0-1001rpm/s���ٶ�:��Χ0-1001rpm/s ���ڼ�����������
	DRY_UISendData,			//205 0=UI version,��λ��ǰ 1=���� data length data��
	DRY_EXTRA_QUERY,//��ѯ
	DRY_EXTRA_SETBUFFER,//����
	DRY_UIEVENT_TstStep,//ϴЬ�� testmode��ֲ
}DRY_UI_EVENT_TYPE;