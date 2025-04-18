#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <map>

class File_Logger
{
public:
    enum class Name_File
    {
        Error,
        User_Activity
    };
private:
    QFile log_file;
    QTextStream log_stream;
    static std::map<Name_File, QString> file_name;
public:
    File_Logger(const QString& filename);
    ~File_Logger();
    static File_Logger& get_instance(const File_Logger::Name_File& file);
    void log(const QString& username, const QString& message);
};

#endif // FILE_LOGGER_H
