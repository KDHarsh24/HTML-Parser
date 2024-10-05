#include <bits/stdc++.h>
#include <curl/curl.h>
#define attribute map<string, string> 
#include "tree.h"
#include "punctuate.h"
#include "parser.h"
#include "gui.h"
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
	//********curl library need to be learned**************
    CURL *curl = curl_easy_init();
    if (curl)
    {
        string thingsToWrite;//string consisting of all direct internet html file
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &thingsToWrite);
        CURLcode res = curl_easy_perform(curl);
        ofstream internetFile("rawDocument.html");//direct Internet code pasted in rawDocument.html
        internetFile << thingsToWrite;
        curl_easy_cleanup(curl);
		internetFile.close();

		ifstream internetReadingFile("rawDocument.html");
		ofstream outputFile("parseDocument.html");//file with no comment
		string htmlCode((istreambuf_iterator<char>(internetReadingFile)), istreambuf_iterator<char>());
        string result = removeComments(htmlCode);
		outputFile << result;
		internetReadingFile.close();
		outputFile.close();
	}
}

int main() {
    char website [1000];
    clear();
    int termWidth = getTerminalWidth(); // Get the current terminal width
    cout << string(termWidth, '*') << endl;
    printFullWidth("WebTree");
    cout << string(termWidth, '*') << endl<<endl;
	cout<<"Enter Your Webpage url : ";
	cin>>website;
	readfromWeb(website);
	ifstream readFile("parseDocument.html");
	string lineText;
	vector<string> htmlString;
	while (getline (readFile, lineText))
		htmlString.push_back(lineText);
	readFile.close();
	vector<string> allBlocks = identifyTags(htmlString);
	node * htmlTag = htmlParser(allBlocks);
    getch();
	char s;
	
	while(true)
	{
		vector <string> options = {"Search Data in the website", "Print HTML tree", "Check Errors", "Exit"};
		int option = selectDialogueBox(options);
		if (option == 0)
		{
            while(true)
            {
                char g;
                vector <string> options2 = {"Search by Tag Details", "Search for content", "Exit"};
                int option2 = selectDialogueBox(options2);
                if (option2 == 0)
                {
                    string searchKey;
                    string searchValue;
                    cout<<"Enter Attribute Key : ";
                    cin>>searchKey;
                    cout<<"Enter Attribute value: ";
                    cin>>searchValue;
                    cout << "Searching for nodes with tag attribute [" << searchKey << "] = " << searchValue << "..." << endl;
                    vector<node*> resultById = searchQuery(htmlTag, searchKey, searchValue);
                    int notfound = printSearchResults(resultById);
                    if (!notfound)
                    {
                        cout<<"No Tag Found"<<endl;
                    }
                    system("clear");
                }
                else if (option2 == 1)
                {
                    string innerHTMLSearch;
                    cout<<"Enter Content: ";
                    cin>>innerHTMLSearch;
                    cout << "\nSearching for nodes with innerHTML containing '" << innerHTMLSearch << "'..." << endl;
                    vector<node*> resultByHTML = searchQuery(htmlTag, "", innerHTMLSearch);
                    int notfound = printSearchResults(resultByHTML);
                    if (!notfound)
                    {
                        cout<<"No Content Found"<<endl;
                    }
                }
                else
                {
                    refresh();
                    break;
                }
                getch();
                system("clear");
            }
		}
		else if (option == 1)
		{
			printTree(htmlTag);
			getch();
            system("clear");
		}
		else if (option == 2)
		{
            cout << "\nPrinting Error Tags '"<<endl;
            vector<node*> resultByHTML = searchQuery(htmlTag, "Error", "True");
            int notFound = printSearchResults(resultByHTML);
            if (!notFound)
                cout<<"No Error Congrats!!"<<endl;
            getch();
            system("clear");
		}
		else
		{
			cout << string(termWidth, '*') << endl;
            printFullWidth("WebTree");
            cout << string(termWidth, '*') << endl<<endl;
			break;
		}
        clear();
	}
    return 0;
}
