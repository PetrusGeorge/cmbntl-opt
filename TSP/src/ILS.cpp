#include "ILS.hpp"
#include <algorithm>
#include <vector>

using std::vector, std::swap;

bool bestSwap(Solution *s, Data &data) {
    double bestDelta = 0, delta;
    int bestA, bestB;
    int size = s->sequence.size();
    int vertexA, vertexANext, vertexAPre;
    int vertexB, vertexBNext, vertexBPre;

    for (int i = 1; i < size - 1; i++) {
        vertexA = s->sequence[i];
        vertexANext = s->sequence[i + 1];
        vertexAPre = s->sequence[i - 1];

        for (int j = i + 1; j < size - 1; j++) {
            vertexB = s->sequence[j];
            vertexBNext = s->sequence[j + 1];
            vertexBPre = s->sequence[j - 1];

            if (j != i + 1) {
                delta = data.getDistance(vertexA, vertexBPre) + data.getDistance(vertexA, vertexBNext)
                + data.getDistance(vertexB, vertexAPre) + data.getDistance(vertexB, vertexANext)
                - data.getDistance(vertexA, vertexAPre) - data.getDistance(vertexA, vertexANext)
                - data.getDistance(vertexB, vertexBPre) - data.getDistance(vertexB, vertexBNext);
            }

            else {
                delta = data.getDistance(vertexA, vertexBNext) + data.getDistance(vertexB, vertexAPre)
                - data.getDistance(vertexA, vertexAPre) - data.getDistance(vertexB, vertexBNext);
            }

            if (delta < bestDelta) {
                bestDelta = delta;
                bestA = i;
                bestB = j;
            }
        }
    }

    if (bestDelta < 0) {
        swap(s->sequence[bestA], s->sequence[bestB]);
        s->total += bestDelta;
        return true;
    }

    return false;
}

bool bestOrOPT(Solution *s, int choice, Data &data) {
    double bestDelta = 0, delta;
    int bestA, bestB;
    int size = s->sequence.size();
    int vertexA, vertexAPre, vertexANext;
    int vertexB, vertexBPre, vertexBNext;

    if (choice == 1) {
        for (int i = 1; i < size - 2; i++) {
            vertexA = s->sequence[i];
            vertexANext = s->sequence[i + 1];
            vertexAPre = s->sequence[i - 1];

            for (int j = 0; j < size - 1; j++) {
                if (i - j <= 1 && i - j >= 0) {
                    continue;
                }

                vertexB = s->sequence[j];
                vertexBNext = s->sequence[j + 1];

                delta = data.getDistance(vertexA, vertexB) + data.getDistance(vertexA, vertexBNext)
                + data.getDistance(vertexAPre, vertexANext) - data.getDistance(vertexA, vertexAPre)
                - data.getDistance(vertexA, vertexANext) - data.getDistance(vertexB, vertexBNext);

                if (delta < bestDelta) {
                    bestDelta = delta;
                    bestA = i;
                    bestB = j;
                }
            }
        }

        if (bestDelta < 0) {
            if (bestA > bestB) {
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.erase(s->sequence.begin() + bestA + 1);
            }

            else {
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.erase(s->sequence.begin() + bestA);
            }

            s->total += bestDelta;
            return true;
        }

    }

    else if (choice == 2) {
        int vertexA2;

        for (int i = 1; i < size - 3; i++) {
            vertexA = s->sequence[i];
            vertexA2 = s->sequence[i + 1];
            vertexANext = s->sequence[i + 2];
            vertexAPre = s->sequence[i - 1];

            for (int j = 0; j < size - 1; j++) {
                if (i - j <= 1 && i - j >= -1) {
                    continue;
                }

                vertexB = s->sequence[j];
                vertexBNext = s->sequence[j + 1];

                delta = data.getDistance(vertexA, vertexBNext) + data.getDistance(vertexA2, vertexB)
                + data.getDistance(vertexAPre, vertexANext) - data.getDistance(vertexA, vertexAPre)
                - data.getDistance(vertexA2, vertexANext) - data.getDistance(vertexB, vertexBNext);

                if (delta < bestDelta) {
                    bestDelta = delta;
                    bestA = i;
                    bestB = j;
                }
            }
        }

        if (bestDelta < 0) {
            if (bestA > bestB) {
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.erase(s->sequence.begin() + bestA + 2);
                s->sequence.erase(s->sequence.begin() + bestA + 2);
            } else {
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 1]);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
            }

            s->total += bestDelta;
            return true;
        }

    }

    else if (choice == 3) {
        int vertexA3;

        for (int i = 1; i < size - 4; i++) {
            vertexA = s->sequence[i];
            vertexA3 = s->sequence[i + 2];
            vertexANext = s->sequence[i + 3];
            vertexAPre = s->sequence[i - 1];

            for (int j = 0; j < size - 1; j++) {
                if (i - j <= 2 && i - j >= -2) {
                    continue;
                }

                vertexB = s->sequence[j];
                vertexBNext = s->sequence[j + 1];

                delta = data.getDistance(vertexA, vertexBNext) + data.getDistance(vertexA3, vertexB) + data.getDistance(vertexAPre, vertexANext) - (data.getDistance(vertexA, vertexAPre) + data.getDistance(vertexA3, vertexANext) + data.getDistance(vertexB, vertexBNext));

                if (delta < bestDelta) {
                    bestDelta = delta;
                    bestA = i;
                    bestB = j;
                }
            }
        }

        if (bestDelta < 0) {
            if (bestA > bestB) {
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 4]);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
                s->sequence.erase(s->sequence.begin() + bestA + 3);
            } else {
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 1]);
                s->sequence.insert(s->sequence.begin() + bestB + 1, s->sequence[bestA + 2]);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
                s->sequence.erase(s->sequence.begin() + bestA);
            }
            s->total += bestDelta;
            return true;
        }
    }

    return false;
}

bool best2OPT(Solution *s, Data &data) {
    double bestDelta = 0, delta;
    int bestA, bestB;
    int size = s->sequence.size();
    int vertexA, vertexANext;
    int vertexB, vertexBNext;

    for (int i = 0; i < size - 1; i++) {
        vertexA = s->sequence[i];
        vertexANext = s->sequence[i + 1];

        for (int j = i + 2; j < size - 1; j++) {
            if (i == 0 && j == size - 2) {
                continue;
            }

            vertexB = s->sequence[j];
            vertexBNext = s->sequence[j + 1];

            delta = data.getDistance(vertexA, vertexB) + data.getDistance(vertexANext, vertexBNext) 
            - data.getDistance(vertexA, vertexANext) - data.getDistance(vertexB, vertexBNext);

            if (delta < bestDelta) {
                bestDelta = delta;
                bestA = i;
                bestB = j;
            }
        }
    }

    if (bestDelta < 0) {
        std::reverse(s->sequence.begin() + bestA + 1, s->sequence.begin() + bestB +1);

        s->total += bestDelta;
        return true;
    }

    return false;
}

void localSearch(Solution *s, Data &data) {
    vector<int> options{1, 2, 3, 4, 5};
    bool better = false;

    while (options.empty() == false) {
        int n = rand() % options.size();

        switch (options[n]) {
            case 1:
                better = bestSwap(s, data);
                break;

            case 2:
                better = best2OPT(s, data);
                break;

            case 3:
                better = bestOrOPT(s, 1, data);
                break;

            case 4:
                better = bestOrOPT(s, 2, data);
                break;

            case 5:
                better = bestOrOPT(s, 3, data);
                break;
        }

        if (better) {
            options = {1, 2, 3, 4, 5};
        } else {
            options.erase(options.begin() + n);
        }
    }
}

Solution ILS(int maxIter, int maxIterILS, Data &data) {
    Solution bestOfAll;
    bestOfAll.total = INFINITY;
    int dimension = data.getDimension();

    for (int i = 0; i < maxIter; i++) {
        Solution now = construction(data);
        Solution bestNow = now;

        int ls = 0;

        while (ls <= maxIterILS) {
            localSearch(&now, data);

            if (now.total < bestNow.total) {
                bestNow = now;
                ls = 0;
            }

            Pertubacao(&bestNow, &now, data);
            ls++;
        }

        if (bestNow.total < bestOfAll.total) {
            bestOfAll = bestNow;
        }
    }

    return bestOfAll;
}
