#include "GLCommonXML.h"
#include "QFile"
#include "QDir"
#include "QJsonDocument"

//日志
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
//保存字符串类型的数据
//parentKey默认为空, 不为空时, 存一个parentKey的对象, 对象下面存key的值
//isSave默认为true, 用于同时保存多个字段的时候, 设为false, 然后手动调用一下SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const QString & value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "保存信息到xml文件" << " key:" << key.toStdString() << " value:" << value.toStdString() << " parentKey:" << parentKey.toStdString() << " 是否保存：" << isSave;
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
//保存字符串类型的数据
//parentKey默认为空, 不为空时, 存一个parentKey的对象, 对象下面存key的值
//isSave默认为true, 用于同时保存多个字段的时候, 设为false, 然后手动调用一下SaveJson();
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

//保存int类型的数据
//parentKey默认为空, 不为空时, 存一个parentKey的对象, 对象下面存key的值
//isSave默认为true, 用于同时保存多个字段的时候, 设为false, 然后手动调用一下SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const int value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "保存信息到xml文件" << " key:" << key.toStdString() << " value:" << value << " parentKey:" << parentKey.toStdString() << " 是否保存：" << isSave;
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

//保存bool类型的数据
//parentKey默认为空, 不为空时, 存一个parentKey的对象, 对象下面存key的值
//isSave默认为true, 用于同时保存多个字段的时候, 设为false, 然后手动调用一下SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const bool value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "保存信息到xml文件" << " key:" << key.toStdString() << " value:" << value << " parentKey:" << parentKey.toStdString() << " 是否保存：" << isSave;
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

//保存double类型的数据
//parentKey默认为空, 不为空时, 存一个parentKey的对象, 对象下面存key的值
//isSave默认为true, 用于同时保存多个字段的时候, 设为false, 然后手动调用一下SaveJson();
void GLCommonXML::SaveInfo(const QString & key, const double value, const QString& parentKey, const bool isSave)
{
  //MITK_INFO << "保存信息到xml文件" << " key:" << key.toStdString() << " value:" << value << " parentKey:" << parentKey.toStdString() << " 是否保存：" << isSave;
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

//根据key获取一个QString
//parentKey默认为空, 取key-value, 不为空取parentKey节点下key-value
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

//根据key获取一个int
//parentKey默认为空, 取key-value, 不为空取parentKey节点下key-value
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
//根据key获取一个double
//parentKey默认为空, 取key-value, 不为空取parentKey节点下key-value
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
//根据key获取一个bool
//parentKey默认为空, 取key-value, 不为空取parentKey节点下key-value
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

//删除节点数据
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

//初始化文件, 读取信息
void GLCommonXML::InitData()
{
	QFile file(filePath);
	if (!file.exists()) {
		int first = filePath.lastIndexOf("/");
		//创建目录
		QString dirpath = filePath.mid(0, first);
		QDir dir(dirpath);
		if (!dir.exists())
			dir.mkdir(dirpath);

		//创建文件
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
