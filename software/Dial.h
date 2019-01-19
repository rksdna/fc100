#ifndef DIAL_H
#define DIAL_H

#include <QDial>

class Dial : public QDial
{
    Q_OBJECT

public:
    explicit Dial(QWidget *parent = 0);
};

#endif // DIAL_H
