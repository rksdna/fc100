#ifndef CUSTOMDISPLAY_H
#define CUSTOMDISPLAY_H

#include <QWidget>

class CustomDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit CustomDisplay(const QString &title, QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

    QString title() const;
    void setTitle(const QString &title);

    QString text() const;
    void setText(const QString &text);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor m_color;
    QString m_title;
    QString m_text;
};

#endif // CUSTOMDISPLAY_H

