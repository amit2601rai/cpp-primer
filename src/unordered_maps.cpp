/**
 * @file unordered_maps.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for C++ Standard Library (STL) unordered_map.
 */

// In this file, we will introduce the container std::unordered_map. We won't
// be able to cover every function in this container, but we will try to cover
// the bare bones of using this container. Look to the intro of vectors.cpp for
// a general overview of C++ STL containers.

// There is documentation on all the other functions, and other containers
// on https://en.cppreference.com/w/cpp/container. You will definitely need this
// resource as you complete the assignments in this class, so you should check
// it out!

// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the unordered_map container library header.
#include <unordered_map>
// Includes the C++ string library.
#include <string>
// Includes std::make_pair.
#include <utility>
// Includes algorithms like std::count_if and std::max_element
#include <algorithm>

int main() {
  // The std::unordered_map is a data structure that contains key-value pairs
  // with unique keys. Essentially, this means you can use it as a hash table
  // in your code.

  // You can declare a unordered_map with string keys and int values with
  // the following syntax.
  std::unordered_map<std::string, int> map;

  // The insert function is used to insert items into an unordered map.
  map.insert({"foo", 2});

  // The insert function also takes in a std::pair as the argument. An
  // std::pair is a generic pair type, and you can create one by calling
  // std::make_pair with 2 arguments. std::make_pair is defined in the header
  // <utility>, and constructs an instance of the generic pair type.
  map.insert(std::make_pair("jignesh", 445));

  // You can also insert multiple elements at a time by passing in an
  // initializer list of pairs.
  map.insert({{"spam", 1}, {"eggs", 2}, {"garlic rice", 3}});

  // It is also possible to insert an element via array-style syntax,
  // even if the element did not exist previously.
  map["bacon"] = 5;

  // You can also update an element in the unordered map with the same syntax.
  map["spam"] = 15;

  // The find function is used to find elements in an unordered map. It returns
  // an iterator pointing to the found element if the element exists, and
  // returns an iterator pointing to the end of the unordered map container
  // otherwise.
  std::unordered_map<std::string, int>::iterator result = map.find("jignesh");
  if (result != map.end()) {
    // This is one way of accessing the key/value pair from the iterator.
    std::cout << "Found key " << result->first << " with value "
              << result->second << std::endl;

    // Dereferencing the iterator is another method of accessing the key/value
    // pair from the iterator.
    std::pair<std::string, int> pair = *result;
    std::cout << "DEREF: Found key " << pair.first << " with value "
              << pair.second << std::endl;
  }

  // The count function returns the number of elements in an unordered map with
  // the specified key in the unordered map.
  size_t count = map.count("spam");
  if (count == 1) {
    std::cout
        << "A key-value pair with key spam exists in the unordered map.\n";
  }

  // The erase function deletes values from the unordered map. It can take a
  // key as an argument.
  
  // Let's demonstrate how erase affects the size
  std::cout << "\n=== ERASE AND SIZE DEMONSTRATION ===" << std::endl;
  std::cout << "Size before erasing 'eggs': " << map.size() << std::endl;
  
  map.erase("eggs");
  
  std::cout << "Size after erasing 'eggs': " << map.size() << std::endl;

  // We confirm that the eggs/2 key-value pair isn't in the map anymore.
  if (map.count("eggs") == 0) {
    std::cout << "Key-value pair with key eggs does not exist in the unordered "
                 "map.\n";
  }

  // Additionally, if we want to erase an element at a certain position, we can
  // pass in an iterator to the erase function. The following code will erase
  // the key-value pair with the key "garlic rice". Note that std::next is an
  // iterator function that returns the successor of the iterator passed in as
  // its argument.
  
  std::cout << "Size before erasing 'garlic rice': " << map.size() << std::endl;
  
  map.erase(map.find("garlic rice"));
  
  std::cout << "Size after erasing 'garlic rice': " << map.size() << std::endl;

  // We confirm that garlic rice/3 key-value pair isn't in the map anymore.
  if (map.count("garlic rice") == 0) {
    std::cout << "Key-value pair with key garlic rice does not exist in the "
                 "unordered map.\n";
  }

  // We can iterate through the unordered map elements via the unordered map
  // iterator. You cannot iterate through a unordered map via indexes of any
  // kind.
  std::cout << "Printing the elements of the iterator:\n";
  for (std::unordered_map<std::string, int>::iterator it = map.begin();
       it != map.end(); ++it) {
    // We can access the element itself by dereferencing the iterator.
    std::cout << "(" << it->first << ", " << it->second << "), ";
  }
  std::cout << "\n";

  // Just like std::vector, we can also iterate through the unordered map
  // via a for-each loop.
  std::cout << "Printing the elements of the iterator with a for-each loop:\n";
  for (const std::pair<const std::string, int> &elem : map) {
    std::cout << "(" << elem.first << ", " << elem.second << "), ";
  }
  std::cout << "\n";

  // We discuss more stylistic and readable ways of iterating through C++ STL
  // containers in auto.cpp! Check it out if you are interested.

  std::cout << "\n=== COMPREHENSIVE ELEMENT COUNTING EXAMPLES ===\n" << std::endl;
  
  // Create a fresh map for counting demonstrations
  std::unordered_map<std::string, int> counting_map;
  counting_map.insert({
    {"apple", 5},
    {"banana", 3},
    {"cherry", 8},
    {"date", 2},
    {"elderberry", 1}
  });
  
  std::cout << "Original map contents:" << std::endl;
  for (const auto& pair : counting_map) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }
  
  // Method 1: Using size() to count total elements
  std::cout << "\nMethod 1 - Total element count using size():" << std::endl;
  std::cout << "Total number of key-value pairs: " << counting_map.size() << std::endl;
  
  // Method 2: Using count() to check if specific key exists (returns 0 or 1)
  std::cout << "\nMethod 2 - Using count() to check key existence:" << std::endl;
  std::string search_keys[] = {"apple", "grape", "banana", "kiwi"};
  
  for (const std::string& key : search_keys) {
    size_t key_count = counting_map.count(key);
    std::cout << "Key '" << key << "' exists: " << (key_count > 0 ? "Yes" : "No") 
              << " (count: " << key_count << ")" << std::endl;
  }
  
  // Method 3: Counting elements that meet certain criteria
  std::cout << "\nMethod 3 - Counting elements based on value criteria:" << std::endl;
  
  // Count elements with value > 3
  int count_greater_than_3 = 0;
  for (const auto& pair : counting_map) {
    if (pair.second > 3) {
      count_greater_than_3++;
    }
  }
  std::cout << "Elements with value > 3: " << count_greater_than_3 << std::endl;
  
  // Count elements with value <= 2
  int count_less_equal_2 = 0;
  for (const auto& pair : counting_map) {
    if (pair.second <= 2) {
      count_less_equal_2++;
    }
  }
  std::cout << "Elements with value <= 2: " << count_less_equal_2 << std::endl;
  
  // Method 4: Counting using std::count_if algorithm
  
  // Count elements with even values
  int even_count = std::count_if(counting_map.begin(), counting_map.end(),
    [](const std::pair<const std::string, int>& pair) {
      return pair.second % 2 == 0;
    });
  std::cout << "Elements with even values: " << even_count << std::endl;
  
  // Count elements with odd values  
  int odd_count = std::count_if(counting_map.begin(), counting_map.end(),
    [](const std::pair<const std::string, int>& pair) {
      return pair.second % 2 == 1;
    });
  std::cout << "Elements with odd values: " << odd_count << std::endl;
  
  // Method 5: Counting elements based on key characteristics
  std::cout << "\nMethod 5 - Counting based on key characteristics:" << std::endl;
  
  // Count keys with length > 5
  int long_key_count = 0;
  for (const auto& pair : counting_map) {
    if (pair.first.length() > 5) {
      long_key_count++;
    }
  }
  std::cout << "Keys with length > 5: " << long_key_count << std::endl;
  
  // Count keys starting with specific letter
  char target_letter = 'a';
  int keys_starting_with_a = std::count_if(counting_map.begin(), counting_map.end(),
    [target_letter](const std::pair<const std::string, int>& pair) {
      return !pair.first.empty() && pair.first[0] == target_letter;
    });
  std::cout << "Keys starting with '" << target_letter << "': " << keys_starting_with_a << std::endl;
  
  // Method 6: Using empty() to check if map has any elements
  std::cout << "\nMethod 6 - Checking if map is empty:" << std::endl;
  std::cout << "Map is empty: " << (counting_map.empty() ? "Yes" : "No") << std::endl;
  std::cout << "Map has elements: " << (!counting_map.empty() ? "Yes" : "No") << std::endl;
  
  // Method 7: Counting elements in buckets (hash table internals)
  std::cout << "\nMethod 7 - Hash table bucket information:" << std::endl;
  std::cout << "Number of buckets: " << counting_map.bucket_count() << std::endl;
  std::cout << "Load factor: " << counting_map.load_factor() << std::endl;
  std::cout << "Max load factor: " << counting_map.max_load_factor() << std::endl;
  
  // Count non-empty buckets
  size_t non_empty_buckets = 0;
  for (size_t i = 0; i < counting_map.bucket_count(); ++i) {
    if (counting_map.bucket_size(i) > 0) {
      non_empty_buckets++;
    }
  }
  std::cout << "Non-empty buckets: " << non_empty_buckets << std::endl;
  
  // Method 8: Real-world example - frequency counting
  std::cout << "\nMethod 8 - Practical example: Word frequency counting" << std::endl;
  std::unordered_map<std::string, int> word_frequency;
  std::string text_words[] = {"hello", "world", "hello", "cpp", "world", "programming", "hello"};
  
  // Count frequency of each word
  for (const std::string& word : text_words) {
    word_frequency[word]++;  // If key doesn't exist, it's created with value 0, then incremented
  }
  
  std::cout << "Word frequencies:" << std::endl;
  for (const auto& pair : word_frequency) {
    std::cout << "'" << pair.first << "': " << pair.second << " times" << std::endl;
  }
  
  std::cout << "Total unique words: " << word_frequency.size() << std::endl;
  
  // Find most frequent word
  auto max_element = std::max_element(word_frequency.begin(), word_frequency.end(),
    [](const std::pair<const std::string, int>& a, const std::pair<const std::string, int>& b) {
      return a.second < b.second;
    });
  
  if (max_element != word_frequency.end()) {
    std::cout << "Most frequent word: '" << max_element->first 
              << "' (appears " << max_element->second << " times)" << std::endl;
  }
  
  // Method 9: Comparing sizes of different maps
  std::cout << "\nMethod 9 - Comparing map sizes:" << std::endl;
  std::unordered_map<int, std::string> number_map = {{1, "one"}, {2, "two"}, {3, "three"}};
  
  std::cout << "counting_map size: " << counting_map.size() << std::endl;
  std::cout << "word_frequency size: " << word_frequency.size() << std::endl;
  std::cout << "number_map size: " << number_map.size() << std::endl;
  
  if (counting_map.size() > word_frequency.size()) {
    std::cout << "counting_map has more elements than word_frequency" << std::endl;
  } else if (counting_map.size() < word_frequency.size()) {
    std::cout << "word_frequency has more elements than counting_map" << std::endl;
  } else {
    std::cout << "counting_map and word_frequency have the same number of elements" << std::endl;
  }

  // === COMPREHENSIVE ERASE AND SIZE DEMONSTRATION ===
  std::cout << "\n=== COMPREHENSIVE ERASE OPERATIONS AND SIZE CHANGES ===" << std::endl;
  
  // Create a fresh map for erase demonstrations
  std::unordered_map<std::string, int> erase_demo_map = {
    {"apple", 5}, {"banana", 3}, {"cherry", 8}, {"date", 2}, 
    {"elderberry", 1}, {"fig", 6}, {"grape", 4}
  };
  
  std::cout << "Initial map contents and size:" << std::endl;
  std::cout << "Size: " << erase_demo_map.size() << std::endl;
  for (const auto& pair : erase_demo_map) {
    std::cout << "  " << pair.first << ": " << pair.second << std::endl;
  }
  
  // 1. Erase by key - returns number of elements removed
  std::cout << "\n1. Erasing by key ('banana'):" << std::endl;
  std::cout << "Size before erase: " << erase_demo_map.size() << std::endl;
  size_t erased_count = erase_demo_map.erase("banana");
  std::cout << "Number of elements erased: " << erased_count << std::endl;
  std::cout << "Size after erase: " << erase_demo_map.size() << std::endl;
  
  // 2. Erase by iterator - returns iterator to next element
  std::cout << "\n2. Erasing by iterator ('cherry'):" << std::endl;
  std::cout << "Size before erase: " << erase_demo_map.size() << std::endl;
  auto it = erase_demo_map.find("cherry");
  if (it != erase_demo_map.end()) {
    auto next_it = erase_demo_map.erase(it);
    std::cout << "Element erased successfully" << std::endl;
    if (next_it != erase_demo_map.end()) {
      std::cout << "Next element after erased: " << next_it->first << std::endl;
    }
  }
  std::cout << "Size after erase: " << erase_demo_map.size() << std::endl;
  
  // 3. Erase by range - erase multiple elements
  std::cout << "\n3. Erasing by range (first two elements):" << std::endl;
  std::cout << "Size before range erase: " << erase_demo_map.size() << std::endl;
  auto range_start = erase_demo_map.begin();
  auto range_end = std::next(range_start, 2);  // Erase first 2 elements
  auto after_range = erase_demo_map.erase(range_start, range_end);
  std::cout << "Size after range erase: " << erase_demo_map.size() << std::endl;
  
  // 4. Attempt to erase non-existent key
  std::cout << "\n4. Attempting to erase non-existent key ('orange'):" << std::endl;
  std::cout << "Size before erase attempt: " << erase_demo_map.size() << std::endl;
  size_t not_found_count = erase_demo_map.erase("orange");
  std::cout << "Number of elements erased: " << not_found_count << std::endl;
  std::cout << "Size after erase attempt: " << erase_demo_map.size() << std::endl;
  
  // 5. Clear all elements
  std::cout << "\n5. Clearing all elements:" << std::endl;
  std::cout << "Size before clear: " << erase_demo_map.size() << std::endl;
  erase_demo_map.clear();
  std::cout << "Size after clear: " << erase_demo_map.size() << std::endl;
  std::cout << "Is map empty? " << (erase_demo_map.empty() ? "Yes" : "No") << std::endl;
  
  // 6. Conditional erase based on value
  std::cout << "\n6. Conditional erase (remove all values < 5):" << std::endl;
  std::unordered_map<std::string, int> conditional_map = {
    {"alpha", 2}, {"beta", 7}, {"gamma", 3}, {"delta", 9}, {"epsilon", 1}
  };
  
  std::cout << "Before conditional erase - Size: " << conditional_map.size() << std::endl;
  for (const auto& pair : conditional_map) {
    std::cout << "  " << pair.first << ": " << pair.second << std::endl;
  }
  
  // Erase elements with value < 5
  for (auto it = conditional_map.begin(); it != conditional_map.end();) {
    if (it->second < 5) {
      it = conditional_map.erase(it);  // erase returns iterator to next element
    } else {
      ++it;
    }
  }
  
  std::cout << "After conditional erase - Size: " << conditional_map.size() << std::endl;
  for (const auto& pair : conditional_map) {
    std::cout << "  " << pair.first << ": " << pair.second << std::endl;
  }
  
  // 7. Performance consideration: erase vs clear
  std::cout << "\n7. Performance note:" << std::endl;
  std::cout << "- erase(key): O(1) average, O(n) worst case" << std::endl;
  std::cout << "- erase(iterator): O(1) always" << std::endl;
  std::cout << "- erase(range): O(distance) between iterators" << std::endl;
  std::cout << "- clear(): O(n) - removes all elements" << std::endl;
  std::cout << "- Each erase operation reduces size by number of elements removed" << std::endl;
  
  // Method 10: Performance considerations for counting
  std::cout << "\nMethod 10 - Performance notes:" << std::endl;
  std::cout << "- size(): O(1) time complexity" << std::endl;
  std::cout << "- count(): O(1) average case, O(n) worst case" << std::endl;
  std::cout << "- find(): O(1) average case, O(n) worst case" << std::endl;
  std::cout << "- Iterating all elements: O(n) time complexity" << std::endl;
  std::cout << "- empty(): O(1) time complexity" << std::endl;

  return 0;
}