#include <QDoubleSpinBox>
#include <QStyleOptionSpinBox>
#include <QLocale>

#include <qt/top_header.h>
#include <qt/qstring.hpp>
#include <shared/spin_box_double.hpp>
#include <xaml/delegate.h>

using namespace std;

using XDoubleSpinBox = TopHeader<QDoubleSpinBox, QStyleOptionSpinBox>;

namespace {

double Text2Double(const QString & text, bool * isOk)
{
    if (text.isEmpty()) {
        if (isOk) {
            *isOk = true;
        }
        return 0.0;
    }
    bool ok = false;
    auto value = QLocale(QLocale::English).toDouble(text, &ok);
    if (!ok) {
        value = QLocale(QLocale::Russian).toDouble(text, &ok);
    }
    if (isOk) {
        *isOk = ok;
    }
    if (ok) {
        return value;
    }
    return 0.0;
}

} // namespace

xaml_result xaml_spin_box_double_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<XDoubleSpinBox>());
        auto edit = static_cast<QDoubleSpinBox *>(m_handle);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_decimals());
        XAML_RETURN_IF_FAILED(draw_min());
        XAML_RETURN_IF_FAILED(draw_max());
        XAML_RETURN_IF_FAILED(draw_step());
        XAML_RETURN_IF_FAILED(draw_value());
        XAML_RETURN_IF_FAILED(draw_topHeader());
        QObject::connect(edit, &QDoubleSpinBox::textChanged,
                         xaml_mem_fn(&xaml_spin_box_base_internal::on_value_changed_event, static_cast<xaml_spin_box_base_internal *>(this)));
    }
    return set_rect(region);
}

xaml_result xaml_spin_box_double_internal::draw_value() noexcept
{
    if (auto edit = qobject_cast<QDoubleSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_value, &text));
        bool ok = false;
        auto value = Text2Double(text, &ok);
        if (ok && !text.isEmpty()) {
            edit->setValue(value);
        }
        else {
            edit->setValue(0.0);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_double_internal::draw_topHeader() noexcept
{
    if (auto edit = dynamic_cast<XDoubleSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_topHeader, &text));
        edit->setHeader(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_double_internal::draw_min() noexcept
{
    if (auto edit = dynamic_cast<XDoubleSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_min, &text));
        bool ok = false;
        auto value = Text2Double(text, &ok);
        if (ok && !text.isEmpty()) {
            edit->setMinimum(value);
        }
        else {
            edit->setMinimum(std::numeric_limits<double>::min());
        }
        return draw_value();
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_double_internal::draw_max() noexcept
{
    if (auto edit = dynamic_cast<XDoubleSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_max, &text));
        bool ok = false;
        auto value = Text2Double(text, &ok);
        if (ok && !text.isEmpty()) {
            edit->setMaximum(value);
        }
        else {
            edit->setMaximum(std::numeric_limits<double>::max());
        }
        return draw_value();
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_double_internal::draw_step() noexcept
{
    if (auto edit = dynamic_cast<XDoubleSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_step, &text));
        bool ok = false;
        auto value = Text2Double(text, &ok);
        if (ok && !text.isEmpty()) {
            edit->setSingleStep(value);
        }
        else {
            edit->setSingleStep(1.0);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_spin_box_double_internal::draw_decimals() noexcept
{
    if (auto edit = dynamic_cast<XDoubleSpinBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_decimals, &text));
        bool ok = false;
        auto value = text.toInt(&ok);
        if (ok && !text.isEmpty()) {
            edit->setDecimals(value);
        }
        else {
            edit->setDecimals(2);
        }
    }
    return XAML_S_OK;
}
