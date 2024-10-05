#include<bits/stdc++.h>
using namespace std;

class node
{
    private:
        attribute tagData;
        string innerHTML = "";
        node * parent;
    public:
        vector <node *> childs = {};
        node(attribute data, string value, node * prev = NULL)
        {
            innerHTML = value;
            prev = parent;
            tagData = data;
        };
        void insertChild(node * nodeElement)
        {
            childs.push_back(nodeElement);
        }
        string fetchTagData(string attribute_data)
        {
            return tagData[attribute_data];
        }
        string fetchinnerHTML()
        {
            return innerHTML;
        }
        node * fetchparent()
        {
            return parent;
        }
        void putinnerHTML(string data)
        {
            innerHTML += data;
        }
        void putParent(node * p)
        {
            parent = p;
        }
        void putTagdata(string a, string b)
        {
            tagData[a] = b;
        }
        attribute tagDataInfo()
        {
            return tagData;
        }
};

vector<node*> searchQuery(node* root, string key, string value)
{
    vector<node*> result;

    // Check if the current node's tag data contains the key-value pair
    if (key != "" && root->fetchTagData(key) == value)
    {
        result.push_back(root); // Add the current node if it matches
    }

    // Check if the innerHTML contains the search value as substring
    if (key == "" && root->fetchinnerHTML().find(value) != string::npos)
    {
        result.push_back(root); // Add the current node if innerHTML contains the search value
    }

    // Recursively search in child nodes
    for (auto child : root->childs)
    {
        vector<node*> childResult = searchQuery(child, key, value);
        result.insert(result.end(), childResult.begin(), childResult.end()); // Add child results
    }

    return result;
}

int printSearchResults(vector<node*> results)
{
    if (results.empty())
    {
        return 0;
    }
    cout << "Found node with innerHTML: \n";
    for (node* n : results)
    {
        for(auto k: n->tagDataInfo())
        {
            cout<<k.first<<" : "<<k.second<<endl;
        }
        cout <<"HTML text : "<< n->fetchinnerHTML() << endl << endl;
    }
    return 1;
}

//*Remove printing htmlStartTag, but not needed to do*
void printTree(node * head, string prefix = "", bool isLast = true) {
        // Print the current node
        if (head->fetchTagData("tagname") != "htmlStartTag"){
            cout << prefix;
            // Print connectors and node data
            if (isLast) {
                cout << "└─"; // This is the last child
                prefix += "  "; // Adjust the prefix for the next level
            } else {
                cout << "├─"; // Intermediate child
                prefix += "│ "; // Adjust the prefix for the next level
            }
            // Print the node's data
            cout << head->fetchTagData("tagname") << endl;
        }
        // Recursively print all child nodes
        for (size_t i = 0; i < head->childs.size(); ++i) {
            printTree(head->childs[i], prefix, i == head->childs.size() - 1);
        }
    }
