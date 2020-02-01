//Programmer: Jessica Yee
//Email: jessicayee23424@gmail.com
//Last edited: July 30, 2018
//This program is a chat bot that answers questions pertaining to computer science.
//It looks for an exact match to the question from a file and displays the exact match answer correspondingly.
//If it is not an exact match, it displays the questions with the most matches and allows the user to
//select from the given questions or give an email.
//The user can ask as many questions until they input X for a question.
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;
struct record
{
    int number;
    string sentence;
    int matches=0;
};
void getQuestion(string& question, bool& quit);
void makeLower(string& question);
void parseLine(string question, vector<string>& keyword);
void sort(vector<string> &keyword);
void swap(string& key1, string& key2);
void readQuestionFile(ifstream& instream1, vector<record>& questions);
void readAnswerFile(ifstream& instream2, vector<record>& answers);
void match(vector<record> questions, vector<string> keyword, int topMatch[], int& exactMatch);
void askingFrequency(int topAsked[], vector<record> &questions);
void display(int topMatch[], int topAsked[], int exactMatch, vector<record>& questions, vector<record> answers, ofstream& outstream);
void storeEmail(ofstream& outstream, string email);
bool completeMatch = false;
bool quit = false;

int main()
{
    cout << "Enter 'X' to escape at any time.\n";
    cout << "Hello! Welcome to the PCC Computer Science Online Directory.\nAsk me a question.\n";


        while (true)
        {
            int topAsked[]={0, 1};
            ifstream instream1, instream2;
            ofstream outstream;
            int topMatch[]={0, 1, 2};
            vector<record> questions, answers;
            readQuestionFile(instream1, questions);
            readAnswerFile(instream2, answers);
            int exactMatch=0;
            string question, email;
            vector<string> keyword;
            getQuestion(question, quit);
            if (quit)
            {
                instream1.close();
                instream2.close();
                return 0;
            }
            parseLine(question, keyword);
            if (keyword.size()==0)
            {
                continue;
            }
            sort(keyword);
            match(questions, keyword, topMatch, exactMatch);
            askingFrequency(topAsked, questions);
            display(topMatch, topAsked, exactMatch, questions, answers, outstream);
            storeEmail(outstream, email);
        }
}
//This function obtains a question from the user and stores it in a string.
void getQuestion(string& question, bool& quit)
{
    getline(cin,question);
    if (question == "X" || question == "x")
        quit = true;
}
//This function puts the words into a keyword vector.
void parseLine(string question, vector<string>& keyword)
{
    string word = "";
    makeLower(question);
    question += "\n";
    char ch;
    for(int i=0; i<question.length(); i++)
    {
        ch = question[i];
        if ((ch != ' ') && (ch != '\n'))
        {
            word += ch;
        }
        else
        {
            if (word.size()>2)
            {
                keyword.push_back(word);
                word.clear();
            }
            else if (word=="cs")
            {
                keyword.push_back("computer");
                keyword.push_back("science");
                word.clear();
            }
            else if (word == "hi")
            {
                keyword.push_back("hi");
                word.clear();

            }
            else
                word.clear();
        }
    }

}
//This function makes the uppercases lower-case in order to match and it gets rid of punctuation.
void makeLower(string& question)
{
    char ch;
    for (int i = 0, length = question.size(); i < length; i++)
    {
        ch = question.at(i);
        if (ispunct(ch))
        {
            question.erase(i--, 1);
            length = question.size();
        }
        if (isupper(ch))
        {
            transform(question.begin(), question.end(), question.begin(), ::tolower);
            ch = tolower(ch);
        }
    }
}
//This sorts the keywords in alphabetical order.
void sort(vector <string>& keyword)
{
    if (keyword.size()<=1)
        return;
    for (int i=0; i < keyword.size()-1; i++)
    {
        for(int j=i+1; j>0; j--)
        {
            if (keyword[j-1] > keyword[j])
            {
                swap(keyword[j-1], keyword[j]);
            }
        else
           break;
        }
    }
}
//This function swaps the vector location of two values.
void swap(string& key1, string& key2)
{
    string temp;
    temp = key1;
    key1 = key2;
    key2 = temp;
}
//This function obtains the questions from the file and stores them in a vector of records (number and question)
void readQuestionFile(ifstream& instream1, vector<record>& questions)
{
    instream1.open("questionRank.txt");
    record question;
    while(instream1 >> question.number)
    {
        getline(instream1, question.sentence);
        questions.push_back(question);
    }
}
//This function obtains the answers from the file and stores them in a vector of records (number and answer)
void readAnswerFile(ifstream& instream2, vector<record>& answers)
{
    instream2.open("answerQuestionNumber.txt");
    record answer;
    while(instream2 >> answer.number)
    {
        getline(instream2, answer.sentence);
        answers.push_back(answer);
    }
}

//This function checks the amount of matches between each question and the input vector.
//If the matches are equal to both the vectors' sizes, then it changes the completeMatch statement to true.
void match(vector<record> questions, vector<string> keyword, int topMatch[], int& exactMatch)
{
    int matchCount=0;
    exactMatch=0;
    vector<string> fileQuestions;

    for (int i=0; i<questions.size(); i++)
    {
        parseLine(questions[i].sentence, fileQuestions);
        sort(fileQuestions);
        size_t keySize = keyword.size();
        for (int j=0; j<keySize; j++)
        {
            for (int k=0; k < fileQuestions.size(); k++)
            {
                if (keyword[j]==fileQuestions[k])
                {
                    matchCount++;
                }
            }
        }

        questions[i].matches = matchCount;
        completeMatch = true;
        for (int i=0; i<min(keyword.size(), fileQuestions.size()); i++)
        {
            if (keyword[i] != fileQuestions[i])
                completeMatch =false;
        }
        if (completeMatch)
        {
            exactMatch=i;
            break;
        }
        fileQuestions.clear();
        matchCount =0;

    }


    int max1=0;
    for (int i=0; i<questions.size(); i++)
    {
        if (questions[i].matches > max1)
        {
            max1 = questions[i].matches;
            topMatch[0] = i;
        }
    }
    questions[topMatch[0]].matches = -1;
    int max2=0;
    for (int i=0; i<questions.size(); i++)
    {
        if (questions[i].matches > max2)
        {
            max2 = questions[i].matches;
            topMatch[1]=i;
        }
    }
    questions[topMatch[1]].matches = -1;
    int max3=0;
    for (int i=0; i<questions.size(); i++)
    {
        if (questions[i].matches > max3)
        {
            max3 = questions[i].matches;
            topMatch[2]=i;
        }
    }

}
//This function finds the question with the maximum number of times asked (the number in the record)
void askingFrequency(int topAsked[], vector<record>& questions)
{
    int max1;
    for (int i=0; i<questions.size(); i++)
    {
        if(questions[i].number>max1)
        {
            max1 = questions[i].number;
            topAsked[0]=i;
        }
    }
    int max2;
    for (int i=0; i<questions.size(); i++)
    {
        if(questions[i].number>max2)
        {
            max2 = questions[i].number;
            topAsked[1]=i;
        }
    }
}
//This function either displays the answer to the exact matching question (if the completeMatch is true)
//or this function displays the 3 questions with the most matches and 2 questions most with most asking frequency.
void display(int topMatch[], int topAsked[], int exactMatch, vector<record>& questions, vector<record> answers, ofstream& outstream)
{
    int questionSelection;
    char email_option;
    string email;

    if (completeMatch)
    {
        cout << answers[exactMatch].sentence << endl;
        questions[exactMatch].number += 1;
    }
    else
    {
        cout << "Sorry, I do not understand your question.\n"
             << "Here are the top five questions I think you may be asking\n";
        for (int i=0; i<3; i++)
        {
            cout << i+1 << " " << questions[topMatch[i]].sentence << endl;
        }
        for (int i=0; i<2; i++)
        {
            cout << i+4 << " " << questions[topAsked[i]].sentence << endl;
        }
    cout << " Please input a selection. Enter 0 if you question is not here: ";
    cin >> questionSelection;
    while (questionSelection<0 && questionSelection>5)
    {
        cout << " Please input a valid selection."
             << " Enter 0 if your question is not listed.\n";
        cin >> questionSelection;
    }
    switch (questionSelection)
    {
    case 1:
    {
        cout << answers[topMatch[0]].sentence << endl;
        questions[topMatch[0]].number += 1;
        break;
    }
    case 2:
    {
        cout << answers[topMatch[1]].sentence << endl;
        questions[topMatch[1]].number += 1;
        break;
    }
    case 3:
    {
        cout << answers[topMatch[2]].sentence << endl;
        questions[topMatch[2]].number += 1;
        break;
    }
    case 4:
    {
        cout << answers[topAsked[0]].sentence << endl;
        questions[topAsked[0]].number += 1;
        break;
    }
    case 5:
    {
        cout << answers[topAsked[1]].sentence << endl;
        questions[topAsked[1]].number += 1;
        break;
    }
    case 0:
    {
        cout << "I am sorry, I cannot help you at this time,\n"
             << "but if you are willing to leave your name and email address,\n"
             << "I will have someone email you back with the correct answer\n"
             << "Would you like to leave your email? (Y/N): ";
        cin >> email_option;
        if (email_option == 'y' || email_option=='Y')
        {
            cout << "Please input your email: ";
            cin >> email;
            cout << "Thank you! Our team will contact you as soon as possible.";
            storeEmail(outstream, email);

        }
        else
            cout << "Sorry!\n";

        break;
    }

    }
    }
}
//This function stores the email from the user.
void storeEmail(ofstream& outstream, string email)
{
    outstream.open("questionsNotAnswered.txt");
    outstream << email << endl;
    outstream.close();
}
