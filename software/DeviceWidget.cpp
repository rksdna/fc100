#include <QBoxLayout>
#include "Device.h"
#include "DeviceWidget.h"
#include "DeviceChannelWidget.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device)
{
    DeviceChannelWidget * const channel1Widget = new DeviceChannelWidget("Ch1", tr("Channel 1"), device->channel1());
    DeviceChannelWidget * const channel2Widget = new DeviceChannelWidget("Ch2", tr("Channel 2"), device->channel2());

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(channel1Widget);
    layout->addWidget(channel2Widget);

    updateDevice();
}

void DeviceWidget::updateDevice()
{

}
