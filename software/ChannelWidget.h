#ifndef DEVICECHANNELWIDGET_H
#define DEVICECHANNELWIDGET_H

#include <QGroupBox>
#include "ChannelOptions.h"

class QDial;
class QSettings;
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
    ChannelOptions options() const;

    void saveToSettings(QSettings &settings);
    void restoreFromSettings(const QSettings &settings);

signals:
    void optionsChanged();

private:
    void updateWidget();

private:
    const QString m_title;
    PopupButton * const m_couplingButton;
    PopupButton * const m_probeButton;
    QDial * const m_thresholdDial;
};

#endif // DEVICECHANNELWIDGET_H
