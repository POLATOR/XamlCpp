#include <QFrame>
#include <QLabel>
#include <QResizeEvent>
#include <QVBoxLayout>

#include <qt/qstring.hpp>
#include <qt/titled_group_control.h>
#include <shared/titled_group.hpp>

xaml_result xaml_titled_group_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<TitledGroup>(this));
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_header());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    XAML_RETURN_IF_FAILED(draw_child());
    return XAML_S_OK;
}

xaml_result xaml_titled_group_internal::draw_size() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::draw_size());

    return XAML_S_OK;
}

xaml_result xaml_titled_group_internal::draw_child() noexcept
{
    if (m_child) {
        xaml_rectangle region;
        XAML_RETURN_IF_FAILED(get_client_region(&region));
        return m_child->draw(region);
    }
    return XAML_S_OK;
}

xaml_result xaml_titled_group_internal::draw_header() noexcept
{
    if (auto group = qobject_cast<TitledGroup *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_header, &text));
        group->setHeaderText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_titled_group_internal::draw_alignment() noexcept
{
    if (auto group = qobject_cast<TitledGroup *>(m_handle)) {
        Qt::Alignment align;
        switch (m_header_halignment) {
        case xaml_halignment_center:
            align = Qt::AlignHCenter;
            break;
        case xaml_halignment_right:
            align = Qt::AlignRight;
            break;
        default:
            align = Qt::AlignLeft;
            break;
        }
        group->setHeaderAlignment(align);
    }
    return XAML_S_OK;
}

xaml_result xaml_titled_group_internal::get_client_region(xaml_rectangle * region) noexcept
{
    if (auto group = qobject_cast<TitledGroup *>(m_handle)) {
        auto size = QSizeF(group->size());
        *region = {0, 0, size.width(), size.height()};
    }
    return XAML_S_OK;
}
