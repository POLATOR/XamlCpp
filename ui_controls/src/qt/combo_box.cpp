#include <QComboBox>
#include <QStringListModel>
#include <qt/top_header.h>
#include <qt/qstring.hpp>
#include <shared/combo_box.hpp>

using namespace std;

class XComboBox : public TopHeader<QComboBox, QStyleOptionComboBox>
{
    Q_OBJECT
public:
    using Base = TopHeader<QComboBox, QStyleOptionComboBox>;
    using Base::Base;
};

xaml_result xaml_combo_box_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<XComboBox>());
        auto combo = static_cast<QComboBox *>(m_handle.data());
        XAML_RETURN_IF_FAILED(draw_items());
        XAML_RETURN_IF_FAILED(draw_sel());
        XAML_RETURN_IF_FAILED(draw_editable());
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_top_header());
        QObject::connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), xaml_mem_fn(&xaml_combo_box_internal::on_current_index_changed, this));
        QObject::connect(combo, &QComboBox::currentTextChanged, xaml_mem_fn(&xaml_combo_box_internal::on_current_text_changed, this));
    }
    return set_rect(region);
}

xaml_result xaml_combo_box_internal::draw_text() noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle); combo && m_text) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        combo->setCurrentText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_items() noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle)) {
        QStringList list;
        if (m_items) {
            XAML_FOREACH_START(xaml_object, item, m_items);
            {
                XAML_RETURN_IF_FAILED(create_item(item));
                xaml_ptr<xaml_string> s = item.query<xaml_string>();
                if (s) {
                    QString ss;
                    XAML_RETURN_IF_FAILED(to_QString(s, &ss));
                    list.append(std::move(ss));
                }
            }
            XAML_FOREACH_END();
        }
        else if (m_text_items) {
            XAML_FOREACH_START(xaml_combo_box_item, item, m_text_items);
            {
                xaml_ptr<xaml_string> s;
                XAML_RETURN_IF_FAILED(item->get_text(&s));
                if (s) {
                    QString ss;
                    XAML_RETURN_IF_FAILED(to_QString(s, &ss));
                    list.append(std::move(ss));
                }
            }
            XAML_FOREACH_END();
        }
        combo->clear();
        combo->addItems(list);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_sel() noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle)) {
        combo->setCurrentIndex(m_sel_id);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_editable() noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle)) {
        combo->setEditable(m_is_editable);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_top_header() noexcept
{
    if (auto edit = qobject_cast<XComboBox *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_top_header, &text));
        edit->setHeader(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::insert_item(int32_t index, xaml_ptr<xaml_object> const & value) noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle)) {
        xaml_ptr<xaml_string> s = value.query<xaml_string>();
        if (s) {
            QString ss;
            XAML_RETURN_IF_FAILED(to_QString(s, &ss));
            combo->insertItem(index, ss);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::remove_item(int32_t index) noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle)) {
        combo->removeItem(index);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::clear_items() noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle)) {
        combo->clear();
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::replace_item(int32_t index, xaml_ptr<xaml_object> const & value) noexcept
{
    if (auto combo = qobject_cast<QComboBox *>(m_handle)) {
        xaml_ptr<xaml_string> s = value.query<xaml_string>();
        if (s) {
            QString ss;
            XAML_RETURN_IF_FAILED(to_QString(s, &ss));
            combo->setItemText(index, ss);
        }
    }
    return XAML_S_OK;
}

void xaml_combo_box_internal::on_current_index_changed(int index) noexcept
{
    XAML_ASSERT_SUCCEEDED(set_sel_id(index));
}

void xaml_combo_box_internal::on_current_text_changed(QString const & text) noexcept
{
    if (m_is_editable) {
        xaml_ptr<xaml_string> text_str;
        XAML_ASSERT_SUCCEEDED(xaml_string_new(text, &text_str));
        XAML_ASSERT_SUCCEEDED(set_text(text_str));
    }
    else {
        XAML_ASSERT_SUCCEEDED(set_text(nullptr));
    }
}

#include "combo_box.moc"
