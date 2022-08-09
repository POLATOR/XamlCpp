#include <qt/titled_group_control.h>

#include <QLabel>
#include <QVBoxLayout>

#include <shared/titled_group.hpp>

TitledGroup::TitledGroup(xaml_titled_group_internal * internal, QWidget * parent)
    : QFrame(parent)
    , m_internal(internal)
{
    m_header = new QLabel(this);
    m_header->setObjectName("TitledGroupHeader");
    m_header->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_header->move(0, 0);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_header);
    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->setContentsMargins({});
}

Qt::Alignment TitledGroup::headerAlignment() const
{
    return m_header->alignment();
}
void TitledGroup::setHeaderAlignment(Qt::Alignment alignment)
{
    m_header->setAlignment(alignment | Qt::AlignVCenter);
}

QString TitledGroup::headerText() const
{
    return m_header->text();
}
void TitledGroup::setHeaderText(const QString & headerText)
{
    m_header->setText(headerText);
}

void TitledGroup::resizeEvent(QResizeEvent * event)
{
    QFrame::resizeEvent(event);
    m_internal->on_resize_event(event);
}
