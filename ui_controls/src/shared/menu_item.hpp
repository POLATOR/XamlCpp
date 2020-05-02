#ifndef XAML_UI_CONTROLS_SHARED_MENU_ITEM_HPP
#define XAML_UI_CONTROLS_SHARED_MENU_ITEM_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/menu_item.h>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <xaml/ui/win/controls/menu_item.h>
#include <xaml/ui/win/menu_bar.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_menu_item_implement : xaml_control_implement<T, Base..., xaml_menu_item>
{
    XAML_PROP_PTR_IMPL(text, xaml_string)
    XAML_EVENT_IMPL(click)

#ifdef XAML_UI_WINDOWS
    template <typename T, typename D, typename Base>
    struct xaml_win32_menu_item_implement : xaml_inner_implement<T, D, Base>
    {
        xaml_result XAML_CALL get_id(UINT* pvalue) noexcept override { return m_outer->get_menu_id(pvalue); }
        xaml_result XAML_CALL set_id(UINT value) noexcept override { return m_outer->set_menu_id(value); }
        xaml_result XAML_CALL get_parent_handle(HMENU* pvalue) noexcept override { return m_outer->get_menu_parent(pvalue); }
        xaml_result XAML_CALL set_parent_handle(HMENU value) noexcept override { return m_outer->set_menu_parent(value); }
    };

    struct xaml_win32_menu_item_impl : xaml_win32_menu_item_implement<xaml_win32_menu_item_impl, T, xaml_win32_menu_item>
    {
    } m_native_menu_item;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_win32_menu_item>)
        {
            add_ref();
            *ptr = static_cast<xaml_win32_menu_item*>(&m_native_menu_item);
            return XAML_S_OK;
        }
        else
        {
            return xaml_control_implement::query(type, ptr);
        }
    }

    XAML_PROP_IMPL(menu_id, UINT, UINT*, UINT)
    XAML_PROP_IMPL(menu_parent, HMENU, HMENU*, HMENU)

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept override
    {
        switch (msg.Msg)
        {
        case WM_COMMAND:
        {
            if (LOWORD(msg.wParam) == m_menu_id)
            {
                XAML_RETURN_IF_FAILED(on_click(this));
            }
            break;
        }
        }
        return XAML_E_NOTIMPL;
    }

    virtual xaml_result XAML_CALL draw_impl(UINT flags) noexcept
    {
        HMENU hpmenu = nullptr;
        if (m_parent)
        {
            xaml_ptr<xaml_win32_popup_menu_item> native_menu_item;
            if (XAML_SUCCEEDED(m_parent->query(&native_menu_item)))
            {
                XAML_RETURN_IF_FAILED(native_menu_item->get_handle(&hpmenu));
            }
            else
            {
                xaml_ptr<xaml_win32_menu_bar> native_menu_bar;
                if (XAML_SUCCEEDED(m_parent->query(&native_menu_bar)))
                {
                    XAML_RETURN_IF_FAILED(native_menu_bar->get_handle(&hpmenu));
                }
            }
        }
        if (hpmenu)
        {
            if (!m_menu_id)
            {
                XAML_RETURN_IF_FAILED(draw_append(hpmenu, flags));
            }
            else if (hpmenu != m_menu_parent)
            {
                XAML_RETURN_IF_WIN32_BOOL_FALSE(DeleteMenu(m_menu_parent, m_menu_id, MF_BYCOMMAND));
                m_menu_parent = hpmenu;
                XAML_RETURN_IF_FAILED(draw_append(hpmenu, flags));
            }
        }
        return XAML_S_OK;
    }

    virtual xaml_result XAML_CALL draw_append(HMENU pmenu, UINT flags) noexcept
    {
        if (!m_menu_id)
        {
            m_menu_parent = pmenu;
            XAML_RETURN_IF_FAILED(xaml_win32_menu_item_generate_id(&m_menu_id));
        }
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        XAML_RETURN_IF_WIN32_BOOL_FALSE(InsertMenu(m_menu_parent, m_menu_id, flags, m_menu_id, data));
        return XAML_S_OK;
    }

    xaml_menu_item_implement() noexcept : xaml_control_implement() { m_native_menu_item.m_outer = static_cast<T*>(this); }
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_control_implement::init());

        XAML_RETURN_IF_FAILED(xaml_event_new(&m_click));
        return XAML_S_OK;
    }
};

struct xaml_menu_item_impl : xaml_menu_item_implement<xaml_menu_item_impl>
{
    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
};

struct xaml_popup_menu_item_impl : xaml_menu_item_implement<xaml_popup_menu_item_impl, xaml_popup_menu_item>
{
protected:
    xaml_ptr<xaml_vector> m_submenu;

public:
    xaml_result XAML_CALL get_submenu(xaml_vector_view** ptr) noexcept override
    {
        return m_submenu->query(ptr);
    }

    xaml_result XAML_CALL add_submenu(xaml_menu_item*) noexcept override;
    xaml_result XAML_CALL remove_submenu(xaml_menu_item*) noexcept override;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
    virtual xaml_result XAML_CALL draw_submenu() noexcept;

#ifdef XAML_UI_WINDOWS
    struct xaml_win32_popup_menu_item_impl : xaml_win32_menu_item_implement<xaml_win32_popup_menu_item_impl, xaml_popup_menu_item_impl, xaml_win32_popup_menu_item>
    {
        xaml_result XAML_CALL get_handle(HMENU* pvalue) noexcept override { return m_outer->get_menu(pvalue); }
        xaml_result XAML_CALL set_handle(HMENU value) noexcept override { return m_outer->set_menu(value); }
    } m_native_popup_menu_item;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_win32_popup_menu_item>)
        {
            add_ref();
            *ptr = static_cast<xaml_win32_popup_menu_item*>(&m_native_popup_menu_item);
            return XAML_S_OK;
        }
        else
        {
            return xaml_menu_item_implement::query(type, ptr);
        }
    }

protected:
    wil::unique_hmenu m_menu{ nullptr };

public:
    xaml_result XAML_CALL get_menu(HMENU* pvalue) noexcept
    {
        *pvalue = m_menu.get();
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_menu(HMENU value) noexcept
    {
        m_menu.reset(value);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept override;
    xaml_result XAML_CALL draw_append(HMENU pmenu, UINT flags) noexcept override;

    xaml_popup_menu_item_impl() noexcept : xaml_menu_item_implement() { m_native_popup_menu_item.m_outer = this; }
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_check_menu_item_impl : xaml_menu_item_implement<xaml_check_menu_item_impl, xaml_check_menu_item>
{
    XAML_EVENT_IMPL(is_checked_changed)
    XAML_PROP_EVENT_IMPL(is_checked, bool, bool*, bool)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_checked() noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_radio_menu_item_impl : xaml_menu_item_implement<xaml_radio_menu_item_impl, xaml_radio_menu_item>
{
    XAML_EVENT_IMPL(is_checked_changed)
    XAML_PROP_EVENT_IMPL(is_checked, bool, bool*, bool)
    XAML_PROP_PTR_IMPL(group, xaml_string)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_checked() noexcept;
    virtual xaml_result XAML_CALL draw_group() noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_separator_menu_item_impl : xaml_menu_item_implement<xaml_separator_menu_item_impl, xaml_separator_menu_item>
{
    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_MENU_ITEM_HPP
