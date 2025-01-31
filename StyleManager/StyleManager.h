#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "global_vars.h"

class StyleManager : public QObject{
    Q_OBJECT
public:
    
    ~StyleManager();

    static StyleManager* getInstance();

    using OwnerStyle = QString; // Владелец стиля (например, имя окна или виджета)
    using NameStyle = QString;  // Название стиля (например, "day", "night")
    using PropertyStyle = QJsonObject; // JSON-настройки для стиля
    using StylesQss = QMap<OwnerStyle, QString>; // QSS-файлы для владельцев
    using StyleMap = QMap<OwnerStyle, QMap<NameStyle, PropertyStyle>>; // Хранилище стилей

    using CustomStyles = QList<QPair<QString, QString>>;

    CustomStyles getCustomStyles();
    EnumCode::ErrorCodeDir init();
    bool isCustomStyles();

    // Установить QSS-файл для владельца
    void setQssFile(const OwnerStyle &owner, const QString &qssFilePath);
    // Добавить JSON-настройки для стиля
    void addStyle(const OwnerStyle &owner, const NameStyle &styleName, const QString &jsonFilePath);

    bool isRegistered(const OwnerStyle &owner);
    bool isRegisteredStyle(const OwnerStyle &owner, const NameStyle &styleName);

    // Получить стиль по названию
    QString getStyle(const OwnerStyle &owner, const NameStyle &styleName);
private:
    // Загрузить JSON из файла
    QJsonObject loadJson(const QString &filePath);
    // Заменить переменные в QSS на значения из JSON
    QString parseQss(const QString &qssContent, const PropertyStyle &jsonData);
signals:
    void changeStyle(const NameStyle &styleName);
private:
    StyleManager(QObject * parent = nullptr);
    StylesQss m_qssFiles; // Хранилище QSS-файлов
    StyleMap m_styles;    // Хранилище стилей
    static StyleManager* _instance; // Экземпляр


};

