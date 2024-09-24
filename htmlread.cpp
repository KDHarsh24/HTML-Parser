#include <bits/stdc++.h>
#include <curl/curl.h>
#include "parser.cpp"
using namespace std;

//Inbuilt writefunction for web extraction on file writing
size_t writefunc(void *ptr, size_t size, size_t nmemb, string *s) 
{
  s->append(static_cast<char *>(ptr), size*nmemb);
  return size*nmemb;
}

//Read from internet and creating file named fileName and writing the webpage in it
void readfromWeb(const char * url)
{
    CURL *curl = curl_easy_init();
    if (curl)
    {
        string thingsToWrite;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &thingsToWrite);
        CURLcode res = curl_easy_perform(curl);
        ofstream MyFile("rawDocument.html");
        MyFile << thingsToWrite;
        curl_easy_cleanup(curl);
		MyFile.close();

		ifstream file("rawDocument.html");
		ofstream outputFile("parseDocument.html");
		string htmlCode((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        string result = removeComments(htmlCode);
		outputFile << result;
		file.close();
		outputFile.close();
	}
}

vector<string> removeExtra(vector<string> s)
{
  vector<string> h;
  for (auto line: s)
  {
	string k = "";
	for (int i = 0; i < line.length(); i++)
	{
		if (!isspace(line[i]))
		{
			k+=line[i];
		}
	}
	h.push_back(k);
  }
  return h;
}

node * htmlParser(vector<string> allBlock)
{
	vector<node *> onGoingBerth;
	attribute headTagInfo = extractInfofromTag("<htmlStartTag>");
	node * headTag = new node(headTagInfo, "", NULL);
	onGoingBerth.push_back(headTag);
	bool scriptrun = false;
	int i = 0;
	vector<string> selfClosingTags = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr", "!DOCTYPE", "circle", "!--"};
	for (auto items: allBlock)
	{
		attribute tagElement = extractInfofromTag(items);
		if (tagElement.size() == 0)	onGoingBerth.back()->putinnerHTML(items);
		else
			{
				// cout<<tagElement["tagname"]<<endl;
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
						tagElement["Error"] = "True";
						cout<<tagElement["tagname"]<<endl;
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
				node * nd = new node(tagElement, "", onGoingBerth.back());
				if(!searchNode(selfClosingTags, tagElement["tagname"]))
				{
					if(tagElement["tagname"] == "script")
						scriptrun = true;
					onGoingBerth.push_back(nd);
				}
				else{
					if(tagElement["tagname"].substr(0, 3) != "!--")
					{
						nd->putParent(onGoingBerth.back());
						onGoingBerth.back()->insertChild(nd);
					}
				}
			}
		}
	}
}

int main(void)
{
  	//readfromWeb("http://iiitmanipur.ac.in");
	ifstream f("parseDocument.html");
	string myText;
	vector<string> s;
	while (getline (f, myText))
		s.push_back(myText);
	f.close();

	vector<string> allBlock = identifyTags(s);
	cout<<"Enter Your Website Name"<<endl;
	return 0;
}