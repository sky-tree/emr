#ifndef _TEMPLETETYPEENUMERATION_H_
#define _TEMPLETETYPEENUMERATION_H_
#include "QString.h"
#include "QMap.h"
struct DataType
{
	QString name;
	bool isChecked;
};
class TempleteTypeEnumeration
{
public:
	TempleteTypeEnumeration();
	~TempleteTypeEnumeration();
	static const QString INPUT_TEXT;
	static const QString KEYWORD;
	static const QString RADIO ;
	static const QString MULTIPLE_CHOICE;
	static const QString HAVE_CHOICE;
	static const QString STRUCTURE;
	static const QString EXCEL;
	static const QString TIME;
	static const QString NUMBER_FILTER;
	static const QString PAGEDIVIDE;
	static const QString FIXTEXT;
	static const QString MENSTRUATION;
	static const QString UNFOLD_RADIO;
	static const QString UNFOLD_MULTIPLE_CHOICE;
	static const QString MACRO;
	static const QString TYPE;
	static const QString IS_DELETE;
	static const QString NAME;
	static const QString INFO;
	static const QString CODE;
	static const QString ISEDIT; 
	static const QString CODE_NAME;
	static const QString ID;
	static const QString CONTENT;
	static const QString OPTIONS;
	static const QString PADDINGLEFT;
	static const QString SIGN_BEFOR_SELECT;
	static const QString SIGN_BEFOR_NOT_SELECT;
	static const QString SPLIT_STR;
	static const QString TITLE_POSTION;
	static const QString PROPERTY_IS_CHANG;
	static const QString TITLE_DIVISABLE;
	static const QString TITLE_LEFT;
	static const QString TITLE_RIGHT;
	static const QString MACRO_VALUSE;
	static const QString MENSTRUATION_TOP;
	static const QString MENSTRUATION_BOTTOM;
	static const QString PARAGRAPH_SETTTING;
	static const QString PARAGRAPH_SET_LEFT_DIS;
	static const QString PARAGRAPH_SET_RIGHT_DIS;
	static const QString PARAGRAPH_ROW_DIS;
	static const QString PARAGRAPH_INDENT_TYPE;
	static const QString PARAGRAPH_INDENT_LEFT;

	static const QString PARAGRAPH_INDENT_TYPE_INDENT_FIRST;
	static const QString PARAGRAPH_INDENT_TYPE_INDENT_FROM_SECOND;


	//表格信号
	static const QString EXCEL_MERGER;
	static const QString EXCEL_CHOOSE_SIGN;
	static const QString EXCEL_DEVIDE_H;
	static const QString EXCEL_DEVIDE_V;
	static const QString SISTEM_SIGN;
	static const QString EXCEL_DEVIDE_H_V;
	static const QString EXCEL_ADD_ROW_OR_COL;
	static const QString EXCEL_DEL_ROW_OR_COL;
	static const QString EXCEL_PROPERTY;
	//插入特殊符号也用这个 EXCEL_SIGN 
	static const QString INPUT_PATTICULAR_SIGN;


	static const QString VALUETYPE;
	static const QString STR_TRUE;
	static const QString STR_FALSE;
	//static const QString CONTENT;
	static const QString LENGTH;
	static const QString DECIMALEN;
	static const QString DATATIME;
	static const QString TIME_FORMAT;
	static const QString NORMAL_TIME_FORMAT;
	static const QString MAXVALUE;
	static const QString MINVALUE;

	//宏id
	static const QString MACRO_NAME;
	static const QString MACRO_BIRTHDAY;
	static const QString MACRO_CITIZENSHIP;
	static const QString MACRO_BIRTH_PLACE;
	static const QString MACRO_OCCUPATION;
	static const QString MACRO_NATION;
	static const QString MACRO_EDUCATION;
	static const QString MACRO_MARITAL_STATUS;
	static const QString MACRO_MAILING_ADDRESS;
	static const QString MACRO_ZIP_CODE;
	static const QString MACRO_PHONE;
	static const QString MACRO_SEX;
	static const QString MACRO_HOME_ADDRESS;
	static const QString MACRO_HOME_ZIP_CODE;
	static const QString MACRO_CERTIFICATE_TYPE;
	static const QString MACRO_ID_NO;
	static const QString MACRO_OUTP_CARD_NO;
	//
	static const QString MACRO_INP_NO;
	static const QString MACRO_ADMISSION_DATE;
	static const QString MACRO_BED_ID;
	static const QString MACRO_DEPT_VALUE;
	static const QString MACRO_CONTACT;
	static const QString MACRO_CONTACT_PHONE;
	static const QString MACRO_CONTACT_RELATION;
	static const QString MACRO_COMPANY;
	static const QString MACRO_COMPANY_ADDR;
	static const QString MACRO_COMPANY_PHONE;
	static const QString MACRO_DIAGNOSIS;
	static const QString MACRO_CURRENT_DIAG_NAME;
	static const QString MACRO_MR_NO;
	static const QString MACRO_HOME_PHONE;
	static const QString MACRO_AGE;
	static const QString MACRO_HOSPITALIZED_TIMES;
	static const QString MACRO_PATIENT_COST_CODE;
	static const QString MACRO_FROM_CODE;
	static const QString MACRO_DOCTOR;

	static const int MACRO_NAME_ID = 11;
	static const int MACRO_BIRTHDAY_ID = MACRO_NAME_ID+1;
	static const int MACRO_CITIZENSHIP_ID = MACRO_BIRTHDAY_ID + 1;
	static const int MACRO_BIRTH_PLACE_ID = MACRO_CITIZENSHIP_ID + 1;
	static const int MACRO_OCCUPATION_ID = MACRO_BIRTH_PLACE_ID + 1;
	static const int MACRO_NATION_ID = MACRO_OCCUPATION_ID + 1;
	static const int MACRO_EDUCATION_ID = MACRO_NATION_ID + 1;
	static const int MACRO_MARITAL_STATUS_ID = MACRO_EDUCATION_ID + 1;
	static const int MACRO_MAILING_ADDRESS_ID = MACRO_MARITAL_STATUS_ID + 1;
	static const int MACRO_ZIP_CODE_ID = MACRO_MAILING_ADDRESS_ID + 1;
	static const int MACRO_PHONE_ID = MACRO_ZIP_CODE_ID + 1;
	static const int MACRO_SEX_ID = MACRO_PHONE_ID + 1;
	static const int MACRO_HOME_ADDRESS_ID = MACRO_SEX_ID + 1;
	static const int MACRO_HOME_ZIP_CODE_ID = MACRO_HOME_ADDRESS_ID + 1;
	static const int MACRO_CERTIFICATE_TYPE_ID = MACRO_HOME_ZIP_CODE_ID + 1;
	static const int MACRO_ID_NO_ID = MACRO_CERTIFICATE_TYPE_ID + 1;
	static const int MACRO_OUTP_CARD_NO_ID = MACRO_ID_NO_ID + 1;
	static const int MACRO_INP_NO_ID = MACRO_OUTP_CARD_NO_ID + 1;
	static const int MACRO_ADMISSION_DATE_ID = MACRO_INP_NO_ID + 1;
	static const int MACRO_BED_ID_ID = MACRO_ADMISSION_DATE_ID + 1;
	static const int MACRO_DEPT_VALUE_ID = MACRO_BED_ID_ID + 1;
	static const int MACRO_CONTACT_ID = MACRO_DEPT_VALUE_ID + 1;
	static const int MACRO_CONTACT_PHONE_ID = MACRO_CONTACT_ID + 1;
	static const int MACRO_CONTACT_RELATION_ID = MACRO_CONTACT_PHONE_ID + 1;
	static const int MACRO_COMPANY_ID = MACRO_CONTACT_RELATION_ID + 1;
	static const int MACRO_COMPANY_ADDR_ID = MACRO_COMPANY_ID + 1;
	static const int MACRO_COMPANY_PHONE_ID = MACRO_COMPANY_ADDR_ID + 1;
	static const int MACRO_DIAGNOSIS_ID = MACRO_COMPANY_PHONE_ID + 1;
	static const int MACRO_CURRENT_DIAG_NAME_ID = MACRO_DIAGNOSIS_ID + 1;
	static const int MACRO_MR_NO_ID = MACRO_CURRENT_DIAG_NAME_ID + 1;
	static const int MACRO_HOME_PHONE_ID = MACRO_MR_NO_ID + 1;
	static const int MACRO_AGE_ID = MACRO_HOME_PHONE_ID + 1;
	static const int MACRO_HOSPITALIZED_TIMES_ID = MACRO_AGE_ID + 1;
	static const int MACRO_PATIENT_COST_CODE_ID = MACRO_HOSPITALIZED_TIMES_ID + 1;
	static const int MACRO_FROM_CODE_ID = MACRO_PATIENT_COST_CODE_ID + 1;
	static const int MACRO_DOCTOR_ID = MACRO_FROM_CODE_ID + 1;
	
	static bool showDialog(QMap<QString, QString>* map,bool isTempleteEdit);
	static int getMacroIdBaseIdString(QString str);
};
#endif
