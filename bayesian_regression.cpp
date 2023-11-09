#include <iostream>
#include <fstream>
#include <sstream>
#define testNum 10
#define dataSize 10

void readDataFromCSV(const std::string& filename, double* features, double* labels, int num_samples) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line) && count < num_samples) {
        std::istringstream linestream(line);
        std::string value;
        if (std::getline(linestream, value, ',') && !value.empty()) {
            try {
                features[count] = std::stod(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error converting height data at line " << count + 1 << ": " << e.what() << std::endl;
                continue;
            }
        } else {
            std::cerr << "Empty or invalid height data at line " << count + 1 << std::endl;
            continue;
        }

        if (std::getline(linestream, value, ',') && !value.empty()) {
            try {
                labels[count] = std::stod(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error converting weight data at line " << count + 1 << ": " << e.what() << std::endl;
                continue;
            }
        } else {
            std::cerr << "Empty or invalid weight data at line " << count + 1 << std::endl;
            continue;
        }

        count++;
    }

    file.close();
}

int main() {
    const int num_samples = dataSize; // Number of data points

    double features[num_samples];
    double labels[num_samples];

    readDataFromCSV("SOCR-HeightWeight.csv", features, labels, num_samples);


    double weights[2] = {0}; // Weights: slope and intercept
    double prior_covariance[2][2] = {{1, 0}, {0, 1}}; // Simple prior covariance matrix
    double prior_precision = 0.1; // Set the precision for the prior covariance
    double sigma = 0.1; // Noise parameter (for likelihood)

    for (int i = 0; i < num_samples; ++i) {
        double X[2] = {1, features[i]}; // Add bias term to features

        double prior_mean[2] = {weights[0], weights[1]}; // Use current weights as prior mean

        double X_T_X[2][2] = {
            {X[0] * X[0], X[0] * X[1]},
            {X[1] * X[0], X[1] * X[1]}
        };

        double posterior_covariance[2][2] = {
            {X_T_X[0][0] / sigma + prior_covariance[0][0], X_T_X[0][1] / sigma + prior_covariance[0][1]},
            {X_T_X[1][0] / sigma + prior_covariance[1][0], X_T_X[1][1] / sigma + prior_covariance[1][1]}
        };

        double determinant = (posterior_covariance[0][0] * posterior_covariance[1][1]) - (posterior_covariance[0][1] * posterior_covariance[1][0]);

        double posterior_precision[2][2] = {
            {posterior_covariance[1][1] / determinant, -posterior_covariance[0][1] / determinant},
            {-posterior_covariance[1][0] / determinant, posterior_covariance[0][0] / determinant}
        };

        double X_T_labels[2] = {X[0] * labels[i], X[1] * labels[i]};

        weights[0] = weights[0] + (
            posterior_precision[0][0] * (prior_precision * prior_covariance[0][0] * prior_mean[0] + X_T_labels[0] / sigma) +
            posterior_precision[0][1] * (prior_precision * prior_covariance[0][1] * prior_mean[1] + X_T_labels[1] / sigma)
        );

        weights[1] = weights[1] + (
            posterior_precision[1][0] * (prior_precision * prior_covariance[1][0] * prior_mean[0] + X_T_labels[0] / sigma) +
            posterior_precision[1][1] * (prior_precision * prior_covariance[1][1] * prior_mean[1] + X_T_labels[1] / sigma)
        );

        prior_covariance[0][0] = posterior_covariance[0][0];
        prior_covariance[0][1] = posterior_covariance[0][1];
        prior_covariance[1][0] = posterior_covariance[1][0];
        prior_covariance[1][1] = posterior_covariance[1][1];
    }

    // Make predictions using the obtained weights
    for (int i = 0; i < testNum; ++i) {
        double predicted_label = weights[1] * features[i] + weights[0]; // Predicted labels
        std::cout << "Predicted label for feature " << features[i] << ": " << predicted_label << std::endl;
    }

    return 0;
}
