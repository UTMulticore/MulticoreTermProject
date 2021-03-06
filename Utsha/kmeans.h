#ifndef __KMEANS__H
#define __KMEANS__H

#include <iostream>
#include <vector>
#include <cmath>

class KMeans{
private:
    int n;  // number of data points
    int d;  // dimension of data points (vectors)
    int k;  // number of clusters
    std::vector<std::vector<float> > dataPoints;  // input data points
    std::vector<int> labels;  // stores info that which set a vector belong to
    std::vector<std::vector<float> > centeroids; // centers of each of k sets
    bool converged;
    float previousError;

    // return L2 distance between two points
    float getDistance(std::vector<float> x1, std::vector<float> x2);

    // return current Mean Squared Error value
    float getMSE(void);

    // add two vectors
    std::vector<float> addVector(std::vector<float> x1, std::vector<float> x2);

    // divide vector by scaler
    std::vector<float> divideVector(std::vector<float> v, int s);

    // initialize each center values u_i to a randomly chosen data point
    void initCenters();

    // Assign each data point x_i to the closest center u_j
    void assignDataPoints();

    // Update each center of sets u_i to the average of all data points who belong to that set
    void updateCenters();

    // Make decision if it's converged or not
    bool hasConverged(float prevError, float currentError);

public:
    KMeans(int n, int d, int k, std::vector<std::vector<float> > x);

    // Calling this function will do everything for the user
    void kMeansClustering();
    std::vector<std::vector<float> > getData();
    std::vector<int> getLabel();

};

#endif
