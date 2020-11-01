#include "network.h"

Network::Network(int receiveCount, int hiddenCount, int recoilCont)
{
    //srand( unsigned(time(nullptr)) );
    this->receiveCount = receiveCount;
    this->recoilCount = recoilCont;
    this->hiddenCount = hiddenCount;
    this->minWeight = -4;
    this->maxWeight = 4;

    for(int i=0; i<receiveCount; ++i){
        Neuron neuron("Receive", getRandVec(1, minWeight , maxWeight));
        this->receive.push_back(neuron);
        //std::cout<<neuron.getSignal()<<" ";
    }
    for(int i=0; i<hiddenCount; ++i){
        Neuron neuron("Hidden_1", getRandVec(receiveCount, minWeight , maxWeight));
        this->hidden1.push_back(neuron);
        //std::cout<<neuron.getSignal();
    }
    for(int i=0; i<hiddenCount; ++i){
        Neuron neuron("Hidden_2", getRandVec(hidden1.size(), minWeight , maxWeight));
        this->hidden2.push_back(neuron);
        //std::cout<<neuron.getSignal();
    }
    for(int i=0; i<recoilCont; ++i){
        Neuron neuron("Recoil", getRandVec(hidden2.size(), minWeight , maxWeight));
        this->recoil.push_back(neuron);
        //std::cout<<neuron.getSignal();
    }
}

void Network::activate(QVector<float> signals_)
{
    this->inputData = signals_;
    signals_ = prepearData(signals_);
    for(int i = 0; i < signals_.size(); ++i){
        QVector<float>signal;
        signal.push_back(signals_[i]);
        receive[i].activate(signal);
    }

    QVector<float>sigFromReceive = getSignals(receive);
    for(auto &neuron : hidden1){
        neuron.activate(sigFromReceive);
    }
    QVector<float>sigFromHidden1 = getSignals(hidden1);
    for(auto &neuron : hidden2){
        neuron.activate(sigFromHidden1);
    }
    QVector<float>sigFromHidden2 = getSignals(hidden2);
    for(auto &neuron : recoil){
        neuron.activate(sigFromHidden2);
    }
}

QVector<float> Network::getRecoil()
{
    QVector<float> result;
    for(auto neuron : recoil){
        result.push_back(neuron.getSignal());
    }
    return result;
}


void Network::showData()
{
    for(auto num : inputData){
        std::cout<<num<<"   \t";
    }
    std::cout<<"\n\n";
    for(auto neur : receive){
        std::cout<<neur.getSignal()<<"   \t";
    }
    std::cout<<"\n\n";
    for(auto neur : hidden1){
        std::cout<<neur.getSignal()<<"  \t";
    }
    std::cout<<"\n\n";
    for(auto neur : hidden2){
        std::cout<<neur.getSignal()<<"   \t";
    }
    std::cout<<"\n\n";
    for(auto neur : recoil){
        std::cout<<neur.getSignal()<<"   \t";
    }
    std::cout<<"\n\n----------------------------------------\n\n";
}


QVector<float> Network::getRandVec(int size, int min, int max)
{
    max = max * 1000;
    min = min * 1000;
    int range = max - min;
    QVector<float>result;
    for (int i=0; i<size; ++i){
        float weight = float(rand()%range + min)/1000;
        result.push_back(weight);
        //std::cout<<weight << "\n";
    }
    return result;
}

float Network::getError(QVector<float> expected)
{
    float errorVal = 0;
    QVector<float>signals_ = getSignals(recoil);
    if(signals_.size() != expected.size() || expected.size() == 0){
        std::cout<<"getError:expected wrong size()!";
        throw "getError:expected wrong size()!";
    }
    for(int i = 0; i<expected.size(); ++i){
        if(expected[i]<0 || expected[i]>1){
            std::cout<<"getError:expected cen be in 0..1";
            throw "getError:expected cen be in 0..1";
        }
        errorVal = errorVal + abs(expected[i] - signals_[i]);
    }
    return errorVal/expected.size();
}

void Network::setRandomWeights()
{
    for(auto &neur : receive){
        neur.setSlots(getRandVec(1,this->minWeight,this->maxWeight));
    }
    for(auto &neur : hidden1){
        neur.setSlots(getRandVec(this->receiveCount,this->minWeight,this->maxWeight));
    }
    for(auto &neur : hidden2){
        neur.setSlots(getRandVec(this->hiddenCount,this->minWeight,this->maxWeight));
    }
    for(auto &neur : recoil){
        neur.setSlots(getRandVec(this->hiddenCount,this->minWeight,this->maxWeight));
    }
}

void Network::addSamle(QVector<float> input, QVector<float> expected)
{
    if(input.size() == receive.size() && expected.size() == recoil.size()){
        Sample sample(input, expected);
        this->samples.push_back(sample);
    }
}

void Network::bruteTraining(float maxError)
{
    float currentError = 1;

    while(currentError > maxError){
        QVector<float>errors;
        float averageError = 0;
        Network experimental = *this;
        experimental.setRandomWeights();
        averageError = experimental.getAverageError();
        if (averageError < currentError){
            *this = experimental;
            currentError = averageError;
            std::cout<<"Training... currentError = "<<currentError<<"\n";
        }
    }
}

void Network::smartTraining(float maxError)
{
    int gen = 0;
    float currentError = 1;
    float step = 0.02f;
    enum {FOREWARD, BACKWARD};

    while(currentError > maxError){
        int layerNum = rand()%4;
        QVector<Neuron>*layer = nullptr;
        if (layerNum == RECEIVE) layer = &receive;
        if (layerNum == HIDDEN_1) layer = &hidden1;
        if (layerNum == HIDDEN_2) layer = &hidden2;
        if (layerNum == RECOIL) layer = &recoil;
        int neuronNum = rand()%(layer->size());
        int slotNum = rand()%((*layer)[neuronNum].getSlots().size());
        //float &weight = (*layer)[neuronNum].getSlots()[slotNum];
#define WEIGHT (*layer)[neuronNum].slots_[slotNum]
        //std::cout<<"_"<<WEIGHT<<"_\n";
        bool way = FOREWARD;
        currentError = this->getAverageError();
        if(WEIGHT < maxWeight) WEIGHT = WEIGHT + step;

        //std::cout<<"_"<<WEIGHT<<"\n";
        if(this->getAverageError() <= currentError){
            way = FOREWARD;
        }else{
            WEIGHT = WEIGHT - step;
            way = BACKWARD;
        }
        currentError = this->getAverageError();
        float newError = 0;
        //std::cout<<"Training... currentError before = "<<currentError<<"_"<<WEIGHT<<"\n";
        while(true){

            if(way == FOREWARD && WEIGHT < maxWeight) WEIGHT += step;
            if(way == BACKWARD && WEIGHT > minWeight) WEIGHT -= step;
            newError = this->getAverageError();
            if(newError>=currentError){
                currentError = newError;
                break;
            }
            //std::cout<<WEIGHT<<"_";
            currentError = newError;
        }
        currentError = newError;
        if (gen%100 == 0) std::cout<<"Training... currentError = "<<currentError<<" gen: "<<gen<<"\n";
    gen++;
    }
}



QVector<float> Network::prepearData(QVector<float> input)
{
    QVector<float>result;
    int range = maxWeight - minWeight;
    try{
        for (auto number : input) {
            if(number>=0.0f && number<=1.0f){
                    result.push_back((number*range + minWeight));
                    //std::cout<<"("<<number<<")\n";
            }else{
                throw "Wrong input. Can be (0...1)";
            }
        }
    }
    catch(const char*){
        std::cout<<"Wrong input. Can be (0...1)";
    }
    return result;
}

float Network::getAverageError(){
    QVector<float>errors;
    float averageError = 0;
    for(auto sample : samples){
        this->activate(sample.input);
        errors.push_back(this->getError(sample.expected));
    }
    for(auto err : errors) averageError +=err;
    averageError = averageError/errors.size();
    return averageError;
}

QVector<float> Network::getSignals(QVector<Neuron> neurons)
{
    QVector<float> result;
    for(auto neuron : neurons){
        result.push_back(neuron.getSignal());
    }
    return result;
}
