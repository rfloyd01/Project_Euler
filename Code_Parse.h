#pragma once

#include <Header_Files/pch.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

std::vector<std::string> blockwords = { "for", "while", "if", "else" };
std::vector<std::string> keywords = { "continue", "break", "return" };
std::vector<std::string> types = { "short", "int", "long", "float", "double", "void", "char", "bool"};
std::vector<std::string> classes = { "vector", "pair", "string", "ifstream"};
std::vector<std::string> objects = { "class", "struct", "enum" };
std::vector<std::string> scopes = { "private:", "public:", "protected:" };
std::vector<char> whiteSpaceCharacters = { ' ', '\t' };
std::vector<char> newLineCharacters = { '\n', '\r' };

bool debugPrint = false;

/*
Code Block types are:
("blocks without sub-blocks")
0 - Blank line
1 - Preprocessor command
2 - Comment
3 - Standard code line
4 - Function declaration block
5 - Quotation/String literal

("blocks with sub-blocks")
6 - Loop/If/Block without curly braces
7 - Loop/If/Block with curly braces
8 - Function/Class/Struct definition
*/

class CodeBlock
{
public:
    //CodeBlock(int blockType) { this->blockType = blockType; }
    CodeBlock(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine);

    std::vector<std::string> begginingLines;
    std::vector<std::string> endingLines;
    std::vector<CodeBlock*> subBlocks;
    int blockType;
    std::string blockLine = "";
    //char beginningCharacter, endingCharacter;

    //void addBeginningLine(std::string line) { this->begginingLines.push_back(line); }
    //void addEndingLine(std::string line) { this->endingLines.push_back(line); }
    //void determineBlockType(std::vector<std::string>& allCodeLines, int currentLine, int placeInLine);
    void advanceToNextCharacter(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine);
    void advanceToEndOfComment(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine, char startChar);

    //std::pair<int, int> endingCharacterLocation; //keeps track of location the terminates the 'beginning' of the block

    /*char getBeginningEndCharacter()
    {
        return this->beginningCharacter;
    }
    char getEndingEndCharacter()
    {
        return this->endingCharacter;
    }*/

    void printBlock()
    {
        for (int i = 0; i < this->begginingLines.size(); i++) std::cout << this->begginingLines[i];
        for (int i = 0; i < this->subBlocks.size(); i++) subBlocks[i]->printBlock();
        for (int i = 0; i < this->endingLines.size(); i++) std::cout << this->endingLines[i];
    }
};

void CodeBlock::advanceToNextCharacter(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine)
{
    //This function gets used a lot. It iterates the currentLine and placeInLine variables to the next
    //non-white space character. If a quote is encountered then the function scans through the entire quote and adds
    //it as a separate line to the current block.

    char currentCharacter;
    bool inQuote = false;
    bool escapeCharacter = false;

    while (true)
    {
        placeInLine++;
        if (placeInLine >= allCodeLines[currentLine].size())
        {
            currentLine++;
            placeInLine = 0;
        }
        if (currentLine >= allCodeLines.size())
        {
            //we've reached the end of the code, whatever we're currently looking at
            //should be added to the
            return;
        }
        currentCharacter = allCodeLines[currentLine][placeInLine];

        //Regardless of whether we're in a comment, a quote, or just normal code, white space gets added just the same
        if (std::count(whiteSpaceCharacters.begin(), whiteSpaceCharacters.end(), currentCharacter))
        {
            this->blockLine += currentCharacter; //white space is added
            continue;
        }
        else if (std::count(newLineCharacters.begin(), newLineCharacters.end(), currentCharacter))
        {
            //If we encounter a new line character then we add the current block line to the array of beginning lines
            //and then reset blockLine to handle characters from the next line of code
            this->begginingLines.push_back(this->blockLine + currentCharacter);
            this->blockLine = "";
            continue;
        }

        //we aren't inside of a comment so we proceed normally
        if (!inQuote)
        {
            if (currentCharacter == '\'' || currentCharacter == '\"')
            {
                //Since we aren't currently inside of a comment block, we've found the start of a quotation. Add
                //The current block line to the beggining lines array and then start a new one for the quote.
                this->begginingLines.push_back(this->blockLine);
                this->blockLine = std::to_string(currentCharacter);
                inQuote = true;
            }
            else
            {
                //Any other character encountered will break us out of this function. We don't add the character
                //to the current block line as we're not sure of what the character means yet.
                return;
            }
        }
        else
        {
            //The only thing that will break us out is a non-escaped version of the character that started the quote.
            //The character that started the quote will be at the very beginning of the current blockLine string. We use
            //the escapeCharacter boolean to keep track of whether or not the previous character was a '\' as it helps
            //for keeping track across different lines

            this->blockLine += currentCharacter; //the current character get's added regardless of what it is
            if (currentCharacter == '\\')
            {
                //We use XOR on the boolean so that if we have two escape characters in a row it will un-escape
                //the third character (i.e. '\'' is an escaped apostraphe but '\\' is en escaped backslash)
                escapeCharacter ^= 1;
                continue;
            }
            else if (currentCharacter == this->blockLine[0])
            {
                if (!escapeCharacter)
                {
                    //There's no escape character before this so we've reached the end of the quote.
                    //Add the quote to the current code block as a separate entity
                    this->begginingLines.push_back(this->blockLine);
                    this->blockLine = "";
                    inQuote = false;
                    continue;
                }
            }
            escapeCharacter = false; //didn't encounter a '\' so this variable becomes false
        }
    }
}

void CodeBlock::advanceToEndOfComment(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine, char startChar)
{
    //If we're find comment block, then this function will keep advancing until the end of the comment is reached.
    //When in a single line comment, we can add whole lines as there's no way to break out of one other than
    //by going to a new line. If we're in a multi-line comment, ironically, it's possible for the comment to
    //end on the same line so we must scan one character at a time.

    char currentCharacter;
    bool nonSpaceEncountered = true;

    while (true)
    {
        placeInLine++;
        if (placeInLine >= allCodeLines[currentLine].size())
        {
            currentLine++;
            placeInLine = 0;
        }
        if (currentLine >= allCodeLines.size()) return; //we've reached the end of the code
        currentCharacter = allCodeLines[currentLine][placeInLine];

        //First, check to see if we're dealing with a white space or newline character
        if (std::count(whiteSpaceCharacters.begin(), whiteSpaceCharacters.end(), currentCharacter))
        {
            this->blockLine += currentCharacter; //white space is added
            continue;
        }
        else if (std::count(newLineCharacters.begin(), newLineCharacters.end(), currentCharacter))
        {
            //If we encounter a new line character then we add the current block line to the array of beginning lines
            //and then reset blockLine to handle characters from the next line of code
            this->begginingLines.push_back(this->blockLine + currentCharacter);
            this->blockLine = "";
            nonSpaceEncountered = false; //this needs to reset for each new line (used for single line comments)
            continue;
        }

        if (startChar == '/')
        {
            //--Single line comment--
            //White space is handled at the beginning of this function so we're going to be looking at an actual
            //character here. We need to figure out if we're at the very beginning of the line or not. If we are
            //and the current character is anything but a '/' symbol then we're out of the current comment.
            if (!nonSpaceEncountered)
            {
                if (currentCharacter == '/')
                {
                    //we've encountered the start of another comment, see if it's another single line comment
                    //or if a multi-line comment has been started.
                    if (allCodeLines[currentLine][placeInLine + 1] == '*')
                    {
                        //There's a multi-line comment after a single line comment. These should be considered 
                        //as separate blocks. Any leading spaces gathered should belong to the next block, so 
                        //we reset our place in line to 0
                        placeInLine = 0;
                        return;
                    }

                    //It's another single line comment. This line will be added to the existing block
                    nonSpaceEncountered = true;
                }
                else
                {
                    //We didn't start another single line comment block so the current block is done. Drop the final
                    //line of this current block (as it contains white space needed for the next block) and reset the 
                    //line position to the start of the line so appropriate amount of white space can be added to 
                    //the front of the next block
                    this->begginingLines.pop_back();
                    placeInLine = 0;
                    return;
                }
            }

            this->blockLine += currentCharacter; //if we didn't return already in the above if-statement then it's safe to add character
        }
        else
        {
            //--Multi-line Comment--
            //In a multi-line comment, the only thing that can break us out are the consecutive "*/" characters
            if (currentCharacter == '*')
            {
                if (allCodeLines[currentLine][placeInLine + 1] == '/')
                {
                    //the comment is complete, add the final two characters and then advance to the start of the next block.
                    //Advance via the advanceToNextCharacter() method so and trailing white space can be added to this comment's
                    //block
                    this->blockLine += "*/";
                    placeInLine++;
                    int lineNumber = currentLine;
                    advanceToNextCharacter(allCodeLines, currentLine, placeInLine);
                    if (currentLine == lineNumber)
                    {
                        //the next block starts on the same line so we need to manuall add our current
                        //blockLine
                        this->begginingLines.push_back(blockLine);
                    }
                    else
                    {
                        //if we've gone to a new line, reset the placeInLine variable so
                        //white space can be added to the front of the next block
                        placeInLine = 0;
                    }
                    return;
                }
            }

            //If we make it here, either we encountered a '*' character that didn't end the quote, or we encountered
            //any other character. Either way, add the character and then continue
            this->blockLine += currentCharacter;
        }
    }
}

std::string getFirstWord(std::string line, int start = 0)
{
    //returns the first word in the given line of code, a "word" ends when it 
    //encounters white space, or a '(' character. The parentheses is uncluded
    //because there doesn't actually have to be white space between certain keywords
    //like. while(i < 5) and while (i < 5) are both syntactically correct

    //we also ignore the '<' chracter so if we're dealing with a class, i.e.
    //std::pair<std::vector<std::vector<int>>, int>, all that really matters is 
    //that we're working with a pair on the outermost layer, so the first word will
    //just be std::pair.

    std::string firstWord = "";
    for (int i = start; i < line.size(); i++)
    {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r' || line[i] == '\t' || line[i] == '(') break;
        else if (line[i] == '<') break;
        else firstWord += line[i];
    }

    //see if we actually have any letters
    if (firstWord.size() == 0) return firstWord;

    //see if the last character of the found word is a semi-colon, if so remove it
    if (firstWord[firstWord.size() - 1] == ';') firstWord.pop_back();

    //before returning the word, see if there are any namespaces before the name and truncate them
    int startPoint = 0;
    for (int j = firstWord.length() - 1; j--; j >= 0)
    {
        if (firstWord[j] == ':')
        {
            startPoint = j + 1;
            break;
        }
    }
    return firstWord.substr(startPoint);

    //return firstWord;
}

void findQuoteEnd(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine)
{
    //This function advances us to the end of a quote. A quote can either start with " or ',
    //whatever is starts with it will also have to end with. While still inside the quote, make
    //sure that any potential end characters aren't preceeded by the '\' escape character
    char endCharacter = allCodeLines[currentLine][placeInLine++];
    bool lastCharacterEscape = false;

    while (true)
    {
        if (placeInLine >= allCodeLines[currentLine].size())
        {
            //go to the next line
            currentLine++;
            placeInLine = 0;
        }

        if (allCodeLines[currentLine][placeInLine] == '\\')
        {
            lastCharacterEscape ^= 1; //if we hit two escape characters in a row then we actually break the escape
            placeInLine++;
        }
        else if (allCodeLines[currentLine][placeInLine] == endCharacter && !lastCharacterEscape)
        {
            //placeInLine++; //advance one spot further so we don't accidentally call this function again
            return; //we've reached the end of the quote
        }
        else
        {
            //found a non-escape character, set lastCharacterEscape to false and increment
            lastCharacterEscape = false;
            placeInLine++;
        }
    }
}

void findFirstNonQuoteCharacter(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine, char c)
{
    //finds the first instance of the given character that isn't inside of a quote string
    while (true)
    {
        if (allCodeLines[currentLine][placeInLine] == c) break;
        else if (allCodeLines[currentLine][placeInLine] == '\'' || allCodeLines[currentLine][placeInLine] == '\"') findQuoteEnd(allCodeLines, currentLine, placeInLine);

        if (placeInLine >= allCodeLines[currentLine].size())
        {
            currentLine++;
            placeInLine = 0;
        }
        else placeInLine++;
    }
}

void findClosingParenthese(std::vector<std::string>& allCodeLines, int &currentLine, int &placeInLine)
{
    //this function is used to find the closing parentheses of a for, if, or else block. This function
    //is necessary because there can be multiple parentheses to sort through, for example:
    //for (int i = 0; i < vector.size(); i++) {..logic..}. If we were to just search this line for the
    //first ')' character it wouldn't actually be the character that closes the for statement.

    //Scan until we find the first parentheses
    while (true)
    {
        if (allCodeLines[currentLine][placeInLine] == '(') break;

        if (placeInLine >= allCodeLines[currentLine].size())
        {
            currentLine++;
            placeInLine = 0;
        }
        else placeInLine++;
    }

    int parenthesesLevel = 0;

    //scan again until we find more parentheses, if we find a '(' character the parentheses level
    //goes up by 1 and if we find a ')' character, we check to see if the current level is at 1, if
    //so we're done, otherwise the level goes down one.
    while (true)
    {
        if (allCodeLines[currentLine][placeInLine] == '(')
        {
            parenthesesLevel++;
        }
        else if (allCodeLines[currentLine][placeInLine] == ')')
        {
            if (parenthesesLevel > 1) parenthesesLevel--;
            else return;
        }
        else if (allCodeLines[currentLine][placeInLine] == '\'' || allCodeLines[currentLine][placeInLine] == '\"')
        {
            //There's a quotation, skip to the end of the quote in case it has any parentheses in it
            //which shouldn't be counted.
            findQuoteEnd(allCodeLines, currentLine, placeInLine);
        }

        if (placeInLine >= allCodeLines[currentLine].size())
        {
            currentLine++;
            placeInLine = 0;
        }
        else placeInLine++;
    }
}

void findEndOfMultiLineQuote(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine)
{
    //a multi line quote depends on two consecutive characters to terminate instead of just one. This is
    //out of the ordinary so it get's its own dedicated function to do this. We pass the location of the 
    //start of the quote to this function, so we need to skip the first two characters

    placeInLine += 2;
    while (true)
    {
        if (placeInLine >= allCodeLines[currentLine].size())
        {
            currentLine++;
            placeInLine = 0;
        }

        if (allCodeLines[currentLine][placeInLine] == '*')
        {
            if (allCodeLines[currentLine][++placeInLine] == '/') return;
        }
        else placeInLine++;
    }
}

bool contains(std::vector<std::string>& vec, std::string item)
{
    //checks to see if the given item is contained in the given vector
    for (int i = 0; i < vec.size(); i++)
        if (vec[i] == item) return true;

    return false;
}

CodeBlock::CodeBlock(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine)
{
    //The constructor for a new block of code. As inputs we give the vector containing all of our lines
    //of code, as well as a current location in this vector
    char currentCharacter = allCodeLines[currentLine][placeInLine];

    //If the current block of code starts on a white space, advance to the next non-white space
    if (currentCharacter == ' ' || currentCharacter == '\t')
    {
        advanceToNextCharacter(allCodeLines, currentLine, placeInLine);
        currentCharacter = allCodeLines[currentLine][placeInLine];
    }

    //First we check to see if we can figure out the current block type by looking at
    //only the first character
    if (currentCharacter == '#')
    {
        //This is a pre-processor command, add the whole line as our block
        if (debugPrint) std::cout << "Found a pre-processor command" << std::endl;
        this->blockType = 0;
        this->begginingLines.push_back(allCodeLines[currentLine]);
        currentLine++;
        placeInLine = 0;
        return;
    }
    else if (currentCharacter == '/')
    {
        //We're looking at a comment, add the first two characters, advancing our place in line as well
        this->blockLine += allCodeLines[currentLine][placeInLine++];
        this->blockLine += allCodeLines[currentLine][placeInLine];
        advanceToEndOfComment(allCodeLines, currentLine, placeInLine, allCodeLines[currentLine][placeInLine]); //advance to the end of the comment
        
        if (debugPrint) std::cout << "Found a comment block" << std::endl;
        this->blockType = 1;

        return;
    }
    else if (currentCharacter == ';')
    {
        //We've encountered a rogue semi-colon. Technically, a single semi-colon could be
        //it's own block (i.e. int x = 3;;;;;;; would be legal)
        this->blockLine += currentCharacter;
        int lineLocation = currentLine;
        advanceToNextCharacter(allCodeLines, currentLine, placeInLine); //add any trailing whitespace

        if (lineLocation != currentLine)
        {
            //if the next viable character starts on a different line, then we need to reset the
            //placeInLine variable so that any leading whitespace can be captured in the next block
            placeInLine = 0;
        }
        this->blockType = 2;
        return;
    }

    //Our block type couldn't be determined from the first character, instead it will be 
    //determined by the first word.
    std::string firstWord = getFirstWord(allCodeLines[currentLine], placeInLine);
    int location = firstWord.size() - 1 + placeInLine; //The location of the charcter at end of the first word

    //Check to see if the block is a loop, if statement, or something similar
    if (contains(blockwords, firstWord)) {
        //**note: an else if block can't be separated between lines, otherwise the else and 
        //the if will be recognized as different. i.e. else if (something) is ok but
        //else
        //   if (something), isn't. It is possible though, to have the part in parentheses 
        //start on another line, i.e.
        //if
        //   (something) would be allowed. Need to make sure that this is taken into account while
        //iterating through the code below

        //need to find out if this is a block with curly braces or without curly braces.
        //If firstWord is "else", we need to check and see if the word "if" comes after it.
        //I so, or, if the word isn't "else" we increment location until we find the appropriate ')'
        //character.
        if (firstWord == "else")
        {
            if (allCodeLines[currentLine][location + 1] != '\n' && getFirstWord(allCodeLines[currentLine], location + 2) == "if")
            {
                findClosingParenthese(allCodeLines, currentLine, placeInLine);
                location = placeInLine;
            }
        }
        else
        {
            findClosingParenthese(allCodeLines, currentLine, placeInLine);
            location = placeInLine;
        }

        //At this point, the location variable will be on a ')' character, or 
        //an 'e' character. In both cases, if the next non-whitespace character is a
        //'{' character then we have a curly brace block, if it's anything else then
        //we have a non-curlybrace block.
        char currentCharacter;
        int scannedLine = currentLine;
        while (true)
        {
            location++;
            if (location == allCodeLines[scannedLine].size())
            {
                location = 0;
                scannedLine++;
            }
            
            currentCharacter = allCodeLines[scannedLine][location];
            if (currentCharacter == ' ' || currentCharacter == '\t' || currentCharacter == '\n' || currentCharacter == '\r') continue;
            
            if (currentCharacter == '{')
            {
                if (debugPrint) std::cout << "Found a curly brace block" << std::endl;
                this->blockType = 7;
                this->beginningCharacter = '{';
                this->endingCharacter = '}';
                this->beginningCharacterLocation = { scannedLine, location };
                return;
            }
            else if (currentCharacter == '/')
            {
                //It's possible for comments to be written between the end of the block definition
                //and the start of the physical block, i.e. void function yo() //whaddup {stuff}

                if (allCodeLines[scannedLine][++location] == '/')
                {
                    //it's a single line comment, just advance to the next line
                    location = 0;
                    scannedLine++;
                }
                else if (allCodeLines[scannedLine][location] == '*')
                {
                    //it's a multi-line comment, advance to the end of it
                    findEndOfMultiLineQuote(allCodeLines, scannedLine, ++location); //must start directly after the comment
                }
            }
            else 
            {
                if (debugPrint) std::cout << "Found a non-curly brace block" << std::endl;
                this->blockType = 4;
                if (firstWord == "else")
                {
                    this->beginningCharacter = ' '; //the space after else ends the block
                    this->beginningCharacterLocation = { currentLine, location - 1 }; //location of the space
                }
                else
                {
                    this->beginningCharacter = ')'; //the end of the parentheses ends the block
                    this->beginningCharacterLocation = { currentLine, placeInLine };
                }
                this->endingCharacter = ' ';
                return;
            }
        }
    }
    //else if (contains(types, firstWord) || contains(classes, firstWord))
    //{
    //    //If the first word of the line is a class or built-in type it means one of two things.
    //    //Either we're about to define a function, i.e. void func() {}, or we're about to define
    //    //some variables, i.e. int x = 5, y = 6; It will depend on whether we first find an '=',
    //    //a ';', or a '(' character. If we encounter an '=' of ';' first then the line is for defining variables.

    //    bool functionFound = false;
    //    //iterate until we hit one of these characters
    //    while (true)
    //    {
    //        location++;
    //        if (location == allCodeLines[currentLine].size())
    //        {
    //            location = 0;
    //            currentLine++;
    //        }

    //        if (!functionFound)
    //        {
    //            if (allCodeLines[currentLine][location] == '=' || allCodeLines[currentLine][location] == ';')
    //            {
    //                if (debugPrint) std::cout << "Found a variable definition block" << std::endl;
    //                this->blockType = 3;
    //                this->beginningCharacter = ';';
    //                this->endingCharacter = ' ';
    //                findFirstNonQuoteCharacter(allCodeLines, currentLine, location, ';');
    //                this->beginningCharacterLocation = { currentLine, location };
    //                return; //this is considered a standard line of code
    //            }
    //            else if (allCodeLines[currentLine][location] == '(')
    //            {
    //                //we've either found a function definition, or a function declaration, jump forwards
    //                //to the next ')' character
    //                functionFound = true;
    //                findFirstNonQuoteCharacter(allCodeLines, currentLine, location, ')');
    //            }
    //        }
    //        else
    //        {
    //            if (allCodeLines[currentLine][location] == '{')
    //            {
    //                if (debugPrint) std::cout << "Found a function definition block" << std::endl;
    //                this->blockType = 6;
    //                this->beginningCharacter = '{';
    //                this->endingCharacter = '}';
    //                this->beginningCharacterLocation = { currentLine, location };
    //                return;
    //            }
    //            
    //            else if (allCodeLines[currentLine][location] == ';')
    //            {
    //                if (debugPrint) std::cout << "Found a function declaration block" << std::endl;
    //                this->blockType = 3;
    //                this->beginningCharacter = ';';
    //                this->endingCharacter = ' ';
    //                this->beginningCharacterLocation = { currentLine, location };
    //                return;
    //            }
    //        }
    //    }
    //}
    //else if (contains(objects, firstWord))
    //{
    //    if (debugPrint) std::cout << "Found a class/struct/enum definition block" << std::endl;
    //    this->blockType = 6;
    //    this->beginningCharacter = '{';
    //    this->endingCharacter = '}';
    //    findFirstNonQuoteCharacter(allCodeLines, currentLine, location, '{');
    //    this->beginningCharacterLocation = { currentLine, location };
    //    return;
    //}
    //else if (contains(scopes, firstWord))
    //{
    //    //For my purposes I don't need to actually make this a block, treat it as a standard line
    //    //but have it end with the \n character instead of the ; character
    //    if (debugPrint) std::cout << "Found a scope block" << std::endl;
    //    this->blockType = 3;
    //    this->beginningCharacter = '\n';
    //    this->endingCharacter = ' ';
    //    this->beginningCharacterLocation = { currentLine, location };
    //    return;
    //}
    //else if (contains(keywords, firstWord))
    //{
    //    //we've found a word like return
    //    if (debugPrint) std::cout << "Found a keyword block of code" << std::endl;
    //    this->blockType = 3;
    //    this->beginningCharacter = ';';
    //    this->endingCharacter = ' ';
    //    this->beginningCharacterLocation = { currentLine, location };
    //    return;
    //}

    //If we can't determine what the block is then it must just be a standard block
    //i.e. x += 10; where x is an already predefined variable. It's also possible
    //that we've found a plain semi-colon. For debugging sake I'll treat that 
    //as separate for now
    if (debugPrint) std::cout << "Found a standard block of code" << std::endl;

    this->blockType = 3;
    this->beginningCharacter = ';';
    this->endingCharacter = ' ';
    findFirstNonQuoteCharacter(allCodeLines, currentLine, location, ';');
    this->beginningCharacterLocation = { currentLine, location };
    return;
}

void recursiveCodeBlockCreation(std::vector<CodeBlock*>& blockArray, std::vector<std::string>& allCodeLines, int& currentLineNumber, int& placeInLine)
{
    //We're always going to be starting at a new block when this function gets called, the first step is to determine
    //what kind of block it is. The below CodeBlock
    CodeBlock* codeBlock = new CodeBlock(allCodeLines, currentLineNumber, placeInLine);

    if (codeBlock->blockType >= 4)
    {
        //This code block will have some sub-blocks and potentially an ending curly brace.
        //We recursively call this function with the sub-block array of the current code
        //block at the current location.

        while (allCodeLines[currentLineNumber][placeInLine] != '}')
        {
            //the recursive function will automatically increment the currentLineNumber and
            //placeInLine variables for us, we just need to reassign currentCharacter each 
            //time
            recursiveCodeBlockCreation(codeBlock->subBlocks, allCodeLines, currentLineNumber, placeInLine);

            //BlockType 6 is a block without curly braces, which means it can only have a
            //single sub-block so there's no need to keep looping
            if (codeBlock->blockType == 6) break;
        }
    }
    else
    {
        //It's possible that code blocks without any sub-blocks could coincide
        //with the end of the file (like a final comment after all code for example).
        //Check to make sure that the blockLine variable isn't blank
        if (codeBlock->blockLine != "") codeBlock->begginingLines.push_back(codeBlock->blockLine);
    }

    //before adding the current block of code, check to see if we've reached the end of the
    //file and need to append any last lines

    blockArray.push_back(codeBlock);
    return;
}

void codeParse()
{
    auto run_time = std::chrono::steady_clock::now();

    //First, get the test code from .txt file
    std::string sampleCode = "", reader = "";
    std::ifstream myFile;
    std::vector<std::string> linesOfCode;
    myFile.open("testParse.txt");
    while (std::getline(myFile, reader))
    {
        reader += '\n'; //add in newline characters that are removed from getLine()
        linesOfCode.push_back(reader);
    }
    linesOfCode.back().pop_back(); //remove the final \n as it isn't needed
    myFile.close();

    std::vector<CodeBlock*> outermostBlocks;
    int currentLineNumber = 0, placeInLine = 0;

    //for (int i = 0; i < linesOfCode.size(); i++) std::cout << linesOfCode[i];

    while (currentLineNumber < linesOfCode.size())
    {
        recursiveCodeBlockCreation(outermostBlocks, linesOfCode, currentLineNumber, placeInLine);
        outermostBlocks.back()->printBlock();
    }
    std::cout << std::endl << std::endl;
    std::cout << "Parsed " << linesOfCode.size() << " lines of code in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 << 
        " seconds." << std::endl;

    //for (int i = 0; i < outermostBlocks.size(); i++) outermostBlocks[i]->printBlock();
    //after getting all of the blocks, search for any adjacent single line comment blocks as they 
    //can be combined into a single block for ease.
}