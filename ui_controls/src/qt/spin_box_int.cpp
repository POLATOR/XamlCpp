#include <QSpinBox>
#include <QStyleOptionSpinBox>

#include <qt/top_header.h>
#include <qt/qstring.hpp>
#include <shared/spin_box_int.hpp>
#include <xaml/delegate.h>

using namespace std;

class XSpinBox : public TopHeader<QSpinBox, QStyleOptionSpinBox>
{
public:
    using Base = TopHeader<QSpinBox, QStyleOptionSpinBox>;
    using Base::Base;
};

xaml_result xaml_spin_box_int_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<XSpinBox>());
        auto edit = static_cast<QSpinBox *>(m_handle);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_min());
        XAML_RETURN_IF_FAILED(draw_max());
        XAML_RETURN_IF_FAILED(draw_step());
        XAML_RETURN_IF_FAILED(draw_value());
        XAML_RETURN_IF_FAILED(draw_top_header());
        QObject::connect(edit, &QSpinBox::textChanged,
                         xaml_mem_fn(&xaml_spin_box_base_internal::on_value_changed_event, static_cast<xaml_spin_box_base_internal *>(this)));
    }
    return set_rect(region);
}

xaml_result xaml_spin_box_int_internal::draw_value() noexcept
{
    if (auto edit = qobject_cast<QSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_value, &text));
        bool ok = false;
        auto value = text.toInt(&ok);
        if (ok && !text.isEmpty()) {
            edit->setValue(value);
        }
        else {
            edit->setValue(0);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_int_internal::draw_top_header() noexcept
{
    if (auto edit = dynamic_cast<XSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_top_header, &text));
        edit->setHeader(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_int_internal::draw_min() noexcept
{
    if (auto edit = dynamic_cast<XSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_min, &text));
        bool ok = false;
        auto value = text.toInt(&ok);
        if (ok && !text.isEmpty()) {
            edit->setMinimum(value);
        }
        else {
            edit->setMinimum(std::numeric_limits<int>::min());
        }
        return draw_value();
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_int_internal::draw_max() noexcept
{
    if (auto edit = dynamic_cast<XSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_max, &text));
        bool ok = false;
        auto value = text.toInt(&ok);
        if (ok && !text.isEmpty()) {
            edit->setMaximum(value);
        }
        else {
            edit->setMaximum(std::numeric_limits<int>::max());
        }
        return draw_value();
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_int_internal::draw_step() noexcept
{
    if (auto edit = dynamic_cast<XSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_step, &text));
        bool ok = false;
        auto value = text.toInt(&ok);
        if (ok && !text.isEmpty()) {
            edit->setSingleStep(value);
        }
        else {
            edit->setSingleStep(1);
        }
    }
    return XAML_S_OK;
}
