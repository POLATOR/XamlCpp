#ifndef XAML_UI_STRINGS_HPP
#define XAML_UI_STRINGS_HPP

#include <sstream>
#include <string>
#include <string_view>

#ifndef U
#ifdef UNICODE
#define U(x) L##x
#else
#define U(x) x
#endif // UNICODE
#endif // !U

namespace xaml
{
#ifdef UNICODE
    using char_t = wchar_t;
    using string_t = std::wstring;
    using string_view_t = std::wstring_view;
    using stringstream_t = std::wstringstream;
    using istringstream_t = std::wistringstream;
    using ostringstream_t = std::wostringstream;
#else
    using char_t = char;
    using string_t = std::string;
    using string_view_t = std::string_view;
    using stringstream_t = std::stringstream;
    using istringstream_t = std::istringstream;
    using ostringstream_t = std::ostringstream;
#endif // UNICODE
} // namespace xaml

#endif // !XAML_UI_STRINGS_HPP
