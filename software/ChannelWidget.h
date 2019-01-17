#ifndef DEVICECHANNELWIDGET_H
#define DEVICECHANNELWIDGET_H

#include <QGroupBox>
#include "ChannelOptions.h"

class QDial;
class PopupButton;

class ChannelWidget : public QGroupBox
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
    explicit ChannelWidget(const QString &title, QWidget *parent = 0);

    Probe probe() const;
    void setProbe(Probe probe);

    ChannelOptions options() const;
    void setOptions(const ChannelOptions &options);

signals:
    void optionsChanged();

private:
    void updateWidget();

private:
    const QString m_title;
    PopupButton * const m_couplingButtons;
    PopupButton * const m_probeButtons;
    QDial * const m_thresholdDial;
};

#endif // DEVICECHANNELWIDGET_H
