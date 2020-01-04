#ifndef XAML_PARSER_HPP
#define XAML_PARSER_HPP

#include <libxml/xmlreader.h>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <vector>
#include <xaml/markup/binding.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/ui/meta.hpp>

namespace xaml
{
    inline void init_parser() noexcept
    {
        init_meta();
        register_class<binding>();
    }

    struct xaml_construct_property;

    struct xaml_property
    {
        property_info info;
        std::string value;
    };

    struct markup_node
    {
        std::type_index type;
        std::vector<xaml_property> properties;
    };

    struct xaml_extension_property
    {
        property_info info;
        markup_node value;
    };

    struct xaml_event
    {
        event_info info;
        std::string value;
    };

    struct xaml_node
    {
        std::type_index type;
        std::string name;
        std::vector<xaml_property> properties;
        std::vector<xaml_extension_property> extension_properties;
        std::vector<xaml_construct_property> construct_properties;
        std::vector<xaml_event> events;
        std::vector<xaml_node> children;
    };

    struct xaml_construct_property
    {
        property_info info;
        xaml_node value;
    };

    struct xaml_no_default_constructor : std::logic_error
    {
        XAML_API xaml_no_default_constructor(std::type_index t);
        ~xaml_no_default_constructor() override {}
    };

    struct xaml_bad_type : std::logic_error
    {
        XAML_API xaml_bad_type(std::string_view ns, std::string_view name);
        ~xaml_bad_type() override {}
    };

    struct xaml_parse_error : std::logic_error
    {
        xaml_parse_error() : logic_error("Failed to parse XAML file.") {}
        ~xaml_parse_error() override {}
    };

    struct xaml_no_member : std::logic_error
    {
        XAML_API xaml_no_member(std::type_index type, std::string_view name);
        ~xaml_no_member() override {}
    };

    struct xaml_not_multicontainer : std::logic_error
    {
        XAML_API xaml_not_multicontainer(std::type_index type);
        ~xaml_not_multicontainer() override {}
    };

    class parser
    {
    private:
        xmlTextReaderPtr reader;

    public:
        constexpr bool is_open() const noexcept { return reader; }
        XAML_API void open(std::string_view file);

        XAML_API parser();
        parser(std::string_view file) : parser() { open(file); }
        XAML_API ~parser();

    private:
        XAML_API markup_node parse_markup(std::string_view value);
        XAML_API int parse_members(xaml_node& mc);
        XAML_API std::tuple<int, xaml_node> parse_impl();
        XAML_API void clean_up(int ret);

    public:
        XAML_API xaml_node parse();
    };
} // namespace xaml

#endif // !XAML_PARSER_HPP
