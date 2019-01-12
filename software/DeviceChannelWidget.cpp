#include <QDial>
#include <QDebug>
#include <QtMath>
#include <QSettings>
#include <QGridLayout>
#include <QToolButton>
#include <QButtonGroup>
#include "DeviceChannel.h"
#include "DeviceChannelWidget.h"

DeviceChannelWidget::DeviceChannelWidget(const QString &tag, const QString &title, DeviceChannel *channel, QWidget *parent)
    : QGroupBox(parent),
      m_tag(tag),
      m_title(title),
      m_channel(channel),
      m_dial(new QDial),
      m_couplingGroup(new QButtonGroup(this)),
      m_attenuationGroup(new QButtonGroup(this))
{
    m_dial->setRange(m_channel->minThreshold(), m_channel->maxThreshold());
    connect(m_dial, &QDial::valueChanged, this, &DeviceChannelWidget::updateDeviceChannel);

    m_couplingGroup->setExclusive(true);
    connect(m_couplingGroup, static_cast<void (QButtonGroup::*)(int, bool)>(&QButtonGroup::buttonToggled), this, &DeviceChannelWidget::updateDeviceChannel);

    m_attenuationGroup->setExclusive(true);
    connect(m_attenuationGroup, static_cast<void (QButtonGroup::*)(int, bool)>(&QButtonGroup::buttonToggled), this, &DeviceChannelWidget::updateDeviceChannel);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(createButton(m_attenuationGroup, tr("x1"), 0), 0, 0);
    layout->addWidget(createButton(m_attenuationGroup, tr("x10"), 1), 1, 0);
    layout->addWidget(createButton(m_attenuationGroup, tr("x100"), 2), 2, 0);

    layout->addWidget(createButton(m_couplingGroup, tr("DC"), DeviceChannel::DcCoupling), 2, 1);
    layout->addWidget(createButton(m_couplingGroup, tr("AC"), DeviceChannel::AcCoupling), 2, 2);

    layout->addWidget(m_dial, 0, 1, 2, 2);

    QSettings settings;
    settings.beginGroup(m_tag);
    m_attenuationGroup->button(settings.value("AttenuationGroup").toInt())->setChecked(true);
    m_couplingGroup->button(settings.value("CouplingGroup").toInt())->setChecked(true);
    m_dial->setValue(settings.value("Dial").toInt());

    updateDeviceChannel();
}

DeviceChannelWidget::~DeviceChannelWidget()
{
    QSettings settings;
    settings.beginGroup(m_tag);
    settings.setValue("AttenuationGroup", m_attenuationGroup->checkedId());
    settings.setValue("CouplingGroup", m_couplingGroup->checkedId());
    settings.setValue("Dial", m_dial->value());
}

QToolButton *DeviceChannelWidget::createButton(QButtonGroup *group, const QString &text, int id)
{
    QToolButton * const button = new QToolButton;
    button->setText(text);
    button->setCheckable(true);
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    group->addButton(button, id);
    return button;
}

void DeviceChannelWidget::updateDeviceChannel()
{
    m_channel->setCoupling(DeviceChannel::Coupling(m_couplingGroup->checkedId()));
    m_channel->setThreshold(m_dial->value());

    const int decimals = m_attenuationGroup->checkedId();
    setTitle(tr("%1: %2V").arg(m_title).arg(m_channel->thresholdVoltage() * qPow(10, decimals), 0, 'f', 2 - decimals));
}

