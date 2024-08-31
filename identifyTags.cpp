#include<bits/stdc++.h>
using namespace std;


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
					if (!isspace(line[i]))
                    tagData += line[i];
				}
            }
			
        }
		
    }

    return blockHtml;
}

int main()
{
ifstream f("htmlFile.html");
  string myText;
  vector<string> s, h;
  while (getline (f, myText))
    s.push_back(myText);
  f.close();
  vector<string> allBlock = identifyTags(s);
  for(auto a: allBlock)
	cout<<a<<endl;
  return 0;
}