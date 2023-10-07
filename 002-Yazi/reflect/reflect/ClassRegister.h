#pragma once

#include <reflect/ClassFactory.h>

namespace demo {
    namespace reflect {
        class ClassRegister {
        public:
            ClassRegister(const std::string & className, create_object method) {
                Singleton<ClassFactory>::instance()->register_class(className, method);
            }
        };

#define REGISTER_CLASS(className)                                                   \
        Object * createObject##className() {                                        \
            Object * obj = new className();                                         \
            return obj;                                                             \
        }                                                                           \
        ClassRegister classRegister##className(#className, createObject##className);
    }
}