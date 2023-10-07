#pragma once

#include <cstring>

namespace demo {
    namespace utilities {

        template<typename T>
        class Singleton {
        public:
            static T * instance() {
                if (NULL == m_instance) m_instance = new T();
                return m_instance;
            }

        private:
            Singleton() = default;

            ~Singleton() = default;

            Singleton(const Singleton<T> &) = delete;

            Singleton<T> & operator=(const Singleton<T> &) = delete;

        private:
            static T * m_instance;
        };

        template<typename T>
        T * Singleton<T>::m_instance = NULL;
    }
}