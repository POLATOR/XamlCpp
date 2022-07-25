#pragma once 

#include "type_info.h"

// {C36C4DDF-E587-49EF-B7A6-FD75BA47774F}
XAML_CLASS(xaml_dynamic_type_info, { 0x28d6f084, 0x3d3f, 0x4c5b, { 0x8f, 0x25, 0xae, 0x97, 0x2a, 0x38, 0xce, 0x1c } });


#define XAML_DYNAMIC_TYPE_INFO_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_TYPE_INFO_VTBL(type));                               

XAML_DECL_INTERFACE_(xaml_dynamic_type_info, xaml_type_info)
{
    XAML_DECL_VTBL(xaml_dynamic_type_info, XAML_DYNAMIC_TYPE_INFO_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result XAML_CALL add_attribute(T * ptr) noexcept
    {
        return add_attribute(xaml_type_guid_v<T>, (xaml_object*)ptr);
    }
#endif // __cplusplus
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_dynamic_type_info_new(xaml_guid XAML_CONST_REF, xaml_string*, xaml_string*, xaml_dynamic_type_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
xaml_result XAML_CALL xaml_dynamic_type_info_new(xaml_string* name, xaml_string* include_file, xaml_dynamic_type_info** ptr) noexcept
{
    return xaml_dynamic_type_info_new(xaml_type_guid_v<T>, name, include_file, ptr);
}
#endif // __cplusplus