#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Define the Point struct
struct Point {
    double x;
    double y;
};

double stringToDouble(const string& str) {
    double result = 0.0;
    stringstream ss(str);
    ss >> result;
    return result;
}

vector<Point> readCSVFile(const string& filename) {
    vector<Point> data;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        Point point;
        stringstream linestream(line);
        string cell;

        if (getline(linestream, cell, ',')) {
            point.x = stringToDouble(cell);
        }

        if (getline(linestream, cell, ',')) {
            point.y = stringToDouble(cell);
        }

        data.push_back(point);
    }

    file.close();
    return data;
}

int main() {
    // Provided dataset
    // vector<Point> data = {
    //     {180, 70},
    //     {175, 64},
    //     {192, 88},
    //     {165, 59},
    //     {160, 56},
    //     {210, 110},
    //     {183, 68},
    //     {176, 64},
    //     {188, 72},
    //     {172, 59}
    // };

    vector<Point> data = readCSVFile("SOCR-HeightWeight.csv");

    // Bayesian Linear Regression using random sampling

    // Priors for the slope (m) and intercept (c)
    double prior_mean_slope = 0.0; // Prior mean for slope
    double prior_mean_intercept = 0.0; // Prior mean for intercept
    double prior_stddev = 1.0; // Standard deviation for the priors

    // Resampling for estimating parameters using random sampling
    const int num_iterations = 10000; // Number of iterations

    double slope = 0.0;
    double intercept = 0.0;

    default_random_engine gen;
    normal_distribution<double> dist(0.0, prior_stddev);

    for (int iter = 0; iter < num_iterations; ++iter) {
        double proposed_slope = slope + dist(gen);
        double proposed_intercept = intercept + dist(gen);

        // Calculate likelihood by assuming a normal distribution
        double log_likelihood_current = 0.0;
        double log_likelihood_proposed = 0.0;

        for (const auto& point : data) {
            double error_current = point.y - (slope * point.x + intercept);
            double error_proposed = point.y - (proposed_slope * point.x + proposed_intercept);

            log_likelihood_current += -(error_current * error_current) / (2 * prior_stddev * prior_stddev);
            log_likelihood_proposed += -(error_proposed * error_proposed) / (2 * prior_stddev * prior_stddev);
        }

        // Accept or reject the proposed parameters based on likelihood
        if (log_likelihood_proposed - log_likelihood_current > 0 || 
            exp(log_likelihood_proposed - log_likelihood_current) > uniform_real_distribution<double>(0, 1)(gen)) {
            slope = proposed_slope;
            intercept = proposed_intercept;
        }

        // Output estimated parameters at certain intervals or at the end
        if ((iter + 1) % 1000 == 0 || iter == num_iterations - 1) {
            cout << "Iteration " << iter + 1 << ": Slope: " << slope << ", Intercept: " << intercept << endl;
        }
    }
    float testInput=165;
    float testOutput=testInput*slope+intercept;

    cout<<"If height is "<<testInput<<" weight is "<<testOutput<<endl;

    return 0;
}
