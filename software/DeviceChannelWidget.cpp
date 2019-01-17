#include <QDial>
#include <QGridLayout>
#include "PopupButton.h"
#include "DeviceChannel.h"
#include "DeviceChannelWidget.h"

DeviceChannelWidget::DeviceChannelWidget(const QString &title, QWidget *parent)
    : QGroupBox(parent),
      m_title(title),
      m_couplingButtons(new PopupButton),
      m_probeButtons(new PopupButton),
      m_thresholdDial(new QDial)
{
    m_thresholdDial->setRange(DeviceChannel::min(), DeviceChannel::max());
    connect(m_thresholdDial, &QDial::valueChanged, this, &DeviceChannelWidget::updateWidget);
    connect(m_couplingButtons, &PopupButton::currentDataChanged, this, &DeviceChannelWidget::deviceChannelChanged);

    m_couplingButtons->addData(tr("DC"), DeviceChannel::DcCoupling);
    m_couplingButtons->addData(tr("AC"), DeviceChannel::AcCoupling);
    connect(m_couplingButtons, &PopupButton::currentDataChanged, this, &DeviceChannelWidget::updateWidget);
    connect(m_couplingButtons, &PopupButton::currentDataChanged, this, &DeviceChannelWidget::deviceChannelChanged);

    m_probeButtons->addData("1:1", x1Probe);
    m_probeButtons->addData("1:10", x10Probe);
    m_probeButtons->addData("1:100", x100Probe);
    connect(m_probeButtons, &PopupButton::currentDataChanged, this, &DeviceChannelWidget::updateWidget);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_thresholdDial, 0, 0, 2, 2);
    layout->addWidget(m_couplingButtons, 2, 0);
    layout->addWidget(m_probeButtons, 2, 1);

    updateWidget();
}

DeviceChannelWidget::Probe DeviceChannelWidget::probe() const
{
    return DeviceChannelWidget::Probe(m_probeButtons->currentData().toInt());
}

void DeviceChannelWidget::setProbe(DeviceChannelWidget::Probe probe)
{
    m_probeButtons->setCurrentData(probe);
}

DeviceChannel DeviceChannelWidget::deviceChannel() const
{
    return DeviceChannel(DeviceChannel::Coupling(m_couplingButtons->currentData().toInt()), m_thresholdDial->value());
}

void DeviceChannelWidget::setDeviceChannel(const DeviceChannel &channel)
{
    m_couplingButtons->setCurrentData(channel.coupling);
    m_thresholdDial->setValue(channel.threshold);
    updateWidget();
}

void DeviceChannelWidget::updateWidget()
{
    const DeviceChannel value = deviceChannel();
    switch (probe())
    {
    case DeviceChannelWidget::x1Probe:
        setTitle(tr("%1: %2V").arg(m_title).arg(value.voltage(), 0, 'f', 2));
        break;

    case DeviceChannelWidget::x10Probe:
        setTitle(tr("%1: %2V").arg(m_title).arg(10.0 * value.voltage(), 0, 'f', 1));
        break;

    case DeviceChannelWidget::x100Probe:
        setTitle(tr("%1: %2V").arg(m_title).arg(100.0 * value.voltage(), 0, 'f', 0));
        break;
    }
}
