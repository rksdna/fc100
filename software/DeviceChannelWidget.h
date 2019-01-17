#ifndef DEVICECHANNELWIDGET_H
#define DEVICECHANNELWIDGET_H

#include <QGroupBox>
#include "DeviceChannel.h"

class QDial;
class PopupButton;

class DeviceChannelWidget : public QGroupBox
{
    Q_OBJECT

public:
    enum Probe
    {
        x1Probe,
        x10Probe,
        x100Probe
    };

public:
    explicit DeviceChannelWidget(const QString &title, QWidget *parent = 0);

    Probe probe() const;
    void setProbe(Probe probe);

    DeviceChannel deviceChannel() const;
    void setDeviceChannel(const DeviceChannel &channel);

signals:
    void deviceChannelChanged();

private:
    void updateWidget();

private:
    const QString m_title;
    PopupButton * const m_couplingButtons;
    PopupButton * const m_probeButtons;
    QDial * const m_thresholdDial;
};

#endif // DEVICECHANNELWIDGET_H
