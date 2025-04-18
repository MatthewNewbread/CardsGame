#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <iostream>
#include "file_logger.h"

class Error_Handler
{
private:
    File_Logger* logger;
public:
    Error_Handler(const QString& filename);
    void handle_error(const QString& username, const QString& message);
    int validate_input_int(const QString& username, const std::string& message, const int& min, const int& max);
    int validate_input_int(const QString& username, const std::string& message, const std::vector<int>& list);
};

#endif // ERROR_HANDLER_H
