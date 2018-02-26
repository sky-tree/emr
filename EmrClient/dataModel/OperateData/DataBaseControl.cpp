#include "dataModel/OperateData/DataBaseControl.h"

#include "commonWidgets/TabWidgetCollection.h"
#include "EMR/entrance/MyAppEntrance.h"
#include "EMR/output/HtmlDocument.h"
#include "AuthorityConstants.h"
#include<QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include<QTextCodec>
#include<QFileDialog>
#include <QDateTime>
#include "CompressTool.h"
DocutorData* DataBaseControl::m_docutorData = NULL;
PatientData* DataBaseControl::m_patientData = NULL;
QMap<QString,operatorMedicalRight>* DataBaseControl::m_operatorMedical = NULL;//权限 CLASSIFYCODE  增删改
QMap<QString,operatorTemplateRight>* DataBaseControl::m_operatorTemplate = NULL;//权限 个人/科室/全院（公共）模板
QSqlQueryModel* DataBaseControl::m_QSqlQueryModel = NULL;
QSqlTableModel* DataBaseControl::m_QSqlTableModel = NULL;
QSqlTableModel* DataBaseControl::m_QSqlSpecialTableModel = NULL;
DBManager* DataBaseControl::m_dbManager = NULL;


DataBaseControl::DataBaseControl(DBManager *pManager) 
{
	//m_dbManager = pManager;
	m_QSqlTableModelTest = NULL;
	if (m_dbManager==NULL)
	{
		m_dbManager = new DBManager();
		m_dbManager->connectDataBase();
	}
	
}

DataBaseControl::~DataBaseControl()
{
	m_database.close();
	if (m_QSqlTableModel!=NULL)
	{
	    //delete m_QSqlTableModel;
		m_QSqlTableModel = NULL;
	}
	if (m_QSqlQueryModel != NULL)
	{
		//delete m_QSqlQueryModel;
		m_QSqlQueryModel = NULL;
	}
	//if (m_dbManager)
	//{
	//	delete m_dbManager;
	//	m_dbManager = NULL;
	//}
	//if (m_docutorData)

	//{
	//	delete m_docutorData;
	//	m_docutorData = NULL;
	//}
	//if (m_operatorMedical)
	//{
	//	delete m_operatorMedical;
	//	m_operatorMedical = NULL;
	//}
	//if (m_patientData)
	//{
	//	delete m_patientData;
	//	m_patientData = NULL;
	//}
	//if (m_operatorTemplate)
	//{
	//	delete m_operatorTemplate;
	//	m_operatorTemplate = NULL;
	//}
}

//list:"93064§004§14§false§0§true";
void DataBaseControl::initData(QStringList list, WriteAuthority& wrtAth, QMap<QString, MdcAuthority>& stlMdcAth)
{
	QString inpNo = "", doctorID = "", doctorCode = "", isSuperUser = "", flags = "", isPhysician = "";
	if (list.size() >= 5)
	{
		inpNo = list[0];
		doctorID = list[1];
		doctorCode = list[2];
		isSuperUser = list[3];
		flags = list[4];
		isPhysician = list[5];
		QString test = inpNo + "   " + doctorID + "   " + doctorCode + "    " + isSuperUser + "    " + flags + "   " + isPhysician;
		//QMessageBox::about(NULL, "提示", test);
	}
	//获取医生的相关信息
	initDocutorData(doctorCode, doctorID, isSuperUser, isPhysician);
	//获取病人的相关信息
	initPatientData(inpNo);
	//初始化病历的权限
	initMedicalRight();
	//初始化模板的权限
	initTemplateRight();
	//初始化权限
	initAuthority(inpNo, wrtAth, stlMdcAth);

}
void DataBaseControl::initDocutorData(QString doctorCode, QString doctorID, QString isSuperUser, QString Physician)
{
	//门诊医生无权限,添加部门为住院的科室，人员性质为医生?
	QString sql = "SELECT A.*, B.*FROM user_info A \
				  	INNER JOIN USER_DEPT_REL C \
					ON C.USER_ID = A.USER_ID \
					INNER JOIN DEPT_LIST B \
					ON B.dept_code = C.DEPT_CODE \
					WHERE   A.USER_ID = '" + doctorID + "' and  C.DEPT_CODE = '" + doctorCode + "'";
	QSqlQuery query(m_database);
	query.exec(sql);
	if (m_docutorData == NULL)
	{
		m_docutorData = new DocutorData();
		m_docutorData->user_ser_code = 0;
		m_docutorData->user_id = doctorID;
		m_docutorData->dept_code = doctorCode;
		m_docutorData->isPhysician = Physician;
		if (isSuperUser == "false")
		{
			m_docutorData->isSupperUser = false;
		}
		else
		{
			m_docutorData->isSupperUser = true;
		}
	}
	while (query.next())
	{
		//m_docutorData->user_id = doctorID;
		m_docutorData->user_name = query.value("USER_NAME").toString();
		m_docutorData->dept_code = query.value("DEPT_CODE").toString();
		m_docutorData->dept_name = query.value("DEPT_NAME").toString();
		m_docutorData->dept_No = query.value("DEPT_NO").toString();
	//	m_docutorData->isPhysician = Physician;
		break;
	}
	//测试使用数据
	if (m_docutorData->user_id == "0001")
	{
		m_docutorData->isSupperUser = true;
	}
	query.finish();
	sql = "select * from USER_SERV_REL t where t.user_id = '" + doctorID + "'";
	query.exec(sql);
	while (query.next())
	{
		QString serCode = query.value("USER_SERV_CODE").toString();
		if (serCode == "03" || serCode == "28")
		{
			m_docutorData->user_ser_code |= 0x0001;
		}
		else if (serCode == "04" || serCode == "05")
		{
			m_docutorData->user_ser_code |= 0x0010;
		}
	}
	query.finish();
}
void DataBaseControl::initPatientData(QString inpNo)
{
	//A.ID_TYPE 身份证件类型
	QString sql1 = "select A.*,B.VALUE_NAME CERTIFICATE_VALUE,C.VALUE_NAME SEX_VALUE,\
				   	D.VALUE_NAME CITIZENSHIP_VALUE, E.VALUE_NAME OCCUPATION_VALUE,\
					F.VALUE_NAME NATION_VALUE, G.VALUE_NAME EDUCATION_VALUE,\
					H.VALUE_NAME MARITAL_VALUE, I.VaLUE_NAME RELATIONSHIP_VALUE, J.DEPT_NAME DEPT_VALUE,\
					K.NAME COST_FROM,L.VALUE_NAME FORM_CODE,M.USER_NAME DOCTOR_NAME\
					from inp_registration A\
					LEFT JOIN PUBLIC_DICT B ON A.ID_TYPE = B.VALUE_ID AND B.DICT_NAME LIKE '%CERTIFICATE_TYPE_DICT%'\
					LEFT JOIN PUBLIC_DICT C ON A.SEX = C.VALUE_ID AND C.DICT_NAME LIKE '%SEX_DICT%'\
					LEFT JOIN PUBLIC_DICT D ON A.NATIONALITY_CODE = D.VALUE_ID AND D.DICT_NAME LIKE '%COUNTRY_DICT%'\
					LEFT JOIN PUBLIC_DICT E ON A.OCCUPATION_CODE = E.VALUE_ID AND E.DICT_NAME LIKE '%OCCUPATION_DICT%'\
					LEFT JOIN PUBLIC_DICT F ON A.NATION_code = F.VALUE_ID AND F.DICT_NAME LIKE '%NATION_DICT%'\
					LEFT JOIN PUBLIC_DICT G ON A.DEGREE_CODE = G.VALUE_ID AND G.DICT_NAME LIKE '%EDUCATION_DICT%'\
					LEFT JOIN PUBLIC_DICT H ON A.MARITAL_STATUS_CODE = H.VALUE_ID AND H.DICT_NAME LIKE '%MARITAL_STATUS_DICT%'\
					LEFT JOIN PUBLIC_DICT I ON A.Contact_Relationship_Code = I.VALUE_ID AND I.DICT_NAME LIKE'%CONTACT_RELATION_DICT%'\
					LEFT JOIN PUBLIC_DICT L ON A.FROM_CODE=L.VALUE_ID AND L.DICT_NAME LIKE'%INPATIENT_SOURCE%'\
					LEFT JOIN DEPT_LIST   J ON A.Dept_Code = J.DEPT_CODE\
					LEFT JOIN INP_COST_CATEGORY K ON A.PATIENT_COST_CODE=K.CODE\
					LEFT JOIN USER_INFO M  ON M.USER_ID=A.DOCTOR\
					where INP_NO='" + inpNo + "'";
	QSqlQuery query(m_database);
	query.exec(sql1);
	if (m_patientData == NULL)
	{
		m_patientData = new PatientData();
	}
	while (query.next())
	{
		m_patientData->id = query.value("PATIENT_ID").toString();;
		m_patientData->outp_card_no = query.value("OUTP_CARD_NO").toString();
		m_patientData->name = query.value("NAME").toString();
		m_patientData->sex = query.value("SEX_VALUE").toString();
		m_patientData->phone = query.value("PHONE_NUMBER").toString();
		m_patientData->birthday = query.value("BIRTHDATE").toString();
		m_patientData->certificate_Type = query.value("CERTIFICATE_VALUE").toString();
		m_patientData->id_No = query.value("ID_NO").toString();
		m_patientData->education = query.value("EDUCATION_VALUE").toString();
		m_patientData->nation = query.value("NATION_VALUE").toString();
		m_patientData->citizenship = query.value("CITIZENSHIP_VALUE").toString();
		m_patientData->marital_status = query.value("MARITAL_VALUE").toString();
		m_patientData->home_address = query.value("ADDR").toString();
		m_patientData->home_phone = query.value("HOME_PHONE").toString();
		//m_patientData->birth_place = query.value("BIRTH_PLACE").toString();
		//	m_patientData->home_zip_code = query.value("HOME_ZIP_CODE").toString();
		m_patientData->occupation = query.value("OCCUPATION_VALUE").toString();
		//	m_patientData->mailing_address = query.value("MAILING_ADDRESS").toString();
		//	m_patientData->zip_code = query.value("ZIP_CODE").toString();
		m_patientData->inp_NO = query.value("INP_NO").toString();
		m_patientData->admission_date = query.value("ADMISSION_DATE").toString();
		m_patientData->bed_id = query.value("BED_ID").toString();
		m_patientData->dept_Name = query.value("DEPT_VALUE").toString();
		m_patientData->contact = query.value("CONTACT").toString();
		m_patientData->contact_phone = query.value("CONTACT_PHONE").toString();
		m_patientData->contact_relation = query.value("RELATIONSHIP_VALUE").toString();
		m_patientData->company = query.value("COMPANY").toString();
		m_patientData->company_addr = query.value("COMPANY_ADDR").toString();
		m_patientData->company_phone = query.value("COMPANY_PHONE").toString();
		m_patientData->diagnosis = query.value("REG_DIAGNOSIS").toString();
		m_patientData->current_diag_name = query.value("DIAGNOSIS").toString();
		m_patientData->mr_NO = query.value("MR_NO").toString();
		m_patientData->dept_CODE = query.value("DEPT_CODE").toString();
       //登记时间
		QString operTime = query.value("OPER_TIME").toString();
		m_patientData->age = getAge(m_patientData->birthday, operTime);
		//2016 - 06 - 19T00:00 : 00
		//m_patientData->age = getAge("1989-08-31T01:00:00", "2016-08-30T13:00:00");		
		m_patientData->nurse = query.value("NURSE").toString();
		m_patientData->doctor = query.value("DOCTOR").toString();
		m_patientData->doctorName = query.value("DOCTOR_NAME").toString();
		m_patientData->attending_Physician = query.value("ATTENDING_PHYSICIAN").toString();
		m_patientData->chief_Physician = query.value("CHIEF_PHYSICIAN").toString();
		m_patientData->birth_place = query.value("NATIVE_PLACE_CODE_NAME").toString();
		m_patientData->patiemt_Cost_Code = query.value("COST_FROM").toString();
		m_patientData->from_Code = query.value("FORM_CODE").toString();

		//住院次数
		QString hospitalcounts = "SELECT COUNT(*) HOSPITAL_COUNTS FROM INP_REGISTRATION WHERE PATIENT_ID ='" + m_patientData->id + "'";
		query.exec(hospitalcounts);
		while (query.next())
		{
			m_patientData->hopspitalized_Times = query.value("HOSPITAL_COUNTS").toString();
		}
		//床位
		//QString sqlBedName = QString("select NAME from INP_SICKBED where ID ='%1' ").arg(m_patientData->bed_id);
		QString sqlBedName = QString("select NAME FROM(SELECT T1.NAME FROM INP_TRANSFER T left join inp_sickbed T1 ON T.IN_BED_ID = T1.ID WHERE T.INP_NO = '%1' AND IN_BED_ID IS NOT NULL ORDER BY T.IN_OPER_TIME DESC ) WHERE ROWNUM = 1").arg(m_patientData->inp_NO);
		query.exec(sqlBedName);
		while (query.next())
		{
			m_patientData->bedName = query.value("NAME").toString();
		}
		break;
	}
	query.finish();

}
void DataBaseControl::initAuthority(QString InpNo, WriteAuthority& wrtAth, QMap<QString, MdcAuthority>& stlMdcAth)
{
	QString sql_ = "select * from INP_EMR_ROLE_DOCTOR t where t.inp_no = '"+ InpNo + "'";
	QString strSql_, userName_;
	QSqlQueryModel& qModel_ = getQueryData(sql_);
	int count_ = qModel_.rowCount();
	
	QString doctorId_, roleCode_;
	int nRole;
	bool ok_;
	for(int i = 0; i < count_; i++)
	{
		QSqlRecord sqlRecord_ = qModel_.record(i);
		roleCode_ = sqlRecord_.value("role_code").toString();
		doctorId_ = sqlRecord_.value("doctor").toString();
		nRole = roleCode_.toInt(&ok_, 10);
		strSql_ = "select t.user_name from USER_INFO t where t.user_id = '" + doctorId_ + "'";
		QSqlQuery query(m_database);
		query.exec(strSql_);
		while (query.next())
		{
			userName_ = query.value("user_name").toString();
		}
		query.finish();
		switch (nRole)
		{
		case 1:
			wrtAth.MdcDctr = doctorId_;
			wrtAth.MdcDctrName = userName_;
			break;
		case 2:
			wrtAth.PhyDctr = doctorId_;
			wrtAth.PhyDctrName = userName_;
			break;
		case 3:
			wrtAth.RhtDctr = doctorId_;
			wrtAth.RhtDctrName = userName_;
			break;
		case 4:
			wrtAth.ChfDctr = doctorId_;
			wrtAth.ChfDctrName = userName_;
			break;
		case 5:
			wrtAth.SgnDctr = doctorId_;
			wrtAth.SgnDctrName = userName_;
			break;
		case 6:
			wrtAth.AhgDctr = doctorId_;
			wrtAth.AhgDctrName = userName_;
			break;
		case 7:
			wrtAth.CnlDctr = doctorId_;
			wrtAth.CnlDctrName = userName_;
			break;
		case 8:
			wrtAth.IcuDctr = doctorId_;
			wrtAth.IcuDctrName = userName_;
			break;
		case 9:
			wrtAth.IptNurse = doctorId_;
			wrtAth.IptNurseName = userName_;
			break;
		case 10:
			wrtAth.TrhDctr = doctorId_;
			wrtAth.TrhDctrName = userName_;
			break;
		default:
			break;
		}
	}
	delete &qModel_;

	sql_ = "select t.isoptions,t.name,t.id from MEDICALRECORDS_LIST t where t.inp_no = '"+ InpNo + "'";
	QSqlQuery query(m_database);
	query.exec(sql_);
	while (query.next())
	{
		MdcAuthority mdcTh_;
		mdcTh_.mdcId = query.value("id").toString();
		mdcTh_.mdcName = query.value("name").toString();
		mdcTh_.mdcAuthor = query.value("isoptions").toString();
		stlMdcAth.insert(mdcTh_.mdcId, mdcTh_);
	}
	query.finish();
}


bool DataBaseControl::is_Leap(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}
int DataBaseControl::sum_Day(int month)
{
	int day_tab[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return day_tab[month];
}
//pBirthday:出生日期
//pAdminData:登记日期
QString DataBaseControl::getAge(QString pBirthday, QString pAdminDate)
{
	if (pAdminDate.length() == 0 || pBirthday.length() == 0)
	{
		return 0;
	}
	//日期格式:2016-06-19T00:00:00
	QString admYear = pAdminDate.mid(0, 4);
	QString admMonth = pAdminDate.mid(5, 2);
	QString admDay = pAdminDate.mid(8, 2);
	QString admHour = pAdminDate.mid(11, 2);

	QString birYear = pBirthday.mid(0, 4);
	QString birMonth = pBirthday.mid(5, 2);
	QString birDay = pBirthday.mid(8, 2);
	QString birHour = pBirthday.mid(11, 2);

	int age = 0, month = 0, day = 0, hour = 0;
	age= admYear.toInt() - birYear.toInt();
	month = admMonth.toInt() - birMonth.toInt();
	day = admDay.toInt() - birDay.toInt();
	hour = admHour.toInt() - birHour.toInt();
	QString strAge = "岁";
	QString strMonth = "个月";
	QString strDay = "天";
	QString strHour = "小时";
	QString strYou = "又";
	//age>1;表明肯定不是新生儿童
	if (age>1)
	{
		if (month > 0){ return  QString::number(age) + strAge;}
		else if (month==0)
		{
			if (day >= 0){ return  QString::number(age) + strAge; }
			else{ return  QString::number(age-1) + strAge; }
		}
		else
		{
			return  QString::number(age - 1) + strAge;
		}
	}else if (age==1)//2015年12月出生。2016年1月登记，其实真实的可能有或没有一岁，根据月和天去判断，所以得特殊处理
	{
		//5月，8月,8-5>0
		if (month > 0) { return QString::number(age) + strAge; }
		else if (month == 0) //月份相同
		{
			if (day >= 0){ return  QString::number(age) + strAge; }//登记日期16，出生日期15；
			else //岁数退一位，月数变成11
			{ 
				QString str = "11个月" + strYou;
				int iDay = 0;
				iDay = sum_Day(admMonth.toInt());//获取当月的天数
				if (admMonth.toInt() == 2 && is_Leap(admYear.toInt()))
				{
					 iDay++;
				}
				QString returnValue = str+QString::number(iDay + day)+strDay;
				return  returnValue;
			}
		}
		else
		{
			int iMonth = 12 + month;
			if (day > 0) //11月15号，1月16号
			{
				QString returnValue = QString::number(iMonth) + strMonth + strYou + QString::number(day) + strDay;
				return returnValue;
			}
			else if (day == 0)//11月15号，1月15号
			{
				if (hour>=0)
				{
					return QString::number(iMonth) + strMonth;
				}
				else
				{
					iMonth--;
					int iDay = 0;
					iDay = sum_Day(admMonth.toInt());//获取当月的天数
					if (admMonth.toInt() == 2 && is_Leap(admYear.toInt()))
					{
						iDay++;
					}
					QString returnValue = QString::number(iMonth) + strMonth + strYou + QString::number(iDay - 1) + strDay;
					return returnValue;
				}
			}
			else if (day < 0)//11月15号，1月14号
			{
				iMonth--;
				int iDay = 0;
				iDay = sum_Day(admMonth.toInt());//获取当月的天数
				if (admMonth.toInt() == 2 && is_Leap(admYear.toInt()))
				{
					iDay++;
				}
				int returnDay = iDay + day;
				QString returnValue = "";
				if (iMonth>0)
				{
					returnValue = QString::number(iMonth) + strMonth + strYou + QString::number(returnDay) + strDay;
					return returnValue;
				}
				//当月数小于0，例如,2015年12月，2016年1月
				else
				{
					if (hour<0)
					{
						returnDay--;
						hour = 24 + hour;
						
					}
					if (returnDay<=0)
					{
						returnValue = QString::number(hour) + strHour;
					}
					else
					{
						returnValue = QString::number(returnDay) + strDay + strYou+QString::number(hour) + strHour;
					}
					
					return returnValue;	
				}
				
			}
		}
	}   
	else if (age==0) //表明当年出生的;
	{
		if (month>0) //1月出生，2月登记 
		{
			//1月1号，2月2号
			if (day>0){ return QString::number(month) + strMonth + strYou + QString::number(day) + strDay; }
			else if (day==0)//1月1号，2月1号
			{
				//1月1号11点，2月1号12点
				if (hour > 0){ return QString::number(month) + strMonth + strYou + QString::number(day) + strDay; }
				//1月1号12点，2月1号12点
				else if (hour == 0){ return QString::number(month) + strMonth; }
				//1月1号12点，2月1号11点
				else
				{
					month--;
					int iDay = 0;
					iDay = sum_Day(birMonth.toInt());//获取当月的天数
					if (birMonth.toInt() == 2 && is_Leap(birYear.toInt()))
					{
						iDay++;
					}
					int iRestDay = iDay - birDay.toInt() + admDay.toInt();
					if (month==0)//表示月数是相邻的
					{
						if (hour >= 0)
						{
							return QString::number(iRestDay) + strDay + strYou + QString::number(hour);
						}
						else
						{
							iRestDay--;
							if (iRestDay > 0){ return QString::number(iRestDay) + strDay + strYou + QString::number(hour) + strHour; }
							else{ return QString::number(hour) + strHour; }
						}
					}										
				}					
			}
		}
		else //表明当月出生
		{	
			QString returnValue = "";
			if (day > 0)
			{
				if (hour >= 0){ returnValue = QString::number(day) + strDay + strYou + QString::number(hour) + strHour; }
				else
				{
					day--;
					hour = 24 + hour;
					if (day == 0){ returnValue = QString::number(hour) + strHour; }
					else if (day > 0){ returnValue = QString::number(day) + strDay + strYou + QString::number(hour) + strHour; }
				}
				return returnValue;
			}
			else if (day == 0)
			{
				if (hour == 0){ hour = 1; }
				returnValue = QString::number(hour) + strHour;
			}
			return returnValue;
		}
	}		
	return "";
}
//初始化模板权限
void DataBaseControl::initTemplateRight()
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	if (m_operatorTemplate==NULL)
	{
		m_operatorTemplate = new QMap<QString, operatorTemplateRight>();
	}
	QString user_Id = m_docutorData->user_id;
	QSqlQuery query(m_database);
	QString sql = "select * from inp_emr_tmpl_func where EMP_ID='" + user_Id + "'";
	query.exec(sql);
	while (query.next())
	{
		QString temType = query.value("TMPL_TYPE").toString();
		QString funcCode = query.value("FUNC_CODE").toString();
		initTemplateRightData(temType, funcCode);
	}
	query.finish();

}
//初始化病历权限
#define  CREATESPECIALRGIHT() \
	strSql = "select * from INP_EMR_ROLE_FUNC where ROLE_CODE='" + strCode + "'"; \
 initSpecialRight(strSql);

void DataBaseControl::initMedicalRight()
{
	QString inp_No= m_patientData->inp_NO;
	QString user_Id = m_docutorData->user_id;
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QSqlQuery queryRoleCode(m_database);
	QSqlQuery  queryData(m_database);
	if (m_operatorMedical==NULL)
	{
		m_operatorMedical = new QMap<QString, operatorMedicalRight>();
	}
	QString sqlData = "";
	QString sqlRoleCode = "select ROLE_CODE from INP_EMR_ROLE_DOCTOR Where INP_NO ='" + inp_No + "' and DOCTOR='" + user_Id + "'";
	queryRoleCode.exec(sqlRoleCode);
	while (queryRoleCode.next())
	{
		//role_Code,角色编码,病历医生，主管医生，主治医生........
		QString role_Code = queryRoleCode.value("ROLE_CODE").toString();
		sqlData = "select * from INP_EMR_ROLE_FUNC where ROLE_CODE='" + role_Code + "'";
		queryData.exec(sqlData);
		while (queryData.next())
		{
			QString cateGory_Code = queryData.value("CATEGORY_CODE").toString();
			QString func_Code = queryData.value("FUNC_CODE").toString();
			//病历类别,功能编码
			initMedicalRightData(cateGory_Code, func_Code);
		}
	}

	//主管医生，主治医生，护士，特殊处理
	QString strCode = "";
	QString strSql = "";
	if (user_Id==m_patientData->nurse)
	{
		strCode = "09";
		CREATESPECIALRGIHT()
	}
	if (user_Id==m_patientData->doctor)
	{
		strCode = "02";
		CREATESPECIALRGIHT()
	}
	if (user_Id==m_patientData->attending_Physician)
	{
		strCode = "03";
		CREATESPECIALRGIHT()
	}
	if (user_Id == m_patientData->chief_Physician)
	{
		strCode = "04";
		CREATESPECIALRGIHT()
	}
	//会诊医生又特殊处理
	//'2016110045' 测试数据
	//QString strInp_Np = m_patientData->inp_NO;
	QString sqlApply = "select A.* FROM inp_consultation_apply_detail A \
	LEFT JOIN inp_consultation_apply B ON B.NO = A.Apply_No \
	WHERE B.Is_Cancel = '0' AND A.Is_Cancel = '0' AND \
		A.Is_Arrange_Complete = '1' AND  B.INP_NO = '" + m_patientData->inp_NO + "' AND A.BY_ARRANGE_USER='" + m_docutorData->user_id + "'";
	//A.BY_ARRANGE_USER张磊修改之前为A.ARRANGE_USER
	initApplyDoctorRight(sqlApply);


	QString sqlOperationApply = "select * from INP_OPERATION_APPLY WHERE INP_NO='" + m_patientData->inp_NO + "'";
	queryData.exec(sqlOperationApply);
	while (queryData.next())
	{
		QString surgeonId = queryData.value("SURGEON").toString();
		//手术医生
		if (surgeonId==m_docutorData->user_id)
		{
			strCode = "05";
			CREATESPECIALRGIHT()
				continue;
		}
		surgeonId = queryData.value("ANESTHESIA_DOCTOR").toString();
		//麻醉医生
		if (surgeonId == m_docutorData->user_id)
		{
			strCode = "06";
			CREATESPECIALRGIHT()
			continue;
		}
	}


	queryData.finish();
	queryRoleCode.finish();
	//query.exec(sql1);
}
void DataBaseControl::initApplyDoctorRight(QString sql)
{
	QSqlQuery query(m_database);
	query.exec(sql);
	QString strCode = "";
	QString strSql = "";
	while (query.next())
	{
		strCode = "07";
		CREATESPECIALRGIHT()
	}
	query.finish();   
}
//病历类别,功能编码
void DataBaseControl::initSpecialRight(QString sql)
{
	QSqlQuery query(m_database);
	query.exec(sql);
	while (query.next())
	{
		QString cateGory_Code = query.value("CATEGORY_CODE").toString();
		QString func_Code = query.value("FUNC_CODE").toString();
		initMedicalRightData(cateGory_Code, func_Code);
	}
	query.finish();
}
inline void DataBaseControl::initTemplateRightData(QString temType, QString func_Code)
{
	operatorTemplateRight *pOMR = NULL;
	if (m_operatorTemplate->contains(temType))
	{
		pOMR = &(*m_operatorTemplate)[temType];
		if (func_Code == "01")
		{
			pOMR->addTemplate = true;
		}
		else if (func_Code == "02"){ pOMR->modifyTemplate = true; }
		else if (func_Code == "03"){ pOMR->deleteTemplate = true; }
		else if (func_Code == "04" || func_Code == "05"){ pOMR->startUpTemplate = true; }

	}
	else
	{
		operatorTemplateRight oTR;
		if (func_Code == "01")
		{
			oTR.addTemplate = true;
		}
		else if (func_Code == "02"){ oTR.modifyTemplate = true; }
		else if (func_Code == "03"){ oTR.deleteTemplate = true; }
		else if (func_Code == "04" || func_Code == "05"){ oTR.startUpTemplate = true; }
		m_operatorTemplate->insert(temType, oTR);
	}
}
inline void DataBaseControl::initMedicalRightData(QString cateGory_Code, QString func_Code)
{
	operatorMedicalRight *pOMR = NULL;
	if (m_operatorMedical->contains(cateGory_Code))
	{
		pOMR = &(*m_operatorMedical)[cateGory_Code];
		if (func_Code == "01")
		{
			pOMR->addTemplate = true;
		}
		else if (func_Code == "02")
		{
			pOMR->modifyTemplate = true;
		}
		else if (func_Code == "03")
		{
			pOMR->deleteTemplate = true;
		}

	}
	else
	{
		operatorMedicalRight oMR;
		if (func_Code == "01")
		{
			oMR.addTemplate = true;
		}
		else if (func_Code == "02")
		{
			oMR.modifyTemplate = true;
		}
		else if (func_Code == "03")
		{
			oMR.deleteTemplate = true;
		}
		m_operatorMedical->insert(cateGory_Code, oMR);
	}

}
void DataBaseControl::InitDataBaseName(const QString pDataBaseName, QString pdataBase, QString pUserName, QString pPassWord)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	m_database = QSqlDatabase::addDatabase(pdataBase);
	m_database.setDatabaseName(pDataBaseName);
	if (pUserName.isEmpty() == false && pPassWord.isEmpty() == false)
	{
		m_database.setUserName(pUserName);
		m_database.setPassword(pPassWord);
	}
	if (!m_database.open())
	{
		qDebug() << "open m_database failed!";
	}
}

/*!
pSql:SQL查询语句
*/
QSqlQueryModel& DataBaseControl::getQueryData(QString pSql)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	//if (m_QSqlQueryModel==NULL)
	//{
		m_QSqlQueryModel = new QSqlQueryModel;
	//}
	//m_QSqlQueryModel.clear();
	m_QSqlQueryModel->setQuery(pSql);
	while (m_QSqlQueryModel->canFetchMore())
	{
		m_QSqlQueryModel->fetchMore();
	}
	return *m_QSqlQueryModel;
}
QSqlTableModel* DataBaseControl::getTableModel(QString pSql, QString pTableName)
{

	//m_QSqlTableModel.clear();
	//m_QSqlTableModel.clear();
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	//在外部进行了删除，特殊问题，违背了谁new谁delete的原则,后续找到问题再修改
	if (m_QSqlTableModel == NULL)
	{
		m_QSqlTableModel = new QSqlTableModel;
		//delete m_QSqlTableModel;
	}
	else
	{
		//int i = m_QSqlTableModel->rowCount();
		//m_QSqlTableModel->clear();
		//delete m_QSqlTableModel;
		m_QSqlTableModel = NULL;
		m_QSqlTableModel = new QSqlTableModel;
	}
	//m_QSqlTableModel->clear();
	if (pSql == "1")
	{
		/*if (m_QSqlSpecialTableModel == NULL)
		{
			m_QSqlSpecialTableModel = new QSqlTableModel;
		}*/
	}
	m_QSqlTableModel->setTable(pTableName);
	while (m_QSqlTableModel->canFetchMore())
	{
		m_QSqlTableModel->fetchMore();
	}
	return m_QSqlTableModel;
}
QByteArray DataBaseControl::getMedicalData(QString PID, bool isTemplate)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QByteArray byteArray;
	QSqlQuery query(m_database);
	QString sql = "select DATA from medicalrecords_data where PID='" + PID + "'";
	QString sql1 = "select DATA from medicalrecords_template_data where PID='" + PID + "'";
	if (isTemplate == false)
	{
		//query.prepare(sql);
		query.exec(sql);
	}
	else
	{
		//query.prepare(sql1);
		query.exec(sql1);
	}

	while (query.next())
	{
		QByteArray	sqlbyteArray = query.value("DATA").toByteArray();
		return sqlbyteArray;
	}
	query.finish();
	return byteArray;
}

QByteArray DataBaseControl::getMedicalData(QString PID, bool isTemplate, bool& isCompress)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QByteArray byteArray;
	QSqlQuery query(m_database);
	QString sql = "select DATA,is_compress from medicalrecords_data where PID='" + PID + "'";
//	QString sql = "select DATA,HTML_DATA,is_compress from medicalrecords_data where PID='" + PID + "'";
	QString sql1 = "select DATA,is_compress from medicalrecords_template_data where PID='" + PID + "'";
	if (isTemplate == false)
	{
		//query.prepare(sql);
		query.exec(sql);
	}
	else
	{
		//query.prepare(sql1);
		query.exec(sql1);
	}

	while (query.next())
	{
		QString strIsComperss_ = query.value("is_compress").toByteArray();
		if (strIsComperss_ == "0")
		{
			isCompress = false;
		}
		else
		{
			isCompress = true;
		}
		byteArray = query.value("DATA").toByteArray();
		//byteHtmlArray = query.value("HTML_DATA").toByteArray();
	}
	query.finish();
// 	QByteArray desData1;
// 	CompressTool::extractByteArray(byteHtmlArray, desData1);
// 	QFile qFile_("test.html");
// 	if (qFile_.open(QIODevice::WriteOnly))
// 	{
// 		QDataStream out(&qFile_);
// 		out << desData1;
// 	}
// 	qFile_.close();
	return byteArray;
}
void DataBaseControl::updateMedicalData(QByteArray &byteArray, QString PID, bool isTemplate)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QSqlQuery query(m_database);
	QString pid = PID;
	HTMLDocument * doc = NULL;
	if (isTemplate == false)
	{
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL) qDebug() << "The Current Page is NULL";
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		//doc = HTMLDocument::create(appEntrance);

		doc = HTMLDocument::createAndCompressed(appEntrance);

		QByteArray desData1;
		CompressTool::extractByteArray(doc->data, desData1);
		QString htmlStr1 = QString(desData1);
		query.prepare("update  medicalrecords_data set IS_COMPRESS=?,HTML_DATA=?,DATA=? where PID='" + pid + "'");
		query.addBindValue(QString("1"));
		query.addBindValue(doc->data);
		query.addBindValue(byteArray);
	}
	else
	{
		query.prepare("update  medicalrecords_template_data set IS_COMPRESS=?,DATA=? where PID='" + pid + "'");
		query.addBindValue(QString("1"));
		query.addBindValue(byteArray);
	}
	bool bRtn = query.exec();
	if (!bRtn)
	{
		qDebug() << query.lastError().text();
	}
	query.finish();

	if (doc != NULL)
	{
		delete doc;
	}
}

bool DataBaseControl::insertData(QString pSql)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QSqlQuery query(m_database);
	bool ret = query.exec(pSql);
	if (!ret)
	{
		QString strMsg_ = query.lastError().text();
		char* msg_ = strMsg_.toLocal8Bit().data();
		qDebug(msg_);
	}
	query.finish();
	return ret;
}
QString DataBaseControl::getId(QString pSql)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QString id = "";
	QSqlQuery query(m_database);
	query.exec(pSql);
	while (query.next())
	{
		id = query.value(0).toString();
	}
	return id;
}
void DataBaseControl::deleteMedicalData(QString ID, bool isTemplate, bool isCourseDisease, bool isDeleteData)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QSqlQuery query(m_database);
	bool deleteList = false, deleteData = false;
	if (isTemplate == false)
	{
		// 病程记录
		if (isCourseDisease == true)
		{

			if (isDeleteData == true)//确定删除的是树节点的数据，还是病历的数据,
			{
				deleteData = query.exec("delete from medicalrecords_data where PID='" + ID + "'");
			}
			else
			{
				deleteList = query.exec("delete from medicalrecords_list where ID='" + ID + "'");
			}
		}
		else
		{
			deleteList = query.exec("delete from medicalrecords_list where ID='" + ID + "'");
			deleteData = query.exec("delete from medicalrecords_data where PID='" + ID + "'");
		}
	}
	else
	{
		deleteData = query.exec("delete from medicalrecords_template_data where PID='" + ID + "'");
		deleteList = query.exec("delete from medicalrecords_template_list where ID='" + ID + "'");
	}
	query.finish();
}
void DataBaseControl::insertMedicalData(QByteArray &byteArray, QString ID, QString PID, bool isTemplate)
{
	if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	QSqlQuery query(m_database);
	QString id = ID;
	QString PId = PID;
	HTMLDocument * doc = NULL;
	if (isTemplate == false)
	{
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL) qDebug() << "The Current Page is NULL";
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		//doc = HTMLDocument::create(appEntrance);
		doc = HTMLDocument::createAndCompressed(appEntrance);

		query.prepare("insert into  medicalrecords_data(ID,PID,IS_COMPRESS,HTML_DATA,DATA) values (?,?,?,?,?)");
		query.addBindValue(id);
		query.addBindValue(PId);
		query.addBindValue(QString("1"));
		query.addBindValue(doc->data);
		query.addBindValue(byteArray);
	}
	else
	{
		query.prepare("insert into  medicalrecords_template_data(ID,PID,IS_COMPRESS,DATA) values (?,?,?,?)");
		query.addBindValue(id);
		query.addBindValue(PId);
		query.addBindValue(QString("1"));
		query.addBindValue(byteArray);
	}

	bool bRtn = query.exec();
	if (!bRtn)
	{
	}

	if (doc != NULL)
	{
		delete doc;
	}

	/*QString sql1 = "select * from medicalrecords_data";
	query.exec(sql1);
	if (query.next())
	{
	QByteArray test1 = query.value("DATA").toByteArray();
	}*/
	query.finish();
}
/*保存数据到数据库*/
void DataBaseControl::saveDataToDataBase()
{
	/*if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	m_QSqlTableModel.database().transaction();
	if (m_QSqlTableModel.submitAll())
	{
		m_QSqlTableModel.database().commit();
	}
	else
	{
		m_QSqlTableModel.database().rollback();
	}*/
}
/*撤销更改*/
void DataBaseControl::rollData()
{
	/*if (m_dbManager != NULL)
	{
		m_dbManager->connectDataBase();
	}
	m_QSqlTableModel.database().transaction();

	m_QSqlTableModel.revertAll();*/
}
//测试，下周继续完成此类
void DataBaseControl::testModel()
{
	//model->setHeaderData(0, Qt::Horizontal, tr("id"));
	//model->setHeaderData(1, Qt::Horizontal, tr("name"));
	//QTableView *view = new QTableView;
	//view->setModel(model);  //重新定义模型，model直接从database.db的数据库中插入数据
	//view->show();
}

QString DataBaseControl::getDataBasetime()
{
	QString DataBasetTimeSql="select to_char(sysdate,'yyyy-mm-dd hh:mm:ss')AS systemTime from dual";
	QSqlQuery query(m_database);
	query.exec(DataBasetTimeSql);
	QString returnTime=query.value("SYSTEMTIME").toString();
	query.finish();
	return returnTime;
}
