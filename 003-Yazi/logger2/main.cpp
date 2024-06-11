#include <iostream>

#include <utilities/logger.h>

using namespace demo::utilities;

int main()
{
//    std::cout << "logger demo" << std::endl;

//    Logger log;
//    log.open("./../main.log");
//    log.log(Logger::LOG_DEBUG, __FILE__, __LINE__, "%s", "hello world");
//    log.log(Logger::LOG_DEBUG, __FILE__, __LINE__, "%s %d", "hello world", 123);

//    log.set_max_size(1024);
//    log.set_console(true);
//    for (int i = 0; i < 100; ++i)
//    {
//        log.log(Logger::LOG_DEBUG, __FILE__, __LINE__, "%s %d", "hello world", 123);
//    }



//    Logger::instance()->open("./../main.log");
//    Logger::instance()->set_max_size(1024);
//    Logger::instance()->set_console(true);
//    for (int i = 0; i < 100; ++i)
//    {
////        Logger::instance()->log(Logger::LOG_DEBUG, __FILE__, __LINE__, "%s %d", "hello world", 123);
//        log_debug("%s %d", "hello world", 123);
//        log_error("%s", "it is a error");
//    }



    auto logger = Singleton<Logger>::instance();
    logger->open("./../main.log");
    logger->set_max_size(1024);

    log_debug("hello world");
    log_error("%s %d", "this is an error", 123);

    return 0;
}
