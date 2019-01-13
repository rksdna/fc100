#include <QDial>
#include <QGridLayout>
#include "ButtonGroup.h"
#include "DeviceChannel.h"
#include "DeviceChannelWidget.h"

DeviceChannelWidget::DeviceChannelWidget(const QString &title, QWidget *parent)
    : QGroupBox(parent),
      m_title(title),
      m_couplingButtons(new ButtonGroup),
      m_probeButtons(new ButtonGroup),
      m_thresholdDial(new QDial)
{
    m_thresholdDial->setRange(DeviceChannel::min(), DeviceChannel::max());
    connect(m_thresholdDial, &QDial::valueChanged, this, &DeviceChannelWidget::updateChannel);

    QGridLayout * const layout = new QGridLayout(this);

    layout->addWidget(m_thresholdDial, 0, 1, 2, 2);

    layout->addWidget(m_couplingButtons->addDataButton(tr("DC"), DeviceChannel::DcCoupling), 2, 1);
    layout->addWidget(m_couplingButtons->addDataButton(tr("AC"), DeviceChannel::AcCoupling), 2, 2);
    connect(m_couplingButtons, &ButtonGroup::currentDataChanged, this, &DeviceChannelWidget::updateChannel);

    layout->addWidget(m_probeButtons->addDataButton("1:1", DeviceChannel::x1Probe), 0, 0);
    layout->addWidget(m_probeButtons->addDataButton("1:10", DeviceChannel::x10Probe), 1, 0);
    layout->addWidget(m_probeButtons->addDataButton("1:100", DeviceChannel::x100Probe), 2, 0);
    connect(m_probeButtons, &ButtonGroup::currentDataChanged, this, &DeviceChannelWidget::updateChannel);

    updateChannel();
}

DeviceChannel DeviceChannelWidget::deviceChannel() const
{
    return DeviceChannel(DeviceChannel::Coupling(m_couplingButtons->currentData().toInt()),
                         DeviceChannel::Probe(m_probeButtons->currentData().toInt()),
                         m_thresholdDial->value());
}

void DeviceChannelWidget::setDeviceChannel(const DeviceChannel &channel)
{
    m_couplingButtons->setCurrentData(channel.coupling);
    m_probeButtons->setCurrentData(channel.probe);
    m_thresholdDial->setValue(channel.threshold);
    updateChannel();
}

void DeviceChannelWidget::updateChannel()
{
    const DeviceChannel value = deviceChannel();
    setTitle(tr("%1: %2V").arg(m_title).arg(value.voltage(), 0, 'f', value.decimals()));

    emit deviceChannelChanged(value);
}

