#include "error_handler.h"

Error_Handler::Error_Handler(const QString& filename): logger{new File_Logger(filename)}
{
}

void Error_Handler::handle_error(const QString& username, const QString& message)
{
    logger->log(username, message);
}

int Error_Handler::validate_input_int(const QString& username, const std::string &message, const int &min, const int &max)
{
    int choice;
    while(true)
    {
        std::cout << message;
        QString error_message;
        if (!(std::cin >> choice))
        {
            error_message = "Invalid input. Please enter a number.\n";
            handle_error(username, error_message);
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        }
        else if (choice < min || choice > max)
        {
            error_message = "Invalid input. Please enter a valid action number.\n";
            handle_error(username, error_message);
        }
        else
        {
            return choice;
        }
    }
}

int Error_Handler::validate_input_int(const QString& username, const std::string &message, const std::vector<int> &list)
{
    int choice;
    while(true)
    {
        std::cout << message;
        QString error_message;
        if (!(std::cin >> choice))
        {
            error_message = "Invalid input. Please enter a number.\n";
            handle_error(username, error_message);
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        }
        else if (std::find(list.begin(), list.end(), choice - 1) == list.end())
        {
            error_message = "Invalid input. Please enter a valid action number.\n";
            handle_error(username, error_message);
        }
        else
        {
            return choice;
        }
    }
}
