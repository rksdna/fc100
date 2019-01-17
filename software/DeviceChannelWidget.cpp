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
    connect(m_thresholdDial, &QDial::valueChanged, this, &DeviceChannelWidget::updateChannel);

    m_couplingButtons->addData(tr("DC"), DeviceChannel::DcCoupling);
    m_couplingButtons->addData(tr("AC"), DeviceChannel::AcCoupling);
    connect(m_couplingButtons, &PopupButton::currentDataChanged, this, &DeviceChannelWidget::updateChannel);

    m_probeButtons->addData("1:1", DeviceChannel::x1Probe);
    m_probeButtons->addData("1:10", DeviceChannel::x10Probe);
    m_probeButtons->addData("1:100", DeviceChannel::x100Probe);
    connect(m_probeButtons, &PopupButton::currentDataChanged, this, &DeviceChannelWidget::updateChannel);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_thresholdDial, 0, 0, 2, 2);
    layout->addWidget(m_couplingButtons, 2, 0);
    layout->addWidget(m_probeButtons, 2, 1);

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
