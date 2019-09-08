#ifndef CUSTOMTEXTDISPLAY_H
#define CUSTOMTEXTDISPLAY_H

#include "CustomDisplay.h"

class CustomTextDisplay : public CustomDisplay
{
    Q_OBJECT

public:
    explicit CustomTextDisplay(const QString &title, QWidget *parent = 0);

    QString title() const;
    void setTitle(const QString &title);

    QString text() const;
    void setText(const QString &text);

protected:
    void paintContent(const QRect &content, QPainter &painter) const;
    QSize contentSize() const;

private:
    QString m_title;
    QString m_text;
};

#endif // CUSTOMTEXTDISPLAY_H

