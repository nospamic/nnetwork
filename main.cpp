#include <QCoreApplication>
#include <iostream>
#include <math.h>
#include <time.h>
#include <QVector>
#include <neuron.h>
#include <network.h>
#include <windows.h>




QVector<float> getRandVec(int size){
    QVector<float>result;
    for (int i=0; i<size; ++i){
        float weight = float(rand() %1000)/1000;
        result.push_back(weight);
        //std::cout<<weight << "\n";
    }
    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<float>act1;
    act1<<1<<0<<0<<0<<1;
    QVector<float>expected1;
    expected1<<1<<0<<0;

    QVector<float>act2;
    act2<<0<<0<<1<<0<<0;
    QVector<float>expected2;
    expected2<<0<<0<<1;

    QVector<float>act3;
    act3<<0<<0.5<<1<<0.5<<0;
    QVector<float>expected3;
    expected3<<0<<1<<0;


    Network net(5,15,3);
    net.addSamle(act1, expected1);
    net.addSamle(act2, expected2);
    net.addSamle(act3, expected3);
    net.smartTraining(0.001f);

    QVector<float>act4;
    act4<<0<<0.5f<<1<<0.5f<<0;
    net.activate(act4);
    net.showData();
    std::cout<<"Error = "<< net.getError(expected3) <<"\n";
    return a.exec();

}
