#pragma once

#include <QDir>

#define DEFAULT_DATA_DIR QDir::toNativeSeparators(QDir::homePath()) + QDir::separator() +"Lazuli" + QDir::separator() 
#define PLAGIN_DATA_DIR DEFAULT_DATA_DIR + "Plugins"
#define CUSTOM_STYLES DEFAULT_DATA_DIR + "Styles"
#define CONFIG DEFAULT_DATA_DIR + "Lazuli.ini"
class EnumCode : public QObject {
public:
    enum class ErrorCodeDir{
        CREATED_DIRECTORY,
        FAILED_TO_CREATE,
        DIRECTORY_ALREADY_EXISTS
    };  
    Q_ENUM(ErrorCodeDir)

};