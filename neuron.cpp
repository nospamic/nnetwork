#include "neuron.h"

Neuron::Neuron(QString name, QVector<float>slots_){
    this->name = name;
    this->slots_ = slots_;
    this->signal = 0;
    setMaxSumm();
}

Neuron::Neuron()
{

}

QString Neuron::getName()
{
    return name;
}

float Neuron::getSignal()
{
    return signal;
}

void Neuron::activate(QVector<float> signals_){
    if(signals_.size()==slots_.size()){
        this->signal = 0;
        for(int i = 0; i < signals_.size(); ++i){
            this->signal = this->signal + signals_[i] * slots_[i];
        }
        //std::cout<<signal<<"\n";
        this->signal = 1.0f / (1.0f + exp(-this->signal));
        if(this->signal < 0.3f) this->signal = this->signal * 0.001f;


    }else{
        //throw "Different number of slots and signals";
        std::cout<<this->name.toLocal8Bit().constData()<<": Different number of slots="<< slots_.size()<< "and signals="<<signals_.size();

    }

}

void Neuron::setSlots(QVector<float> slots_)
{
    this->slots_ = slots_;
    setMaxSumm();
}

QVector<float> Neuron::getSlots()
{
    return slots_;
}

void Neuron::setMaxSumm()
{
    this->maxSumm = 0;
    for(auto weight : slots_) maxSumm+=weight;
}
