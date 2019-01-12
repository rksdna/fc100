#ifndef DEVICECHANNELWIDGET_H
#define DEVICECHANNELWIDGET_H

#include <QGroupBox>

class QDial;
class QToolButton;
class QButtonGroup;

class DeviceChannel;

class DeviceChannelWidget : public QGroupBox
{
    Q_OBJECT

public:
    DeviceChannelWidget(const QString &tag, const QString &title, DeviceChannel *channel, QWidget *parent = 0);
    ~DeviceChannelWidget();

private:
    QToolButton *createButton(QButtonGroup *group, const QString &text, int id);
    void updateDeviceChannel();

private:
    const QString m_tag;
    const QString m_title;
    DeviceChannel * const m_channel;
    QDial * const m_dial;
    QButtonGroup * const m_couplingGroup;
    QButtonGroup * const m_attenuationGroup;
};

#endif // DEVICECHANNELWIDGET_H
