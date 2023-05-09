#include "GLCommonXML.h"
#include "QFile"
#include "QDir"
#include "QJsonDocument"

//��־
//#include <mbilog.h>
//#include <mitkLogMacros.h>
//#include <mitkLog.h>

GLCommonXML::GLCommonXML(const QString& path):filePath(path)
{
	InitData();
}

GLCommonXML::~GLCommonXML()
{
	QStringList list = info.keys();
	for (int i = 0; i < list.count(); i++) {
		info.remove(list[i]);
	}
}
//�����ַ������͵�����
//parentKeyĬ��Ϊ��, ��Ϊ��ʱ, ��һ��parentKey�Ķ���, ���������key��ֵ
//isSaveĬ��Ϊtrue, ����ͬʱ�������ֶε�ʱ��, ��Ϊfalse, Ȼ���ֶ�����һ��SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const QString & value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "������Ϣ��xml�ļ�" << " key:" << key.toStdString() << " value:" << value.toStdString() << " parentKey:" << parentKey.toStdString() << " �Ƿ񱣴棺" << isSave;
	if (parentKey == "") {
		info[key] = value;
	}
	else {
		QJsonObject data;
		if (info.contains(parentKey)) {
			data = info[parentKey].toObject();
		}
		data[key] = value;
		info[parentKey] = data;
	}
	if (isSave) SaveJson();
}
//�����ַ������͵�����
//parentKeyĬ��Ϊ��, ��Ϊ��ʱ, ��һ��parentKey�Ķ���, ���������key��ֵ
//isSaveĬ��Ϊtrue, ����ͬʱ�������ֶε�ʱ��, ��Ϊfalse, Ȼ���ֶ�����һ��SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const char*  value, const QString& parentKey, const bool isSave)
{
	if (parentKey == "") {
		info[key] = value;
	}
	else {
		QJsonObject data;
		if (info.contains(parentKey)) {
			data = info[parentKey].toObject();
		}
		data[key] = value;
		info[parentKey] = data;
	}
	if (isSave) SaveJson();
}

//����int���͵�����
//parentKeyĬ��Ϊ��, ��Ϊ��ʱ, ��һ��parentKey�Ķ���, ���������key��ֵ
//isSaveĬ��Ϊtrue, ����ͬʱ�������ֶε�ʱ��, ��Ϊfalse, Ȼ���ֶ�����һ��SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const int value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "������Ϣ��xml�ļ�" << " key:" << key.toStdString() << " value:" << value << " parentKey:" << parentKey.toStdString() << " �Ƿ񱣴棺" << isSave;
	if (parentKey == "") {
		info[key] = value;
	}
	else {
		QJsonObject data;
		if (info.contains(parentKey)) {
			data = info[parentKey].toObject();
		}
		data[key] = value;
		info[parentKey] = data;
	}
	if (isSave) SaveJson();
}

//����bool���͵�����
//parentKeyĬ��Ϊ��, ��Ϊ��ʱ, ��һ��parentKey�Ķ���, ���������key��ֵ
//isSaveĬ��Ϊtrue, ����ͬʱ�������ֶε�ʱ��, ��Ϊfalse, Ȼ���ֶ�����һ��SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const bool value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "������Ϣ��xml�ļ�" << " key:" << key.toStdString() << " value:" << value << " parentKey:" << parentKey.toStdString() << " �Ƿ񱣴棺" << isSave;
	if (parentKey == "") {
		info[key] = value;
	}
	else {
		QJsonObject data;
		if (info.contains(parentKey)) {
			data = info[parentKey].toObject();
		}
		data[key] = value;
		info[parentKey] = data;
	}
	if (isSave) SaveJson();
}

//����double���͵�����
//parentKeyĬ��Ϊ��, ��Ϊ��ʱ, ��һ��parentKey�Ķ���, ���������key��ֵ
//isSaveĬ��Ϊtrue, ����ͬʱ�������ֶε�ʱ��, ��Ϊfalse, Ȼ���ֶ�����һ��SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const double value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "������Ϣ��xml�ļ�" << " key:" << key.toStdString() << " value:" << value << " parentKey:" << parentKey.toStdString() << " �Ƿ񱣴棺" << isSave;
	if (parentKey == "") {
		info[key] = value;
	}
	else {
		QJsonObject data;
		if (info.contains(parentKey)) {
			data = info[parentKey].toObject();
		}
		data[key] = value;
		info[parentKey] = data;
	}
	if (isSave) SaveJson();
}

//����key��ȡһ��QString
//parentKeyĬ��Ϊ��, ȡkey-value, ��Ϊ��ȡparentKey�ڵ���key-value
QString GLCommonXML::GetStringInfo(const QString & key, const QString& parentKey)
{
	if (parentKey == "") {
		if (!info.contains(key)) return "";
		return info[key].toString();
	}
	QJsonObject data = info[parentKey].toObject();
	if (!data.contains(key)) return "";
	return data[key].toString();
}

//����key��ȡһ��int
//parentKeyĬ��Ϊ��, ȡkey-value, ��Ϊ��ȡparentKey�ڵ���key-value
int GLCommonXML::GetIntInfo(const QString & key, const QString& parentKey)
{
	if (parentKey == "") {
		if (!info.contains(key)) return -1;
		return info[key].toInt();
	}
	QJsonObject data = info[parentKey].toObject();
	if (!data.contains(key)) return -1;
	return data[key].toInt();
}
//����key��ȡһ��double
//parentKeyĬ��Ϊ��, ȡkey-value, ��Ϊ��ȡparentKey�ڵ���key-value
double GLCommonXML::GetDoubleInfo(const QString & key, const QString& parentKey)
{
	if (parentKey == "") {
		if (!info.contains(key)) return -1;
		return info[key].toDouble();
	}
	QJsonObject data = info[parentKey].toObject();
	if (!data.contains(key)) return -1;
	return data[key].toDouble();
}
//����key��ȡһ��bool
//parentKeyĬ��Ϊ��, ȡkey-value, ��Ϊ��ȡparentKey�ڵ���key-value
bool GLCommonXML::GetBoolInfo(const QString & key, const QString& parentKey)
{
	if (parentKey == "") {
		if (!info.contains(key)) return false;
		return info[key].toBool();
	}
	QJsonObject data = info[parentKey].toObject();
	if (!data.contains(key)) return false;
	return data[key].toBool();
}

//ɾ���ڵ�����
void GLCommonXML::DeleteKey(const QString & key, const QString& parentKey)
{
	if(parentKey == "")	info.remove(key);
	else {
		if (info.contains(parentKey)) {
			QJsonObject data = info[parentKey].toObject();
			data.remove(key);
			info[parentKey] = data;
		}
	}
	SaveJson();
}

//��ʼ���ļ�, ��ȡ��Ϣ
void GLCommonXML::InitData()
{
	QFile file(filePath);
	if (!file.exists()) {
		int first = filePath.lastIndexOf("/");
		//����Ŀ¼
		QString dirpath = filePath.mid(0, first);
		QDir dir(dirpath);
		if (!dir.exists())
			dir.mkdir(dirpath);

		//�����ļ�
		if (dir.exists())
		{
			if (file.open(QIODevice::WriteOnly))
			{
				file.close();
			}
		}
	}

	QFile loadFile(filePath);
	if (!loadFile.open(QIODevice::ReadOnly)) {
		return;
	}
	QByteArray saveData = loadFile.readAll();
	QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);
	info = loadDoc.object();
}

void GLCommonXML::SaveJson()
{
	QFile saveFile(filePath);
	if (!saveFile.open(QIODevice::WriteOnly)) {
		return;
	}
	QJsonDocument saveDoc(info);
	saveFile.write(saveDoc.toJson());
	saveFile.close();
}
