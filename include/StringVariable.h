#pragma once

#include <string>
#include <utility>
#include <functional>
#include <future>
#include <deque>
#include <iostream>

namespace strvar {
    class StringProperties;
    class Value;

    namespace exceptions {
        class StrvarException : public std::exception {
            public:
                StrvarException(std::string message) : message(std::move(message)) {}
                StrvarException(const StrvarException& ref) : message(ref.message) {}
                StrvarException(StrvarException&& ref) : message(ref.message) {}

                const char *what() const noexcept {
                    return this->message.c_str();
                }
            private:
                const std::string message;
        };

#define DefineException(ClassName) \
        class ClassName : public StrvarException { \
            public: \
                ClassName(const std::string msg) : StrvarException(msg) {} \
        };

        DefineException(InvalidValueTypeException)

#undef DefineException
    }

    struct Value {
        public:
            Value(std::string key) : _key(std::move(key)) {}

            const std::string key() const { return this->_key; };
            virtual std::string value(const StringProperties&, std::deque<std::string>) const = 0;

        private:
            std::string _key;
    };

    struct StringValue : public Value {
        public:
            StringValue(const std::string& key, std::string value) : Value(key), _value(std::move(value)) { }

            std::string value(const StringProperties&, std::deque<std::string>) const override {
                return this->_value;
            }

            void value(const std::string& str) {
                this->_value = str;
            }
        private:
            std::string _value;
    };

    struct FunctionValue : public Value {
            typedef std::function<std::string(std::deque<std::string>)> FValueFNEasy;
            typedef std::function<std::string(const StringProperties&, std::deque<std::string>)> FValueFN;
        public:
            FunctionValue(const std::string& key, const FValueFNEasy &fn)
                    : Value(std::move(key)), _value([fn](const StringProperties&, std::deque<std::string> val){ return fn(val); }) { }
            FunctionValue(const std::string& key, FValueFN fn) : Value(std::move(key)), _value(std::move(fn)) { }

            std::string value(const StringProperties& props, std::deque<std::string> values) const override {
                return this->_value(props, values);
            }
        private:
            FValueFN _value;
    };

    struct StringProperties {
        public:
            StringProperties() = default;
            virtual ~StringProperties() = default;

            std::deque<std::shared_ptr<Value>> avariableValues() { return this->values; }

            void registerValue(const std::shared_ptr<Value>& value) {
                this->values.push_back(value);
            }

            std::shared_ptr<Value> findValue(const std::string& key) const {
                for(const auto& value : this->values)
                    if(value->key() == key)
                        return value;
                return nullptr;
            }

            size_t max_deep = 255;
        private:
            std::deque<std::shared_ptr<Value>> values;
    };

    namespace impl {
        typedef std::shared_ptr<strvar::Value> ValType;

        template<class VariableType, class Enable = void>
        class VariableTransform {
            public:
                static ValType transform(const VariableType&) {
                    throw exceptions::InvalidValueTypeException(
                            "Could not convert from " +
                                    std::string(typeid(VariableType).name()) +
                                    " to " + std::string(typeid(ValType).name()));
                }
        };

        template <typename VariableType>
        class VariableTransform<VariableType, typename std::enable_if<std::is_base_of<strvar::Value, VariableType>::value>::type> {
            public:
                static ValType transform(const VariableType& value) {
                    return std::make_shared<VariableType>(value);
                }
        };

        template <typename VariableType>
        class VariableTransform<VariableType, typename std::enable_if<std::is_convertible<VariableType, ValType>::value>::type> {
            public:
                static ValType transform(const VariableType& type) {
                    return std::dynamic_pointer_cast<strvar::Value>(type);
                }
        };

        inline void unrollVariadicValues(std::deque<ValType>& list) {}

        template <typename ValueType, typename... ValueTypes>
        inline void unrollVariadicValues(std::deque<ValType>& list, const ValueType& value, ValueTypes... values) {
            list.push_back(VariableTransform<ValueType>::transform(value));
            unrollVariadicValues(list, values...);
        }

    }

    extern std::string transform(std::string in, const StringProperties& properties);

    template <typename... Values>
    inline std::string transform(std::string in, Values... values) {
        std::deque<std::shared_ptr<Value>> mappedValues;
        impl::unrollVariadicValues(mappedValues, values...);

        StringProperties properties{};
        for(const auto& value : mappedValues)
            properties.registerValue(value);

        return transform(std::move(in), properties);
    }
}