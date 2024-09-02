#include <bits/stdc++.h>
#include <curl/curl.h>
#include "parser.cpp"
using namespace std;

size_t writefunc(void *ptr, size_t size, size_t nmemb, string *s) 
{
  s->append(static_cast<char *>(ptr), size*nmemb);
  return size*nmemb;
}


void readfromWeb(const char * url)
{
    CURL *curl = curl_easy_init();
    if (curl)
    {
        string s;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        CURLcode res = curl_easy_perform(curl);

        ofstream MyFile("a.html");

        // Write to the file
        MyFile << s;

        // Close the file
        MyFile.close();
        curl_easy_cleanup(curl);
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

int main(void)
{
  readfromWeb("https://www.apple.com");
  ifstream f("a.html");
  string myText;
  vector<string> s, h;
  while (getline (f, myText))
    s.push_back(myText);
  f.close();
  
  vector<string> allBlock = identifyTags(s);
  
  vector<node *> onGoingBerth;
  attribute headTagInfo = extractInfofromTag("<htmlStartTag>");
  node headTag(headTagInfo, "", NULL);
  node * head = &headTag;
  onGoingBerth.push_back(head);
  bool scriptrun = false;
  vector<string> selfClosingTags = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr", "!DOCTYPE", "circle", "!--"};
  for (auto items: allBlock)
  {
    attribute tagElement = extractInfofromTag(items);
    //cout<<items<<endl;
    if (tagElement.size() == 0)
    {
      onGoingBerth.back()->putinnerHTML(items);
    }
    else
		{
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
  printTree(onGoingBerth.back(), 0);
  return 0;
}