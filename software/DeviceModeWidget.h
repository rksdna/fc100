#ifndef DEVICEMODEWIDGET_H
#define DEVICEMODEWIDGET_H

#include <QGroupBox>

class DeviceMode;
class PopupButton;

class DeviceModeWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit DeviceModeWidget(const QString &tag, const QString &title, DeviceMode *mode, QWidget *parent = 0);
    ~DeviceModeWidget();

private:
    void updateDeviceMode();

private:
    const QString m_tag;
    DeviceMode * const m_mode;
    PopupButton * const m_startButton;
    PopupButton * const m_stopButton;
    PopupButton * const m_counterButton;
    PopupButton * const m_timerButton;
    PopupButton * const m_durationButton;
};

#endif // DEVICEMODEWIDGET_H
