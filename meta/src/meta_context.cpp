#include <xaml/meta/meta_context.h>
#include <xaml/observable_vector.h>

struct xaml_meta_context_impl : xaml_implement<xaml_meta_context_impl, xaml_meta_context, xaml_object>
{
private:
    xaml_ptr<xaml_map> m_modules;
    xaml_ptr<xaml_map> m_namespace;
    xaml_ptr<xaml_map> m_type_info_map;
    xaml_ptr<xaml_map> m_basic_type_info_map;
    xaml_ptr<xaml_map> m_name_info_map;

public:
    xaml_result init() noexcept
    {
        xaml_ptr<xaml_hasher> guid_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_new<xaml_guid>(&guid_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(guid_hasher, &m_type_info_map));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(guid_hasher, &m_basic_type_info_map));
        xaml_ptr<xaml_hasher> string_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher, &m_modules));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher, &m_namespace));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher, &m_name_info_map));

#define AT(type)                                                                                           \
    do                                                                                                     \
    {                                                                                                      \
        xaml_ptr<xaml_string> __name;                                                                      \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__name));                                         \
        xaml_ptr<xaml_basic_type_info> __info;                                                             \
        XAML_RETURN_IF_FAILED(xaml_basic_type_info_new(xaml_type_guid_v<type>, __name, nullptr, &__info)); \
        XAML_RETURN_IF_FAILED(add_type(__info));                                                           \
    } while (0)

        AT(xaml_object);
        AT(xaml_string);
        AT(xaml_vector_view);
        AT(xaml_vector);
        AT(xaml_map_view);
        AT(xaml_map);
        AT(xaml_observable_vector);
        AT(bool);
        AT(char);
        AT(int8_t);
        AT(int16_t);
        AT(int32_t);
        AT(int64_t);
        AT(uint8_t);
        AT(uint16_t);
        AT(uint32_t);
        AT(uint64_t);
        AT(float);
        AT(double);
        AT(xaml_guid);

#undef AT
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_modules(xaml_vector_view** ptr) noexcept override
    {
        return m_modules->query(ptr);
    }

    xaml_result XAML_CALL add_module(xaml_module* mod) noexcept override
    {
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(mod->get_name(&name));
        bool contains;
        XAML_RETURN_IF_FAILED(m_modules->has_key(name, &contains));
        if (!contains)
        {
            xaml_result (*pregister)(xaml_meta_context*) noexcept;
            XAML_RETURN_IF_FAILED(mod->get_method("xaml_module_register", (void**)&pregister));
            XAML_RETURN_IF_FAILED(pregister(this));
            bool replaced;
            XAML_RETURN_IF_FAILED(m_modules->insert(name, mod, &replaced));
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add_module_recursive(xaml_module* mod) noexcept override
    {
        XAML_RETURN_IF_FAILED(add_module(mod));
        xaml_result (*pdeps)(char const* const**) noexcept;
        if (XAML_SUCCEEDED(mod->get_method("xaml_module_dependencies", (void**)&pdeps)))
        {
            char const* const* arr;
            XAML_RETURN_IF_FAILED(pdeps(&arr));
            for (size_t i = 0; arr[i]; i++)
            {
                XAML_RETURN_IF_FAILED(xaml_meta_context::add_module_recursive(arr[i]));
            }
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_namespace(xaml_string* xml_ns, xaml_string** ptr) noexcept override
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_namespace->lookup(xml_ns, &item));
        return item->query(ptr);
    }

    xaml_result XAML_CALL add_namespace(xaml_string* xml_ns, xaml_string* ns) noexcept override
    {
        bool replaced;
        return m_namespace->insert(xml_ns, ns, &replaced);
    }

    xaml_result XAML_CALL get_types(xaml_map_view** ptr) noexcept override
    {
        return m_type_info_map->query(ptr);
    }

    xaml_result XAML_CALL get_type(xaml_guid const& type, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_type_info_map->lookup(key, &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL get_type_by_name(xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_name_info_map->lookup(name, &item));
        return item->query(ptr);
    }

    xaml_result XAML_CALL get_type_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(get_name_by_namespace_name(ns, name, &real_name));
        return get_type_by_name(real_name, ptr);
    }

    xaml_result XAML_CALL get_name_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_string** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_ns = ns;
        xaml_ptr<xaml_object> item;
        if (XAML_SUCCEEDED(m_namespace->lookup(ns, &item)))
        {
            real_ns = nullptr;
            XAML_RETURN_IF_FAILED(item->query(&real_ns));
        }
        std::string_view ns_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(real_ns, &ns_view));
        std::string_view name_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(name, &name_view));
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(xaml_string_new((std::string)ns_view + U("_") + (std::string)name_view, &real_name));
        return real_name->query(ptr);
    }

    xaml_result XAML_CALL add_type(xaml_reflection_info* info) noexcept override
    {
        xaml_guid type;
        XAML_RETURN_IF_FAILED(info->get_type(&type));
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        bool replaced;
        XAML_RETURN_IF_FAILED(m_type_info_map->insert(key, info, &replaced));
        return m_name_info_map->insert(name, info, &replaced);
    }

    xaml_result XAML_CALL get_basic_type(xaml_guid const& type, xaml_string** ptr) noexcept
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_basic_type_info_map->lookup(key, &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL add_basic_type(xaml_guid const& type, xaml_string* name) noexcept
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        bool replaced;
        return m_basic_type_info_map->insert(key, name, &replaced);
    }

    static xaml_result XAML_CALL get_property_changed_event_name(xaml_ptr<xaml_string> const& name, xaml_string** ptr) noexcept
    {
        std::string name_view;
        XAML_RETURN_IF_FAILED(to_string(name, &name_view));
        name_view += U("_changed");
        return xaml_string_new(move(name_view), ptr);
    }

    xaml_result XAML_CALL bind(xaml_object* target, xaml_string* target_prop, xaml_object* source, xaml_string* source_prop, xaml_binding_mode mode, xaml_converter* converter, xaml_object* parameter, xaml_string* language) noexcept override
    {
        xaml_ptr<xaml_type_info> target_type;
        {
            xaml_guid id;
            XAML_RETURN_IF_FAILED(target->get_guid(&id));
            xaml_ptr<xaml_reflection_info> info;
            XAML_RETURN_IF_FAILED(get_type(id, &info));
            XAML_RETURN_IF_FAILED(info->query(&target_type));
        }
        xaml_ptr<xaml_type_info> source_type;
        {
            xaml_guid id;
            XAML_RETURN_IF_FAILED(source->get_guid(&id));
            xaml_ptr<xaml_reflection_info> info;
            XAML_RETURN_IF_FAILED(get_type(id, &info));
            XAML_RETURN_IF_FAILED(info->query(&source_type));
        }
        xaml_ptr<xaml_property_info> targetp;
        XAML_RETURN_IF_FAILED(target_type->get_property(target_prop, &targetp));
        xaml_ptr<xaml_event_info> targete;
        {
            xaml_ptr<xaml_string> name;
            XAML_RETURN_IF_FAILED(get_property_changed_event_name(target_prop, &name));
            XAML_RETURN_IF_FAILED(target_type->get_event(name, &targete));
        }
        xaml_ptr<xaml_property_info> sourcep;
        XAML_RETURN_IF_FAILED(source_type->get_property(source_prop, &sourcep));
        xaml_ptr<xaml_event_info> sourcee;
        {
            xaml_ptr<xaml_string> name;
            XAML_RETURN_IF_FAILED(get_property_changed_event_name(source_prop, &name));
            XAML_RETURN_IF_FAILED(source_type->get_event(name, &sourcee));
        }
        xaml_ptr<xaml_converter> conv_ptr = converter;
        xaml_ptr<xaml_object> conv_param = parameter;
        xaml_ptr<xaml_string> conv_lang = language;
        if (mode & xaml_binding_one_way)
        {
            xaml_ptr<xaml_delegate> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void>(
                [source, sourcep, target, targetp, conv_ptr, conv_param, conv_lang]() noexcept -> xaml_result {
                    xaml_ptr<xaml_object> value;
                    XAML_RETURN_IF_FAILED(sourcep->get(source, &value));
                    if (conv_ptr)
                    {
                        xaml_guid type;
                        XAML_RETURN_IF_FAILED(sourcep->get_type(&type));
                        xaml_ptr<xaml_object> conv_value;
                        XAML_RETURN_IF_FAILED(conv_ptr->convert(value, type, conv_param, conv_lang, &conv_value));
                        value = conv_value;
                    }
                    XAML_RETURN_IF_FAILED(targetp->set(target, value));
                    return XAML_S_OK;
                },
                &callback)));
            xaml_ptr<xaml_vector> args;
            XAML_RETURN_IF_FAILED(xaml_vector_new(&args));
            xaml_ptr<xaml_object> obj;
            XAML_RETURN_IF_FAILED(callback->invoke(args, &obj));
            int32_t token;
            XAML_RETURN_IF_FAILED(sourcee->add(source, callback, &token));
        }
        if (mode & xaml_binding_one_way_to_source)
        {
            xaml_ptr<xaml_delegate> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void>(
                [source, sourcep, target, targetp, conv_ptr, conv_param, conv_lang]() noexcept -> xaml_result {
                    xaml_ptr<xaml_object> value;
                    XAML_RETURN_IF_FAILED(targetp->get(target, &value));
                    if (conv_ptr)
                    {
                        xaml_guid type;
                        XAML_RETURN_IF_FAILED(targetp->get_type(&type));
                        xaml_ptr<xaml_object> conv_value;
                        XAML_RETURN_IF_FAILED(conv_ptr->convert_back(value, type, conv_param, conv_lang, &conv_value));
                        value = conv_value;
                    }
                    XAML_RETURN_IF_FAILED(sourcep->set(source, value));
                    return XAML_S_OK;
                },
                &callback)));
            xaml_ptr<xaml_vector> args;
            XAML_RETURN_IF_FAILED(xaml_vector_new(&args));
            xaml_ptr<xaml_object> obj;
            XAML_RETURN_IF_FAILED(callback->invoke(args, &obj));
            int32_t token;
            XAML_RETURN_IF_FAILED(targete->add(target, callback, &token));
        }
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_meta_context_new(xaml_meta_context** ptr) noexcept
{
    return xaml_object_init<xaml_meta_context_impl>(ptr);
}
