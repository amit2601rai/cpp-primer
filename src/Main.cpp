#include <iostream>
#include <vector>
#include <algorithm>

// Function to find the maximum element in a vector and its position
std::pair<int, int> findMaxElement(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return {0, -1}; // Return 0 and -1 for empty vector
    }
    
    // Find the iterator pointing to the maximum element
    auto maxIt = std::max_element(numbers.begin(), numbers.end());
    
    // Calculate the position (index) of the maximum element
    int position = std::distance(numbers.begin(), maxIt);
    
    // Return both the value and its position
    return {*maxIt, position};
}

int main() {
    // Create a sample vector
    std::vector<int> data = {3, 7, 1, 9, 4, 6, 2};
    
    // Call the function
    auto result = findMaxElement(data);
    
    // Display the results
    std::cout << "Original vector: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Maximum element: " << result.first << std::endl;
    std::cout << "Position: " << result.second << std::endl;
    
    return 0;
}