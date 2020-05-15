#include "ProcessingAnimation.h"

#include <QtCore/QPoint>
#include <QtGui/QApplication>
#include <QtGui/QHideEvent>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QShowEvent>

ProcessingAnimation::ProcessingAnimation(QWidget *parent, QPixmap pixmap, bool pixmapEnabled)
    : QWidget(parent)
    , m_segment(0)
    , m_delay(100)
    , m_step(40)
    , m_timerId(-1)
    , m_animated(false)
    , m_pixmap(pixmap)
    , m_pixmapEnabled(pixmapEnabled)
{
}

void ProcessingAnimation::setAnimated(bool value)
{
    if (m_animated == value)
        return;
    m_animated = value;
    if (m_timerId != -1) {
        killTimer(m_timerId);
        m_timerId = -1;
    }
    if (m_animated) {
        m_segment = 0;
        m_timerId = startTimer(m_delay);
    }
    update();
}

void ProcessingAnimation::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    if (m_pixmapEnabled && !m_pixmap.isNull()) {
        p.drawPixmap(0, 0, m_pixmap);
        return;
    }

    const int extent = qMin(width() - 8, height() - 8);
    const int displ = extent / 4;
    const int ext = extent / 4 - 1;

    p.setRenderHint(QPainter::Antialiasing, true);

    if(m_animated)
        p.setPen(Qt::gray);
    else
        p.setPen(QPen(palette().dark().color()));

    p.translate(width() / 2, height() / 2); // center

    for (int segment = 0; segment < segmentCount(); ++segment) {
        p.rotate(QApplication::isRightToLeft() ? m_step : -m_step);
        if(m_animated)
            p.setBrush(colorForSegment(segment));
        else
            p.setBrush(palette().background());
        p.drawEllipse(QRect(displ, -ext / 2, ext, ext));
    }
}

QSize ProcessingAnimation::sizeHint() const
{
    return QSize(32, 32);
}

void ProcessingAnimation::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId) {
        ++m_segment;
        update();
    }
    QWidget::timerEvent(event);
}

QColor ProcessingAnimation::colorForSegment(int seg) const
{
    int index = ((seg + m_segment) % segmentCount());
    int comp = qMax(0, 255 - (index * (255 / segmentCount())));
    return QColor(comp, comp, comp, 255);
}

int ProcessingAnimation::segmentCount() const
{
    return 360 / m_step;
}

void ProcessingAnimation::setPixmapEnabled(bool enable)
{
    m_pixmapEnabled = enable;
}
