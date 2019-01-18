#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QLocale>
#include <QGroupBox>
#include "Sample.h"

class QLabel;

class DisplayWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit DisplayWidget(const QString &title, QWidget *parent = 0);

    void display(Sample::Type type, qreal first, qreal second = 0.0);

private:
    void setFirstLabel(qreal value, int precision, const QString &unit);
    void clearFirstLabel();

    void setSecondLabel(qreal value, int precision, const QString &unit);
    void clearSecondLabel();

private:
    QLocale m_locale;
    QLabel * const m_firstLabel;
    QLabel * const m_secondLabel;
};

#endif // DISPLAYWIDGET_H
