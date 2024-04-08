#include "Construction.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#define BACK 0
#define FRONT 1

using std::vector, std::sort, std::swap;

Solution chooseRandom(Data &data, vector<int> &CL) {
    Solution choice{{1}, 0};

    // Choose three random vertex
    for (int i = 0; i < 3; i++) {
        unsigned random = rand() % CL.size();

        choice.sequence.push_back(CL[random]);
        CL.erase(CL.begin() + random);
    }

    choice.sequence.push_back(1);

    // Calculate total distance
    for (int a = 0, b = 1; b < choice.sequence.size(); a++, b++) {
        choice.total += data.getDistance(choice.sequence[a], choice.sequence[b]);
    }

    return choice;
}

vector<Insertion> calculateInsertionCost(const Solution *subTour, Data &data, vector<int> &CL) {
    vector<Insertion> insertionCost((subTour->sequence.size() - 1) * CL.size());

    int l = 0;

    for (int a = 0, b = 1; b < subTour->sequence.size(); a++, b++) {
        int i = subTour->sequence[a], j = subTour->sequence[b];

        for (auto k : CL) {
            insertionCost[l].delta = data.getDistance(i, k) 
            + data.getDistance(k, j)
            - data.getDistance(i, j);

            insertionCost[l].removedVertex = b;
            insertionCost[l].insertedVertex = k;
            l++;
        }
    }

    return insertionCost;
}

inline bool compare(const Insertion &x, const Insertion &y) {
    return x.delta < y.delta;
}

void insertRandom(Solution *incompleteSolution, vector<Insertion>& list, vector<int> &CL) {
    double alpha = ((double)rand() / RAND_MAX) + 0.00001;
    int chose = rand() % ((int)ceil(alpha * list.size()));

    incompleteSolution->sequence
        .insert(incompleteSolution->sequence.begin() +
                list[chose].removedVertex,
                list[chose].insertedVertex);

    incompleteSolution->total += list[chose].delta;

    for (int i = 0; i < CL.size(); i++) {
        if (list[chose].insertedVertex == CL[i]) {
            CL.erase(CL.begin() + i);
            break;
        }
    }
}

Solution construction(Data &data) {
    // reset candidate vertices
    int dimension = data.getDimension();
    vector<int> CL(dimension - 1);

    for (int i = 0; i < dimension - 1; i++) {
        CL[i] = i + 2;
    }
    Solution s;

    s = chooseRandom(data, CL);

    while (!CL.empty()) {
        vector<Insertion> insertionCost = calculateInsertionCost(&s, data, CL);

        sort(insertionCost.begin(), insertionCost.end(), compare);

        insertRandom(&s, insertionCost, CL);
    }

    return s;
}

void Pertubacao(const Solution *s, Solution *receiver, Data &data) {
    vector<int> sequenceCopy = s->sequence;
    int qtyBack, qtyFront;
    int sizeB, sizeA, indexB, indexA;
    double delta, distance;

    if (s->sequence.size() > 30) {
        sizeA = (rand() % ((int)ceil(s->sequence.size() / 10))) + 3;
    }

    else {
        sizeA = (rand() % 2) + 2;
    }
    indexA = (rand() % (s->sequence.size() - sizeA - 1)) + 1;
    qtyBack = indexA - 1;
    qtyFront = s->sequence.size() - indexA - sizeA - 1;

    int bOrF = rand() % 2;

    if (qtyBack == 0) {
        bOrF = FRONT;
    }

    else if (qtyFront == 0) {
        bOrF = BACK;
    }

    if (bOrF == BACK) {
        sizeB = (rand() % qtyBack) + 1;
        indexB = (rand() % (qtyBack - sizeB + 1)) + 1;

        if (sizeA > sizeB) {
            for (int i = 0; i < sizeB; i++) {
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for (int i = 0; i < sizeA - sizeB; i++) {
                sequenceCopy.insert(sequenceCopy.begin() + indexB + sizeB, sequenceCopy[indexA + sizeA - 1]);
                sequenceCopy.erase(sequenceCopy.begin() + indexA + sizeA);
            }
        }

        else {
            for (int i = 0; i < sizeA; i++) {
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for (int i = 0; i < sizeB - sizeA; i++) {
                sequenceCopy.insert(sequenceCopy.begin() + indexA + sizeA, sequenceCopy[indexB + sizeA]);
                sequenceCopy.erase(sequenceCopy.begin() + indexB + sizeA);
            }
        }
    }

    else if (bOrF == FRONT) {
        sizeB = (rand() % qtyFront) + 1;
        indexB = (rand() % (qtyFront - sizeB + 1)) + indexA + sizeA;

        if (sizeA > sizeB) {
            for (int i = 0; i < sizeB; i++) {
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for (int i = 0; i < sizeA - sizeB; i++) {
                sequenceCopy.insert(sequenceCopy.begin() + indexB + sizeB, sequenceCopy[indexA + sizeB]);
                sequenceCopy.erase(sequenceCopy.begin() + indexA + sizeB);
            }
        } else {
            for (int i = 0; i < sizeA; i++) {
                swap(sequenceCopy[indexB + i], sequenceCopy[indexA + i]);
            }

            for (int i = 0; i < sizeB - sizeA; i++) {
                sequenceCopy.insert(sequenceCopy.begin() + indexA + sizeA, sequenceCopy[indexB + sizeB - 1]);
                sequenceCopy.erase(sequenceCopy.begin() + indexB + sizeB);
            }
        }
    }

    if (indexA + sizeA != indexB && indexB + sizeB != indexA) {
        delta = data.getDistance(s->sequence[indexA], s->sequence[indexB - 1]) 
        + data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexB + sizeB]) 
        + data.getDistance(s->sequence[indexB], s->sequence[indexA - 1]) 
        + data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexA + sizeA]) 
        - data.getDistance(s->sequence[indexA], s->sequence[indexA - 1]) 
        - data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexA + sizeA]) 
        - data.getDistance(s->sequence[indexB], s->sequence[indexB - 1]) 
        - data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexB + sizeB]);
    }

    else if (bOrF == BACK) {
        delta = data.getDistance(s->sequence[indexA], s->sequence[indexB - 1]) 
        + data.getDistance(s->sequence[indexB], s->sequence[indexA + sizeA - 1]) 
        + data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexA + sizeA]) 
        - data.getDistance(s->sequence[indexA], s->sequence[indexA - 1]) 
        - data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexA + sizeA]) 
        - data.getDistance(s->sequence[indexB], s->sequence[indexB - 1]);
    }

    else if (bOrF == FRONT) {
        delta = data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexA]) 
            + data.getDistance(s->sequence[indexA + sizeA - 1], s->sequence[indexB + sizeB]) 
            + data.getDistance(s->sequence[indexB], s->sequence[indexA - 1]) 
            - data.getDistance(s->sequence[indexA], s->sequence[indexA - 1]) 
            - data.getDistance(s->sequence[indexB], s->sequence[indexB - 1]) 
            - data.getDistance(s->sequence[indexB + sizeB - 1], s->sequence[indexB + sizeB]);
    }

    receiver->sequence = sequenceCopy;
    receiver->total = delta + s->total;
}
