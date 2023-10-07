#pragma once

#include <map>
#include <string>

#include <utilities/Singleton.h>

using namespace demo::utilities;

namespace demo {
    namespace reflect {

        class Object {
        public:
            Object() {}

            virtual ~Object() {}

            virtual void show() = 0;
        };

        typedef Object * (* create_object)(void);

        class ClassFactory {
            friend class Singleton<ClassFactory>;

        public:
            void register_class(const std::string & className, create_object method);

            Object * create_class(const std::string & className);

        private:
            ClassFactory() {}

            ~ClassFactory() {}

        private:
            std::map<std::string, create_object> m_classMap;
        };
    }
}