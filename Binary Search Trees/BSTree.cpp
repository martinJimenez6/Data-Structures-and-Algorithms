#include "BSTree.h"
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

///////////////////////////////////
// Already Implemented Functions //
///////////////////////////////////

// These functions are already implemented for you. Scroll below to
// see the functions that you have to implement 


// constructors
BSTree::BSTree() : size(0), root(nullptr) {}

// input / output operators
ostream& operator<<(ostream& out, BSTree& tree)
{
    tree.print_level_by_level(out);
    return out;
}

ostream& operator<<(std::ostream& out, Node& node)
{
    return out << node.value << "[" << node.search_time << "] ";
}

istream& operator>>(istream& in, BSTree& tree)
{
    /*
      take input from the in stream, and build your tree
      input will look like
      4 
      2 
      6 
      1 
      3 
      5 
      7
    */
    int next;
    while(in >> next) {
        tree.insert(next);
    }
    return in;
}

// Example recursive function
// If you try to use it without care, you will get a memory leak.
void BSTree::copy_helper(Node*& newNode, const Node* sourceNode) {
    //Don't copy if the node is nullptr
    if(sourceNode == nullptr)
        return;

    //Change the new node to a copy of sourceNode
    newNode = new Node(sourceNode->value);
    //Copy over the search cost
    newNode->search_time = sourceNode->search_time;

    //Copy left subtree
    if (sourceNode->left != nullptr)
        copy_helper(newNode->left, sourceNode->left);
    //Copy right subtree
    if(sourceNode->right != nullptr)
        copy_helper(newNode->right, sourceNode->right);
}

// recursive function
int BSTree::get_total_search_time(Node* node)
{
  if (node != nullptr) {
    return node->search_time + get_total_search_time(node->left) + get_total_search_time(node->right);
  }
  return 0;
}

// implemented
float BSTree::get_average_search_time()
{
  int total_search_time = get_total_search_time(root);
  if(total_search_time == 0)
    return -1;
	
  return ((float)total_search_time)/size;
}


///////////////////////////////////
//     Functions to Implement    //
///////////////////////////////////

// These are the functions you should implement
// Feel free to call the functions above or create new helper functions

// copy constructor
BSTree::BSTree(const BSTree& other)
{
    if(other.root == nullptr){
        this->root = nullptr;
        this->size = 0;
        return;
    }
    copy_helper(this->root, other.root);
    this->size = other.size;
}

// move constructor
BSTree::BSTree(BSTree&& other)
{
    this->root = other.root;
    this->size = other.size;

    other.root = nullptr;
    other.size = 0;
}

//copy assignment
BSTree& BSTree::operator=(const BSTree& other) 
{
    if(this == &other){
        return *this;
    }

    if (root != nullptr){//check to see if this is empty, if not make it empty
        this->delete_tree(root);
        root = nullptr;
    }

    copy_helper(this->root, other.root);
    this->size = other.size;

    return *this;
}

// move assignment
BSTree& BSTree::operator=(BSTree&& other)
{
    if(this == &other){
        return *this;
    }
    
    if(this->root != nullptr){
        this->delete_tree(this->root);
        root = nullptr;
    }

    this->root = other.root;
    this->size = other.size;

    
    other.root = nullptr;
    other.size = 0;

    return *this;
}

// destructor
BSTree::~BSTree()
{
    // Make sure to call delete on every node of the tree
    // You can use a recursive helper function to do this
    delete_tree(root);
    root = nullptr;
}

void BSTree::delete_tree(Node* root){//root = root of subtree you wanna delete 
    //postorder
    if(root == nullptr){
        return;
    }

    if(root->left != nullptr){
        delete_tree(root->left);
    }

    if(root->right != nullptr){
        delete_tree(root->right);
    }
    delete root;
}

Node* BSTree::insert(int obj)
{
    /* insert a node into the tree
    first find where the node should go
    then modify pointers to connect your new node */
    Node** cursor = &root;
    int depth = 1;//cuz of the plus one you add to depth to get search cost

    while(*cursor){
        depth++;
        if((*cursor)->value < obj)
            cursor = &(*cursor)->right;
        else if((*cursor)->value > obj)
            cursor = &(*cursor)->left;
        //else      don't need this since we are assuming no duplicates
            //return;
    }

    *cursor = new Node(obj);
    (*cursor)->search_time = depth;
    this->size += 1;
    return *cursor;
}

Node* BSTree::search(int obj)
{
    /* recursively search down the tree to find the node that contains obj
    if you don't find anything return nullptr */

    //lets use a temp to make temporary subtrees
    BSTree* temp = new BSTree();

    if(this->root == nullptr){//handles if the tree is empty
        return nullptr;
    }
    else if (obj < this->root->value){
        copy_helper(temp->root, this->root->left);
        return temp->search(obj);
    }
    else if (obj > this->root->value){
        copy_helper(temp->root, this->root->right);
        return temp->search(obj);
    }
    else
        return this->root;//root of the subtree that we are in right?
   
    delete temp;
    return nullptr;
}   

void BSTree::update_helper(Node* node, int depth){
    node->search_time = depth+1;
    depth++;
    if(node->left != nullptr){
        update_helper(node->left, depth);
    }
    if(node->right != nullptr){
        update_helper(node->right, depth);
    }
}

void BSTree::update_search_times()
{
    /* do a BFS or DFS of your tree and update the search times of all nodes at once
      The root has a search time of 1, and each child is 1 more than its parent */
    update_helper(root, 0);
    cout << std::endl;
}

void BSTree::print(Node* node, std::ostream& out){
    if(node == nullptr){
        return;
    }
    print(node->left, out);
    out << *node;
    print(node->right, out);
   

}

void BSTree::inorder(std::ostream& out)
{
    /*print your nodes in infix order
    If our tree looks like this:
 
       4
     2   6
    1 3 5 7

    It should output:
    1[3] 2[2] 3[3] 4[1] 5[3] 6[2] 7[3]
    You can use the << operator to print the nodes in the format shown */
    if(root != nullptr){
        print(this->root, out);
        out <<std::endl;
    }
}

void BSTree::get_depth(Node* node, int& val){
    if(node != nullptr){
        val = std::max(val, node->search_time);
        get_depth(node->left, val);
        get_depth(node->right, val);
    }
}

void BSTree::print_level_by_level(ostream& out)
{
    /* Print the tree using a BFS so that each level contains the values for a level of the tree.
    Use X to mark empty positions. 
    
      if our tree looks like this:

       4
     2   6
    1   5 7
           9

    it should output:

    4[1]
    2[2] 6[2]
    1[3] X 5[3] 7[3]
    X X X X X X X 9[4]

    it might be helpful to do this part with the std::queue data structure.
    Hint: the Nth level contains 2^(N-1) nodes (with a value or with an X). Watch out
    for nodes like the left child of 6 above, and print the descendents of an empty
    node as also empty
    You can use the << operator to print nodes in the format shown */
    std::queue<Node*> queue;
    int depth = 0;
    int max_depth = 0;
    if(root != nullptr){
        depth = root->search_time;
    }
    get_depth(root, max_depth);

    if(root != nullptr){
        queue.push(root);
    }

    while(!queue.empty()){
        Node* node = queue.front();
        queue.pop();
        if(depth != node->search_time){
            out << std::endl;
            depth = node->search_time;
        }

        if(node->value == -1){
            out << "X ";
        }
        else{
            out << *node;
        }

        if(depth < max_depth){
            if(node->left != nullptr){
                queue.push(node->left);
            }
            else{
                Node* null_leaf= new Node(-1);
                null_leaf->search_time = node->search_time+1;
                queue.push(null_leaf);
            }
            if(node->right != nullptr){
                queue.push(node->right);
            }
            else{
                Node* null_leaf = new Node(-1);
                null_leaf->search_time = node->search_time+1;
                queue.push(null_leaf);
            }
        }
        if(node->value == -1){
            delete node;
        }
    }
    out << std::endl;
}





