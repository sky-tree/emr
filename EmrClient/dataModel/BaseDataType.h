#ifndef BASE_DATA_TYPE_H
#define BASE_DATA_TYPE_H

#include <qstring.h>

#include "QList.h"
//#include <qlist.h>

struct DBData
{
	QString hostName;				//主机地址
	QString dbName;					//数据库名称
	int port;								//端口号

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
	QString classifyCode;			//分类代码
	QString type;						//模板类型
	QString name;						//模板名称
	QString code;						//模板代码
	QString py_Code;					//拼音码
	QString departmentName;	//科室名称
	QString departmentCode;		//科室代码
	QString createName;			//创建人
	QString createID;					//创建人ID
	QString data;						//模板数据
	QString remarks;					//备注
	QString isEnable;					//是否启用

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

//用于数据的自动赋值
typedef struct 
{
	QString id;//病人ID
	QString outp_card_no;//就诊卡号
	QString name;//姓名
	QString sex;//病人性别
	QString phone;//病人电话
	QString birthday;//病人出生日期
	QString id_No;//证件号码
	QString certificate_Type;//证件类型
	QString education;//学历
	QString  nation;//民族
	QString  citizenship;//国籍
	QString  marital_status;//婚姻状况
	QString  home_address;//家庭地址
	QString home_phone;//家庭电话
	QString  birth_place;//籍贯
	QString  home_zip_code;//户口邮编
	QString  occupation;//职业
	QString  mailing_address;//常驻地址
	QString  zip_code;//常驻地址邮编

	QString inp_NO;//住院号
	QString admission_date;//入院日期
	QString bed_id;//床位id;
	QString bedName;
	QString dept_Name;//入院科室;
	QString contact;//联系人
	QString contact_phone;//联系人电话
	QString contact_relation;//联系人关系

	QString company;//单位名称
	QString company_addr;//单位地址
	QString company_phone;//单位电话
	QString diagnosis;//入院诊断
	QString current_diag_name;//主诊断
	QString mr_NO;//病案号

	QString dept_CODE;
	QString    age;//年龄

	QString doctor;//主管医生ID
	QString nurse;//责任护士
	QString  attending_Physician;// ATTENDING_PHYSICIAN;//主治医师
	QString chief_Physician;// CHIEF_PHYSICIAN;//主任医生
	QString  hopspitalized_Times;// HOSPITALIZED_TIMES;//住院次数
	QString  patiemt_Cost_Code;// PATIENT_COST_CODE 费用类别
	QString  from_Code;//FROM_CODE 病人来源
	QString  doctorName;

}PatientData;
typedef struct
{
	bool isSupperUser;//是否是超级用户
	QString user_id;//员工编号
	QString user_name;//员工姓名
	QString dept_code;//部门ID ,根据Manage_dept,关联dept_code.
	QString dept_name;//部门名称
	QString dept_No;//部门编码
	QString isPhysician;//是否主治医生
	char user_ser_code;//工作性质
}DocutorData;

typedef struct operatorTemplateRight
{
	bool addTemplate;
	bool modifyTemplate;
	bool deleteTemplate;
	bool startUpTemplate;//启用
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
	QString MdcDctr;    //病历医生
	QString MdcDctrName;    //病历医生
	QString	PhyDctr;    //主管医生
	QString	PhyDctrName;    //主管医生
	QString RhtDctr;    //主治医生
	QString RhtDctrName;    //主治医生
	QString ChfDctr;    //主任医生
	QString ChfDctrName;    //主任医生
	QString SgnDctr;	//手术医生
	QString SgnDctrName;	//手术医生
	QString AhgDctr;	//麻醉医生
	QString AhgDctrName;	//麻醉医生
	QString CnlDctr;	//会诊医生
	QString CnlDctrName;	//会诊医生
	QString IcuDctr;	//ICU医生
	QString IcuDctrName;	//ICU医生
	QString IptNurse;   //住院护士
	QString IptNurseName;   //住院护士
	QString TrhDctr;    //主任医生
	QString TrhDctrName;    //主任医生

}writeAuthority;

typedef struct MdcAuthority
{
	QString mdcId;     //pid
	QString mdcName;   //病历名称
	QString mdcAuthor; //权限代码
}mdcAuthority;
#endif