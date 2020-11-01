#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QVector>
#include <neuron.h>
#include <iostream>
#include <time.h>

class Network
{
    class Sample{
    public:
        Sample(QVector<float>input, QVector<float>expected){
            this->input = input;
            this->expected = expected;
        }
        Sample(){}
        QVector<float>input;
        QVector<float>expected;
    };

    enum {RECEIVE, HIDDEN_1, HIDDEN_2, RECOIL};

public:
    Network(int receiveCount, int hiddenCount, int recoilCont);
    void activate(QVector<float>signals_);
    QVector<float> getRecoil();
    void showData();
    static QVector<float> getRandVec(int size, int min, int max);
    int minWeight, maxWeight;
    float getError(QVector<float>expected);
    void setRandomWeights();
    void addSamle(QVector<float>input, QVector<float>expected);
    void bruteTraining(float maxError = 0.01f);
    void smartTraining(float maxError = 0.01f);


private:
    int receiveCount;
    int recoilCount;
    int hiddenCount;
    QVector<Neuron>receive;
    QVector<Neuron>recoil;
    QVector<Neuron>hidden1;
    QVector<Neuron>hidden2;
    QVector<float>inputData;
    QVector<Sample>samples;
    QVector<float> prepearData(QVector<float>input);
    float getAverageError();

    QVector<float> getSignals(QVector<Neuron>neurons);


};

#endif // NETWORK_H
