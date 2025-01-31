#include "StyleManager.h"
#include <QFile>
#include <QMetaEnum>

StyleManager* StyleManager::_instance = nullptr;

StyleManager::StyleManager(QObject *parent) : QObject(parent)
{
}

StyleManager::~StyleManager()
{
}

EnumCode::ErrorCodeDir StyleManager::init(){
    QDir dir(CUSTOM_STYLES);

    QMetaEnum metaEnum = QMetaEnum::fromType<EnumCode::ErrorCodeDir>();
    
    EnumCode::ErrorCodeDir errorCode;
    if(!dir.exists()) {
        if(!dir.mkpath(".")) {
            const char* errorCodeStr = metaEnum.valueToKey(static_cast<int>(EnumCode::ErrorCodeDir::FAILED_TO_CREATE));
            qWarning() << errorCodeStr << CUSTOM_STYLES;
            return EnumCode::ErrorCodeDir::FAILED_TO_CREATE;
        } else {
            errorCode = EnumCode::ErrorCodeDir::CREATED_DIRECTORY;
        }
    } else {
        errorCode = EnumCode::ErrorCodeDir::DIRECTORY_ALREADY_EXISTS;
    }

    const char* errorCodeStr = metaEnum.valueToKey(static_cast<int>(errorCode));
    qDebug() << errorCode << CUSTOM_STYLES;
    return errorCode;
}

bool StyleManager::isCustomStyles(){
    QDir dir(CUSTOM_STYLES);

    // Проверяем, существует ли директория и доступна ли она для чтения
    if (!dir.exists() || !dir.isReadable()) {
        return false;
    }

    // Устанавливаем фильтры для поиска файлов с расширением .json
    dir.setNameFilters(QStringList() << "*.json");

    // Получаем список файлов, соответствующих фильтру
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);

    // Проверяем, есть ли хотя бы один файл в списке
    return !fileList.isEmpty();
}

StyleManager::CustomStyles StyleManager::getCustomStyles(){
QDir dir(CUSTOM_STYLES);

    // Проверяем, существует ли директория и доступна ли она для чтения
    if (!dir.exists() || !dir.isReadable()) {
        return {};
    }

    // Устанавливаем фильтры для поиска файлов с расширением .json
    dir.setNameFilters(QStringList() << "*.json");

    // Получаем список файлов, соответствующих фильтру
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);

    // Создаем список пар "имя файла - путь к файлу"
    QList<QPair<QString, QString>> result;
    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.fileName().remove(".json"); // Имя файла без пути
        QString filePath = fileInfo.absoluteFilePath(); // Полный путь к файлу
        result.append(qMakePair(fileName, filePath));
    }

    return result;
}

StyleManager* StyleManager::getInstance(){
     if (_instance == nullptr) {
        _instance = new StyleManager();
    }
    return _instance;
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