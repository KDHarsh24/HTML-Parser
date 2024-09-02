#include <bits/stdc++.h>
using namespace std;
#define attribute map<string, string> 

class node
{
    private:
        attribute tagData;
        string innerHTML = "";
        node * parent;
    public:
        vector <node *> childs;
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
};

void printTree(node * head, int n)
{
    for (int i = 0; i < n; i++)
        cout<<"------>";
    cout<<head->fetchTagData("tagname")<<"\n";
    n++;
    for (auto node: head->childs)
        printTree(node, n);
}

vector<string> identifyTags(vector<string> htmlCode)
{
    string tagData = "";
    vector<string> blockHtml;
    bool startTag = false;
    for(auto line : htmlCode)
    {
        for(int i = 0; i < line.length(); i++)
        {
            if(startTag)
            {
                tagData += line[i];
                if (line[i] == '>')
                {
                    startTag = false;
                    blockHtml.push_back(tagData);
                    tagData = "";
                }
            }
            else
            {
                if (line[i] == '<')
                {
                    startTag = true;
                    blockHtml.push_back(tagData);
                    tagData = "<";
                }
                else
                {
                    tagData += line[i];
                }
            }
        }
    }

    return blockHtml;
}

string replacewith(string input, string replace_word, string replace_by)
{
    size_t pos = input.find(replace_word); 
    while (pos != string::npos) { 
        input.replace(pos, replace_word.size(), replace_by); 
        pos = input.find(replace_word, 
        pos + replace_by.size()); 
    }
    return input;
}

string addspaceonEqual(string sentence)
{
    sentence = replacewith(sentence, "=", " = ");
    sentence = replacewith(sentence, ">", " ");
    sentence = replacewith(sentence, "!--", "!-- ");
    return sentence;
};

string checkTwoQuotes(vector<string> taglist, int n)
{
    string valueAnswer = "";
    int no = 0;
    for (int i = n; i < taglist.size(); i++)
    {   
        string lines = taglist[i];
        if (no == 2) break;
        for (int i = 0; i < lines.length(); i++)
        {
            if (lines[i] == '"') no++;
            else
                valueAnswer += lines[i];
            if (no == 2)
                break;
        }
    }
    return valueAnswer;
}

void ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

vector<string> getIntoList(string line)
{
    vector<string> list;
    string p = "";
    bool qu = false;
    for (int i = 0; i < line.length(); i++)
    {
        if(!qu)
        {
            
            if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\v' || line[i] == '\0')
            {
                if(p.length() != 0)
                    list.push_back(p);
                p = "";
            }
            else  
                p += line[i];
            if(line[i] == '"' || line[i] == '\"')
                qu = true;
        }
        else{
            p+=line[i];
            if(line[i] == '"' || line[i] == '\"')
            {
                qu = false;
                list.push_back(p);
                p = "";
            }
        }
    }
    return list;
}

attribute extractInfofromTag(string line)
{
    attribute tagData;
    int i;
    if (line.length() == 0 || line[0] != '<')
        return tagData;
    
    line = line.substr(1);
    ltrim(line);
    line = addspaceonEqual(line);
    vector<string> allSpaceWords = getIntoList(line);
    tagData["tagname"] = allSpaceWords[0];
    allSpaceWords.erase(allSpaceWords.begin());
    for(i = 0; i < allSpaceWords.size(); i++)
    {
        if (allSpaceWords[i] == "=")
        {
            tagData[allSpaceWords[i-1]] = checkTwoQuotes(allSpaceWords, i+1);
        }
    }
    if (allSpaceWords[i-1] == "/")
        tagData["selfClosing"] = "True";
    return tagData;
}

bool searchNode(vector<string> list, string n)
{
    for(auto nodes: list)
        if(nodes == n)
            return true;
    return false;
}

vector<node *> tagDefiners(vector<string> allBlock)
{
  vector<node *> onGoingBerth;
  attribute headTagInfo = extractInfofromTag("<htmlStartTag>");
  node * head = new node(headTagInfo, "", NULL);
  onGoingBerth.push_back(head);
  bool scriptrun = false;
  vector<string> selfClosingTags = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr", "!DOCTYPE", "circle", "!--"};
  for (auto items: allBlock)
  {
    attribute tagElement = extractInfofromTag(items);
    if (tagElement.size() == 0)
    {
      onGoingBerth.back()->putinnerHTML(items);
    }
    else
		{
            cout<<tagElement["tagname"]<<endl;
			if(tagElement["tagname"][0] == '/')
			{
				if (tagElement["tagname"] == "/"+onGoingBerth.back()->fetchTagData("tagname"))
				{
                    head = onGoingBerth.back();
                    onGoingBerth.pop_back();
                    head->putParent(onGoingBerth.back());
                    onGoingBerth.back()->insertChild(head);
                    if (tagElement["tagname"] == "/script")
                        scriptrun = false;
				}
                else if (!scriptrun){
                    cout<<tagElement["tagname"]<<endl;
                    tagElement["Error"] = "True";
                    while(onGoingBerth.back()->fetchTagData("tagname") != "htmlStartTag " && tagElement["tagname"] != "/"+onGoingBerth.back()->fetchTagData("tagname"))
                        {
                            head = onGoingBerth.back();
                            onGoingBerth.pop_back();
                            head->putParent(onGoingBerth.back());
                            onGoingBerth.back()->insertChild(head);
                        }
                        if (onGoingBerth.back()->fetchTagData("tagname") != "htmlStartTag")
                        {
                            head = onGoingBerth.back();
                            onGoingBerth.pop_back();
                            head->putParent(onGoingBerth.back());
                            onGoingBerth.back()->insertChild(head);
                        }
                }
			}
        else{
            node * nd = new node(tagElement, "", onGoingBerth.back());
            if(!searchNode(selfClosingTags, tagElement["tagname"]))
            {
                if(tagElement["tagname"] == "script")
                {
                    scriptrun = true;
                }
                onGoingBerth.push_back(nd);
            }
            else{
                if(tagElement["tagname"] != "!--")
                {
                    nd->putParent(onGoingBerth.back());
                    onGoingBerth.back()->insertChild(nd);
                }
            }
        }
	}
  }
  return onGoingBerth;
}

int main()
{
    // ifstream f("a.html");
    // string myText;
    // vector<string> s;
    // while (getline (f, myText))
    //     s.push_back(myText);
    // f.close();
    // vector<string> allBlocks = identifyTags(s);
    // vector <node *> htmlNode = tagDefiners(allBlocks);
    // printTree(htmlNode[0], 0);
    attribute a = extractInfofromTag("<svg color=\"{red: yellow}\"/>");
    for (auto i: a)
        cout<<i.first<<" : "<<i.second<<endl;
    return 0;
}
