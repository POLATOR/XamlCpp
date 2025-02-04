#include <QCheckBox>
#include <shared/check_box.hpp>

using namespace std;

xaml_result xaml_check_box_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<QCheckBox>());
        auto button = static_cast<QCheckBox *>(m_handle.data());
        QObject::connect(button, &QAbstractButton::clicked, xaml_mem_fn(&xaml_button_internal::on_clicked, static_cast<xaml_button_internal *>(this)));
        QObject::connect(button, &QCheckBox::toggled, xaml_mem_fn(&xaml_check_box_internal::on_toggled, this));
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    return set_rect(region);
}

xaml_result xaml_check_box_internal::draw_checked() noexcept
{
    if (auto button = qobject_cast<QCheckBox *>(m_handle)) {
        button->setCheckState(m_is_checked ? Qt::Checked : Qt::Unchecked);
    }
    return XAML_S_OK;
}

void xaml_check_box_internal::on_toggled(bool checked) noexcept
{
    XAML_ASSERT_SUCCEEDED(set_is_checked(checked));
}
