#ifndef PEARSON_H
#define PEARSON_H

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

double sum(const std::vector<double>& list){
    double total = 0;
    for (auto i : list)
        total += i;
}

double mean(const std::vector<double>& list){
    return sum(list) / list.size();
}

double median(const std::vector<double>& list){
    int n = list.size();
    std::vector<double> new_list = list;
    std::sort(new_list.begin(), new_list.end());

    if (new_list.size() % 2 == 0)
        return (new_list[n/2 - 1] + new_list[n/2]) / 2;
    return new_list[n/2];
}

double sq_sum(const std::vector<double>& list){
    double ss = 0;
    for (auto i : list)
        ss += std::pow(i, 2);
    return ss;
}

double std_dev(const std::vector<double>& list){
    double n = list.size();
    double numerator = n * sq_sum(list) - std::pow(sum(list), 2);
    double denominator = n * (n - 1);
    return std::pow(numerator / denominator, 0.5);
}

#endif