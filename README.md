#Bayesian Machine Learning Algorithms
<ul>
  <li>Naive Bayes Email Classifier</li>
  <li>Bayesian Belief Network</li>
  <li>Bayesian Linear Regression</li>
  <li>Expectation Maximization Clustering Algorithm</li>
</ul>

##Naive Bayes Email Classifier
Naive Bayes classifier is implemented for distinguishing between spam and ham (non-spam) emails. 
The algorithm uses a probabilistic approach to calculate the likelihood of an email being spam or ham based on the occurrence of words in the training dataset. 
The classifier assumes that the presence of each word in the email is independent of the presence of other words, which is a key assumption of the Naive Bayes algorithm.

##Bayesian Belief Network
The implemented algorithm constructs a Bayesian Network based on given data. The following steps were performed for network construction:
<li>Data Reading: The algorithm begins by reading the car data from a CSV file. Each row of the dataset contains information about a car's attributes such as price rating (B), maintenance rating (M), number of doors (D), number of persons (P), luggage-boot rating (LB), safety rating (S), and overall class rating (OC). The data is processed to calculate the frequencies of different attribute values.</li>
<li>Node Initialization: Nodes representing different attributes (B, M, D, P, LB, S, OC) are initialized. Each node has a name and a list of possible values.</li>
<li>Conditional Probability Table (CPT) Construction: For each node, a Conditional Probability Table is constructed based on the frequencies of attribute values in the dataset. The CPT represents the probabilities of the node taking on each possible value
given the values of its parent nodes (if any).</li>
<li>Edge Establishment: Edges are established between parent nodes (B, M, D, P, LB, S)
and the child node (OC) to represent dependencies among variables. For example, an edge from B to OC indicates that the overall class rating (OC) depends on the price rating (B). In this case, OC was a child node, while the remaining nodes (B, M, D, P, LB, S) were parent nodes.</li>

##Bayesian Linear Regression
The problem is to find the best-fitting linear model for a given dataset of height-weight pairs. 
The challenge lies in estimating the parameters (slope and intercept) of the linear equation that describes the relationship between height and weight.
The objective is to perform Bayesian Linear Regression using the Metropolis-Hastings algorithm. 
Bayesian Linear Regression is a probabilistic approach to modeling the relationship between variables. 
By incorporating prior knowledge and iteratively updating estimates based on observed data, Bayesian methods provide a principled way to handle uncertainty in regression tasks.

##Expectation Maximization Clustering Algorithm
The Gaussian Mixture Model (GMM) is a probabilistic model used for clustering tasks. 
It assumes that the data points are generated from a mixture of several Gaussian distributions with unknown parameters. 
The GMM algorithm aims to estimate these parameters and assign each data point to one of the clusters represented by the Gaussian distributions.
The implemented GMM algorithm processes a dataset consisting of points with two features: height and weight. 
It leverages the Expectation-Maximization (EM) algorithm, a statistical technique, to estimate the parameters of multiple Gaussian distributions. 
These parameters include cluster means, covariances, and weights. 
The ultimate objective is to cluster the data points into distinct groups based on these learned parameters.
