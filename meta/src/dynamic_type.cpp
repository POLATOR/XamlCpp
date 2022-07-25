#include <xaml/meta/dynamic_type.h>
#include <xaml/weak_reference.h>

struct xaml_dynamic_object_impl : xaml_weak_implement<xaml_dynamic_object_impl, xaml_dynamic_object>{

	xaml_guid m_id{};

	xaml_result set_guid(xaml_guid const* id) noexcept {
		m_id = *id;
		return XAML_S_OK;
	}

	xaml_result get_guid(xaml_guid* id) noexcept{
		*id = m_id;
		return XAML_S_OK;
	}
	

};

xaml_result XAML_CALL xaml_dynamic_object_new(xaml_object** ptr, xaml_guid* id)
{
	xaml_dynamic_object_impl *ptrNew;
	(xaml_object_new<xaml_dynamic_object_impl>(&ptrNew));
	*ptr = ptrNew;
	return ptrNew->set_guid(&xaml_guid_xaml_dynamic_object);
}
