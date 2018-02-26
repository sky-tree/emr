#ifndef BASE_DATA_TYPE_H
#define BASE_DATA_TYPE_H

#include <qstring.h>

#include "QList.h"
//#include <qlist.h>

struct DBData
{
	QString hostName;				//������ַ
	QString dbName;					//���ݿ�����
	int port;								//�˿ں�

	DBData()
	{
		hostName = "";
		dbName = "";
		port = 1521;
	}
};

struct DoctorData
{
	QString name;
	QString password;
	DBData dbInfo;
	DoctorData()
	{
		name = "";
		password = "";
	}
};

typedef struct  MedicalRecordsTemplate
{
	QString id;							//id
	QString classifyCode;			//�������
	QString type;						//ģ������
	QString name;						//ģ������
	QString code;						//ģ�����
	QString py_Code;					//ƴ����
	QString departmentName;	//��������
	QString departmentCode;		//���Ҵ���
	QString createName;			//������
	QString createID;					//������ID
	QString data;						//ģ������
	QString remarks;					//��ע
	QString isEnable;					//�Ƿ�����

	MedicalRecordsTemplate()
	{
		QString id = "";
		QString classifyCode = "";
		QString type = "";
		QString name = "";
		QString code = "";
		QString py_Code = "";
		QString departmentName = "";
		QString departmentCode = "";
		QString createName = "";
		QString createID = "";
		QString data = "";
		QString remarks = "";
		QString isEnable = "";
	}
}MRTemplate;

//�������ݵ��Զ���ֵ
typedef struct 
{
	QString id;//����ID
	QString outp_card_no;//���￨��
	QString name;//����
	QString sex;//�����Ա�
	QString phone;//���˵绰
	QString birthday;//���˳�������
	QString id_No;//֤������
	QString certificate_Type;//֤������
	QString education;//ѧ��
	QString  nation;//����
	QString  citizenship;//����
	QString  marital_status;//����״��
	QString  home_address;//��ͥ��ַ
	QString home_phone;//��ͥ�绰
	QString  birth_place;//����
	QString  home_zip_code;//�����ʱ�
	QString  occupation;//ְҵ
	QString  mailing_address;//��פ��ַ
	QString  zip_code;//��פ��ַ�ʱ�

	QString inp_NO;//סԺ��
	QString admission_date;//��Ժ����
	QString bed_id;//��λid;
	QString bedName;
	QString dept_Name;//��Ժ����;
	QString contact;//��ϵ��
	QString contact_phone;//��ϵ�˵绰
	QString contact_relation;//��ϵ�˹�ϵ

	QString company;//��λ����
	QString company_addr;//��λ��ַ
	QString company_phone;//��λ�绰
	QString diagnosis;//��Ժ���
	QString current_diag_name;//�����
	QString mr_NO;//������

	QString dept_CODE;
	QString    age;//����

	QString doctor;//����ҽ��ID
	QString nurse;//���λ�ʿ
	QString  attending_Physician;// ATTENDING_PHYSICIAN;//����ҽʦ
	QString chief_Physician;// CHIEF_PHYSICIAN;//����ҽ��
	QString  hopspitalized_Times;// HOSPITALIZED_TIMES;//סԺ����
	QString  patiemt_Cost_Code;// PATIENT_COST_CODE �������
	QString  from_Code;//FROM_CODE ������Դ
	QString  doctorName;

}PatientData;
typedef struct
{
	bool isSupperUser;//�Ƿ��ǳ����û�
	QString user_id;//Ա�����
	QString user_name;//Ա������
	QString dept_code;//����ID ,����Manage_dept,����dept_code.
	QString dept_name;//��������
	QString dept_No;//���ű���
	QString isPhysician;//�Ƿ�����ҽ��
	char user_ser_code;//��������
}DocutorData;

typedef struct operatorTemplateRight
{
	bool addTemplate;
	bool modifyTemplate;
	bool deleteTemplate;
	bool startUpTemplate;//����
	operatorTemplateRight() :addTemplate(false), modifyTemplate(false), deleteTemplate(false), startUpTemplate(false)
	{
	}
}operatorTemplateRight;
typedef struct operatorMedicalRight
{
	bool addTemplate;
	bool modifyTemplate;
	bool deleteTemplate;
	operatorMedicalRight() :addTemplate(false), modifyTemplate(false), deleteTemplate(false)
	{
		
	}

}operatorMedicalRight;

typedef struct WriteAuthority
{
	QString MdcDctr;    //����ҽ��
	QString MdcDctrName;    //����ҽ��
	QString	PhyDctr;    //����ҽ��
	QString	PhyDctrName;    //����ҽ��
	QString RhtDctr;    //����ҽ��
	QString RhtDctrName;    //����ҽ��
	QString ChfDctr;    //����ҽ��
	QString ChfDctrName;    //����ҽ��
	QString SgnDctr;	//����ҽ��
	QString SgnDctrName;	//����ҽ��
	QString AhgDctr;	//����ҽ��
	QString AhgDctrName;	//����ҽ��
	QString CnlDctr;	//����ҽ��
	QString CnlDctrName;	//����ҽ��
	QString IcuDctr;	//ICUҽ��
	QString IcuDctrName;	//ICUҽ��
	QString IptNurse;   //סԺ��ʿ
	QString IptNurseName;   //סԺ��ʿ
	QString TrhDctr;    //����ҽ��
	QString TrhDctrName;    //����ҽ��

}writeAuthority;

typedef struct MdcAuthority
{
	QString mdcId;     //pid
	QString mdcName;   //��������
	QString mdcAuthor; //Ȩ�޴���
}mdcAuthority;
#endif