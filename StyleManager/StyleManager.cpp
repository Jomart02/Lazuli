#include "StyleManager.h"
#include <QFile>

StyleManager::StyleManager(QObject *parent) : QObject(parent)
{
}

StyleManager::~StyleManager()
{
}

void StyleManager::setQssFile(const OwnerStyle &owner, const QString &qssFilePath)
{
    QFile file(qssFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open QSS file:" << qssFilePath;
        return;
    }
    m_qssFiles.insert(owner, file.readAll());
    file.close();
}


bool StyleManager::isRegistered(const OwnerStyle &owner){
    return m_qssFiles.contains(owner);
}
bool StyleManager::isRegisteredStyle(const OwnerStyle &owner, const NameStyle &styleName){
    if(isRegistered(owner)) return m_styles.value(owner).contains(styleName);
    else return false;
}

void StyleManager::addStyle(const OwnerStyle &owner, const NameStyle &styleName, const QString &jsonFilePath)
{
    QJsonObject jsonData = loadJson(jsonFilePath);
    if (!jsonData.isEmpty()) {
        m_styles[owner][styleName] = jsonData;
    } else {
        qWarning() << "Failed to load JSON for style:" << styleName;
    }
}

QString StyleManager::getStyle(const OwnerStyle &owner, const NameStyle &styleName)
{
    if (m_qssFiles.contains(owner) && m_styles.contains(owner) && m_styles[owner].contains(styleName)) {
        QString qssContent = m_qssFiles.value(owner);
        PropertyStyle jsonData = m_styles[owner].value(styleName);
        return parseQss(qssContent, jsonData);
    } else {
        qWarning() << "Style or QSS file not found for owner:" << owner << "and style:" << styleName;
        return QString();
    }
}

QJsonObject StyleManager::loadJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open JSON file:" << filePath;
        return QJsonObject();
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}

QString StyleManager::parseQss(const QString &qssContent, const PropertyStyle &jsonData)
{
    QString qss = qssContent;
    for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
        QString key = "{" + it.key() + "}";
        QString value = it.value().toString();
        qss.replace(key, value);
    }
    return qss;
}