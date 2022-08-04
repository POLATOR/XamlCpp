#include <QFrame>
#include <QLabel>
#include <QResizeEvent>
#include <QVBoxLayout>

#include <qt/qstring.hpp>
#include <shared/titled_group.hpp>

class TitledGroup : public QWidget
{
public:
    explicit TitledGroup(xaml_titled_group_internal * internal, QWidget * parent = nullptr)
        : QWidget(parent)
        , m_internal(internal)
    {
        m_header = new QLabel(this);
        m_header->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setStyleSheet("border: 1px solid #8C8C8C;");
        m_header->setFixedHeight(40);
    }

    Qt::Alignment headerAlignment() const
    {
        return m_header->alignment();
    }
    void setHeaderAlignment(Qt::Alignment alignment)
    {
        m_header->setAlignment(alignment | Qt::AlignVCenter);
    }

    QString headerText() const
    {
        return m_header->text();
    }
    void setHeaderText(const QString & headerText)
    {
        m_header->setText(headerText);
    }

protected:
    void resizeEvent(QResizeEvent * event) override
    {
        QWidget::resizeEvent(event);
        m_internal->on_resize_event(event);
        auto sz = m_header->size();
        sz.setWidth(event->size().width());
        m_header->resize(sz);
    }

private:
    xaml_titled_group_internal * m_internal = nullptr;
    QLabel * m_header = nullptr;
    QFrame * m_bottom = nullptr;
};

xaml_result xaml_titled_group_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<TitledGroup>(this));
        XAML_RETURN_IF_FAILED(draw_header());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    XAML_RETURN_IF_FAILED(draw_size());
    XAML_RETURN_IF_FAILED(draw_child());
    return set_rect(region);
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
    if (auto group = dynamic_cast<TitledGroup *>(m_handle)) {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_header, &text));
        group->setHeaderText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_titled_group_internal::draw_alignment() noexcept
{
    if (auto group = dynamic_cast<TitledGroup *>(m_handle)) {
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
    if (auto group = dynamic_cast<TitledGroup *>(m_handle)) {
        group->layout()->update();
        auto rect = QRectF(group->geometry());
        *region = {rect.x(), rect.y(), rect.width(), rect.height()};
    }
    return XAML_S_OK;
}
