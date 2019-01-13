#ifndef DEVICECHANNELWIDGET_H
#define DEVICECHANNELWIDGET_H

#include <QGroupBox>
#include "DeviceChannel.h"

class QDial;
class ButtonGroup;

class DeviceChannelWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit DeviceChannelWidget(const QString &title, QWidget *parent = 0);

    DeviceChannel deviceChannel() const;
    void setDeviceChannel(const DeviceChannel &channel);

Q_SIGNALS:
    void deviceChannelChanged(const DeviceChannel &channel);

private:
    void updateChannel();

private:
    const QString m_title;
    ButtonGroup * const m_couplingButtons;
    ButtonGroup * const m_probeButtons;
    QDial * const m_thresholdDial;
};

#endif // DEVICECHANNELWIDGET_H
