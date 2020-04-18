#include <xaml/meta/enum_info.h>

using namespace std;

struct xaml_enum_info_impl : xaml_implement<xaml_enum_info_impl, xaml_enum_info, xaml_reflection_info, xaml_object>
{
private:
    xaml_ptr<xaml_map_view> m_map;

public:
    xaml_enum_info_impl(xaml_ptr<xaml_map_view>&& map) noexcept : m_map(move(map)) {}

    xaml_result XAML_CALL get_values(xaml_map_view** ptr) noexcept override
    {
        m_map->add_ref();
        *ptr = m_map.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_value(xaml_string* str, xaml_box** ptr) noexcept override
    {
        xaml_ptr<xaml_object> obj;
        XAML_RETURN_IF_FAILED(m_map->lookup(str, &obj));
        return obj->query(ptr);
    }
};

xaml_result xaml_enum_info_new(xaml_map_view* map, xaml_enum_info** ptr) noexcept
{
    return xaml_object_new<xaml_enum_info_impl>(ptr, map);
}
