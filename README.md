# Example Implementation of a HashMap

### Built With
* [Microsoft Visual Studio](https://visualstudio.microsoft.com/pl/) - IDE
* [C](https://en.wikipedia.org/wiki/C) - Standard Language 

## Introduction to HashMaps
A Hashmap (also known as a hash table) is a data structure that maps key values in order for highly efficient searching (or lookup).The Hashmap implementation utilizes an associative array abstract data type and something called a Hash function. The Hash function is used to compute an index(hash code),into an array of buckets(slots) so the desired value can then be later found.

### Strengths: 
- Fast lookup: runs at O(1) time on average
- Flexible keys: Allows for most data types to be used as keys, as long as they're hashtable.

### Weaknesses: 
- Worst case lookup: Can take O(n) runtime
- Unordered: Keys are not stored in a special order. You must look through all the keys in order to find a specific item that you are looking for.
- Single-Directional: At best you can find an item at O(1) lookup speed but you must loop through the entire data set- O(n) lookup time.
- Not Cache-Friendly.

### Collision
This is something that occurs in a hashmap if two different keys happen to have the same hash code, or if two keys with different hash codes happen to map into the same bucket in the underlying array.This is usually caused by non-uniform distribution and it can be costly to resolve collisions that occur within this data structure.


### Run Times
| Algorithm  | Average | Worst Case |
| ------------- | ------------- | ------------- |
| Space  | O(n) | O(n) |
| Search  | O(1) | O(n) |
| Insert  | O(1) | O(n) |
| Delete  | O(1) | O(n) |

## Project Discription
These files implement the hashmap data structure using a dictionary. The Hashmap structure that is created is utilized in order to create a low level spell checker program.
