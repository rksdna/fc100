#ifndef DEVICEMODEWIDGET_H
#define DEVICEMODEWIDGET_H

#include <QGroupBox>
#include "DeviceMode.h"
#include "DeviceSample.h"
#include "DeviceFilter.h"

class QDial;
class QToolButton;
class PopupButton;

class DeviceControlWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit DeviceControlWidget(const QString &title, QWidget *parent = 0);

    DeviceSample::Type mode() const;
    void setMode(DeviceSample::Type mode);

    DeviceFilter::Type view() const;
    void setView(DeviceFilter::Type view);

    bool isBurstEnabled() const;
    void setBurstEnabled(bool enabled);

    DeviceMode deviceMode() const;
    DeviceMode fixedDeviceMode() const;
    void setDeviceMode(const DeviceMode &mode);

signals:
    void startRequested();
    void clearRequested();
    void modeChanged();
    void viewChanged();

private:
    void updateWidget();

private:
    PopupButton * const m_startEdgeButton;
    PopupButton * const m_stopEdgeButton;
    PopupButton * const m_counterEdgeButton;
    PopupButton * const m_timerClockButton;
    QDial * const m_durationDial;
    PopupButton * const m_modeButton;
    PopupButton * const m_viewButton;
    QToolButton * const m_startButton;
    QToolButton * const m_clearButton;
    QToolButton * const m_burstButton;
};

#endif // DEVICEMODEWIDGET_H
