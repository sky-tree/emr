/*********************************************************************************
*Copyright(C),Shine
*FileName:  customComboBoxDelegate
*Author:  caokewen
*Version:  1.0
*Date:  //2016/2/26
*Description:  //�����ṩ���ݿ����ز�����
**********************************************************************************/
#pragma once
#ifndef DATABASECONTROL_H
#define DATABASECONTROL_H
#include<QtSql\qsql.h>
#include<qstring.h>
#include<QtSql\qsqldatabase.h>
#include<QtSql\qsqlquerymodel.h>
#include<QtSql\qsqltablemodel.h>
#include<QtSql\qsqlrecord.h>
#include"Template/SingletonTemplate.h"
#include<qdebug.h>
#include"dataModel/BaseDataType.h"
#include"dataModel/DBDataModel/DBManager.h"
class DataBaseControl
{
public:
	DataBaseControl(DBManager *pManager=NULL);
	~DataBaseControl();
	void InitDataBaseName(const QString pDataBaseName, QString pdataBase,QString pUserName = "", QString pPassWord = "");
	QSqlQueryModel& getQueryData(QString pSql);
	QSqlTableModel* getTableModel(QString pSql,QString pTableName);
	bool  insertData(QString pSql);
	QString getId(QString pSql);
	QString  getAge(QString pBirthday, QString pAdminDate);
	//����
	void testModel();
	void saveDataToDataBase();
	void rollData();
	void deleteMedicalData(QString ID,bool isTemplate,bool isCourseDisease=false,bool isDeleteData=false);
	void insertMedicalData(QByteArray &byteArray, QString ID, QString PID, bool isTemplate);
	QByteArray getMedicalData(QString PID, bool isTemplate);
	QByteArray getMedicalData(QString PID, bool isTemplate, bool& isCompress);
	void updateMedicalData(QByteArray &byteArray, QString PID, bool isTemplate);
private:
	 QSqlDatabase m_database;
	static  QSqlQueryModel *m_QSqlQueryModel;
	static   QSqlTableModel *m_QSqlTableModel;
	static   QSqlTableModel *m_QSqlSpecialTableModel;
	 QSqlTableModel *m_QSqlTableModelTest;
	

	 public:
	//��ʼ��His����������
		 void initData(QStringList list, WriteAuthority& wrtAth, QMap<QString, MdcAuthority>& stlMdcAth);
private:
	void initDocutorData(QString doctorCode, QString doctorID, QString isSuperUser,QString isPhysician);
	void initPatientData(QString patientID);
	void initAuthority(QString InpNo, WriteAuthority& wrtAth, QMap<QString, MdcAuthority>& stlMdcAth);

	void initTemplateRight();
	void initMedicalRight();
	void initSpecialRight(QString sql);//����ҽ������ʿ������ҽ��������ҽ�������⴦��
	void initApplyDoctorRight(QString sql);//��ʼ������ҽ��
	inline void initTemplateRightData(QString temType, QString func_Code);
	inline void initMedicalRightData(QString cateGory_Code, QString func_Code);
public:
	//�ж��Ƿ������ꣻ
	bool is_Leap(int year);
	//����һ�����ж�����
	int sum_Day(int month);
	QString getDataBasetime();
	bool clinicalPathway(QString templateId);
public:
	static DocutorData *m_docutorData;
	static PatientData *m_patientData;
	static QMap<QString, operatorMedicalRight> *m_operatorMedical;
	static QMap<QString,operatorTemplateRight> *m_operatorTemplate;
	static DBManager *m_dbManager;
};
#endif // DATABASECONTROL_H
