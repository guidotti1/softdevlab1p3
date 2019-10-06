#include "dataLookup.h"
#include "dataEntry.h"
#include "extraFunctions.h"
#include <sstream>

dataLookup::dataLookup(string filename)
{
    //opening input
    ifstream input;
    input.open(filename.c_str());
    if (input.fail())
        {
        //cout << "Failed to open file, program will be quit"<<endl;
        exit(1);
        }
    vector<string> row;
    string line, word;
    while (!input.eof())
        {
        row.clear();
        //take one line of input at a time
        getline(input, line);
        //A number of characters have names in the format : First Last, Jr.
        //because we are dealing with a csv file, it is natural to split up sections based on commas
        //however in such a name that means that First Last will be split from Jr.
        //as such I have these two calls to the findReplaceString function to prevent this from occurring
        findReplaceString(line, ", ", " ");
        findReplaceString(line, ",_", " ");
        //these are a few common character universes that I remove from the string.
        //I decided to not remove all of the information between ( ) because for many characters this is needed information
        //i.e. the entry for Spider-man is : Spider-Man (Peter Parker).
        //for such a character entry it is definitely not a good idea to delete the stuff between the parentheses, as such I only delete it for the very common universe names.
        findReplaceString(line, "Earth-616", "");
        findReplaceString(line, "Counter-Earth", "");
        findReplaceString(line, "Asgard", "");
        stringstream  s(line);
        //if the final column is empty (i.e. final character is ','), year is empty so emptyYear becomes true
        char finalCharacter = line[line.length()-1];
        bool emptyYear = false;
        if (finalCharacter == ',')
            {
            emptyYear = true;
            }
        //separates string based on ',' delimiter
        while(getline(s,word, ','))
            {
            row.push_back(word);
            }
        //makes a new Data entry using the row and then adds it as a new entry to the map based on pageID.
        dataEntry newData(row, emptyYear);
        dataMap[newData.pageID] = newData;
        }
}

void dataLookup::createYearAndNameMaps()
{
    map<string, dataEntry>::iterator it;
    //iterator goes through whole dataMap
    for (it = dataMap.begin(); it != dataMap.end(); it ++ )
    {
        //id -> first is the current pageID (map index)
        //id -> second is the dataEntry (what map points to )
        string id = it -> first;
        dataEntry currentData = it -> second;
        vector<string> name = currentData.name;
        string year = currentData.year;
        year.resize(4);
        //if year (and name) maps haven't been added, basically we are making the first entry in each. This only applies to the first character that we look at
        //for the name map, we add a new entry for each individual word in a character's name (First, Last, Nickname, etc.)
        //i.e. for "Spider-Man Peter Parker", there is one for Spider-Man, one for Peter and one for Parker
        if (yearMap.empty())
            {
            yearMap.insert(pair<string, vector<string> > (year, vector<string>()));
            yearMap[year].push_back(id);
            //one word in name at a time added to map
            for (int e = 0; e<name.size(); e++)
                {
                string current = name[e];
                toUpperCase(current);
                nameMap.insert(pair<string, vector<string> > (current, vector<string>()));
                nameMap[current].push_back(id);
                }
            }
        else
            {
            //if the year (and name) maps aren't empty then for each dataMap entry we need to check if that year or name already has been allocated a vector<string> (i.e. is already in the map)
            //to do this we use the map.count() function. If it is zero then we know that there is no map position and as such need to allocate a vector<string>
            //so if the year has already been allocated a vector<string> , then we just add the next id to that vector that already exists at that position
            //the same goes for name
            //we convert names to upper case so that when a user enters in a name it doesn't matter what case they choose. We can just convert what they enter to uppercase and then compare it against the nameMap indices
            int yearCheck = yearMap.count(year);
            if (yearCheck == 0)
                {
                yearMap.insert(pair<string, vector<string> > (year, vector<string>()));
                yearMap[year].push_back(id);
                }
            else
                {
                yearMap[year].push_back(id);
                }
            for (int e = 0; e <name.size(); e++)
                {
                int nameCheck = nameMap.count(name[e]);
                if (nameCheck == 0)
                    {
                    string current = name[e];
                    toUpperCase(current);
                    nameMap.insert(pair<string, vector<string> > (current, vector<string>()));
                    nameMap[current].push_back(id);
                    }
                else
                    {
                    string current = name[e];
                    toUpperCase(current);
                    nameMap[current].push_back(id);
                    }
                }
            }
        }
}

vector<dataEntry> dataLookup::userSearch(char type, string data)
{
    //user can look up by one name of the word, year introduced, or multiple words in the name (in any order)
    char userChoice=type;
    vector<string> userId;
    map<string, vector<string> >::iterator findIt;
    vector<dataEntry> characters;
    if (userChoice == '1')
        {
        //takes name, converts to upper case and tries to find it in the map
        //if its not there just quit the search
        string userName=data;
        toUpperCase(userName);
        findIt = nameMap.find(userName);
        if (nameMap.count(userName) == 0)
            {
            return characters;
            }
        //if the name is in the map, then we take the vector<string> of page ids and the name of each character is output
        //then the user can enter the number of one of the characters and see the profile for that character
        else
            {
            userId = findIt -> second;
            for (int t = 0; t < userId.size(); t++)
                {
                characters.push_back(dataMap[userId[t]]);
                }
            return characters;
            }
        }
    //works the same as name (one word) search above
    else if (userChoice == '2')
        {
        string userYear=data;
        findIt = yearMap.find(userYear);
        if (yearMap.count(userYear) == 0)
            {
            return characters;
            }
        else
            {
            userId = findIt -> second;
            for (int t = 0; t < userId.size(); t++)
                {
                characters.push_back(dataMap[userId[t]]);
                }
            return characters;
            }
        }
    //multi word search
    else if (userChoice == '3')
        {
        string userName=data;
        userName += " ";
        removeSpecialCharacters(userName);
        toUpperCase(userName);
        vector<string> fullUserName;
        separateWords(userName, fullUserName);
        //vector of iterators. Basically each iterator contained in the vector will be used to look at the nameMap entry for each distinct word of the name that the user has input
        vector< map<string, vector<string> >::iterator> iterators;
        for (int u = 0; u < fullUserName.size(); u++)
            {
                //if one of these words is not in the map then we just quit
                if (nameMap.count(fullUserName[u]) == 0)
                    {
                    return characters;
                    }
            //otherwise we find that word in the nameMap. With this we can find the vector of PIDS for each of the distinct words in the name
            iterators.push_back(nameMap.find(fullUserName[u]));
            }
        //2d vector of userIds
        //basically in each position we store a vector of ids corresponding to a word in the name that the user entered
        vector<vector<string> > userIds;
        for (int u = 0; u < iterators.size(); u++)
            {
            userIds.push_back(iterators[u] -> second);

            }
        //these nested for loops are used to find the pageIDs that all of the individual userIds vectors have in common
        //if all of the userIds vectors have these pageIDs in common than for each word in the name that the user entered, we must be pointing to the same dataEntry in the overall map
        //hence at the end tempIds contains only the pageIds that have each word of the name the user entered present.
        vector <string> tempIds = userIds[0];
        vector <string> finalIds;
        for (int u = 1; u <userIds.size(); u++)
            {
            for (int q = 0; q < userIds[u].size(); q++)
                {
                for  (int r = 0; r < tempIds.size(); r++)
                    {
                    if (tempIds[r] == userIds[u][q])
                        {
                        finalIds.push_back(tempIds[r]);
                        }
                    }
                }
                tempIds = finalIds;
                finalIds.clear();
            }
        //if no characters have that FULL NAME then we quit as well.
        if (tempIds.size() == 0)
            {
            return characters;
            }
        //same as with the above two searches. User can pick one character with this name (words can appear in any order)
        for (int t = 0; t < tempIds.size(); t++)
            {
            characters.push_back(dataMap[tempIds[t]]);
            }
        return characters;
        }
}

string dataLookup::readMatches (vector<dataEntry> characters)
{
    if (characters.size() == 0)
    {
        exit(1);
    }
    string outMessage="";
    for (int i = 0; i < characters.size(); i++)
        {
        string iString;
        stringstream ss;
        ss << i+1;
        iString = ss.str();
        outMessage += iString;
        outMessage += ",";
        //cout << i+1 << ",";
            for (int q = 0; q < characters[i].name.size(); q++)
            {
            outMessage += characters[i].name[q];
            outMessage += " ";
            //cout << characters[i].name[q] << " ";
            }
        outMessage += ",";
        //cout << ",";
        }
    return outMessage;
}


