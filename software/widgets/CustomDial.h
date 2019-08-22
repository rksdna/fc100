#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include <QDial>
#include <QVariant>

class CustomDial : public QAbstractSlider
{
    Q_OBJECT

public:
    explicit CustomDial(const QString &text, QWidget *parent = 0);

    QString description() const;
    void setDescription(const QString &description);

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
    void initStyleOption(class QStyleOptionButton *option) const;

    qreal pointToAngle(const QPoint &point) const;
    int pointToValue(const QPoint &point) const;
    qreal valueToAngle(int value) const;
    QPoint valueToPoint(int value, int radius) const;

private:
    QString m_text;
    QString m_description;
    QMargins m_margins;
    int m_clearance;
    int m_thickness;
};

#endif // CUSTOMDIAL_H
