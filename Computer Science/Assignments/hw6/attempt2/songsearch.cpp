//
// songsearch.cpp
//
//   Contains functionality for reading in songs and 
//   Searching them by word.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "songsearch.h"

using namespace std;

SongSearch::SongSearch()
{
	songs = new vector<Song>;
	matches = new vector<Song>;
}

SongSearch::~SongSearch()
{
	delete songs;
	delete matches;
}

//
// read_lyics 
//   purpose: read in song data from a disk file
//   arguments: the name of the file, bool to ask for progress status
//   returns: nothing
//   does: calls a function each time a word is found
//
void SongSearch::read_lyrics(const char * filename, bool show_progress)
{
	ifstream in(filename);			// creates an input stream
	int song_count = 0;			// for progress indicator
	string artist, title, word;
	// -- While more data to read...
	while ( ! in.eof() ) 
	{
		Song song;
		// -- First line is the artist
		getline(in, artist);
		if (in.fail()) break;
		// -- Second line is the title
		getline(in, title);
		if (in.fail()) break;
		song.title = title;
		song.artist = artist;
		song.lyrics = " "; //A word is only clounted if there are spaces on both sides.
		if ( show_progress )
		{
			song_count++;
			if (song_count % 10000 == 0) {
				cout << "At "       << song_count << 
					" Artist: " << artist     << 
					" Title:"   << title << endl;
			}
			//if(title == "Scotty Doesn't Know") cout << "Scotty Doesn't Know: " << song_count << "\n";
		}
		// -- Then read all words until we hit the 
		// -- special <BREAK> token
		//REMEMBER TO CONVERT TO LOWERCASE
		while ( in >> word && word != "<BREAK>" )
		{
			song.lyrics += word;
			song.lyrics += " ";
		}
		// -- Important: skip the newline left behind
		in.ignore();
		//cout << song.lyrics << "\n" << "----------\n";
		songs->push_back(song);
	}
}

//
// alpha_only
//   purpose: converts a string to lowercase and alphabetic characters
//            only. E.g., "Bang!" becomes "bang"
//   arguments: a string
//   returns: the alpha-only string
//   does: converts the string
//
string SongSearch::alpha_only(string s)
{
        ostringstream ss;
        for (size_t i=0;i<s.length();i++)
	{
                if (isalnum(s[i]))
		{
                        ss << (char)(tolower(s[i]));
                }
        }
        return ss.str();
}

void SongSearch::search()
{
	//read in words
	string word;
	cin >> word;
	word = alpha_only(word);
	//search each song and copy it over if there's a match
	for(int i = 0; i < (int)(songs->size()); i++)
	{
		search_lyrics(word, songs->at(i));
		//if(songs[i].count > 0) copy(song[i]);
	}
	//We now sort the array of matches with a selection sort
	//We terminate it ater the 10 largest elements have been found.
	if(matches->size() == 0) //If there's nothing in the matches list, we can just leave.
	{
		cout << "No results found!\n";
		return;
	}
	/*
	int swap = 0;
	int count = 1; //Starting at 1 because we know there's at lest 1 match
	int i = 0; //Array index we're on
	while(count < 10) //This is a simple selection sort algorithm
	//To save time, we terminate it after we've found the 10 unique songs with the highest counts
	{
		swap = i;
		for(int j = i+1; j < (int)(matches->size()); j++)
		{
			if(matches->at(j).count > matches->at(swap).count)
			{
				swap = j;
			}
		}
		Song tmp = matches->at(swap);
		matches->at(swap) = matches->at(i);
		matches->at(i) = tmp;
		i++;
		if(matches->at(i).title != matches->at(i-1).title || matches->at(i).artist != matches->at(i-1).artist)
		{
			count++; //We know there's a different song.
		}
	}
	*/
	//Go through and print the songs
	for(int i = 0; i < (int)(matches->size()); i++)
	{
		print_song(matches->at(i));
	}
	cout << "<END-OF-REPORT>\n";
}

void SongSearch::copy(Song song, int index)
{
	Song newSong;
	newSong.title = song.title;
	newSong.artist = song.artist;
	newSong.lyrics = get_context(song.lyrics, index);
	matches->push_back(newSong);
}

string SongSearch::get_context(string lyrics, int index)
{
	//bool closeToBeginning = false;
	//bool closeToEnd = false;
	int spaces = 0;
	int from = 0; //Safety values, guaranteed not to segfault
	int len = lyrics.length();
	for(int i = index; i > 0; i--) //Walk backwards through the string
	//If we see a certain amount of spaces before the loop, we know we can grab with impunity.
	{
		if(lyrics[i] == ' ') spaces++;
		if(spaces == 11) //Words = spaces-1
		{
			from = i;
			break;
		}
		//closeToBeginning = true; //We'll only hit this line if there aren't enough spaces
	}
	spaces = 0;
	for(int i = index; i < (int)(lyrics.length()); i++) //Same going forwards
	{
		if(lyrics[i] == ' ') spaces++;
		if(spaces == 10)
		{
			len = i;
			break;
		}
		//closeToEnd = true;
	}
	return lyrics.substr(from, len);
}

//This function creates a "Last Position Table". By the end of the method,
//the index corresponding to a given character c will contain the index of
//c's final appearance in the pattern. (temporarily cast as a char)
//The reason I'm storing everything as chars in a string is because I
//spent 4 hours in Halligan wrestling with pointers and this is much
//easier because C++ handles pointers and memory allocation for you if you use
//strings.
//
//If there is no occurence of the character in the pattern, its index will
//contain a NULL character, representing index 0.
string SongSearch::lastPositionTable(string pattern)
{
	string lastPositionTable = "";
	for(int i = 0; i < 128; i++) lastPositionTable += (char)(0);
	for(int i = 0; i < (int)(pattern.length()-1); i++)
	{
		lastPositionTable[(int)(pattern[i])] = (pattern.length())-i-1;
	}
	return lastPositionTable;
}

//This function steps backwards through the pattern, and determines how far
//ahead we can jump based on the position difference between what we've
//already searched and what we're searching now.
int SongSearch::jumpindex(char search, string pattern, string alreadySearched)
{
	for(int i = pattern.length(); i >= 1; i--)
	{
        	bool flag = true;
        	for(int j = 0; j < (int)(alreadySearched.length()); j++)
		{
            		int k = i-alreadySearched.length()-1+j;
            		if(!(k < 0) && alreadySearched[j] != pattern[k])
			{
				flag = false;
			}
		}
		signed int term_index = i-alreadySearched.length()-1;
		if(flag)
		{
			if(term_index <= 0 || pattern[term_index-1] != search)
			{
				return (pattern.length()-i+1);
			}
		}
	}
	return -1; 	//Emergency return statement. If the search is working,
			//this should be called rarely, if at all.
}

//This function builds a "jumping table" that keeps track of how far ahead we
//can jump at every index that we could land on.
//
//Again, I'm storing the jumping table in a string because I'm growing to
//despise pointers.
string SongSearch::jumpingTable(string pattern)
{
	string jumpingTable = "";
	for(int i = 0; i < 128; i++) jumpingTable += (char)(0);
	string alreadySearched = "";
	for(int i = 0; i < (int)(pattern.length()); i++)
	{
		jumpingTable[alreadySearched.length()] = jumpindex(pattern[pattern.length()-1-i], pattern, alreadySearched);
		alreadySearched = pattern[pattern.length()-1-i] + alreadySearched;
	}
	return jumpingTable;
}

void SongSearch::search_lyrics(string pattern, Song song)
{
	pattern = " " + pattern;
	pattern += " "; //This prevents us from picking up partial searches.
	//HERE THERE BE DRAGONS
	string jT = jumpingTable(pattern);
	string lPT = lastPositionTable(pattern);
	int index = 0;
	while(index < (int)(song.lyrics.length()-pattern.length()+1))
	{
		int j = pattern.length();
		while((j > 0) && (pattern[j-1] == song.lyrics[index+j-1]))
		{
			j -= 1;
		}
		if(j <= 0)
		{
			copy(song, index);
			//song.count++;
			index++;
		}
		else
		{
 			int lastPosition = lPT[(int)(song.lyrics[index+j-1])];
			if(lastPosition == 0) lastPosition = pattern.length();
			int jump = jT[pattern.length()-j];
			if(lastPosition > jump) index += lastPosition;
			else index += jump;
		}
	}
}

void SongSearch::print_song(Song song)
{
	cout << "Title: " << song.title << "\nArtist: " << song.artist << "\nContext: " << song.lyrics << "\n\n";
}