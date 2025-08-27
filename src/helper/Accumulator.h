#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#define MAXSAMPLES 10

class Accumulator {
    int valIndex = 0;
    double valSum = 0;
    int valList[MAXSAMPLES] = {};
    int totalSamples = 0;

public:
    void accumulate(const int newVal) {
        valSum -= valList[valIndex];
        valSum += newVal;
        valList[valIndex] = newVal;
        if (totalSamples < MAXSAMPLES) {
            totalSamples++;
        }
        if (++valIndex == MAXSAMPLES)
            valIndex = 0;
    }

    [[nodiscard]] double getAverageRate() const {
        const int samples = totalSamples == 0 ? 1 : totalSamples;
        return valSum / samples;
    }
};



#endif //ACCUMULATOR_H
