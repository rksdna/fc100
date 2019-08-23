#ifndef COMMANDBUTTON_H
#define COMMANDBUTTON_H

#include <QAbstractButton>

class CommandButton : public QAbstractButton
{
    Q_OBJECT

public:
    explicit CommandButton(const QString &text, QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initStyleOption(class QStyleOptionButton *option) const;
    void changeValue();

private:
    QMargins m_margins;
};

#endif // COMMANDBUTTON_H
