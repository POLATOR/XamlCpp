#pragma once

#include <algorithm>

#include <QEvent>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QString>

constexpr int c_sideMargins = 8;
constexpr int c_textMargin = 2;
constexpr double c_headerToTextRatio = 10.0 / 14.0;

template <typename BaseWidget, typename BaseWidgetStyleOptions>
class TopHeader : public BaseWidget
{
public:
    using BaseWidgetType = BaseWidget;
    using BaseWidgetStyleOptionsType = BaseWidgetStyleOptions;

    template <typename... Args>
    TopHeader(Args &&... args)
        : BaseWidgetType(std::forward<Args>(args)...)
    {
        this->makeHeaderFont();
    }

    void setHeader(const QString & header)
    {
        m_header = header;
        this->update();
    }

protected:
    void changeEvent(QEvent * event) override
    {
        if (event->type() == QEvent::FontChange) {
            makeHeaderFont();
        }
        BaseWidgetType::changeEvent(event);
    }

    void paintEvent(QPaintEvent * event) override
    {
        BaseWidgetType::paintEvent(event);
        if (m_header.isEmpty()) {
            return;
        }

        BaseWidgetStyleOptionsType options;
        this->initStyleOption(&options);

        QPalette pal = options.palette;

        QFontMetrics metrics{m_headerFont};
        auto r = metrics.boundingRect(m_header);
        QRect drawRect{c_sideMargins, 0, std::min(r.width() + c_textMargin * 2, this->size().width() - c_sideMargins * 2), r.height() + c_textMargin * 2};

        QPainter p(this);
        p.fillRect(drawRect, pal.base());
        p.setFont(m_headerFont);
        p.setPen(pal.text().color());
        auto text = metrics.elidedText(m_header, Qt::ElideLeft, drawRect.width());
        p.drawText(drawRect, Qt::AlignCenter, text);
    }

private:
    void makeHeaderFont()
    {
        m_headerFont = this->font();
        m_headerFont.setPointSizeF(m_headerFont.pointSizeF() * c_headerToTextRatio);
    }

    QFont m_headerFont;
    QString m_header;
};
