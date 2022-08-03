#pragma once 

#include <xaml/object.h>
// {1491FB2F-1633-4D25-88C3-54037A0D0447}
XAML_CLASS(xaml_dynamic_object,{ 0x1491fb2f, 0x1633, 0x4d25, { 0x88, 0xc3, 0x54, 0x3, 0x7a, 0xd, 0x4, 0x47 } });


#define XAML_DYNAMIC_OBJECT_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                           \
    XAML_METHOD(set_guid, type, xaml_guid const*);                            

XAML_DECL_INTERFACE_(xaml_dynamic_object, xaml_object)
{
    XAML_DECL_VTBL(xaml_dynamic_object, XAML_DYNAMIC_OBJECT_VTBL)

};

XAML_META_API xaml_result XAML_CALL xaml_dynamic_object_new(xaml_object ** ptr, xaml_guid const * id);
XAML_META_API xaml_result XAML_CALL xaml_dynamic_object_new(xaml_dynamic_object ** ptr, xaml_guid const * id);