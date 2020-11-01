#ifndef NEURON_H
#define NEURON_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <iostream>
#include <math.h>

class Neuron
{
public:
    Neuron(QString name, QVector<float>slots_);
    Neuron();
    QString getName();
    float getSignal();
    QVector<float> slots_;
    void activate(QVector<float>signals_);
    void setSlots(QVector<float>slots_);
    QVector <float> getSlots();

private:
    QString name;

    float signal;
    void setMaxSumm();
    float maxSumm;

};

#endif // NEURON_H
