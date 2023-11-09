 std::ifstream file("Clustering_gmm.csv");

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    std::vector<Point> data; // Vector to store points

    std::string line;
    // Read each line from the file
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Point p{0,0};

        // Read the weight and height from the current line
        char delimiter;
        if (iss >> p.x >> delimiter >> p.y) {
            data.push_back(p); // Add the point to the vector
        } else {
            //std::cerr << "Error reading line: " << line << std::endl;
            ;
        }
    }

    // Close the file
    file.close();
