#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include <QAbstractSlider>

class CustomDial : public QAbstractSlider
{
    Q_OBJECT

public:
    explicit CustomDial(const QString &title, QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

    int clearance() const;
    void setClearance(int clearance);

    int thickness() const;
    void setThickness(int thickness);

    qreal minAngle() const;
    qreal maxAngle() const;
    void setAngleRange(qreal min, qreal max);

    QString title() const;
    void setTitle(const QString &title);

    QString text() const;
    void setText(const QString &text);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    qreal pointToAngle(const QPoint &point) const;
    int pointToValue(const QPoint &point) const;
    qreal valueToAngle(int value) const;
    QPoint valueToPoint(int value, int radius) const;

private:
    QColor m_color;
    int m_clearance;
    int m_thickness;
    qreal m_minAngle;
    qreal m_maxAngle;
    QString m_title;
    QString m_text;
};

#endif // CUSTOMDIAL_H
