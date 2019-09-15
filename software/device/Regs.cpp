#include <QDataStream>
#include "Regs.h"

Regs::Regs()
    : dac1(0),
      dac2(0),
      mode(0),
      ctrl(0),
      id(0),
      ack(0),
      tac_strt(0),
      tac_stop(0),
      cnt(0),
      tmr(0)
{
}

QByteArray Regs::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << dac1;
    stream << dac2;
    stream << mode;
    stream << ctrl;

    return data;
}

void Regs::deserialize(const QByteArray &data)
{
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream >> id;
    stream >> ack;
    stream >> tac_strt;
    stream >> tac_stop;
    stream >> cnt;
    stream >> tmr;
}
