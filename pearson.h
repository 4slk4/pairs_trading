#ifndef PEARSON_H
#define PEARSON_H

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>

double sum(const std::vector<double>& list){
    double total = 0;
    for (auto i : list)
        total += i;
    return total;
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

std::vector<double> operator*(const std::vector<double>& a, const std::vector<double>& b){
    if (a.size() != b.size()){
        throw std::length_error("Vector sizes are not equal");
        exit(1);
    }
    size_t i = 0;
    size_t j = 0;
    std::vector<double> result;
    while (i < a.size() && j < b.size()){
        result.push_back(a[i] * b[i]);
        i++; j++;
    }
    return result;
}

double pearson_co(const std::vector<double>& list_a, const std::vector<double>& list_b){
    if (list_a.size() != list_b.size()){
        throw std::length_error("Vector sizes are not equal");
        exit(1);
    }
    double n = list_a.size();
    double numerator = (n * sum(list_a * list_b) - sum(list_a) * sum(list_b));
    double denominator = (n * (n - 1)) * std_dev(list_a) * std_dev(list_b);
    return numerator / denominator;
}

#endif