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
    void findClosingParenthese(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine);
    void addClosingWhiteSpace(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine);
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
            //character here. We need to figure out if we're at the very beginning of the line or not. If we are,
            //and the current character is anything but a '/' symbol then we're out of the current comment.
            if (!nonSpaceEncountered)
            {
                if (currentCharacter == '/')
                {
                    //We've either encountered another single line comment, or the start of a multi-line comment.
                    //In the case of another single line comment, it gets added to the current block and we keep going.
                    if (allCodeLines[currentLine][placeInLine + 1] == '/')
                    {
                        nonSpaceEncountered = true;
                        this->blockLine += currentCharacter;
                        continue; //go back to beginning of loop
                    }
                }
                
                //We didn't start another single line comment block so the current block is done. Go to the end
                //of the previous line so that the addClosingWhiteSpace() method will bring us back to this location
                //without messing up the indentation of the following block
                currentLine--;
                placeInLine = allCodeLines[currentLine].size() - 1;
                this->blockLine = "";
                return;
            }

            //If we're down here then it means were confirmed to still be in a comment
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
                    placeInLine++; //advance to the closing '/' character
                    return;
                }
            }

            //If we make it here, either we encountered a '*' character that didn't end the quote, or we encountered
            //any other character. Either way, add the character and then continue
            this->blockLine += currentCharacter;
        }
    }
}

void CodeBlock::addClosingWhiteSpace(std::vector<std::string>& allCodeLines, int& currentLine, int& placeInLine)
{
    //When we've reached the end of our code block's start, this function will add any trailing white
    //space and/or empty lines before the sub blocks start.

    //As an example:
    //for (int i = 0; i < 10; i++)
    //{
    //
    //    i += 1;
    //}
    //This function will tak on the space after the '{' character and the blank line right after it.

    int lineNumberCopy = currentLine;
    this->advanceToNextCharacter(allCodeLines, currentLine, placeInLine);

    if (lineNumberCopy == currentLine)
    {
        //if lineNumberCopy equals the currentLine variable, it means that we're on the same
        //physical line, in which case the advanceToNextCharacter function will NOT have added
        //the white space to the code block yet.
        this->begginingLines.push_back(this->blockLine);
        blockLine = "";
        return;
    }
    else
    {
        //We're on a different line than we were before, which means the appropriate white space
        //has already been added by the advanceToNextCharacter() method. However, if the new line
        //has leading white space (i.e. because it's an indented block) it won't be added to the
        //new block. To get around this we reset the placeInLine variable to the start of the new line.
        placeInLine = 0;
        blockLine = "";
        return;
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

void CodeBlock::findClosingParenthese(std::vector<std::string>& allCodeLines, int &currentLine, int &placeInLine)
{
    //this function is used to iterate to the appropriate closing parentheses of our current
    //block. This function is needed because there can be multiple parentheses nested inside of
    //each other like in loops [for (int i = 0; i < vec.size(); i++)] or nested functions 
    //[choose(floor(5/2), 2)]

    //Scan until we find the first parentheses (any white space should be automatically added to current block)
    this->advanceToNextCharacter(allCodeLines, currentLine, placeInLine);
    this->blockLine += '(';

    int parenthesesLevel = 1;
    //we're now located just after the opening '(' character. We keep scanning and every time we find a '('
    //character we inncrease the value of the parentheses level variable by one and every ')' character will
    //decrease the level by 1. We keep scanning until the value of the variable drops to 0. Any parentheses
    //that are part of strings will be automatically ignored by the advanceToNextCharacter() method.
    while (parenthesesLevel)
    {
        //Add the next character
        this->advanceToNextCharacter(allCodeLines, currentLine, placeInLine);
        this->blockLine += allCodeLines[currentLine][placeInLine];

        if (allCodeLines[currentLine][placeInLine] == '(') parenthesesLevel++;
        else if (allCodeLines[currentLine][placeInLine] == ')') parenthesesLevel--;
    }

    return; //we return without advancing again, so the placeInLine index will still be on the closing parentheses
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
        this->begginingLines.push_back(allCodeLines[currentLine]); //add the whole line
        placeInLine = allCodeLines[currentLine].size() - 1; //advance to end of the line
        this->addClosingWhiteSpace(allCodeLines, currentLine, placeInLine);

        return;
    }
    else if (currentCharacter == '/')
    {
        //We're looking at a comment, add the first two characters, advancing our place in line as well
        if (debugPrint) std::cout << "Found a comment block" << std::endl;
        this->blockType = 1;
        this->blockLine += allCodeLines[currentLine][placeInLine++];
        this->blockLine += allCodeLines[currentLine][placeInLine];
        advanceToEndOfComment(allCodeLines, currentLine, placeInLine, allCodeLines[currentLine][placeInLine]); //advance to the end of the comment
        this->addClosingWhiteSpace(allCodeLines, currentLine, placeInLine);
        
        return;
    }
    else if (currentCharacter == ';')
    {
        //We've encountered a rogue semi-colon. Technically, a single semi-colon could be
        //it's own block (i.e. int x = 3;;;;;;; would be legal)
        if (debugPrint) std::cout << "Found a stand alone semi-colon" << std::endl;
        this->blockType = 2;
        this->blockLine += currentCharacter;
        advanceToNextCharacter(allCodeLines, currentLine, placeInLine); //add any trailing whitespace
        this->addClosingWhiteSpace(allCodeLines, currentLine, placeInLine);

        return;
    }

    //Our block type couldn't be determined from the first character, instead it will be 
    //determined by the first word.
    std::string firstWord = getFirstWord(allCodeLines[currentLine], placeInLine);
    this->blockLine += firstWord;
    placeInLine += (firstWord.size() - 1); //advance to the last letter of the first word

    //Check to see if the block is a loop, if statement, or something similar
    if (contains(blockwords, firstWord)) {
        //First we need to figure out if this block will have a set of '()', i.e.
        //like in for (int i = ...), while(), if (). The only way that parentheses won't
        //be included is if the word is else, however, if the phrase is 'else if' then it
        //will have parentheses. We need a separate function to scan through the parentheses
        //as there could be other paretheses embedded inside (i.e. for (int i = 0; i < vector.size(); i++))
        //so we need to make sure we find the right closing parentheses character
        
        if (firstWord == "else")
        {
            //check to see if the word if comes right after the else
            if (allCodeLines[currentLine][placeInLine] != '\n' && getFirstWord(allCodeLines[currentLine], placeInLine + 1) == "if")
            {
                //"if" was the second word, which means there are parentheses
                this->blockLine += " if";
                placeInLine += 3; //we're currently on the space between else and if, advance to first char after "if"
                findClosingParenthese(allCodeLines, currentLine, placeInLine);
                //location = placeInLine;
            }
        }
        else
        {
            findClosingParenthese(allCodeLines, currentLine, placeInLine);
            //location = placeInLine;
        }

        //At this point, the location variable will be on a ')' character, or 
        //an 'e' character. In both cases, if the next non-whitespace character is a
        //'{' character then we have a curly brace block, if it's '/' character then
        //we have an embedded comment to handle. Anything else then we have a non-curlybrace block.
        this->advanceToNextCharacter(allCodeLines, currentLine, placeInLine);
        currentCharacter = allCodeLines[currentLine][placeInLine];

        if (currentCharacter == '/')
        {
            //It's possible for comments to be written between the end of the block definition
            //and the start of the physical block, i.e. void function yo() //whaddup {stuff}.
            //scan the comment into the current block and then proceed to the next non-space
            //character
            this->blockLine += allCodeLines[currentLine][placeInLine++];
            this->blockLine += allCodeLines[currentLine][placeInLine];
            this->advanceToEndOfComment(allCodeLines, currentLine, placeInLine, allCodeLines[currentLine][placeInLine]); //advance to the end of the comment
            this->advanceToNextCharacter(allCodeLines, currentLine, placeInLine); //and then go to the next non-sommented character
        }

        if (currentCharacter == '{')
        {
            if (debugPrint) std::cout << "Found a curly brace block" << std::endl;
            this->blockLine += currentCharacter;
            this->addClosingWhiteSpace(allCodeLines, currentLine, placeInLine);
            this->blockType = 7;
            return;
        }
        else
        {
            if (debugPrint) std::cout << "Found a non-curly brace block" << std::endl;
            this->blockLine += currentCharacter;
            this->addClosingWhiteSpace(allCodeLines, currentLine, placeInLine);
            this->blockType = 6;
            return;
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
    /*if (debugPrint) std::cout << "Found a standard block of code" << std::endl;

    this->blockType = 3;
    this->beginningCharacter = ';';
    this->endingCharacter = ' ';
    findFirstNonQuoteCharacter(allCodeLines, currentLine, location, ';');
    this->beginningCharacterLocation = { currentLine, location };*/
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