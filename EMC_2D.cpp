#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point(double _x, double _y) : x(_x), y(_y) {}
};

// Initialize cluster parameters
void initializeClusters(int N,int K,vector<Point>& data,vector<double>& cluster_weights,vector<pair<double, double>>& cluster_means,vector<pair<double, double>>& cluster_covariances) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    // Define a distribution
    std::uniform_int_distribution<> dis(1, 500);
    for (int k = 0; k < K; ++k) {
        // Generate a random number
        int random_number = dis(gen);
        cout<<"random no"<<random_number<<endl;
        cluster_weights[k] = 1.0 / K;
        cluster_means[k] =make_pair(data[random_number].x,data[random_number].y);
        cluster_covariances[k] = std::make_pair(1.0, 1.0);
    }
}

// Define a Gaussian function for 2D data
double Gaussian2D(Point& x, pair<double, double>& mean,pair<double, double>& covariance) {
    double normalization = 1.0 / (2 * M_PI * sqrt(covariance.first * covariance.second));
    double exponent = -0.5 * ((x.x - mean.first) * (x.x - mean.first) / covariance.first + (x.y - mean.second) * (x.y - mean.second) / covariance.second);
    return normalization * exp(exponent);
}

// Check for convergence
bool hasConverged(double prevLL, double currLL, double epsilon) {
    double logLikelihoodChange = std::abs(currLL - prevLL);
    return logLikelihoodChange < epsilon;
}

int prevLL;
// Expectation-Maximization algorithm
void expectationMaximization(int N,int K,vector<Point>& data,vector<std::vector<double>>& responsibilities,vector<double>& cluster_weights,vector<pair<double, double>>& cluster_means,vector<pair<double, double>>& cluster_covariances) {
    const double epsilon = 1; // Convergence criterion
    bool converged = false;
    int max_iterations = 100;
    int iteration = 0;

    initializeClusters(N,K,data,cluster_weights,cluster_means,cluster_covariances);
    while (!converged && iteration < max_iterations) {
        // E-step
        double currLL=0;
        for (int i = 0; i < N; i++) {
            double total_weight = 0.0;
            for (int k = 0; k < K; k++) {
                responsibilities[i][k] = cluster_weights[k] * Gaussian2D(data[i], cluster_means[k], cluster_covariances[k]);
                total_weight += responsibilities[i][k];
            }

            // Normalize responsibilities
            for (int k = 0; k < K; k++) {
                responsibilities[i][k] /= total_weight;
                currLL+=log(responsibilities[i][k]);
            }
        }

        // M-step
        for (int k = 0; k < K; k++) {
            double Nk = 0.0;
            double sum_x = 0.0;
            double sum_y = 0.0;
            double sum_x2 = 0.0;
            double sum_y2 = 0.0;

            for (int i = 0; i < N; i++) {
                Nk += responsibilities[i][k];
                sum_x += responsibilities[i][k] * data[i].x;
                sum_y += responsibilities[i][k] * data[i].y;
                sum_x2 += responsibilities[i][k] * (data[i].x * data[i].x);
                sum_y2 += responsibilities[i][k] * (data[i].y * data[i].y);
            }

            // Update cluster parameters
            cluster_means[k] = std::make_pair(sum_x / Nk, sum_y / Nk);
            cluster_covariances[k] = std::make_pair(sum_x2 / Nk - cluster_means[k].first * cluster_means[k].first, sum_y2 / Nk - cluster_means[k].second * cluster_means[k].second);
            cluster_weights[k] = Nk / N;
        }

        // Check for convergence 
        converged = hasConverged(prevLL, currLL, epsilon);
        if(converged)
            cout<<"\nhs Converged, iter no :"<<iteration<<"\n";
        prevLL=currLL;

        iteration++;
    }
}

int main() {

    std::ifstream file("Clustering_gmm.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }
    std::vector<Point> data; // Vector to store points
    std::string line;
    // File reading
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Point p{0,0};

        // Read the weight and height from the current line and add the point to the vector
        char delimiter;
        if (iss >> p.x >> delimiter >> p.y) {
            data.push_back(p); // Add the point to the vector
        }
    }
    file.close();

    int N = data.size();
    cout<<N<<"\n";

    // Define the number of clusters (K)
    int K = 3;
    // Define the cluster parameters
    std::vector<double> cluster_weights(K);
    std::vector<std::pair<double, double>> cluster_means(K);
    std::vector<std::pair<double, double>> cluster_covariances(K);
    // Responsibilities matrix
    std::vector<std::vector<double>> responsibilities(N, std::vector<double>(K));

    // Run the Expectation-Maximization algorithm
    expectationMaximization(N,K,data,responsibilities,cluster_weights,cluster_means,cluster_covariances);

    vector<int> c(K,0);
    // Printing results
    for(int i=0;i<N;i++){
        //rounding to convert probability into 1 or 0;
        cout<<i+1<<". "<<data[i].y/(data[i].x*data[i].x)<<" : "<<round(responsibilities[i][0])<<" , "<<round(responsibilities[i][1])<<" , "<<round(responsibilities[i][2])<<endl;
        for(int j=0;j<K;j++){
            c[j]+=round(responsibilities[i][j]);
        }
    }

    //count of data-points in each cluster
    for(int j=0;j<K;j++){
        cout<<"Cluster"<<j<<" : "<<"Cluster_Mean = ("<<cluster_means[j].first<<","<<cluster_means[j].second<<"), Cluster_Covariance = ("<<cluster_covariances[j].first<<","<<cluster_covariances[j].second<<"), Cluster_Size = "<<c[j]<<endl;
    }
    return 0;
}