#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <fstream>
#include <iostream>

class ErrorHandler {
public:
    static void logError(const std::string& errorMessage) {
        std::ofstream errorLog("error.log", std::ios::app);
        if (errorLog.is_open()) {
            errorLog << errorMessage << std::endl;
            errorLog.close();
        } else {
            std::cerr << "Failed to open error.log for writing." << std::endl;
        }
    }
};

#endif // ERROR_HANDLER_H
