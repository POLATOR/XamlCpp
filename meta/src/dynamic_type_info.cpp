#include <xaml/meta/dynamic_type_info.h>
#include <xaml/meta/dynamic_type.h>
#include <reflection_info.hpp>

#include <xaml/meta/event_info.h>
#include <xaml/event.h>

struct xaml_dynamic_type_info_impl : xaml_reflection_info_implement<xaml_dynamic_type_info_impl, xaml_dynamic_type_info>
{
    xaml_ptr<xaml_map<xaml_guid, xaml_object>> m_attr_map;
    xaml_ptr<xaml_constructor_info> m_ctor{ nullptr };
    xaml_ptr<xaml_map<xaml_string, xaml_method_info>> m_method_map;
    xaml_ptr<xaml_map<xaml_string, xaml_property_info>> m_prop_map;
    xaml_ptr<xaml_map<xaml_string, xaml_collection_property_info>> m_cprop_map;
    xaml_ptr<xaml_map<xaml_string, xaml_event_info>> m_event_map;
    xaml_ptr<xaml_map<xaml_string, xaml_string>> m_property_values_map;
    xaml_ptr<xaml_map<xaml_string, xaml_event<xaml_object,xaml_string>>> m_events;

    using xaml_reflection_info_implement::xaml_reflection_info_implement;

    xaml_result init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_map_new(&m_attr_map));
        xaml_ptr<xaml_hasher<xaml_string>> string_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_method_map));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_prop_map));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_cprop_map));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_event_map));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_events));

        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_property_values_map));
        xaml_ptr<xaml_constructor_info> constructorInfo;
        auto constructor = [&](xaml_object** obj) {
            return xaml_dynamic_object_new(obj, &m_type);
        };

        xaml_constructor_info_new(m_type, constructor, &constructorInfo);
        m_ctor = constructorInfo;

        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_attributes(xaml_map_view<xaml_guid, xaml_object>** ptr) noexcept override
    {
        return m_attr_map->query(ptr);
    }

    xaml_result XAML_CALL get_attribute(xaml_guid const& type, void** ptr) noexcept override
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_attr_map->lookup(type, &item));
        return item->query(type, ptr);
    }




    xaml_result XAML_CALL get_constructor(xaml_constructor_info** ptr) noexcept override
    {
        if (m_ctor)
        {
            xaml_ptr<xaml_object> obj;
            //xaml_object_new<xaml_object>(&obj);

            return m_ctor->query(ptr);
        }
        else
        {
            *ptr = nullptr;
            return XAML_S_OK;
        }
    }

    xaml_result XAML_CALL get_methods(xaml_map_view<xaml_string, xaml_method_info>** ptr) noexcept override
    {
        return m_method_map->query(ptr);
    }

    xaml_result XAML_CALL get_method(xaml_string* name, xaml_method_info** ptr) noexcept override
    {

        return m_method_map->lookup(name, ptr);
    }

    xaml_result XAML_CALL get_properties(xaml_map_view<xaml_string, xaml_property_info>** ptr) noexcept override
    {

        return m_prop_map->query(ptr);
    }

    xaml_result XAML_CALL get_property(xaml_string* name, xaml_property_info** ptr) noexcept override
    {
        auto result = m_prop_map->lookup(name, ptr);
        if (result != XAML_S_OK) {
            xaml_ptr<xaml_string> new_value;
            XAML_RETURN_IF_FAILED(xaml_string_new("", &new_value));
            bool is_inserted = false;
            m_property_values_map->insert(name, new_value, &is_inserted);
            xaml_ptr<xaml_string> name_;
            xaml_string_clone(name, &name_);
            xaml_ptr<xaml_property_info> prop_info;
            

            xaml_ptr<xaml_event_info> change_event;
            xaml_ptr<xaml_string> event_name;
            xaml_ptr<xaml_string> changed_string;
            xaml_string_new(u8"_changed", &changed_string);
            xaml_string_concat(name_, changed_string, &event_name);
            xaml_ptr<xaml_event<xaml_object, xaml_string>> event;
            m_events->lookup(event_name, &event);
            if (event == nullptr) {
                bool isInserted = false;
                XAML_RETURN_IF_FAILED(xaml_event_new(&event));
                XAML_RETURN_IF_FAILED(m_events->insert(event_name, event, &isInserted));
            }
            XAML_RETURN_IF_FAILED(xaml_event_info_new(event_name,
                //adder
                [=](xaml_object* object, xaml_method_info* method, int32_t* handle) -> xaml_result {
                    
                    xaml_ptr<xaml_event<xaml_object, xaml_string>> event;
                    m_events->lookup(event_name,&event);
                    if (event == nullptr) {
                        bool isInserted = false;
                        XAML_RETURN_IF_FAILED(xaml_event_new(&event));
                        XAML_RETURN_IF_FAILED(m_events->insert(event_name, event, &isInserted));
                        if (!isInserted) {
                            return XAML_E_FAIL;
                        }
                    }
                    event->add([method = xaml_ptr<xaml_method_info>{ method }](xaml_interface_t<xaml_object> sender, xaml_interface_t<xaml_string> e) noexcept -> xaml_result {
                        xaml_ptr<xaml_vector_view<xaml_object>> args;
                        XAML_RETURN_IF_FAILED(xaml_method_info_pack_args(&args, sender, e));
                        return method->invoke(args);
                    }, handle);
                    return XAML_S_OK;
                },
                //deleter
                [=](xaml_object* object, int32_t handle) ->xaml_result {
                    xaml_ptr<xaml_event<xaml_object, xaml_string>> event;
                    XAML_RETURN_IF_FAILED(m_events->lookup(event_name, &event));
                    event->remove(handle);
                    return XAML_E_FAIL;
                },
                & change_event));
            is_inserted = false;
            m_event_map->insert(event_name, change_event, &is_inserted);
            xaml_property_info_new(name, xaml_guid_xaml_string, 
                //getter
                [=](xaml_object* obj, xaml_object** val)->xaml_result {
                    m_property_values_map->lookup(name_, (xaml_string**)(val));
                    return XAML_S_OK;
                }, 
                //setter
                     [=](xaml_object* obj, xaml_object* val)->xaml_result {
                        xaml_ptr<xaml_string> value;
                        val->query<xaml_string>(&value);
                        if (value) {
                            bool isInserted = false;
                            m_property_values_map->insert(name_, value, &isInserted);
                            event->invoke(obj, value);
                        }
                        return XAML_S_OK;
                    }, &prop_info);
            is_inserted = false;
            m_prop_map->insert(name, prop_info, &is_inserted);
            *ptr = prop_info;
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_collection_properties(xaml_map_view<xaml_string, xaml_collection_property_info>** ptr) noexcept override
    {
        return m_cprop_map->query(ptr);
    }

    xaml_result XAML_CALL get_collection_property(xaml_string* name, xaml_collection_property_info** ptr) noexcept override
    {
        return m_cprop_map->lookup(name, ptr);
    }

    xaml_result XAML_CALL get_events(xaml_map_view<xaml_string, xaml_event_info>** ptr) noexcept override
    {
        return m_event_map->query(ptr);
    }

    xaml_result XAML_CALL get_event(xaml_string* name, xaml_event_info** ptr) noexcept override
    {
        return m_event_map->lookup(name, ptr);
    }

};

EXTERN_C  xaml_result XAML_CALL xaml_dynamic_type_info_new(xaml_guid XAML_CONST_REF type, xaml_string* name, xaml_string* include_file, xaml_dynamic_type_info**ptr) XAML_NOEXCEPT
{
    return xaml_object_new_and_init<xaml_dynamic_type_info_impl>(ptr, type, name, include_file);
}
