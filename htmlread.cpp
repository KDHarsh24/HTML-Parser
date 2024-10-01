#include <bits/stdc++.h>
#include <curl/curl.h>
#include "parser.cpp"
#include <ncurses.h>
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

//DialogueBox Maker
int selectDialogueBox(vector<string> options)
{
    // Initialize ncurses
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);

    // Print the selection prompt to the console
    refresh();

    // Create a window for the options
    WINDOW *win = newwin(options.size() + 2, 30, 2, 2);
    box(win, 0, 0);

    // Print the options
    for (int i = 0; i < options.size(); i++) {
        mvwprintw(win, i + 1, 2, "%s", options[i].c_str());
    }

    // Create a cursor for selection
    int cursor = 0;
    wrefresh(win);
    int c;
    do {
        // Clear the previous selection
        for (int i = 0; i < options.size(); i++) {
            mvwprintw(win, i + 1, 1, " ");
            mvwprintw(win, i + 1, 2, "%s", options[i].c_str());
        }

        // Highlight the selected option
        mvwprintw(win, cursor + 1, 1, ">");
        mvwprintw(win, cursor + 1, 2, "%s", options[cursor].c_str());

        // Refresh the window
        wrefresh(win);

        // Handle arrow key presses
        c = getch();
        switch (c) {
            case KEY_UP:
                if (cursor > 0) {
                    cursor--;
                }
                break;
            case KEY_DOWN:
                if (cursor < options.size() - 1) {
                    cursor++;
                }
                break;
        }
    } while (c != '\n');

    // Get the selected option
    string selected_option = options[cursor];

    // Clean up
    delwin(win);
    endwin();

    // Print the selected option
    return cursor;
}

int main() {
    char website [1000];
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
	char s;
	printw("Select an option:\n");
	while(true)
	{
		vector <string> options = {"Search Data in the website", "Print HTML tree", "Check Errors", "Exit"};
		int option = selectDialogueBox(options);
		if (option == 0)
		{

		}
		else if (option == 1)
		{
			printTree(htmlTag);
			cin>>s;
            system("clear");
		}
		else if (option == 2)
		{

		}
		else
		{
			refresh();
            system("clear");
			cout<<"*******************WebTree************************\n"<<endl;
			return 0;
		}
	}
    return 0;
}