#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

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

double likelihood(const vector<Point>& data, double slope, double intercept, double prior_stddev) {
    double log_likelihood = 0.0;
    for (const auto& point : data) {
        double error = point.y - (slope * point.x + intercept);
        log_likelihood += -(error * error) / (2 * prior_stddev * prior_stddev);
    }
    return log_likelihood;
}

int main() {
    vector<Point> data = readCSVFile("SOCR-HeightWeight.csv");

    // Priors for the slope (m) and intercept (c)
    double prior_stddev = 1.0; // Standard deviation for the priors

    const int num_iterations = 10000; // Number of iterations

    double slope = 0.0;
    double intercept = 0.0;

    default_random_engine gen;
    normal_distribution<double> dist(0.0, prior_stddev);

    for (int iter = 0; iter < num_iterations; ++iter) {
        double proposed_slope = slope + dist(gen);
        double proposed_intercept = intercept + dist(gen);

        double log_likelihood_current = likelihood(data, slope, intercept, prior_stddev);
        double log_likelihood_proposed = likelihood(data, proposed_slope, proposed_intercept, prior_stddev);

        // Metropolis-Hastings step
        if (log_likelihood_proposed - log_likelihood_current > log(uniform_real_distribution<double>(0, 1)(gen))) {
            slope = proposed_slope;
            intercept = proposed_intercept;
        }

        // Output estimated parameters at certain intervals or at the end
        if ((iter + 1) % 1000 == 0 || iter == num_iterations - 1) {
            cout << "Iteration " << iter + 1 << ": Slope: " << slope << ", Intercept: " << intercept << endl;
        }
    }

    float testInput = 165;
    float testOutput = testInput * slope + intercept;

    cout << "If height is " << testInput << " weight is " << testOutput << endl;

    return 0;
}
