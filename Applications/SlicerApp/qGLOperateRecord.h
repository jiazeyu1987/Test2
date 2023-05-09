#pragma once
#include <QJsonObject>
#include <QObject>
#include "qSlicerAppExport.h"
#include <QCoreApplication>
#include <QMap>
#include <QVector>

#if defined(GLORecord)
#undef GLORecord
#endif
#define GLORecord (static_cast<GLOperateRecord *>(GLOperateRecord::GetInstance()))
class Q_SLICER_APP_EXPORT RecordFiberData {
public:
	RecordFiberData();
	//RecordFiberData(mitk::Point3D point_input, mitk::Point3D point_target, const QString& fiber_name);
	~RecordFiberData();
	//mitk::Point3D pInput;
	//mitk::Point3D pTarget;
	QString fiberName;
};
class Q_SLICER_APP_EXPORT GLOperateRecord : public QObject
{
	Q_OBJECT
public :
	static GLOperateRecord* GetInstance();
	~GLOperateRecord();
	void RecordData(const QString& key, const QString& value, bool isSave = true);
	void RecordData(const QString& key, const int value, bool isSave = true);
	void RecordData(QMap<QString, QString> map);
	void RecordData(QMap<QString, int> map);
	void RecordFiber(QVector<RecordFiberData *> dataVector);
	void Record2DImageData(const QString& imageName, const QString& type, const int value);
	void Record2DImageColorData(const QString& imageName, const int rColor, const int gColor, const int bColor);
	//void RecordTempPoint(const QString& fiberName, QVector<mitk::Point3D> pointList);
	QString GetStringByKey(const QString key, bool isDelete=true);
	int GetNumberByKey(const QString key, bool isDelete = true);
	void DeleteJsonFile();
	void Get2DImageData(const QString& imageName, int& isOpen, int& opacity, int& rColor, int& gColor, int& bColor);
	void GetFiberData(QVector<RecordFiberData *> &dataVector);
	//void GetTempPoint(const QString& fiberName, QVector<mitk::Point3D>& pointList);
	void DeleteKey(const QString key);
private:
	GLOperateRecord();
	void InitData();
	void SaveJson();

public:
	bool isHaveRecord;
	bool isUseRecord;

private:
	static GLOperateRecord *Instance;
	//存储本次操作数据
	QJsonObject info;
	//存储上次上次操作数据
	QJsonObject recordInfo;
	QString fileName;
};

