#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QGroupBox>
#include "Sample.h"
#include "Computer.h"
#include "ControlOptions.h"

class QDial;
class QToolButton;
class PopupButton;

class ControlWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit ControlWidget(const QString &title, QWidget *parent = 0);

    Sample::Type type() const;
    void setType(Sample::Type type);

    Computer::Function function() const;
    void setFunction(Computer::Function function);

    ControlOptions options() const;
    void setOptions(const ControlOptions &options);

    bool isBurstEnabled() const;
    void setBurstEnabled(bool enabled);

signals:
    void typeChanged();
    void functionChanged();
    void optionsChanged();
    void startRequested();
    void clearRequested();

private:
    void updateWidget();

private:
    PopupButton * const m_typeButton;
    PopupButton * const m_functionButton;
    PopupButton * const m_startEdgeButton;
    PopupButton * const m_stopEdgeButton;
    PopupButton * const m_counterEdgeButton;
    PopupButton * const m_timerClockButton;
    QDial * const m_durationDial;
    QToolButton * const m_startButton;
    QToolButton * const m_burstButton;
    QToolButton * const m_clearButton;
};

#endif // CONTROLWIDGET_H
