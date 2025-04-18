#include "file_logger.h"

File_Logger::File_Logger(const QString& filename): log_file{filename}, log_stream{&log_file}
{
    if (!log_file.open(QFile::WriteOnly | QFile::Append))
        throw std::runtime_error("Unable to open log file");
    file_name =
    {
        {Name_File::Error, "error.log"},
        {Name_File::User_Activity, "user_activity.log"}
    };
}

File_Logger::~File_Logger()
{
    log_file.close();
}

File_Logger& File_Logger::get_instance(const Name_File& file)
{
    QString name = file_name.find(file)->second;
    static File_Logger instance{name};
    return instance;
}

void File_Logger::log(const QString& username, const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    log_stream << timestamp << "[" << username << "]: " << message << "\n";
    log_stream.flush();
}
