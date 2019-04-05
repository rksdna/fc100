#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QGroupBox>
#include "Sample.h"
#include "Computer.h"
#include "ControlOptions.h"

class QSettings;
class Dial;
class ToolButton;
class PopupButton;

class ControlWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit ControlWidget(const QString &title, QWidget *parent = 0);

    Sample::Type type() const;
    Computer::Function function() const;
    ControlOptions options() const;
    bool isBurstEnabled() const;

    void saveToSettings(QSettings &settings);
    void restoreFromSettings(const QSettings &settings);

signals:
    void typeChanged();
    void functionChanged();
    void optionsChanged();
    void startRequested();
    void clearRequested();

private:
    void updateWidget();

private:
    const QString m_title;
    PopupButton * const m_typeButton;
    PopupButton * const m_functionButton;
    PopupButton * const m_startEdgeButton;
    PopupButton * const m_stopEdgeButton;
    PopupButton * const m_counterEdgeButton;
    PopupButton * const m_timerClockButton;
    Dial * const m_durationDial;
    ToolButton * const m_startButton;
    PopupButton * const m_burstButton;
    PopupButton * const m_gatedButton;
    ToolButton * const m_clearButton;
    QHash<int, QPair<int, QString> > m_durations;
};

#endif // CONTROLWIDGET_H
