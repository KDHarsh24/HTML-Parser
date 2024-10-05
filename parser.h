#include <bits/stdc++.h>
#include <regex>
using namespace std;


vector<string> identifyTags(vector<string> htmlCode)
{
    /*** Distinguish Tags from InnerHTMLs  ***/
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
                else if(line[i] == '<')
                {
                    startTag = true;
                    tagData.pop_back();
                    blockHtml.push_back(tagData);
                    tagData = "<";
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
 
vector<string> getIntoList(string line)
{
    /*** Inside tag Divide everything into different elements/tokens ***/
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
    if (p.length()!=0)
        list.push_back(p);
    return list;
}

attribute extractInfofromTag(string line)
{
    /*** Collects all info inside a tag ***/
    attribute tagData;
    int i;
    if (line.length() == 0 || line[0] != '<')
        return tagData;
    
    line = line.substr(1);
    leftTrim(line);
    line = addspaceonEqual(line);
    
    vector<string> allSpaceWords = getIntoList(line);
    
    tagData["tagname"] = allSpaceWords[0];
    //cout<<"*******"<<line<<endl;
    
    allSpaceWords.erase(allSpaceWords.begin());
    
    for(i = 0; i < allSpaceWords.size(); i++)
    {
        attribute a;
        if (allSpaceWords[i] == "=")
        {
            if (allSpaceWords[i+1][0] != '"')
            {
                return a;
            }
            tagData[allSpaceWords[i-1]] = checkTwoQuotes(allSpaceWords, i+1);
        }
    }
    if (allSpaceWords[i] == "/")
        tagData["selfClosing"] = "True";
    return tagData;
}

bool searchNode(vector<string> list, string n)
{
    /*** function that search if something in a string list present/absent ***/
    for(auto nodes: list)
        if(nodes == n)
            return true;
    return false;
}

//Main Parsing function and Parsetree genrator
//******************* Error Solving issue still exist ***********************/
node * htmlParser(vector<string> allBlock)
{
	vector<node *> onGoingBerth;
	attribute headTagInfo = extractInfofromTag("<htmlStartTag>");
	node * headTag = new node(headTagInfo, "", NULL);
	onGoingBerth.push_back(headTag);
	bool scriptrun = false;
	int i = 0;
	vector<string> selfClosingTags = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr", "!DOCTYPE", "circle", "!--", "!doctype"};
	for (auto items: allBlock)
	{
		//cout<<items<<endl;
		attribute tagElement = extractInfofromTag(items);
		if (tagElement.size() == 0)	onGoingBerth.back()->putinnerHTML(items);
		else
			{
				//cout<<tagElement["tagname"]<<endl;
				if(tagElement["tagname"][0] == '/')
				{
					headTag = onGoingBerth.back();
					if (tagElement["tagname"] == "/"+(headTag->fetchTagData("tagname")))
					{
						onGoingBerth.pop_back();
						headTag->putParent(onGoingBerth.back());
						onGoingBerth.back()->insertChild(headTag);
						if (tagElement["tagname"] == "/script")
							scriptrun = false;
					}
					else if (!scriptrun){
                        onGoingBerth.back()->putTagdata("Error", "True");
						//cout<<tagElement["tagname"]<<endl;
						for (auto a: onGoingBerth)
						{
							if (tagElement["tagname"] == "/"+a->fetchTagData("tagname"))
							{
								while(onGoingBerth.back()->fetchTagData("tagname") != "htmlStartTag" && tagElement["tagname"] != "/"+onGoingBerth.back()->fetchTagData("tagname"))
								{
									headTag = onGoingBerth.back();
									//cout<<headTag->fetchTagData("tagname")<<endl;
									onGoingBerth.pop_back();
									headTag->putParent(onGoingBerth.back());
									onGoingBerth.back()->insertChild(headTag);
								}
								break;
							}

						}
						//cout<<onGoingBerth.front()->fetchTagData("tagname")<<endl;
						if (onGoingBerth.back()->fetchTagData("tagname") != "htmlStartTag")
						{
							headTag = onGoingBerth.back();
							onGoingBerth.pop_back();
							headTag->putParent(onGoingBerth.back());
							onGoingBerth.back()->insertChild(headTag);
						}
					}
			}
			else{
				if (!scriptrun)
				{
					node * nd = new node(tagElement, "", onGoingBerth.back());
					if(!searchNode(selfClosingTags, tagElement["tagname"]))
					{
						if(tagElement["tagname"] == "script")
							scriptrun = true;
						onGoingBerth.push_back(nd);
					}
					else{		
						nd->putParent(onGoingBerth.back());
						onGoingBerth.back()->insertChild(nd);
					}
				}
				else onGoingBerth.back()->putinnerHTML(items);
			}
		}
	}
	return onGoingBerth.back();
}
