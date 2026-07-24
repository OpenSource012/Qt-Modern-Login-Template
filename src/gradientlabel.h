#ifndef GRADIENTLABEL_H
#define GRADIENTLABEL_H

#include <QLabel>
#include <QPainter>
#include <QLinearGradient>

// Custom label for rendering gradient text
class GradientLabel : public QLabel {
    Q_OBJECT
public:
    explicit GradientLabel(const QString &text, QWidget *parent = nullptr) : QLabel(text, parent) {}

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Catppuccin soft blue gradient
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(0.0, QColor("#7287FD"));
        gradient.setColorAt(1.0, QColor("#1E66F5"));

        painter.setPen(QPen(QBrush(gradient), 0));
        painter.setFont(this->font());

        painter.drawText(rect(), alignment(), text());
    }
};

#endif // GRADIENTLABEL_H