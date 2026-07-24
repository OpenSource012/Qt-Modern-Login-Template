#ifndef REVEALOVERLAY_H
#define REVEALOVERLAY_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>

// Custom overlay for circular reveal animation
class RevealOverlay : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int radius READ radius WRITE setRadius)

public:
    explicit RevealOverlay(QWidget *parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_TranslucentBackground);
    }

    int radius() const { return m_radius; }
    void setRadius(int r) {
        m_radius = r;
        update();
    }

    void setPixmap(const QPixmap &pm) { m_pixmap = pm; }
    void setCenter(const QPoint &c) { m_center = c; }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        QPainterPath rectPath;
        rectPath.addRect(this->rect());

        QPainterPath circlePath;
        circlePath.addEllipse(m_center, m_radius, m_radius);

        QPainterPath finalPath = rectPath.subtracted(circlePath);

        p.setClipPath(finalPath);
        p.drawPixmap(0, 0, m_pixmap);
    }

private:
    int m_radius = 0;
    QPixmap m_pixmap;
    QPoint m_center;
};

#endif // REVEALOVERLAY_H