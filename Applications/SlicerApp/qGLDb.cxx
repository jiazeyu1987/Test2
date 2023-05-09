#include "qGLDb.h"
#include <QVector>
#include <qmessagebox.h>
#include <qsqlquery.h>
#include <qvariant.h>
#include <qsqlerror.h>
#include <qdatetime.h>
#include <qcryptographichash.h>
#include <QScopedArrayPointer>
#include <qapplication.h>
#include <qdebug.h>
#include <qsqlrecord.h>
//#include "internal/DicomConflictEliminate.h"
//#include "qGLMessageDialog.h"
#include <qdesktopwidget.h>
#include "qGLTool.h"
#include "qJMainWindow.h"
Account GLDb::glAccount;
GLDb::GLDb(QObject *parent)
	:QObject(parent)
{
  
}

GLDb::~GLDb()
{

}

QVector<PatientsSeries> GLDb::getPatientsSeries(QVector<PatientsAnalyseSeries> patientsAnalyseSeriesVec)
{
    QVector<PatientsSeries> patientsSeriesVec;
    for (int i = 0; i < patientsAnalyseSeriesVec.size(); i++)
    {
        QString sql = QString("SELECT * FROM PatientsSeries WHERE patientsSeriesId = %1 AND enableColumn = true")
            .arg(patientsAnalyseSeriesVec.at(i).patientsSeriesId);
        QSqlQuery query(sql, db);

        PatientsSeries patientsSeries;
        while (query.next())
        {
            patientsSeries.patientsSeriesId = query.value("patientsSeriesId").toInt();
            patientsSeries.patientId = query.value("patientId").toInt();
            patientsSeries.dicomPatientId = query.value("dicomPatientId").toString();
            patientsSeries.dicomPatientId = query.value("dicomPatientId").toString();
            patientsSeries.accessionNumber = query.value("accessionNumber").toString();
            patientsSeries.dicomBirthday = query.value("dicomBirthday").toString();
            patientsSeries.dicomSex = query.value("dicomSex").toString();
            patientsSeries.seriesInstanceUID = query.value("seriesInstanceUID").toString();
            patientsSeries.seriesNumber = query.value("seriesNumber").toInt();
            patientsSeries.scanTime = query.value("scanTime").toString();
            patientsSeries.seriesDescribe = query.value("seriesDescribe").toString();
            patientsSeries.modality = query.value("modality").toString();
            patientsSeries.pixelSpacing = query.value("pixelSpacing").toString();
            patientsSeries.thickness = query.value("thickness").toString();
            patientsSeries.spaceBetween = query.value("spaceBetween").toString();
            patientsSeries.sliceCount = query.value("sliceCount").toInt();
            patientsSeries.seriesDir = query.value("seriesDir").toString();
            patientsSeries.screenshot = query.value("screenshot").toString();
            patientsSeries.enableSeries = query.value("enableSeries").toInt();
            patientsSeries.enableColumn = query.value("enableColumn").toInt();
            patientsSeries.createTime = query.value("createTime").toString();
            patientsSeries.changedTime = query.value("changedTime").toString();
            patientsSeries.remark = query.value("remark").toString();
            patientsSeries.md5 = query.value("md5").toString();
            //验证md5是否有效待加
            patientsSeriesVec.append(patientsSeries);
        }       
    }
    return patientsSeriesVec;
}

void GLDb::slot_init()
{/*
	hashTableToPrimaryId.insert(tableNameDoctors, doctorsId);
	hashTableToPrimaryId.insert(tableNameAccount, accountId);
	hashTableToPrimaryId.insert(tableNamePatient, patientId);
	hashTableToPrimaryId.insert(tableNamePatientsOperation, patientsOperationId);
	hashTableToPrimaryId.insert(tableNamePatientsSequenceList, patientsSequenceListId);
	hashTableToPrimaryId.insert(tableNamePatientsAnalyse, patientsAnalyseId);
	hashTableToPrimaryId.insert(tableNamePatientsAnalyseSequenceList, patientsAnalyseSequenceListId);
	hashTableToPrimaryId.insert(tableNamePatientsImageSequence, patientsImageSequenceId);
	hashTableToPrimaryId.insert(tableNameSoftwareOperationRecord, softwareOperationRecordId);
	*/
	timer.callOnTimeout(this, &GLDb::slot_heartbeat, Qt::QueuedConnection);
}

QString GLDb::calMd5softwareoperationrecord(int rowId, bool updateToDb)
{
	//计算md5
	QString sqlMd5 = QString("SELECT * FROM softwareOperationRecord WHERE softwareOperationRecordId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int softwareOperationRecordId = queryMd5.value("softwareOperationRecordId").toInt();
		md5List.append(QString::number(softwareOperationRecordId));
		QString	 content = queryMd5.value("content").toString();
		md5List.append(content);
		QString	 classes = queryMd5.value("classes").toString();
		md5List.append(classes);
		QString	 module = queryMd5.value("module").toString();
		md5List.append(module);
		int	 accountId = queryMd5.value("accountId").toInt();
		md5List.append(QString::number(accountId));
		QString	 userName = queryMd5.value("userName").toString();
		md5List.append(userName);
		bool	 enableColumn = queryMd5.value("enableColumn").toBool();
		//bool值true转换为oka，false 转 koa
		QString enableColumnStr = enableColumn ? "oka" : "koa";
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE softwareOperationRecord SET md5 = '%1' WHERE softwareOperationRecordId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
			//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
			//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5accounts(int rowId, bool updateToDb)
{
	//计算md5
	QString sqlMd5 = QString("SELECT * FROM accounts WHERE accountId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int accountId = queryMd5.value("accountId").toInt();
		md5List.append(QString::number(accountId));
		QString	 userName = queryMd5.value("userName").toString();
		md5List.append(userName);
		QString	 _password = queryMd5.value("_password").toString();
		md5List.append(_password);
		QString	 isFirstUse = queryMd5.value("isFirstUse").toString();
		md5List.append(isFirstUse);

        bool isSoftwareUserAgreementPrivacyPolicy = queryMd5.value("isSoftwareUserAgreementPrivacyPolicy").toBool();
        QString isSoftwareUserAgreementPrivacyPolicyStr = isSoftwareUserAgreementPrivacyPolicy ? "oka" : "koa"; //bool值true转换为oka，false 转 koa
        md5List.append(isSoftwareUserAgreementPrivacyPolicyStr);

		int	 doctorId = queryMd5.value("doctorId").toInt();
		md5List.append(QString::number(doctorId));
		QString	 doctorName = queryMd5.value("doctorName").toString();
		md5List.append(doctorName);
		bool	 enableColumn = queryMd5.value("enableColumn").toBool();
		//bool值true转换为oka，false 转 koa
		QString enableColumnStr = enableColumn ? "oka" : "koa";
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE accounts SET md5 = '%1' WHERE accountId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();

			//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
			//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
		}
		return md5Str;
	}

	return QString(); 
}

QString GLDb::calMd5accounts1(int rowId, bool updateToDb)
{
    //计算md5
    QString sqlMd5 = QString("SELECT * FROM accounts WHERE accountId = %1 AND enableColumn = true").arg(rowId);
    QSqlQuery queryMd5(sqlMd5, db);
    if (queryMd5.next())
    {
        QStringList md5List;
        int accountId = queryMd5.value("accountId").toInt();
        md5List.append(QString::number(accountId));
        QString	 userName = queryMd5.value("userName").toString();
        md5List.append(userName);
        QString	 _password = queryMd5.value("_password").toString();
        md5List.append(_password);
        QString	 isFirstUse = queryMd5.value("isFirstUse").toString();
        md5List.append(isFirstUse);

        bool isSoftwareUserAgreementPrivacyPolicy = queryMd5.value("isSoftwareUserAgreementPrivacyPolicy").toBool();
        QString isSoftwareUserAgreementPrivacyPolicyStr = isSoftwareUserAgreementPrivacyPolicy ? "oka" : "koa"; //bool值true转换为oka，false 转 koa
        md5List.append(isSoftwareUserAgreementPrivacyPolicyStr);

        int	 doctorId = queryMd5.value("doctorId").toInt();
        md5List.append(QString::number(doctorId));
        QString	 doctorName = queryMd5.value("doctorName").toString();
        md5List.append(doctorName);
        bool enableColumn = queryMd5.value("enableColumn").toBool();        
        QString enableColumnStr = enableColumn ? "oka" : "koa"; //bool值true转换为oka，false 转 koa
        md5List.append(enableColumnStr);
        QString	 createTime = queryMd5.value("createTime").toString();
        md5List.append(createTime);
        QString	 changedTime = queryMd5.value("changedTime").toString();
        md5List.append(changedTime);
        QString	 remark = queryMd5.value("remark").toString();
        md5List.append(remark);

        QString md5Str = md5(md5List);

        //更新md5到数据库
        if (updateToDb)
        {
            QString sqlUpdateMd5 = QString("UPDATE accounts SET md5 = '%1' WHERE accountId = %2 AND enableColumn = true")
                .arg(md5Str).arg(rowId);
            QSqlQuery queryMd5(sqlUpdateMd5, db);
            int num = queryMd5.numRowsAffected();

            //num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
            //	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
        }
        return md5Str;
    }

    return QString();

}

QString GLDb::calMd5patients(int rowId, bool updateToDb)
{
	//计算md5
	QString sqlMd5 = QString("SELECT * FROM Patients WHERE patientId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int patientId = queryMd5.value("patientId").toInt();
		md5List.append(QString::number(patientId));

        QString	 dicomPatientId = queryMd5.value("dicomPatientId").toString();
        md5List.append(dicomPatientId);

        int accountId = queryMd5.value("accountId").toInt();
        md5List.append(QString::number(accountId));

		QString	 patientName = queryMd5.value("patientName").toString();
		md5List.append(patientName);

		QString	 birthday = queryMd5.value("birthday").toString();
		md5List.append(birthday);

		QString sex = queryMd5.value("sex").toString();
		md5List.append(sex);

        QString height = queryMd5.value("height").toString();
        md5List.append(height);

        QString weight = queryMd5.value("weight").toString();
        md5List.append(weight);

        QString surgerSummary = queryMd5.value("surgerSummary").toString();
        md5List.append(surgerSummary);

        QString recordModificationTime = queryMd5.value("recordModificationTime").toString();
        md5List.append(recordModificationTime);

        QString projectPath = queryMd5.value("projectPath").toString();
        md5List.append(projectPath);

        QString logPath = queryMd5.value("logPath").toString();
        md5List.append(logPath);

        QString mergeSign = queryMd5.value("mergeSign").toString();
        md5List.append(mergeSign);

		bool enableColumn = queryMd5.value("enableColumn").toBool();		
		QString enableColumnStr = enableColumn ? "oka" : "koa"; //bool值true转换为oka，false 转 koa
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE Patients SET md5 = '%1' WHERE patientId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
			//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
			//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5patientsoperation(int rowId, bool updateToDb)
{
	QString sqlMd5 = QString("SELECT * FROM patientsoperation WHERE patientOperationId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int patientOperationId = queryMd5.value("patientOperationId").toInt();
		md5List.append(QString::number(patientOperationId));
		int	 patientId = queryMd5.value("patientId").toInt();
		md5List.append(QString::number(patientId));
		int	 accountId = queryMd5.value("accountId").toInt();
		md5List.append(QString::number(accountId));
		QString	 dataChangedTime = queryMd5.value("dataChangedTime").toString();
		md5List.append(dataChangedTime);
		QString	 accessionNumber = queryMd5.value("accessionNumber").toString();
		md5List.append(accessionNumber);
		QString	 modalities = queryMd5.value("modalities").toString();
		md5List.append(modalities);
		QString	 operationSummary = queryMd5.value("operationSummary").toString();
		md5List.append(operationSummary);

		bool	 enableColumn = queryMd5.value("enableColumn").toBool();
		//bool值true转换为oka，false 转 koa
		QString enableColumnStr = enableColumn ? "oka" : "koa";
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE patientsoperation SET md5 = '%1' WHERE patientOperationId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
			//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
			//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5PatientsSeries(int rowId, bool updateToDb)
{
	QString sqlMd5 = QString("SELECT * FROM PatientsSeries WHERE patientsSeriesId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int patientsSeriesId = queryMd5.value("patientsSeriesId").toInt();
		md5List.append(QString::number(patientsSeriesId));
		int	 patientId = queryMd5.value("patientId").toInt();
		md5List.append(QString::number(patientId));
        int	 dicomPatientId = queryMd5.value("dicomPatientId").toInt();
        md5List.append(QString::number(dicomPatientId));
        QString	 accessionNumber = queryMd5.value("accessionNumber").toString();
        md5List.append(accessionNumber);
        QString	 dicomPatientName = queryMd5.value("dicomPatientName").toString();
        md5List.append(dicomPatientName);
        QString	 dicomBirthday = queryMd5.value("dicomBirthday").toString();
        md5List.append(dicomBirthday);
        QString	 dicomSex = queryMd5.value("dicomSex").toString();
        md5List.append(dicomSex);
        QString	 dicomHeight = queryMd5.value("dicomHeight").toString();
        md5List.append(dicomHeight);
        QString	 dicomWeight = queryMd5.value("dicomWeight").toString();
        md5List.append(dicomWeight);
		bool enableSeries = queryMd5.value("enableSeries").toBool();
		QString enableSequenceStr = enableSeries ? "oka" : "koa";
		md5List.append(enableSequenceStr);
        QString	 seriesInstanceUID = queryMd5.value("seriesInstanceUID").toString();
        md5List.append(seriesInstanceUID);
		int	 seriesNumber = queryMd5.value("seriesNumber").toInt();
		md5List.append(QString::number(seriesNumber));
		QString	 scanTime = queryMd5.value("scanTime").toString();
		md5List.append(scanTime);
		QString	 seriesDescribe = queryMd5.value("seriesDescribe").toString();
		md5List.append(seriesDescribe);
		QString	 modality = queryMd5.value("modality").toString();
		md5List.append(modality);
        QString	 pixelSpacing = queryMd5.value("pixelSpacing").toString();
        md5List.append(pixelSpacing);
        QString	 thickness = queryMd5.value("thickness").toString();
        md5List.append(thickness);
        QString	 spaceBetween = queryMd5.value("spaceBetween").toString();
        md5List.append(spaceBetween);

		int	 sliceCount = queryMd5.value("sliceCount").toInt();
		md5List.append(QString::number(sliceCount));
        QString seriesDir = queryMd5.value("seriesDir").toString();
        md5List.append(seriesDir);
        QString screenshot = queryMd5.value("screenshot").toString();
        md5List.append(screenshot);

		bool	 enableColumn = queryMd5.value("enableColumn").toBool();		
		QString enableColumnStr = enableColumn ? "oka" : "koa";//bool值true转换为oka，false 转 koa
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE PatientsSeries SET md5 = '%1' WHERE patientsSeriesId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5patientsanalyse(int rowId, bool updateToDb)
{
	QString sqlMd5 = QString("SELECT * FROM patientsanalyse WHERE patientAnalyseId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int patientAnalyseId = queryMd5.value("patientAnalyseId").toInt();
		md5List.append(QString::number(patientAnalyseId));
		int	 accountId = queryMd5.value("accountId").toInt();
		md5List.append(QString::number(accountId));
		QString	 analyseName = queryMd5.value("analyseName").toString();
		md5List.append(analyseName);
		QString	 illnessKind = queryMd5.value("illnessKind").toString();
		md5List.append(illnessKind);
		QString	 doctorRemark = queryMd5.value("doctorRemark").toString();
		md5List.append(doctorRemark);
        QString	 patientName = queryMd5.value("patientName").toString();
        md5List.append(patientName);
        QString	 birthday = queryMd5.value("birthday").toString();
        md5List.append(birthday);
        QString	 sex = queryMd5.value("sex").toString();
        md5List.append(sex);
        QString	 height = queryMd5.value("height").toString();
        md5List.append(height);
        QString	 weight = queryMd5.value("weight").toString();
        md5List.append(weight);

        QString	 reportPath = queryMd5.value("reportPath").toString();
        md5List.append(reportPath);

		bool	 enableColumn = queryMd5.value("enableColumn").toBool();
		//bool值true转换为oka，false 转 koa
		QString enableColumnStr = enableColumn ? "oka" : "koa";
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE patientsanalyse SET md5 = '%1' WHERE patientAnalyseId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
			//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
			//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5patientsAnalyseIdPatientId(int rowId, bool updateToDb)
{
    QString sqlMd5 = QString("SELECT * FROM PatientsAnalyseIdPatientId WHERE patientAnalyseIdPatientIdId = %1 AND enableColumn = true").arg(rowId);
    QSqlQuery queryMd5(sqlMd5, db);
    if (queryMd5.next())
    {
        QStringList md5List;
        int patientAnalyseIdPatientIdId = queryMd5.value("patientAnalyseIdPatientIdId").toInt();
        md5List.append(QString::number(patientAnalyseIdPatientIdId));

        int	 patientAnalyseId = queryMd5.value("patientAnalyseId").toInt();
        md5List.append(QString::number(patientAnalyseId));

        int	 patientId = queryMd5.value("patientId").toInt();
        md5List.append(QString::number(patientId));

        bool	 enableColumn = queryMd5.value("enableColumn").toBool();
        QString enableColumnStr = enableColumn ? "oka" : "koa";
        md5List.append(enableColumnStr);

        QString	 createTime = queryMd5.value("createTime").toString();
        md5List.append(createTime);

        QString	 changedTime = queryMd5.value("changedTime").toString();
        md5List.append(changedTime);

        QString	 remark = queryMd5.value("remark").toString();
        md5List.append(remark);

        QString md5Str = md5(md5List);

        //更新md5到数据库
        if (updateToDb)
        {
            QString sqlUpdateMd5 = QString("UPDATE PatientsAnalyseIdPatientId SET md5 = '%1' WHERE patientAnalyseIdPatientIdId = %2 AND enableColumn = true")
                .arg(md5Str).arg(rowId);
            QSqlQuery queryMd5(sqlUpdateMd5, db);
            int num = queryMd5.numRowsAffected();
        }
        return md5Str;
    }

    return QString();

}

QString GLDb::calMd5patientsanalysesequencelist(int rowId, bool updateToDb)
{
	QString sqlMd5 = QString("SELECT * FROM PatientsAnalyseSeries WHERE patientAnalyseSeriesId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int patientAnalyseSequenceListId = queryMd5.value("patientAnalyseSeriesId").toInt();
		md5List.append(QString::number(patientAnalyseSequenceListId));
        int	 patientId = queryMd5.value("patientId").toInt();
        md5List.append(QString::number(patientId));
		int	 patientAnalyseId = queryMd5.value("patientAnalyseId").toInt();
		md5List.append(QString::number(patientAnalyseId));
        int	 patientsSequenceListId = queryMd5.value("patientsSeriesId").toInt();
        md5List.append(QString::number(patientsSequenceListId));
		bool enableSequence = queryMd5.value("enableSeries").toBool();		
		QString enableSequenceStr = enableSequence ? "oka" : "koa";//bool值true转换为oka，false 转 koa
		md5List.append(enableSequenceStr);

		bool	 enableColumn = queryMd5.value("enableColumn").toBool();		
		QString enableColumnStr = enableColumn ? "oka" : "koa";//bool值true转换为oka，false 转 koa
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE PatientsAnalyseSeries SET md5 = '%1' WHERE patientAnalyseSequenceListId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
			//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
			//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5patientsimagesequence(int rowId, bool updateToDb)
{
	QString sqlMd5 = QString("SELECT * FROM patientsimagesequence WHERE paintentImageSequenceId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int paintentImageSequenceId = queryMd5.value("paintentImageSequenceId").toInt();
		md5List.append(QString::number(paintentImageSequenceId));
		int	 patientId = queryMd5.value("patientId").toInt();
		md5List.append(QString::number(patientId));
		QString	 analyseIdReportPath = queryMd5.value("analyseIdReportPath").toString();
		md5List.append(analyseIdReportPath);
		QString	 projectPath = queryMd5.value("projectPath").toString();
		md5List.append(projectPath);
		QString	 logPath = queryMd5.value("logPath").toString();
		md5List.append(logPath);

		bool	 enableColumn = queryMd5.value("enableColumn").toBool();
		//bool值true转换为oka，false 转 koa
		QString enableColumnStr = enableColumn ? "oka" : "koa";
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE patientsimagesequence SET md5 = '%1' WHERE paintentImageSequenceId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
			//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
			//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryMd5.lastError().text()));
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5doctors(int rowId, bool updateToDb)
{
	QString sqlMd5 = QString("SELECT * FROM doctors WHERE doctorId = %1 AND enableColumn = true").arg(rowId);
	QSqlQuery queryMd5(sqlMd5, db);
	if (queryMd5.next())
	{
		QStringList md5List;
		int doctorId = queryMd5.value("doctorId").toInt();
		md5List.append(QString::number(doctorId));
		QString	 doctorName = queryMd5.value("doctorName").toString();
		md5List.append(doctorName);
		QString	 hostpital = queryMd5.value("hostpital").toString();
		md5List.append(hostpital);
		QString	 department = queryMd5.value("department").toString();
		md5List.append(department);
    QChar sex = queryMd5.value("sex").toChar();
    md5List.append(QString(sex));
    QString mobile = queryMd5.value("mobile").toString();
    md5List.append(mobile);

		bool	 enableColumn = queryMd5.value("enableColumn").toBool();
		//bool值true转换为oka，false 转 koa
		QString enableColumnStr = enableColumn ? "oka" : "koa";
		md5List.append(enableColumnStr);
		QString	 createTime = queryMd5.value("createTime").toString();
		md5List.append(createTime);
		QString	 changedTime = queryMd5.value("changedTime").toString();
		md5List.append(changedTime);
		QString	 remark = queryMd5.value("remark").toString();
		md5List.append(remark);

		QString md5Str = md5(md5List);

		//更新md5到数据库
		if (updateToDb)
		{
			QString sqlUpdateMd5 = QString("UPDATE doctors SET md5 = '%1' WHERE doctorId = %2 AND enableColumn = true")
				.arg(md5Str).arg(rowId);
			QSqlQuery queryMd5(sqlUpdateMd5, db);
			int num = queryMd5.numRowsAffected();
		}
		return md5Str;
	}

	return QString();
}

QString GLDb::calMd5fiberWaterConnectRecord(int rowId, bool updateToDb)
{
  //从数据库获取对应的行
  QString sqlMd5 = QString("SELECT * FROM fiberWaterConnectRecord WHERE fiberWaterConnectRecord = %1 AND enableColumn = true").arg(rowId);
  QSqlQuery queryMd5(sqlMd5, db);

  //按规则拼接所有的列
  if (queryMd5.next())
  {
    QStringList md5List;
    int fiberWaterConnectRecordId = queryMd5.value("fiberWaterConnectRecordId").toInt();
    md5List.append(QString::number(fiberWaterConnectRecordId));

    QString	 content = queryMd5.value("content").toString();
    md5List.append(content);

    QString	 recordTime = queryMd5.value("recordTime").toString();
    md5List.append(recordTime);

    QString	 device = queryMd5.value("device").toString();
    md5List.append(device);

    int	 accountId = queryMd5.value("accountId").toInt();
    md5List.append(QString::number(accountId));

    QString	 userName = queryMd5.value("userName").toString();
    md5List.append(userName);

    bool	 enableColumn = queryMd5.value("enableColumn").toBool();
    //bool值true转换为oka，false 转 koa
    QString enableColumnStr = enableColumn ? "oka" : "koa";
    md5List.append(enableColumnStr);

    QString	 createTime = queryMd5.value("createTime").toString();
    md5List.append(createTime);

    QString	 changedTime = queryMd5.value("changedTime").toString();
    md5List.append(changedTime);

    QString	 remark = queryMd5.value("remark").toString();
    md5List.append(remark);

    QString md5Str = md5(md5List);

    //更新md5到数据库
    if (updateToDb)
    {
      QString sqlUpdateMd5 = QString("UPDATE fiberWaterConnectRecord SET md5 = '%1' WHERE softwareOperationRecordId = %2 AND enableColumn = true")
        .arg(md5Str).arg(rowId);
      QSqlQuery queryMd5(sqlUpdateMd5, db);
      int num = queryMd5.numRowsAffected();
    }

    return md5Str;
  }

  //获取空行或出错
  return QString();
}

//方法无法达成目标，弃用
int GLDb::setMd5(QString tableName, QString md5, int idvalue)
{
	//获取字段值，计算结果，计算单行
	QString sqlMd5 = QString("SELECT * FROM %1 WHERE %2 = %3")
		.arg(tableName).arg(hashTableToPrimaryId.value(tableName)).arg(idvalue);
	QSqlQuery queryMd5(sqlMd5, db);
	QSqlRecord record = queryMd5.record();
	QStringList md5List;
	for (int i = 0; i < record.count(); i++)
	{
		if (record.fieldName(i) != "md5")
		{
			QString value;

			md5List.append(value);
		}
	}

	//设置md5
	QString sqlUpdateMd5 = QString("UPDATE %1 SET md5 = '%2' WHERE %3 = %4 AND enableColumn = true")
		.arg(tableName).arg(md5).arg(hashTableToPrimaryId.value(tableName)).arg(idvalue);
	QSqlQuery queryUpdateMd5(sqlUpdateMd5, db);
	int num = queryUpdateMd5.numRowsAffected();
	//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("md5成功更新%1行！").arg(num)) :
	//	QMessageBox::warning(nullptr, QStringLiteral("数据库"), QStringLiteral("md5更新失败：%1").arg(queryUpdateMd5.lastError().text()));

	return num;
}

int GLDb::createDbConnect()
{
    emit sig_log("数据库操作-创建数据库连接");
    QString connName = "1";
    if (QSqlDatabase::contains(connName))
    {
        db = QSqlDatabase::database(connName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", connName);
		//if (GLTool::odbc)
		//	db = QSqlDatabase::addDatabase("QODBC", connName);
		//else
		//	db = QSqlDatabase::addDatabase("QMYSQL", connName);
    }

    //设置属性
    //db.setHostName(host);
    //db.setUserName(username);
    //db.setPassword(password);
    //dbname = QApplication::applicationDirPath() + "/db/db_jl.db";
    dbname = qJMainWindow::GetProjectDBPath();
    db.setDatabaseName(dbname);
    //db.setConnectOptions("MYSQL_OPT_RECONNECT=true"); //断开重连 odbc的方式不支持

    //打开连接
    if (!db.open())
    {
        QString errorString = QStringLiteral("数据库连接失败：%1").arg(db.lastError().text());
        QAbstractButton *btn = new QPushButton(QStringLiteral("确认"));
        QPair<QAbstractButton *, QMessageBox::ButtonRole> btnPair(btn, QMessageBox::ButtonRole::AcceptRole);
        QVector<QPair<QAbstractButton *, QMessageBox::ButtonRole>> btnVec(1, btnPair);
        QString title = QStringLiteral("数据库");
        //GLMessageBox msg(title, errorString, btnVec, QMessageBox::Critical);
        emit sig_log(errorString);
        return 0; //失败
    }
    else
    {
        timer.start(heartbeatTime); //开始心跳
        return 1; //成功
    }
}

int GLDb::setDoctors(Doctors& doctor)
{
    emit sig_log("数据库-设置医生表");
  int lastId = -1;
  QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
  //先判断用户是否存在（姓名+部门+医院+性别）
  QString sqlExist = QString("SELECT * FROM doctors WHERE doctorName = '%1' AND hostpital = '%2' AND department = '%3' AND sex = '%4' AND enableColumn = true AND enableColumn = true")
    .arg(doctor.doctorName).arg(doctor.hostpital).arg(doctor.department).arg(doctor.sex);
  QSqlQuery queryExist(sqlExist, db);
  bool exist = queryExist.next();

  QString createTime = (exist ? queryExist.value("createTime").toString() : changedTime);
  doctor.changedTime = changedTime;
  doctor.createTime = createTime;

  //这里暂时不考虑更新的情况
  QString sqlSet;
  exist = false; //暂时不考虑更新
  if (exist)
  {
    sqlSet = QString("UPDATE doctors SET mobile = '%1', changedTime = '%2', remark = '%3' AND enableColumn = true").arg(doctor.mobile).arg(doctor.changedTime).arg(doctor.remark);
  }
  else
  {
    sqlSet = QString("INSERT INTO doctors(doctorName, hostpital, department, sex, mobile, enableColumn, createTime, changedTime, remark) values"
      "('%1', '%2', '%3', '%4', '%5', %6, '%7', '%8', '%9')")
      .arg(doctor.doctorName).arg(doctor.hostpital).arg(doctor.department).arg(doctor.sex).arg(doctor.mobile).arg(doctor.enableColumn)
      .arg(doctor.createTime).arg(doctor.changedTime).arg(doctor.remark);
  }

  QSqlQuery querySet(sqlSet, db);

  //计算md5
  lastId = exist ? queryExist.value("doctorId").toInt() : querySet.lastInsertId().toInt();
  calMd5doctors(lastId, true);

  //最后的结果
  int num = querySet.numRowsAffected();
  if (num > 0)
  {
    //
    //GLMessageDialog::ShowInformation(QStringLiteral("注册用户"), QStringLiteral("成功更新或插入%1行！").arg(num));
  }
  else
  {
   // 
   // GLMessageDialog::ShowWarnning(QStringLiteral("注册用户"), QStringLiteral("更新失败：%1").arg(querySet.lastError().text()));
  }

  doctor.doctorId = lastId;

  return num;
}

int GLDb::setAccount(Account & account)
{
  //先判断用户名是否存在
  QString sql = QString("SELECT * FROM accounts WHERE userName = '%1'AND enableColumn = true").arg(account.userName);
  QSqlQuery queryExist(sql, db);
  bool exist = queryExist.next();
  GLMessageBox msg;
  if (exist)
  {    
      msg.queRenWarn(QStringLiteral("注册用户"), QStringLiteral("<font color='red'>用户名已经存在!</font>"));
      return -2;
  }
  else
  {
    QSqlQuery queryReg;
    queryReg.prepare("INSERT INTO accounts (userName, _password, doctorId, doctorName, createTime, changedTime, remark) "
      "VALUES (:userName, :_password, :doctorId, :doctorName, :createTime, :changedTime)");
    queryReg.bindValue(":userName", account.userName);
    queryReg.bindValue(":_password", account._password);
    queryReg.bindValue(":doctorId", account.doctorId);
    queryReg.bindValue(":doctorName", account.doctorName);
    QString unix_stamp = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    queryReg.bindValue(":createTime", unix_stamp);
    queryReg.bindValue(":changedTime", unix_stamp); //注册的时候一样
    queryReg.bindValue(":remark", account.remark);

    queryReg.exec();

    int lastId = queryReg.lastInsertId().toInt();

    //计算md5
    calMd5accounts(lastId, true);

    int num = queryReg.numRowsAffected();
    if (queryReg.isActive())
    {
        msg.queRenInfo(QStringLiteral("注册用户"), QString("成功插入%1行").arg(num));
    }
    else
    {
      QString errorString = queryReg.lastError().text();     
      msg.queRenError(QStringLiteral("注册用户"), QString("插入失败：%1").arg(errorString));
    }

    return num;
  }
}

//注册账户
int GLDb::reg(Doctors& doctors, Account& account)
{
    emit sig_log("数据库操作-用户注册");

    setDoctors(doctors);
    account.doctorId = doctors.doctorId;
    account.doctorName = doctors.doctorName;

    //先判断用户名是否存在
    QString sql = QString("SELECT * FROM accounts WHERE userName = '%1' AND enableColumn = true").arg(account.userName);
    QSqlQuery queryExist(sql, db);
    bool exist = queryExist.next();
    GLMessageBox msg;
    if (exist)
    {
        msg.queRenWarn(QStringLiteral("注册用户"), QStringLiteral("<font color='red'>用户名已经存在!</font>"));
        return -2;
    }
    else
    {
        QString unix_stamp = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        QString sql = QString("INSERT INTO accounts (userName, _password, doctorId, doctorName, createTime, changedTime, remark) "
            "VALUES ('%1', '%2', %3, '%4', '%5', '%6', '%7')")
            .arg(account.userName).arg(account._password).arg(account.doctorId).arg(account.doctorName).arg(unix_stamp).arg(unix_stamp).arg(account.remark);
        QSqlQuery queryReg(sql, db);

        int lastId = queryReg.lastInsertId().toInt();

        //计算md5
        calMd5accounts(lastId, true);

        int num = queryReg.numRowsAffected();
        if (queryReg.isActive())
        {            
            msg.queRenInfo(QStringLiteral("注册用户"), QStringLiteral("注册成功").arg(num));
        }
        else
        {
            QString errorString = queryReg.lastError().text();           
            msg.queRenError(QStringLiteral("注册用户"), QStringLiteral("注册失败！").arg(errorString));
        }

        return num;
    }
}

//注册账户--暂时不用，用户第二版，将账户表和医生表合并使用
int GLDb::reg(Account1& account)
{
    emit sig_log("数据库操作-用户注册");
	//先判断用户名是否存在
	QString sql = QString("SELECT * FROM accounts WHERE userName = '%1' AND enableColumn = true").arg(account.userName); //md5的判断留到后面
	QSqlQuery queryExist(sql, db);
	bool exist = queryExist.next();
    GLMessageBox msg;
	if (exist)
	{		
        msg.queRenWarn(QStringLiteral("注册用户"), QStringLiteral("<font color='red'>用户名已经存在!</font>"));
		return -2;
	}
	else
	{
        QString unix_stamp = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        QString sql = QString("INSERT INTO accounts (userName, _password, doctorName, sex, mobile, hostpital, department, createTime, changedTime, remark) "
			"VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
        .arg(account.userName).arg(account._password).arg(account.doctorName).arg(account.sex).arg(account.mobile).arg(account.hostpital).arg(account.department).arg(unix_stamp).arg(unix_stamp).arg(account.remark);
		QSqlQuery queryReg(sql, db);

        int lastId = queryReg.lastInsertId().toInt();

		//计算md5
        calMd5accounts(lastId, true);

		int num = queryReg.numRowsAffected();
		if (queryReg.isActive())
		{          
            msg.queRenInfo(QStringLiteral("注册用户"), QStringLiteral("注册成功").arg(num));
		}
		else
		{
			QString errorString = queryReg.lastError().text();   
            msg.queRenError(QStringLiteral("注册用户"), QStringLiteral("注册失败！").arg(errorString));
		}	

        return num;
	}
}

enum LoginStatus GLDb::login(Account& account)
{
    emit sig_log("数据库操作-用户登录,用户名："+account.userName);
	//先判断用户名是否存在
	QString sql = QString("SELECT * FROM accounts WHERE userName = '%1' AND enableColumn = true").arg(account.userName);
	QSqlQuery query(sql, db);
	if (!query.next()) 
	{
		return UsernameError; //用户名不存在
	}
	else
	{
		//QString sql = QString("SELECT * FROM accounts WHERE userName = '%1' AND _password = '%2'").arg(username).arg(password);
		//QSqlQuery query(sql, db);
		QString _password = query.value("_password").toString();

		if (account._password == _password) //登录成功
		{
			//验证md5
			int accountId = query.value("accountId").toInt();
			QString md5 = query.value("md5").toString();
			QString _md5 = calMd5accounts(accountId, false);
			if (md5 != _md5)
			{
				return DataTampered; //数据被纂改
			}

			//成功保存账号信息
      glAccount.accountId = account.accountId = accountId;
      glAccount.userName = account.userName;
      glAccount._password = account._password;
      glAccount.doctorId = query.value("doctorId").toInt();
      glAccount.doctorName = query.value("doctorName").toString();
      glAccount.enableColumn = query.value("enableColumn").toBool();
      glAccount.createTime = query.value("createTime").toString();
      glAccount.changedTime = query.value("changedTime").toString();
      glAccount.remark = query.value("remark").toString();
      glAccount.isFirstUse = account.isFirstUse = query.value("isFirstUse").toInt();

			return VerificationSuccess;
		}
		else
		{
			return PasswordError; //密码错误
		}
	}
}

int GLDb::existUsername(Account& account)
{
    emit sig_log("数据库操作-判断是否存在用户名");
  //判断用户名是否存在
  QString sql = QString("SELECT * FROM accounts WHERE userName = '%1' AND enableColumn = true").arg(account.userName);
  QSqlQuery query(sql, db);
  if (query.next())
  {
    account._password = query.value("_password").toString();
    return 1;   
  }
  else
    return 0; //用户名不存在
}

bool GLDb::setPatientAnalyseEnable(PatientsAnalyse analyse)
{
    QString sql = QString("UPDATE PatientsAnalyse SET enableColumn = %1 WHERE patientAnalyseId = %2").arg(analyse.enableColumn).arg(analyse.patientAnalyseId);
    QSqlQuery query(sql, db);

    //计算md5
    int num = query.numRowsAffected();
    if (num > 0)
        calMd5patientsanalyse(analyse.patientAnalyseId, true);

    return num;
}

QVector<PatientsAnalyseSeries> GLDb::getPatientAnalyseSeries(PatientsAnalyse analyse)
{
    emit sig_log("数据库操作-获取患者分析序列");
    QString sql = QString("SELECT * FROM patientsanalyseseries WHERE patientAnalyseId = %1 AND enableColumn = true").arg(analyse.patientAnalyseId);
    QSqlQuery query(sql, db);

    QVector<PatientsAnalyseSeries> patientsSeriesVec;
    while (query.next())
    {
        PatientsAnalyseSeries patientAnalyseSeries;
        patientAnalyseSeries.patientAnalyseSeriesId = query.value("patientAnalyseSeriesId").toInt();
        patientAnalyseSeries.patientAnalyseId = query.value("patientAnalyseId").toInt();
        patientAnalyseSeries.patientsSeriesId = query.value("patientsSeriesId").toInt();
        patientsSeriesVec.append(patientAnalyseSeries);
    }

    return patientsSeriesVec;
}

QVector<Patient> GLDb::getPatients(Account account)
{
    emit sig_log("数据库操作-获取患者数据");
    QString sql = QString("SELECT * FROM Patients WHERE accountId = %1 AND enableColumn = true").arg(account.accountId);
    QSqlQuery query(sql, db);

    QVector<Patient> patientVec;
    while (query.next())
    {
        Patient patient;
        patient.patientId = query.value("patientId").toInt();
        patient.dicomPatientId = query.value("dicomPatientId").toString();
        patient.accountId = account.accountId;
        patient.patientName = query.value("patientName").toString();
        patient.birthday = query.value("birthday").toString();
        patient.sex = query.value("sex").toString();
        patient.height = query.value("height").toString();
        patient.weight = query.value("weight").toString();
        patient.surgerSummary = query.value("surgerSummary").toString();
        patient.recordModificationTime = query.value("recordModificationTime").toString();
        patient.projectPath = query.value("projectPath").toString();
        patient.logPath = query.value("logPath").toString();
        patient.mergeSign = query.value("mergeSign").toString();
        patientVec.append(patient);
    }

    return patientVec;
}

Patient GLDb::getPatients(int patientId)
{
    emit sig_log("数据库操作-获取指定id: " + QString::number(patientId) +  "的患者信息");
    QString sql = QString("SELECT * FROM Patients WHERE patientId = %1 AND enableColumn = true").arg(patientId);
    QSqlQuery query(sql, db);

    Patient patient;
    if (query.next())
    {        
        patient.patientId = query.value("patientId").toInt();
        patient.dicomPatientId = query.value("dicomPatientId").toString();
        patient.accountId = query.value("accountId").toInt();
        patient.patientName = query.value("patientName").toString();
        patient.birthday = query.value("birthday").toString();
        patient.sex = query.value("sex").toString();
        patient.surgerSummary = query.value("surgerSummary").toString();
        patient.recordModificationTime = query.value("recordModificationTime").toString();
        patient.projectPath = query.value("projectPath").toString();
        patient.logPath = query.value("logPath").toString();
    }

    return patient;
}

int GLDb::setPatients(Patient& patient)
{
    emit sig_log("数据库操作-设置患者表");
    QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    //1. 根据dicom 的patientId判断，其他让用户去合并，因为理论上姓名+性别+年龄，dicom的paitentId和accessNumber都无法确定一个人
    //但是这里有个重大的问题是：不管是以哪一个为标准，可能会造成后面的把前面的覆盖掉，比如姓名，生日等
    //2. 不同账户的同一个患者需要不同的患者id，以便于后续管理

    QString sqlExist = QString("SELECT * FROM Patients WHERE dicomPatientId = '%1' AND accountId = '%2' AND enableColumn = true")
        .arg(patient.dicomPatientId).arg(glAccount.accountId);
    QSqlQuery queryExist(sqlExist, db);

    bool exist = queryExist.next();
    QString createTime = exist ? queryExist.value("createTime").toString() : changedTime;
    patient.recordModificationTime = changedTime;
    //构造数据库插入语句，存在则更新，不存在则插入
    QString sqlSet;
    if (exist)
    {
        patient.patientId = queryExist.value("patientId").toInt();
        //对于dicomPatientId相同，但是姓名、病人ID、生日、年龄、性别和Accession Number不一致时，应该让医生去选择，这里需要弹框

        //判断是否完全一致
        QString patientName = queryExist.value("patientName").toString();
        QString birthday = queryExist.value("birthday").toString();
        QString sex = queryExist.value("sex").toString();
        QString height = queryExist.value("height").toString();
        QString weight = queryExist.value("weight").toString();
        if (patientName == patient.patientName && birthday == patient.birthday && sex == patient.sex) //如果一模一样，什么都不用做
        {           
            return 0;
        }   
        QVector<Patient> comboxDataVec;
        comboxDataVec.append(patient);
        Patient dbPatient; //数据库中已存在的患者数据
        dbPatient.patientName = patientName;
        dbPatient.birthday = birthday;
        dbPatient.sex = sex;
        comboxDataVec.append(dbPatient);
        bool acceptOrCancel;
        //DicomConfilctEliminate dce(comboxDataVec, patient, QStringLiteral("导入患者的ID相同，但是有些属性不同，请手动选择"), &acceptOrCancel); //弹框让医生选择，以消除歧义
        //dce.setGeometry(QApplication::desktop()->availableGeometry());

        sqlSet = QString("UPDATE Patients SET patientName = '%1', birthday = '%2', sex = '%3', changedTime = '%4' "
            "WHERE patientId = %5 AND enableColumn = true")
            .arg(patient.patientName).arg(patient.birthday).arg(patient.sex).arg(changedTime).arg(patient.patientId);
        
    }
    else
    {        
        sqlSet = QString("INSERT INTO Patients(accountId, dicomPatientId, patientName, birthday, sex, height, weight, surgerSummary, recordModificationTime, "
            "projectPath, logPath, enableColumn, createTime, changedTime, remark) "
            "VALUES(%1, '%2', '%3', '%4', '%5', '%6', '%7','%8', '%9', '%10', '%11', %12, '%13', '%14', '%15')")
            .arg(glAccount.accountId).arg(patient.dicomPatientId).arg(patient.patientName).arg(patient.birthday).arg(patient.sex)
            .arg(patient.height).arg(patient.weight).arg(patient.surgerSummary).arg(patient.recordModificationTime).arg(patient.projectPath)
            .arg(patient.logPath).arg(patient.enableColumn).arg(createTime).arg(changedTime).arg(patient.remark);

    }
    QSqlQuery querySet(sqlSet, db);

    int num = querySet.numRowsAffected();
    //计算md5
    if (num > 0)
    {
        patient.patientId = (exist ? queryExist.value("patientId").toInt() : querySet.lastInsertId().toInt());//获取更新或者插入的id
        calMd5patients(patient.patientId, true);
    }

    messageBoxSet(exist, num, querySet.lastError().text());

    return num;
}

int GLDb::setPatientMergeSplitSign(QStringList patientIdList, QString mergeSign)
{
    emit sig_log("数据库操作-设置患者合并标志");
    QString sql = QString("UPDATE Patients SET mergeSign = '%1' WHERE patientId IN(%2)")
        .arg(mergeSign).arg(patientIdList.join(","));
    QSqlQuery query(sql, db);

    int num = query.numRowsAffected();
    //计算md5
    if (num > 0)
    {
        for (int i = 0; i < patientIdList.length(); i++)
        {
            calMd5patients(patientIdList.at(i).toInt(), true);
        }       
    }

    messageBoxSet(true, num, query.lastError().text());

    return num;
}

int GLDb::setPatientsSeries(PatientsSeries series)
{
    emit sig_log("数据库操作-设置患者序列");
    QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");

    //先判定是否存在记录（patientId + 序列实体UID）
    //UID已经可以确定是同一个序列了，但是不同账户下，导入的序列，不能混在一起，因为不同账户下，加载的序列是要该账户下的序列
    //因为可能人为的将一个序列分开两个目录，作为两个不同的序列（注意：这样会和不同的序列分布在不同的文件夹下发生冲突，目前这个没有考虑）
    QString sqlExist = QString("SELECT * FROM PatientsSeries WHERE patientId = %1 AND seriesInstanceUID = '%2' AND seriesDir = '%3' AND enableColumn = true")
        .arg(series.patientId).arg(series.seriesInstanceUID).arg(series.seriesDir);
    QSqlQuery queryExist(sqlExist, db);

    bool exist = queryExist.next();
    //bool enableColumn = exist ? queryExist.value("enableColumn").toBool() : seqList.enableColumn;
    //bool enableColumn = seqListVec[i].enableColumn;
    QString createTime = exist ? queryExist.value("createTime").toString() : changedTime;
    //int createTime = changedTime;
    //QString remark = exist ? queryExist.value("remark").toString() : series.remark;

    //构造数据库插入语句（存在则一定是已经导入过，不需要考虑更新）
    QString sqlSet;
    if (exist)
    {
        return 0;
    }
    else
    {
        sqlSet =
            QString("INSERT INTO PatientsSeries"
                "(patientId, dicomPatientId, accessionNumber, dicomPatientName, dicomBirthday, dicomSex, dicomHeight, dicomWeight, seriesInstanceUID, seriesNumber, "
                "scanTime, seriesDescribe, modality, pixelSpacing, thickness, spaceBetween, sliceCount, seriesDir, screenshot, enableColumn, createTime, changedTime, remark) "
                "VALUES(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8','%9', %10, '%11', '%12', '%13', '%14', '%15', '%16', %17, '%18', '%19', %20, '%21', '%22', '%23')")
            .arg(series.patientId).arg(series.dicomPatientId).arg(series.accessionNumber).arg(series.dicomPatientName).arg(series.dicomBirthday).arg(series.dicomSex).arg(series.dicomHeight).arg(series.dicomWeight)
            .arg(series.seriesInstanceUID).arg(series.seriesNumber).arg(series.scanTime).arg(series.seriesDescribe).arg(series.modality).arg(series.pixelSpacing).arg(series.thickness).arg(series.spaceBetween)
            .arg(series.sliceCount).arg(series.seriesDir).arg(series.screenshot).arg(series.enableColumn).arg(createTime).arg(changedTime).arg(series.remark);
    }
    QSqlQuery querySet(sqlSet, db);

    int num = querySet.numRowsAffected();
    //计算md5    
    if (num > 0)
    {
        int rowId = querySet.lastInsertId().toInt();//获取插入的id
        calMd5PatientsSeries(rowId, true);
    }

    messageBoxSet(exist, num, querySet.lastError().text());
    return num;
}

int GLDb::setPatientsAnalyse(PatientsAnalyse& patientsAnalyse)
{
    emit sig_log("数据库操作-设置患者新建分析");
	  QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
		//医生登录时，新建的分析都是当时新建的，所以不需要判断存在
		/*
		QString sqlExist = QString("SELECT * FROM patientsAnalyse WHERE patientId = %1 AND analyseName = '%2'")
			.arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName);
		QSqlQuery queryExist(sqlExist, db);
		bool exist = queryExist.next();
		*/
		
		//bool enableColumn = exist ? queryExist.value("enableColumn").toBool() : patientsAnalyse[i].enableColumn;
      bool enableColumn = patientsAnalyse.enableColumn;
		//QString createTime = exist ? queryExist.value("createTime").toString() : changedTime;
		QString createTime = changedTime;
		//QString remark = exist ? queryExist.value("remark").toString() : patientsAnalyse[i].remark;
		QString remark = patientsAnalyse.remark;

		//构造数据库插入语句，存在则更新，不存在则更新
		/*
		QString sqlSet = exist ?
			QString("UPDATE patientsAnalyse SET patientId = %1, analyseName = %2, illnessKind = '%3',"
				"patientRemark = '%4', changedTime = %5, md5 = '%6'")
				.arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName).arg(patientsAnalyse[i].illnessKind)
				.arg(patientsAnalyse[i].patientRemark).arg(md5_res)
				.arg(changedTime).arg(md5_res) :
			QString("INSERT INTO patitentsOperation"
				"(patientId, analyseName, illnessKind, patientRemark, enableColumn, createTime, changedTime, remark, md5) "
				"VALUES(%1, '%2', '%3', '%4', %5, '%6', '%7', '%8', '%9')")
				.arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName).arg(patientsAnalyse[i].illnessKind).arg(patientsAnalyse[i].patientRemark)
				.arg(enableColumn).arg(createTime).arg(changedTime).arg(remark).arg(md5_res);
		*/
		QString sqlSet = QString("INSERT INTO patientsAnalyse"
			"(accountId, analyseName, illnessKind, doctorRemark, patientName, birthday, sex, height, weight, enableColumn, createTime, changedTime, remark) "
			"VALUES(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', %10, '%11', '%12', '%13')")
			.arg(glAccount.accountId).arg(patientsAnalyse.analyseName).arg(patientsAnalyse.illnessKind)
            .arg(patientsAnalyse.doctorRemark).arg(patientsAnalyse.patientName).arg(patientsAnalyse.birthday)
            .arg(patientsAnalyse.sex).arg(patientsAnalyse.height).arg(patientsAnalyse.weight)
            .arg(enableColumn).arg(createTime).arg(changedTime).arg(remark);
		QSqlQuery querySet(sqlSet, db);
		int num = querySet.numRowsAffected();

        //计算md5    
        if (num > 0)
        {
            int rowId = (querySet.lastInsertId().toInt());//获取更新或者插入的id
            patientsAnalyse.patientAnalyseId = rowId;
            calMd5patientsanalyse(rowId, true);
        }

        messageBoxSet(false, num, querySet.lastError().text());

		return num;
}

int GLDb::setPatientAnalyseChangedTime(PatientsAnalyse& patientsAnalyse)
{
    emit sig_log("数据库操作-更新患者新建分析的记录更新时间");
    QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    QString sqlSet = QString("UPDATE patientsAnalyse SET changedTime = '%1' WHERE patientAnalyseId = %2")
        .arg(changedTime).arg(patientsAnalyse.patientAnalyseId);
    QSqlQuery querySet(sqlSet, db);
    int num = querySet.numRowsAffected();

    //计算md5
    if (num > 0)
    {
        int rowId = (querySet.lastInsertId().toInt());//获取更新或者插入的id
        patientsAnalyse.patientAnalyseId = rowId;
        calMd5patientsanalyse(rowId, true);
    }

    messageBoxSet(true, num, querySet.lastError().text());

    return num;
}

int GLDb::setPatientAnalyseReport(PatientsAnalyse & patientsAnalyse)
{
    emit sig_log("数据库操作-设置患者新建分析报告");
    patientsAnalyse.changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");

    QString sql = QString("UPDATE PatientsAnalyse SET reportPath = '%1', changedTime = '%2' WHERE patientAnalyseId = %3")
        .arg(patientsAnalyse.reportPath).arg(patientsAnalyse.changedTime).arg(patientsAnalyse.patientAnalyseId);
    QSqlQuery query(sql, db);

    int num = query.numRowsAffected();
    //计算md5    
    if (num > 0)
    {
        int rowId = patientsAnalyse.patientAnalyseId;//获取更新或者插入的id
        calMd5patientsanalyse(rowId, true);
    }

    messageBoxSet(true, num, query.lastError().text());

    return num;
}

int GLDb::setPatientsAnalyseSeries(PatientsAnalyseSeries patientsAnalyseSeries)
{
    emit sig_log("数据库操作-设置患者新建分析序列清单");
    //先判定是否存在记录（病人id和分析名称）
    //QString sql = QString("SELET * FROM patientsAnalyse WHERE patientId = %1 AND analyseName = '%2'")
        //.arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName);
    //QSqlQuery queryExist(sql, db);

    QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    //bool exist = queryExist.next();
    //bool enableColumn = exist ? queryExist.value("enableColumn").toBool() : patientsAnalyse[i].enableColumn;
    bool enableColumn = patientsAnalyseSeries.enableColumn;
    //QString createTime = exist ? queryExist.value("createTime").toString() : changedTime;
    QString createTime = changedTime;
    //QString remark = exist ? queryExist.value("remark").toString() : patientsAnalyse[i].remark;
    QString remark = patientsAnalyseSeries.remark;


    /*
    //构造数据库插入语句，存在则更新，不存在则更新
    QString sql = exist ?
        QString("UPDATE patientsAnalyse SET patientId = %1, analyseName = %2, illnessKind = '%3',"
            "patientRemark = '%4', changedTime = %5, md5 = '%6'")
        .arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName).arg(patientsAnalyse[i].illnessKind)
        .arg(patientsAnalyse[i].patientRemark).arg(md5_res)
        .arg(changedTime).arg(md5_res) :
        QString("INSERT INTO patitentsOperation"
            "(patientId, analyseName, illnessKind, patientRemark, enableColumn, createTime, changedTime, remark, md5) "
            "VALUES(%1, '%2', '%3', '%4', %5, '%6', '%7', '%8', '%9')")
        .arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName).arg(patientsAnalyse[i].illnessKind).arg(patientsAnalyse[i].patientRemark)
        .arg(enableColumn).arg(createTime).arg(changedTime).arg(remark).arg(md5_res);
    */
    QString sqlSet = QString("INSERT INTO PatientsAnalyseSeries"
        "(patientAnalyseId, patientsSeriesId, enableSeries, enableColumn, createTime, changedTime, remark) "
        "VALUES(%1, %2, %3, %4, '%5', '%6', '%7')")
        .arg(patientsAnalyseSeries.patientAnalyseId).arg(patientsAnalyseSeries.patientsSeriesId).arg(patientsAnalyseSeries.enableSequence)
        .arg(enableColumn).arg(createTime).arg(changedTime).arg(remark);
    QSqlQuery querySet(sqlSet, db);
    int num = querySet.numRowsAffected();

    //计算md5    
    if (num > 0)
    {
        int rowId = (querySet.lastInsertId().toInt()); //获取更新或者插入的id
        calMd5patientsanalysesequencelist(rowId, true);
    }

    messageBoxSet(true, num, querySet.lastError().text());

    return num;
}

int GLDb::getPatientId(Patient patient)
{
	//获取patientId
	QString sqlPatientId = QString("SELECT * FROM Patients WHERE dicomPatientId = '%1' AND enableColumn = true")
		.arg(patient.dicomPatientId);
	QSqlQuery queryPatientId(sqlPatientId, db);
	bool res = queryPatientId.next();
	if (res)
		return queryPatientId.value("patientId").toInt();
	return -1;
}

int GLDb::setAccountIsFirstUse(Account& account)
{
	QString sql = QString("UPDATE accounts SET isFirstUse = %1 WHERE accountId = %2 AND enableColumn = true").arg(account.isFirstUse).arg(account.accountId);
	QSqlQuery query(sql, db);
	int num = query.numRowsAffected();

	//计算md5
  calMd5accounts(account.accountId, true);

	return num;
}

//设置密码（比如更改密码）
int GLDb::setPassword(Account _account)
{
  QString sql = QString("UPDATE accounts SET _password = '%1' WHERE userName = '%2' AND enableColumn = true")
    .arg(_account._password).arg(_account.userName);
  QSqlQuery query(sql, db);

  int num = query.numRowsAffected(); 
  GLMessageBox msg;
  if (num > 0)
  {
    //计算md5    
    QString sql1 = QString("SELECT accountId FROM accounts WHERE userName = '%1' AND enableColumn = true")
      .arg(_account.userName);
    QSqlQuery query1(sql1, db);
    if (query1.next())
    {
      int rowId = query1.value(0).toInt();
	    calMd5accounts(rowId, true);
    }

    msg.queRenInfo(QStringLiteral("修改密码"), QStringLiteral("密码修改成功！").arg(num));
  }
  else
      msg.queRenInfo(QStringLiteral("修改密码"), QStringLiteral("密码修改失败：%1").arg(query.lastError().text()));

  return num;
}

QVector<PatientsAnalyse> GLDb::getPatientsAnalyse(int n, int sort)
{
    QString limitRows;
    QString sortStr;
    if (sort == 0)
    {
        sortStr = "";
    }       
    else if (sort == 2)
    {
        sortStr = "ORDER BY changedTime DESC";
    }
    else if (sort == 1)
    {
        sortStr = "ORDER BY changedTime ASC";
    }
    limitRows = (n == 10000 ? "" : QString("LIMIT %1").arg(10000));

    QString sql = QString("SELECT * FROM patientsanalyse WHERE accountId = %1 AND enableColumn = true %2 %3")
        .arg(glAccount.accountId).arg(sortStr).arg(limitRows);
    QSqlQuery query(sql, db);

    QVector<PatientsAnalyse> patientAnalyseVec;
    int i = 1;
    while (query.next())
    {
        PatientsAnalyse patientsAnalyse;
        patientsAnalyse.patientAnalyseId = query.value("patientAnalyseId").toInt();
        QString curDirPath = QCoreApplication::applicationDirPath();
        QString backupPath = QStringLiteral("%1/%2").arg(curDirPath).arg("bak");
        auto fileName = QStringLiteral("%1/%2.mrb").arg(backupPath).arg(patientsAnalyse.patientAnalyseId);
        QFileInfo qfile(fileName);
        if (!qfile.isFile()) {
            continue;
        }

        patientsAnalyse.accountId = query.value("accountId").toInt();
        patientsAnalyse.illnessKind = query.value("illnessKind").toString();
        patientsAnalyse.analyseName = query.value("analyseName").toString();
        patientsAnalyse.doctorRemark = query.value("doctorRemark").toString();
        patientsAnalyse.patientName = query.value("patientName").toString();
        patientsAnalyse.birthday = query.value("birthday").toString();

		patientsAnalyse.age = QString::number(GLTool::calAge(patientsAnalyse.birthday));
        patientsAnalyse.sex = query.value("sex").toString();
        patientsAnalyse.height = query.value("height").toString();
        patientsAnalyse.weight = query.value("weight").toString();
        patientsAnalyse.reportPath = query.value("reportPath").toString();
        patientsAnalyse.changedTime = query.value("changedTime").toString();

        patientAnalyseVec.append(patientsAnalyse);
        i += 1;
        if (i == n) {
            break;
        }
    }

    return patientAnalyseVec;
}

PatientsAnalyse GLDb::getPatientsAnalyse1(int patientAnalyseId)
{

    QString sql = QString("SELECT * FROM patientsanalyse WHERE patientAnalyseId = %1 AND enableColumn = true")
        .arg(patientAnalyseId);
    QSqlQuery query(sql, db);

    PatientsAnalyse patientsAnalyse;
    while (query.next())
    {
        patientsAnalyse.patientAnalyseId = query.value("patientAnalyseId").toInt();
        
        patientsAnalyse.accountId = query.value("accountId").toInt();
        patientsAnalyse.illnessKind = query.value("illnessKind").toString();
        patientsAnalyse.analyseName = query.value("analyseName").toString();
        patientsAnalyse.doctorRemark = query.value("doctorRemark").toString();
        patientsAnalyse.patientName = query.value("patientName").toInt();
        patientsAnalyse.birthday = query.value("birthday").toString();
		patientsAnalyse.age = QString::number(GLTool::calAge(patientsAnalyse.birthday));
        patientsAnalyse.sex = query.value("sex").toString();
        patientsAnalyse.height = query.value("height").toString();
        patientsAnalyse.weight = query.value("weight").toString();
        patientsAnalyse.reportPath = query.value("reportPath").toString();
        patientsAnalyse.changedTime = query.value("changedTime").toString();       
    }   
    return patientsAnalyse;
}

int GLDb::setSurgerySummary(Patient & patient)
{
  QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
  QString sqlSet = QString("UPDATE Patients SET surgerSummary = '%1', recordModificationTime = '%2', changedTime = '%2' WHERE patientId = %3 AND enableColumn = true")
    .arg(patient.surgerSummary).arg(changedTime).arg(patient.patientId);
  QSqlQuery querySet(sqlSet, db);
  int num = querySet.numRowsAffected();

  //计算md5
  if (num > 0)
  {
      calMd5patients(patient.patientId);
  }

  messageBoxSet(true, num, querySet.lastError().text());
  return num;
}

int GLDb::setPatientAnalyseIdPatientIdId(PatientAnalyseIdPatientIdId patientAnalyseIdPatientIdId)
{
    emit sig_log("数据库操作-设置患者新建分析Id患者Id");
    QString changedTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    //医生登录时，新建的分析都是当时新建的，所以不需要判断存在
    /*
    QString sqlExist = QString("SELECT * FROM patientsAnalyse WHERE patientId = %1 AND analyseName = '%2'")
        .arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName);
    QSqlQuery queryExist(sqlExist, db);
    bool exist = queryExist.next();
    */

    //bool enableColumn = exist ? queryExist.value("enableColumn").toBool() : patientsAnalyse[i].enableColumn;
    //bool enableColumn = patientsAnalyse.enableColumn;
    //QString createTime = exist ? queryExist.value("createTime").toString() : changedTime;
    QString createTime = changedTime;
    //QString remark = exist ? queryExist.value("remark").toString() : patientsAnalyse[i].remark;
    //QString remark = patientsAnalyse.remark;

    //构造数据库插入语句，存在则更新，不存在则更新
    /*
    QString sqlSet = exist ?
        QString("UPDATE patientsAnalyse SET patientId = %1, analyseName = %2, illnessKind = '%3',"
            "patientRemark = '%4', changedTime = %5, md5 = '%6'")
            .arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName).arg(patientsAnalyse[i].illnessKind)
            .arg(patientsAnalyse[i].patientRemark).arg(md5_res)
            .arg(changedTime).arg(md5_res) :
        QString("INSERT INTO patitentsOperation"
            "(patientId, analyseName, illnessKind, patientRemark, enableColumn, createTime, changedTime, remark, md5) "
            "VALUES(%1, '%2', '%3', '%4', %5, '%6', '%7', '%8', '%9')")
            .arg(patientsAnalyse[i].patientId).arg(patientsAnalyse[i].analyseName).arg(patientsAnalyse[i].illnessKind).arg(patientsAnalyse[i].patientRemark)
            .arg(enableColumn).arg(createTime).arg(changedTime).arg(remark).arg(md5_res);
    */
    QString sqlSet = QString("INSERT INTO PatientsAnalyseIdPatientId"
        "(patientAnalyseId, patientId, createTime, changedTime) "
        "VALUES(%1, %2, '%3', '%4')")
        .arg(patientAnalyseIdPatientIdId.patientAnalyseId).arg(patientAnalyseIdPatientIdId.patientId)
        .arg(createTime).arg(changedTime);
    QSqlQuery querySet(sqlSet, db);
    int num = querySet.numRowsAffected();

    //计算md5    
    if (num > 0)
    {
        int rowId = querySet.lastInsertId().toInt();//获取更新或者插入的id
        calMd5patientsAnalyseIdPatientId(rowId, true);
    }

    messageBoxSet(true, num, querySet.lastError().text());

    return num;

}

//获取某位患者的所有序列
QVector<PatientsSeries> GLDb::getPatientsSeries(Patient patient)
{
    QString sql = QString("SELECT * FROM PatientsSeries WHERE patientId = %1 AND enableColumn = true").arg(patient.patientId);
    QSqlQuery query(sql, db);

    QVector<PatientsSeries> PatientsSeriesVec;
    while (query.next())
    {
        PatientsSeries list;
        list.patientsSeriesId = query.value("patientsSeriesId").toInt();
        list.patientId = query.value("patientId").toInt();
        list.dicomPatientId = query.value("dicomPatientId").toString();
        list.dicomPatientId = query.value("dicomPatientId").toString();
        list.accessionNumber = query.value("accessionNumber").toString();
        list.dicomBirthday = query.value("dicomBirthday").toString();
        list.dicomSex = query.value("dicomSex").toString();
        list.seriesInstanceUID = query.value("seriesInstanceUID").toString();
        list.seriesNumber = query.value("seriesNumber").toInt();
        list.scanTime = query.value("scanTime").toString();
        list.seriesDescribe = query.value("seriesDescribe").toString();
        list.modality = query.value("modality").toString();
        list.pixelSpacing = query.value("pixelSpacing").toString();
        list.thickness = query.value("thickness").toString();
        list.spaceBetween = query.value("spaceBetween").toString();
        list.sliceCount = query.value("sliceCount").toInt();
        list.seriesDir = query.value("seriesDir").toString();
        list.screenshot = query.value("screenshot").toString();
        list.enableSeries = query.value("enableSeries").toInt();
        list.enableColumn = query.value("enableColumn").toInt();
        list.createTime = query.value("createTime").toString();
        list.changedTime = query.value("changedTime").toString();
        list.remark = query.value("remark").toString();
        list.md5 = query.value("md5").toString();
        //验证md5是否有效待加
        PatientsSeriesVec.append(list);
    }
    return PatientsSeriesVec;

}

QVector<Patient> GLDb::getPatientIdFromPatientAnalyseIdPatientIdId(PatientsAnalyse patientsAnalyse)
{
    QString sql = QString("SELECT * FROM PatientsAnalyseIdPatientId WHERE patientAnalyseId = %1 AND enableColumn = true")
        .arg(patientsAnalyse.patientAnalyseId);
    QSqlQuery query(sql, db);

    QVector<Patient> patientVec;
    while (query.next())
    {
        Patient patient;
        patient.patientId = query.value("patientId").toInt();
        patientVec.append(patient);
    }

    return patientVec;
}

QVector<int> GLDb::getPatientAnalyseIdFromPatientAnalyseIdPatientIdId(Patient patient)
{
    QString sql = QString("SELECT * FROM PatientsAnalyseIdPatientId WHERE patientId = %1 AND enableColumn = true")
        .arg(patient.patientId);
    QSqlQuery query(sql, db);

    QVector<int> patientAnalyseIdVec;
    while (query.next())
    {
        int patientAnalyseId = query.value("patientAnalyseId").toInt();
        patientAnalyseIdVec.append(patientAnalyseId);
    }

    return patientAnalyseIdVec;
}

int GLDb::setSoftwareOperationRecord(SoftwareOperationRecord& record)
{
	QString createTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
	record.createTime = createTime;
	QString changeTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
	record.changedTime = changeTime;

	QString sqlSet = QString("INSERT INTO softwareoperationrecord"
		"(content, classes, module, accountId, userName, enableColumn, createTime, changedTime, remark) "
		"VALUES('%1', '%2', '%3', %4, '%5', %6, '%7', '%8', '%9')")
		.arg(record.content).arg(record.classes).arg(record.module)
		.arg(glAccount.accountId).arg(glAccount.userName)
		.arg(record.enableColumn).arg(record.createTime).arg(record.changedTime).arg(record.remark);
	QSqlQuery querySet(sqlSet, db);
	int num = querySet.numRowsAffected();

	//num > 0 ? QMessageBox::information(nullptr, QStringLiteral("数据库"), QStringLiteral("成功插入%1行！").arg(num)) :
	//	QMessageBox::warning(nullptr, "数据库", QStringLiteral("插入失败：%1").arg(querySet.lastError().text()));

	//计算md5
	int lastInsertId = querySet.lastInsertId().toInt(); //返回最后插入的Id
	calMd5softwareoperationrecord(lastInsertId, true);

	return num;
}

int GLDb::setFiberWaterConnectRecord(FiberWaterConnectRecord & record)
{
  //创建时间和改变时间（每产生一条记录，就生成一行，一般情况下，是不会更改的，所以更改时间一般是创建时间（外部更改除外）
  QString createTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
  record.createTime = createTime;
  QString changeTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
  record.changedTime = changeTime;

  //插入数据库
  QString sqlSet = QString("INSERT INTO fiberWaterConnectRecord"
    "(content, recordTime, device, accountId, userName, enableColumn, createTime, changedTime, remark) "
    "VALUES('%1', '%2', '%3', %4, '%5', %6, '%7', '%8', '%9')")
    .arg(record.content).arg(record.recordTime).arg(record.device)
    .arg(glAccount.accountId).arg(glAccount.userName)
    .arg(record.enableColumn).arg(record.createTime).arg(record.changedTime).arg(record.remark);
  QSqlQuery querySet(sqlSet, db);
  int num = querySet.numRowsAffected();

  //更新md5
  int lastInsertId = querySet.lastInsertId().toInt(); //返回最后插入的Id
  calMd5fiberWaterConnectRecord(lastInsertId, true);

  return num;
}

//将所有列的值按照列定义的顺序首尾拼接，中间用pkay隔开，bool值转换为oka，前后加上前缀jui和后缀xqp，然后对此行所有计算md5的值
QString GLDb::md5(QStringList strList)
{
	//对strList排序，为了确保使用SELECT *时，字段的拼接顺序是一定的
	qSort(strList);
	QString joinStr = strList.join("pkay"); //使用pkay相连
	QString addStr = "jui" + joinStr + "xqp";
	QCryptographicHash md(QCryptographicHash::Md5);
	md.addData(addStr.toUtf8());
	QByteArray bb = md.result();
	return bb.toHex();
}

quint8 GLDb::calAge(QString birthday)
{
	int year = birthday.section('/', 0, 0).toInt();
	int month = birthday.section('/', 1, 1).toInt();
	int currentYear = QDate::currentDate().year();
	int currentMonth = QDate::currentDate().month();
	int age = currentYear - year;
	if (currentMonth < month)
		age--;
	return age;
}

void GLDb::messageBoxSet(bool exist, int num, QString errorString)
{
    GLMessageBox msg;
  if (exist)
  {
    if (!errorString.trimmed().isEmpty()) //使用num不能排除更新结果没有变化的情况（此时num等于0）
    {
        msg.queRenError(QStringLiteral("数据库操作"), QStringLiteral("更新失败：%1").arg(errorString));
    }
    else
    {
      //GLMessageDialog::ShowInformation(QStringLiteral("数据库操作"), QStringLiteral("成功更新%1行！").arg(num));
    }
  }
  else
  {
    if (!errorString.trimmed().isEmpty())
    {
        msg.queRenError(QStringLiteral("数据库操作"), QStringLiteral("插入失败：%1").arg(errorString));
    }
    else
    {
      //GLMessageDialog::ShowInformation(QStringLiteral("数据库操作"), QStringLiteral("成功插入%1行！").arg(num));
    }
  }
}

void GLDb::slot_heartbeat()
{
	QSqlQuery query("SELECT 1");
}
