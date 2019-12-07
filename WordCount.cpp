// WordCount.cpp

#include "WordCount.h"
#include <algorithm>

using namespace std;

// Default constructor
WordCount::WordCount() {}

size_t WordCount::hash(std::string word) const {
	size_t accumulator = 0;
	for (size_t i = 0; i < word.size(); i++) {
		accumulator += word.at(i);
	}
	return accumulator % CAPACITY;
}

int WordCount::getTotalWords() const {
	size_t totalWords = 0;
	for (size_t i = 0; i < CAPACITY; i++) {
		if (table[i].size() > 0) {
			for (size_t j = 0; j < table[i].size(); j++) {
				totalWords += table[i][j].second;
			}
		}
	}
	return totalWords;
}

int WordCount::getNumUniqueWords() const {
	size_t totalUniqueWords = 0;
	for (size_t i = 0; i < CAPACITY; i++) {
		totalUniqueWords += table[i].size();
	}
	return totalUniqueWords;
}

int WordCount::getWordCount(std::string word) const {
	string strippedWord = stripWord(word);

	string hashWord = "";
	for (size_t i = 0; i < strippedWord.size(); i++) {
		hashWord += toupper(strippedWord.at(i));
	}

	size_t hashIndex = hash(hashWord);

	vector<pair<string, size_t> > v = table[hashIndex];

	for (size_t i = 0; i < table[hashIndex].size(); i++) {
		if (table[hashIndex][i].first == hashWord) {
			return table[hashIndex][i].second;
		}
	}
	return 0;
}
	

int WordCount::incrWordCount(std::string word) {
	string strippedWord = stripWord(word);

	string hashWord = "";
	for (size_t i = 0; i < strippedWord.size(); i++) {
		hashWord += toupper(strippedWord.at(i));
	}

	if (hashWord == "") {
		return 0;
	}

	size_t hashIndex = hash(hashWord);

	for (size_t i = 0; i < table[hashIndex].size(); i++) {
		if (table[hashIndex][i].first == hashWord) {
			table[hashIndex][i] =
				pair<string, size_t>(hashWord, table[hashIndex][i].second + 1);
			return table[hashIndex][i].second;
		}
	}
	table[hashIndex].push_back(pair<string, size_t>(hashWord, 1));
	return 1;
}


bool WordCount::isWordChar(char c) {
	return
    (c >=65 && c <= 90) ||	// upper case
    (c >=97 && c <= 122);	// lower case
}


std::string WordCount::stripWord(std::string word) {

	if (word == "") {
		return "";
	}

	string firstString = "";
	size_t frontIndex = 0;
	// make sure the first char is alpha
	for (size_t i = 0; i < word.size(); i++) {
		frontIndex = i;
		if (isWordChar(word.at(i))) {
			firstString = word.at(i);
			break;
		}
	}

	// work backwards to make sure last char is alpha
	//char lastChar = ' ';
	size_t endIndex = word.size() - 1;
	string endString = "";
	for (size_t i = word.size() - 1; i > frontIndex; i--) {
		endIndex = i;
		if (isWordChar(word.at(i))) {
			endString = word.at(i);
			break;
		}
	}

	// construct middle string
	string strippedWord = "";
	for (size_t i = frontIndex + 1; i < endIndex; i++) {
		if (isWordChar(word.at(i)) || word.at(i) == 45 || word.at(i) == 39) {
			strippedWord += word.at(i);
		}
	}

	// put the first, end, and strippedWord together
	if (firstString != "") {
		strippedWord.insert(0, firstString);
	}
	if (endString != "") {
		strippedWord.insert(strippedWord.size(), endString);
	}

	return strippedWord;
}

// Driver function to sort the vector elements 
// by second element of pairs 
bool sortBySecond(const pair<string,size_t> &a, 
              const pair<string,size_t> &b) 
{
	if (a.second == b.second) {
		return (a.first < b.first);
	}
    return (a.second > b.second);
} 

void WordCount::dumpWordsSortedByWord(std::ostream &out) const {
  // dump each word,count pair as CSV to std::ostream, sorted by word in
  // lexicographical order based on ASCII values. Each word count pair
  // will be in its own line with as: word,numOccurence
  // For example: "Sentence is a sentence" will be:
  // A,1
  // IS,1
  // SENTENCE,2
	vector<std::pair<string, size_t> > allWords;
	for (size_t i = 0; i < CAPACITY; i++) {
		for (size_t j = 0; j < table[i].size(); j++) {
			allWords.push_back(table[i].at(j));
		}
	}

	sort(allWords.begin(), allWords.end());

	for (size_t i = 0; i < allWords.size(); i++) {
		out << allWords[i].first << "," << allWords[i].second << endl;
	}
}

void WordCount::dumpWordsSortedByOccurence(std::ostream &out) const {
  // dump each word,count as CSV to std::ostream, sorted decreasing by occurence.
  // In the event of a tie, ordering is defined by the lexicographical
  // order of the word key based on ASCII value. Each word count pair
  // will be in its own line with as word,numOccurence. For example,
  // "Sentence is a sentence" will be:
  // SENTENCE,2
  // A,1
  // IS,1
	vector<std::pair<string, size_t> > allWords;
	for (size_t i = 0; i < CAPACITY; i++) {
		for (size_t j = 0; j < table[i].size(); j++) {
			allWords.push_back(table[i].at(j));
		}
	}

	sort(allWords.begin(), allWords.end(), sortBySecond);

	for (size_t i = 0; i < allWords.size(); i++) {
		out << allWords[i].first << "," << allWords[i].second << endl;
	}
}


void WordCount::addAllWords(std::string text) {
	size_t startPosition = 0;
	size_t found = text.find_first_of(" \n", startPosition);
	while (found != string::npos) {
		if (found > startPosition) {
			incrWordCount(text.substr(startPosition, found - startPosition));
		}
		startPosition = found + 1;
		found = text.find_first_of(" \n", startPosition);
	}
	incrWordCount(text.substr(startPosition, found - startPosition));
}
