#pragma once

#include <QFrame>

struct xaml_titled_group_internal;
class QLabel;

class TitledGroup : public QFrame
{
public:
    explicit TitledGroup(xaml_titled_group_internal * internal, QWidget * parent = nullptr);

    Qt::Alignment headerAlignment() const;
    void setHeaderAlignment(Qt::Alignment alignment);

    QString headerText() const;
    void setHeaderText(const QString & headerText);

protected:
    void resizeEvent(QResizeEvent * event) override;

private:
    xaml_titled_group_internal * m_internal = nullptr;
    QLabel * m_header = nullptr;
};
