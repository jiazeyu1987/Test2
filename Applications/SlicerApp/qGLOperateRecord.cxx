#include "qGLOperateRecord.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>

//简单的存储光纤数据靶点, 入点和光纤名称
RecordFiberData::RecordFiberData()
{
}

//RecordFiberData::RecordFiberData(mitk::Point3D point_input, mitk::Point3D point_target, const QString& fiber_name)
//{
//	this->pInput = point_input;
//	this->pTarget = point_target;
//	this->fiberName = fiber_name;
//}

RecordFiberData::~RecordFiberData() {

}

GLOperateRecord * GLOperateRecord::Instance = new GLOperateRecord();

GLOperateRecord * GLOperateRecord::GetInstance() {
	return Instance;
}

GLOperateRecord::GLOperateRecord() : isHaveRecord(false) {
	InitData();
}

GLOperateRecord::~GLOperateRecord(){
}
//通过key记录一个QString类型的数据
//isSave默认值为true同时记录多次时isSave设为false, 记录完成之后设为true
void GLOperateRecord::RecordData(const QString& key, const QString& value, bool isSave)
{
	info[key] = value;
	if (isSave) SaveJson();
}

//通过key记录一个int类型的数据
//isSave默认值为true同时记录多次时isSave设为false, 记录完成之后设为true
void GLOperateRecord::RecordData(const QString & key, const int value, bool isSave)
{
	info[key] = value;
	if (isSave) SaveJson();
}

//同时记录多个数据, 从一个map里读取
void GLOperateRecord::RecordData(QMap<QString, QString> map)
{
	QMap<QString, QString>::Iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		RecordData(it.key(), it.value(), false);
	}
	SaveJson();
}
//同时记录多个数据, 从一个map里读取
void GLOperateRecord::RecordData(QMap<QString, int> map)
{
	QMap<QString, int>::Iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		RecordData(it.key(), it.value(), false);
	}
	SaveJson();
}

//记录光纤数据
//void GLOperateRecord::RecordFiber(QVector<RecordFiberData *> dataVector) {
//	QJsonArray fiberDataArr;
//	for (QVector<RecordFiberData*>::iterator it = dataVector.begin(); it != dataVector.end(); ++it) {
//		QJsonObject data;
//		QJsonObject inputData;
//		QJsonObject targetData;
//		data["FiberName"] = (*it)->fiberName;
//		inputData["X"] = (*it)->pInput[0];
//		inputData["Y"] = (*it)->pInput[1];
//		inputData["Z"] = (*it)->pInput[2];
//		data["Input"] = inputData;
//		targetData["X"] = (*it)->pTarget[0];
//		targetData["Y"] = (*it)->pTarget[1];
//		targetData["Z"] = (*it)->pTarget[2];
//		data["Target"] = targetData;
//		fiberDataArr.append(data);
//	}
//	info["FiberData"] = fiberDataArr;
//	SaveJson();
//}

//记录2D图片数据
void GLOperateRecord::Record2DImageData(const QString & imageName, const QString & type, const int value)
{
	QJsonObject data;
	if (info.contains(imageName)) {
		data = info[imageName].toObject();
	}
	data[type] = value;
	info[imageName] = data;
	SaveJson();
}

//记录2D图片颜色数据
void GLOperateRecord::Record2DImageColorData(const QString & imageName, const int rColor, const int gColor, const int bColor)
{
	QJsonObject data;
	if (info.contains(imageName))
		data = info[imageName].toObject();
	data["R"] = rColor;
	data["G"] = gColor;
	data["B"] = bColor;
	info[imageName] = data;
	SaveJson();
}

//void GLOperateRecord::RecordTempPoint(const QString& fiberName, QVector<mitk::Point3D> pointList)
//{
//	QJsonArray pointDataArr;
//	for (QVector<mitk::Point3D>::iterator it = pointList.begin(); it != pointList.end(); ++it) {
//		QJsonObject pointData;
//		pointData["X"] = (*it)[0];
//		pointData["Y"] = (*it)[1];
//		pointData["Z"] = (*it)[2];
//		pointDataArr.append(pointData);
//	}
//	info["TempPoint"] = pointDataArr;
//	SaveJson();
//}

//初始化存档文件和存档数据
void GLOperateRecord::InitData() {
	isUseRecord = false;
	fileName = qApp->applicationDirPath() + "/record.xml";
	QDir dir(fileName);
	QFile loadFile(fileName);
	if (!loadFile.exists())
	{
	}
	else {
		if (!loadFile.open(QIODevice::ReadOnly)) {
			return;
		}
		isHaveRecord = true;
		QByteArray saveData = loadFile.readAll();
		QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);
		recordInfo = loadDoc.object();
		QStringList list = recordInfo.keys();
		//SaveJson();
	}
}

void GLOperateRecord::SaveJson() {
	QFile saveFile(fileName);
	if (!saveFile.open(QIODevice::WriteOnly)) {
		return;
	}
	QJsonDocument saveDoc(info);
	saveFile.write(saveDoc.toJson());
}

//根据key获取一个字符串
QString GLOperateRecord::GetStringByKey(const QString key, bool isDelete)
{
	if (!isHaveRecord) return "";
	QString result = "";
	if (recordInfo.contains(key)) {
		result = recordInfo[key].toString();
		if(isDelete) recordInfo.remove(key);
	}
	return result;
}

//根据key获取一个Number
int GLOperateRecord::GetNumberByKey(const QString key, bool isDelete) {
	if (!isHaveRecord) return -1;
	int result = -1;
	if (recordInfo.contains(key))
	{
		result = recordInfo[key].toInt();
		if (isDelete) recordInfo.remove(key);
	}
	return result;
}

//正常退出删除备份文件
void GLOperateRecord::DeleteJsonFile() {
	//QFile::remove(fileName);
	QStringList names;
	names.append("record.xml");
	names.append("record_wrapper.mapr");
	names.append("record.nhdr");
	names.append("record_basic.nhdr");
	for (int i = 0; i < names.count(); i++) {
		QString path = qApp->applicationDirPath() + "/" + names[i];
		QFile file(path);
		if (file.exists()) {
			QFile::remove(path);
		}
	}
}

//获取2D图片数据
void GLOperateRecord::Get2DImageData(const QString & imageName, int & isOpen, int & opacity, int & rColor, int & gColor, int & bColor)
{
	if (recordInfo.contains(imageName)) {
		QJsonObject data = recordInfo[imageName].toObject();
		isOpen = data.contains("IsOpen") ? data["IsOpen"].toInt() : -1;
		opacity = data.contains("Opacity") ? data["Opacity"].toInt() : -1;
		rColor = data.contains("R") ? data["R"].toInt() : -1;
		gColor = data.contains("G") ? data["G"].toInt() : -1;
		bColor = data.contains("B") ? data["B"].toInt() : -1;
		recordInfo.remove(imageName);
	}
}

//获取光纤数据, 结果存在dataVector里
void GLOperateRecord::GetFiberData(QVector<RecordFiberData*>& dataVector)
{
	//if (recordInfo.contains("FiberData") && recordInfo["FiberData"].isArray()) {
	//	QJsonArray dataArr = recordInfo["FiberData"].toArray();
	//	for (int i = 0; i < dataArr.size(); ++i) {
	//		QJsonObject data = dataArr[i].toObject();
	//		RecordFiberData *fiberData = new RecordFiberData();
	//		fiberData->fiberName = data["FiberName"].toString();
	//		QJsonObject inputData = data["Input"].toObject();
	//		fiberData->pInput[0] = inputData["X"].toDouble();
	//		fiberData->pInput[1] = inputData["Y"].toDouble();
	//		fiberData->pInput[2] = inputData["Z"].toDouble();
	//		QJsonObject targetData = data["Target"].toObject();
	//		fiberData->pTarget[0] = targetData["X"].toDouble();
	//		fiberData->pTarget[1] = targetData["Y"].toDouble();
	//		fiberData->pTarget[2] = targetData["Z"].toDouble();
	//		dataVector.append(fiberData);
	//	}
	//	recordInfo.remove("FiberData");
	//}
}

//void GLOperateRecord::GetTempPoint(const QString& fiberName, QVector<mitk::Point3D>& pointList)
//{
//	if (recordInfo.contains("TempPoint") && recordInfo["TempPoint"].isArray()) {
//		QJsonArray dataArr = recordInfo["TempPoint"].toArray();
//		for (int i = 0; i < dataArr.size(); ++i) {
//			QJsonObject data = dataArr[i].toObject();
//			mitk::Point3D point;
//			point[0] = data["X"].toDouble();
//			point[1] = data["Y"].toDouble();
//			point[2] = data["Z"].toDouble();
//			pointList.append(point);
//		}
//		recordInfo.remove("FiberData");
//	}
//}

void GLOperateRecord::DeleteKey(const QString key)
{
	info.remove(key);
	SaveJson();
}

