#include <iostream>

#include <utilities/logger.h>

using namespace demo::utilities;

int main()
{
//    std::cout << "logger demo & singleton demo" << std::endl;

    auto logger = Singleton<Logger>::instance();
    logger->open("./test.log");
//    logger->level(Logger::INFO);
//    logger->level(Logger::WARN);
//    logger->level(Logger::ERROR);
//    logger->level(Logger::FATAL);
    logger->max(360);

//    logger->log(Logger::DEBUG, __FILE__, __LINE__, "name is %s, age is %d", "jack", 18);

    log_debug("log message: %s", "DEBUG");
    log_info("log message: %s", "INFO");
    log_warn("log message: %s", "WARN");
    log_error("log message: %s", "ERROR");
    log_fatal("log message: %s", "FATAL");

    return 0;
}