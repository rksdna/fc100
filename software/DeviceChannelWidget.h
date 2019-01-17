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
    explicit DeviceChannelWidget(const QString &title, QWidget *parent = 0);

    DeviceChannel deviceChannel() const;
    void setDeviceChannel(const DeviceChannel &channel);

signals:
    void deviceChannelChanged(const DeviceChannel &channel);

private:
    void updateChannel();

private:
    const QString m_title;
    PopupButton * const m_couplingButtons;
    PopupButton * const m_probeButtons;
    QDial * const m_thresholdDial;
};

#endif // DEVICECHANNELWIDGET_H
