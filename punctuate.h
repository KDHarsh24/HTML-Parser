#include<bits/stdc++.h>
using namespace std;

string replacewith(string input, string replace_word, string replace_by)
{
    /***** A substring replace from a string to another used extractInfoFromTag ******/
    size_t pos = input.find(replace_word); 
    while (pos != string::npos) { 
        input.replace(pos, replace_word.size(), replace_by); 
        pos = input.find(replace_word, 
        pos + replace_by.size()); 
    }
    return input;
}

string removeComments(const string& html) {
    /**** Remove comment from a string ****/
    string result;
    bool inComment = false;
    for (size_t i = 0; i < html.size(); ++i) {
        if (html[i] == '<' && html[i + 1] == '!' && html[i + 2] == '-' && html[i + 3] == '-' && !inComment) inComment = true;
        else if (html[i] == '-' && html[i + 1] == '-' && html[i + 2] == '>' && inComment) {
            inComment = false;
            i += 2;
        }
        else if (!inComment) result += html[i]; 
    }
    return result;
}

string addspaceonEqual(string sentence)
{
    /*** Seperating for tags with =, > !-- intendation ***/
    sentence = replacewith(sentence, "=", " = ");
    sentence = replacewith(sentence, ">", " ");
    sentence = replacewith(sentence, "!--", "!-- ");
    return sentence;
};

string checkTwoQuotes(vector<string> taglist, int n)
{
    /*** All Things inside Quotes as one token ***/
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

void leftTrim(string &s) 
{
    /*** Space removal from leftside ***/
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}
