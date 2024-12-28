/*
 * Richard Adler
 * SNHU CS300 Project 2
 */

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <algorithm>
#include <bits/stdc++.h>
#include <time.h>


using namespace std;

//=======================================================================================
// Course  and course node  structs
//=======================================================================================

// struct to hold course information
struct Course {
	string courseId;
	string courseName;
	vector<string> coursePrereqs;

	// default constructor, sets empty to true to make for an easy check later
	Course() {}
};

// Binary tree node
// credit to SNHU, Prof Norman Lipcott from Module 5 assignment for Node struct
struct Node {
	Course course;
	Node *left;
	Node *right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// constructor with a course
	Node(Course aCourse) :
		Node() {
			course = aCourse;
	}
};

//=====================================================================================
// Binary Tree Class Definition 
//=====================================================================================
// credit to SNHU, Prof Norman Lipcott from Module 5 assignment for tree structure 
// remove function isn't strictly needed for this assignment, but it should still be here, and 
// it's copied and modified slightly to work with course nodes here, so I kept it

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
		Node* root = nullptr;
}

/*
 * credit: Gregory Schoenmakers
 * psuedocode: https://www.quora.com/How-do-you-go-about-deleting-a-subtree-from-a-binary-search-tree-With-the-input-as-the-guide-to-which-subtree-you-want-to-delete
 */
// helper function to recurse through and delete entire tree
void deleteTree(Node* node) {
	if (node != nullptr) {
		deleteTree(node->left);
		deleteTree(node->right);
		delete node;
	}
}

/**
 * Destructor
 */
 BinarySearchTree::~BinarySearchTree() {
		deleteTree(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
		inOrder(root);

}

/*
 * @params course the new course to add to the tree
 */
void BinarySearchTree::Insert(Course course) {
		// check if tree is empty
		if (root == nullptr) {
				// tree is empty, set root equal to created node
				root = new Node(course);	
		} else {
				// tree is not empty, call addNode to insert node into tree
				addNode(root, course);	
		}
}

// simple helper function to print course information
void printCourse(Course course) {
  cout << course.courseId << ", " << course.courseName << endl;
	if (!course.coursePrereqs.empty()) {
		cout << "Prerequisites: | ";
		for (int i = 0; i < course.coursePrereqs.size(); i++) {
    	cout << course.coursePrereqs.at(i) << " | ";
		}
		cout << endl;
	}
}

/*
 * print each element in the tree (recursive)
 */
void BinarySearchTree::inOrder(Node* node) {
	// check for end of list, try left if not
	if (node != nullptr) {
		inOrder(node->left);
		// if left function returns, print course 
		printCourse(node->course);
		// after printing, check for greater values to the right
		inOrder(node->right);
	}
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
		removeNode(root, courseId);
}

/**
 *  @params courseId the id of the course to search for
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {

		Node* currNode = root;
		while (currNode != nullptr) {
			// match found, return course
			if (currNode->course.courseId == courseId) {
				return currNode->course;
			// match not found, node's course id is less than search, try left node
			} else if (currNode->course.courseId < courseId) {
				currNode = currNode->left;
			// match not found, node's course id is greater than or equal to search, try right node
			} else {
				currNode = currNode->right;
			}
		}
    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 */
void BinarySearchTree::addNode(Node* node, Course course) {

		if (node->course.courseId < course.courseId) { 
				// course is less than current node
				if (node->left == nullptr) {
						// left node is empty, add to left of current node
						Node* newNode = new Node(course);
						node->left = newNode;
				} else {
					// left node is not empty, traverse left
					addNode(node->left, course);
				}
		} else if (node->course.courseId >= course.courseId) { 
				// course is greater than or equal to current node
				if (node->right == nullptr) {
						// right node is empty, add to right of current node
						Node* newNode = new Node(course);
						node->right = newNode;
				} else {
					// right node is not empty, traverse right
					addNode(node->right, course);
				}
		}
}

/**
 * Remove a course from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string courseId) {

		// we've reached a node without any data
		if (node == nullptr) 
				return node;
		if (node->course.courseId < courseId) {
				// courseId is less than node, traverse left
				node->left = removeNode(node->left, courseId);
		} else if (node->course.courseId > courseId) {
				// courseId is greater than node, traverse right
				node->right = removeNode(node->right, courseId);
		} else if (node->course.courseId == courseId) {
				// match found, node has either one right child or no children
				if (node->left == nullptr) {
						Node* succNode = node->right;
						delete node;
						return succNode;
				} else if (node->right == nullptr) {
						// node has either one left child or no children
						Node* succNode = node->left;
						delete node;
						return succNode;
				} else {
						// node has two children, find succesor at the leftmost position after the right node
						// delete the current node, and return the successor node to replace the node in that
						// position
						Node* currNode = node->right;
						while (currNode != nullptr && currNode->left != nullptr) {
								currNode = currNode->left;
						}

						node->course = currNode->course;
						node->right = currNode->right;
						node->right = removeNode(node->right, currNode->course.courseId);
				}
		}
		return node;
}

//=====================================================================================
// File Handling
//=====================================================================================
// File handling comes after data formatting, as formatting functions are needed while
// intaking data from the file
/*
 * @params str the string to strip characters from
 * ensures that no unexpected characters make their way into the data structure
 */
void stripInvalidChars(string& str) {
  str.erase(remove(str.begin(), str.end(), '\n'), str.cend());
	// holy heck, finding the carriage return took so long
	// it continually messed up the course prereq vectors
  str.erase(remove(str.begin(), str.end(), '\r'), str.cend());

  while (!str.empty() && str.back() == ',') {
    // Remove the last character if it's a comma
    str.pop_back();
  }
}

/*
 * @params courseTree the binary tree of courses
 * @params fileName the name of the file to intake data from
 * intakes csv file data, adds course data to the correct fields
 * and appends node to binary tree
 */
void parseCsv(BinarySearchTree* courseTree, string fileName) {

  // Open file
  ifstream infile;
  infile.open(fileName);

  // Insert data into courses
  if (infile.is_open()) {
    string line;

    // Read file line by line using while loop
    while (getline(infile, line)) {
      stripInvalidChars(line);
      vector <string> tokens;
      string token;
      stringstream ss(line);

      // Tokenizing at ','
      while (getline(ss, token, ',')) {
        tokens.push_back(token);
      }

      // Create new course and add data from file
      Course newCourse = Course();
      newCourse.courseId = tokens.at(0);
      newCourse.courseName = tokens.at(1);
			
      // If prereqs exist add to course
      if (tokens.size() > 2) {
        for (int i = 2; i < tokens.size(); i++) {
					stripInvalidChars(tokens.at(i));
					if (!tokens.at(i).empty()) { 
						newCourse.coursePrereqs.push_back(tokens[i]);
					}
        }
      }

      // Add new course to courses
      courseTree->Insert(newCourse);
    }

    cout << "Courses loaded successfully." << endl;
  } else {
    cout << fileName << " could not be opened." << endl; 
  }
  infile.close();
}
//=====================================================================================
// Menu handling
//=====================================================================================

/*
 * takes no parameters
 * input validiation, ensures s\user input is either an int or discarded
 */
int validateInput() {

	int userInput;
	cin >> userInput; 
	cout << endl;

	if (!std::cin.good()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return -1;
	} else {
		return userInput;
	}
}
/*
 * Takes no paramaters
 * Simple display function for main menu
 */
void DisplayMenu() {
	cout << "[1] Load courses" << endl;
	cout << "[2] Print all courses" << endl;
	cout << "[3] Search for course" << endl;
	cout << "[9] Exit program" << endl;
	cout << "Menu input: ";
}

/*
 * @params courseTree the binary tree of courses to search through
 */
void menuSearch(BinarySearchTree* courseTree) {
	Course query; 
	cout << "Input the course to search for" << endl;
	string searchQuery;
	cout << "Enter Search: ";
	cin >> searchQuery;
	transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::toupper);
	query = courseTree->Search(searchQuery);	
	if (!query.courseId.empty()) {
		printCourse(query);
	} else {
		cout << searchQuery << " was not found in the course list." << endl;
	}
}

// main function
int main() {

	BinarySearchTree* courseTree = new BinarySearchTree;
  string fileName = "CS 300 ABCU_Advising_Program_Input.csv";

	int userInput = 0;
	clock_t ticks;

	cout << "Welcome to the SNHU course planner, please choose an option from the menu below." << endl << endl;

	while(userInput != 9) {
		DisplayMenu();
		userInput = validateInput();
		cout << endl;
		switch(userInput) {
			case 1:
				// load course info into data stucture
				parseCsv(courseTree, fileName);
        // Calculate elapsed time and display result
        ticks = clock() - ticks; // current clock ticks minus starting clock ticks
        cout << "time: " << ticks << " clock ticks" << endl;
        cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
				break;
			case 2:
				// print all courses 
				courseTree->InOrder();
        // Calculate elapsed time and display result
        ticks = clock() - ticks; // current clock ticks minus starting clock ticks
        cout << "time: " << ticks << " clock ticks" << endl;
        cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
				break;
			case 3:
				// search for course
				menuSearch(courseTree);
        // Calculate elapsed time and display result
        ticks = clock() - ticks; // current clock ticks minus starting clock ticks
        cout << "time: " << ticks << " clock ticks" << endl;
        cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
				break;
			case 9:
				cout << "Exiting program, thank you for using the course planner." << endl;
				break;
			default:
				cout << "Please select from the following menu" << endl;
				break;
		}
		cout << endl;
	}
}
