#pragma once
#include <variant>
#include <string>

namespace RakX { 

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<typename StrType = std::string>
    class Err 
    {
    public:
        Err() noexcept = default;
        Err(const StrType& str): m_err(str) {}


        inline StrType as_str() const noexcept {
            if (std::holds_alternative<std::monostate>(this->m_err)) {
                return StrType("None");
            }
            else {
                const StrType* val = std::get_if<StrType>(&this->m_err);
                return StrType(*val);
            }
            
        }

        inline bool is_err() const noexcept {
            return !std::holds_alternative<std::monostate>(this->m_err);
        }
    private:
        std::variant<std::monostate, StrType> m_err{};
    };
}