#pragma once
/*
QString abv = curPath + "/test/1.xml";
GLCommonXML *xml = new GLCommonXML(abv);
xml->SaveInfo("abcf", true, "er");
xml->SaveInfo("ab1", 1, "er");
int num = xml->GetIntInfo("abc");
bool isa = xml->GetBoolInfo("abcf", "er");
xml->DeleteKey("abc");
xml->DeleteKey("ab1", "er");
xml->DeleteKey("er");
*/
#include <QObject>
#include <QJsonObject>
#include "qSlicerAppExport.h"
class Q_SLICER_APP_EXPORT GLCommonXML : public QObject
{
	Q_OBJECT
public:
	GLCommonXML(const QString& path);
	~GLCommonXML();
	void SaveInfo(const QString& key, const QString& value, const QString& parentKey="", const bool isSave=true);
	void SaveInfo(const QString& key, const char* value, const QString& parentKey="", const bool isSave=true);
	void SaveInfo(const QString& key, const int value, const QString& parentKey = "", const bool isSave = true);
	void SaveInfo(const QString& key, const bool value, const QString& parentKey = "", const bool isSave = true);
	void SaveInfo(const QString& key, const double value, const QString& parentKey = "", const bool isSave = true);
	QString GetStringInfo(const QString& key, const QString& parentKey = "");
	int GetIntInfo(const QString& key, const QString& parentKey = "");
	double GetDoubleInfo(const QString& key, const QString& parentKey = "");
	bool GetBoolInfo(const QString& key, const QString& parentKey = "");
	void DeleteKey(const QString& key, const QString& parentKey = "");
	void SaveJson();
private:
	void InitData();
private :
	QJsonObject info;
	QString filePath;
};

