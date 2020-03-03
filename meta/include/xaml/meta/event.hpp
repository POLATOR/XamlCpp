#ifndef XAML_EVENT_HPP
#define XAML_EVENT_HPP

#include <functional>
#include <map>

namespace xaml
{
    template <typename... Args>
    class event
    {
    public:
        using handler_type = std::function<void(Args...)>;
        using token_type = std::size_t;

    private:
        std::map<token_type, handler_type> m_handlers{};
        token_type m_index{ 0 };

        void generate_new_index() { m_index++; }

    public:
        template <typename F>
        token_type operator+=(F&& f)
        {
            generate_new_index();
            m_handlers.emplace(m_index, handler_type{ std::forward<F>(f) });
            return m_index;
        }

        void operator-=(token_type token) { m_handlers.erase(token); }

        void operator()(Args... args)
        {
            for (auto h : m_handlers)
            {
                h.second(args...);
            }
        }

        void swap(event& e) noexcept
        {
            m_handlers.swap(e.m_handlers);
            std::swap(m_index, e.m_index);
        }
    };

    template <typename T, typename Class, typename... Args>
    constexpr decltype(auto) mem_fn_bind(Class (T::*f)(Args...), T* t)
    {
        return [=](Args... args) { return (t->*f)(std::forward<Args>(args)...); };
    }
} // namespace xaml

#endif // !XAML_EVENT_HPP
