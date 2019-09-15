#ifndef REGS_H
#define REGS_H

#include <QtGlobal>

#define COUNTER_DAC1 ((quint8)0x00)
#define COUNTER_DAC2 ((quint8)0x01)
#define COUNTER_MODE ((quint8)0x02)
#define COUNTER_CTRL ((quint8)0x03)
#define COUNTER_ID ((quint8)0x04)
#define COUNTER_ACK ((quint8)0x05)
#define COUNTER_TAC_START ((quint8)0x06)
#define COUNTER_TAC_STOP ((quint8)0x07)
#define COUNTER_CNT ((quint8)0x08)
#define COUNTER_TMR ((quint8)0x0C)

#define COUNTER_DAC_DAC ((quint8)0xFF)

#define COUNTER_MODE_STRT ((quint8)0x03)
#define COUNTER_MODE_STRT_0 ((quint8)0x01)
#define COUNTER_MODE_STRT_1 ((quint8)0x02)
#define COUNTER_MODE_STRT_CH1R ((quint8)0x00)
#define COUNTER_MODE_STRT_CH1F ((quint8)0x01)
#define COUNTER_MODE_STRT_CH2R ((quint8)0x02)
#define COUNTER_MODE_STRT_CH2F ((quint8)0x03)

#define COUNTER_MODE_STOP ((quint8)0x0C)
#define COUNTER_MODE_STOP_0 ((quint8)0x04)
#define COUNTER_MODE_STOP_1 ((quint8)0x08)
#define COUNTER_MODE_STOP_CH1R ((quint8)0x00)
#define COUNTER_MODE_STOP_CH1F ((quint8)0x04)
#define COUNTER_MODE_STOP_CH2R ((quint8)0x08)
#define COUNTER_MODE_STOP_CH2F ((quint8)0x0C)

#define COUNTER_MODE_CNT ((quint8)0x30)
#define COUNTER_MODE_CNT_0 ((quint8)0x10)
#define COUNTER_MODE_CNT_1 ((quint8)0x20)
#define COUNTER_MODE_CNT_CH1R ((quint8)0x00)
#define COUNTER_MODE_CNT_CH1F ((quint8)0x10)
#define COUNTER_MODE_CNT_CH2R ((quint8)0x20)
#define COUNTER_MODE_CNT_CH2F ((quint8)0x30)

#define COUNTER_MODE_TMR ((quint8)0x40)
#define COUNTER_MODE_TMR_CLK ((quint8)0x00)
#define COUNTER_MODE_TMR_REF ((quint8)0x40)

#define COUNTER_MODE_CLR ((quint8)0x80)

#define COUNTER_CTRL_STRT ((quint8)0x01)
#define COUNTER_CTRL_STOP ((quint8)0x02)
#define COUNTER_CTRL_CLB_ZS ((quint8)0x04)
#define COUNTER_CTRL_CLB_FS ((quint8)0x08)
#define COUNTER_CTRL_HPF_CH1 ((quint8)0x10)
#define COUNTER_CTRL_HPF_CH2 ((quint8)0x20)
#define COUNTER_CTRL_TEST ((quint8)0x40)
#define COUNTER_CTRL_CLR ((quint8)0x80)

#define COUNTER_TAC_TAC ((quint8)0xFF)

#define COUNTER_ACK_STOP ((quint8)0x01)
#define COUNTER_ACK_STRT ((quint8)0x02)

#define COUNTER_ID_ID ((quint8)0xFF)

#define COUNTER_CNT_CNT ((quint32)0xFFFFFFFF)
#define COUNTER_TMR_TMR ((quint32)0xFFFFFFFF)

struct Regs
{
    Regs();

    QByteArray serialize() const;
    void deserialize(const QByteArray &data);

    quint8 dac1;
    quint8 dac2;
    quint8 mode;
    quint8 ctrl;
    quint8 id;
    quint8 ack;
    quint8 tac_strt;
    quint8 tac_stop;
    quint32 cnt;
    quint32 tmr;
};

#endif // REGS_H
