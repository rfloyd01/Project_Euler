#pragma once

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

bool debugPrint = true;

/*
Code Block types are:
0 - Preprocessor command
1 - Single line comment
2 - Multi-line comment
3 - Standard code line
3a. Function declaration block
4 - Loop/If/Block without curly braces
5 - Loop/If/Block with curly braces
6 - Function/Class/Struct definition
*/

class CodeBlock
{
public:
    //CodeBlock(int blockType) { this->blockType = blockType; }

    std::vector<std::string> begginingLines;
    std::vector<std::string> endingLines;
    std::vector<CodeBlock*> subBlocks;
    int blockType;
    char beginningCharacter, endingCharacter;

    void addBeginningLine(std::string line) { this->begginingLines.push_back(line); }
    void addEndingLine(std::string line) { this->endingLines.push_back(line); }
    void determineBlockType(std::vector<std::string>& allCodeLines, int currentLine, int placeInLine);

    std::pair<int, int> beginningCharacterLocation, endingCharacterLocation; //line and character position

    char getBeginningEndCharacter()
    {
        return this->beginningCharacter;
    }
    char getEndingEndCharacter()
    {
        return this->endingCharacter;
    }

    void printBlock()
    {
        for (int i = 0; i < this->begginingLines.size(); i++) std::cout << this->begginingLines[i];
        for (int i = 0; i < this->subBlocks.size(); i++) subBlocks[i]->printBlock();
        for (int i = 0; i < this->endingLines.size(); i++) std::cout << this->endingLines[i];
    }
};

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

void CodeBlock::determineBlockType(std::vector<std::string>& allCodeLines, int currentLine, int placeInLine)
{
    //If we're starting on a new line that's indented we need to skip ahead until we get to a non-white space
    //character
    char currentCharacter = allCodeLines[currentLine][placeInLine];
    while (currentCharacter == ' ' || currentCharacter == '\t' || currentCharacter == '\n' || currentCharacter == '\r')
    {
        if (placeInLine >= (allCodeLines[currentLine].size() - 1))
        {
            //this is a line with only white space on it, which for now will just
            //be considered as a preprocessor command
            if (debugPrint) std::cout << "Found a blank line" << std::endl;
            this->blockType = 0;
            this->beginningCharacter = '\n';
            this->endingCharacter = ' ';
            this->beginningCharacterLocation = { currentLine, placeInLine };
            return;
        }
        placeInLine++;
        currentCharacter = allCodeLines[currentLine][placeInLine];
    }

    //Check to see if current block type is a comment, whether it's a single or multi-line comment
    //at least two characters are needed to initiate the comment
    if (((allCodeLines[currentLine].size() - 1) - placeInLine) >= 2)
    {
        //std::string twoString = std::to_string((allCodeLines[currentLine][placeInLine] + allCodeLines[currentLine][placeInLine + 1]));
        std::string twoString({ allCodeLines[currentLine][placeInLine], allCodeLines[currentLine][placeInLine + 1] });
        if (twoString == "//")
        {
            //We're dealing with a single line comment. The block extends until the end of the current line
            if (debugPrint) std::cout << "Found a single line comment" << std::endl;
            this->blockType = 1;
            this->beginningCharacter = '\n';
            this->endingCharacter = ' ';
            this->beginningCharacterLocation = { currentLine, allCodeLines[currentLine].size() - 1 };
            return;
        }
        else if (twoString == "/*")
        {
            //We're dealing with a multi line comment (although it's possible to have this all
            //on a single line). The block extends until we find the "*/" character combo
            if (debugPrint) std::cout << "Found a multi line comment" << std::endl;
            this->blockType = 2;
            this->beginningCharacter = '*';
            this->endingCharacter = ' ';
            findEndOfMultiLineQuote(allCodeLines, currentLine, placeInLine);
            this->beginningCharacterLocation = { currentLine, placeInLine };
            return;
        }
    }

    //Check to see if the block is a pre-processor command
    if (allCodeLines[currentLine][placeInLine] == '#')
    {
        if (debugPrint) std::cout << "Found a pre-processor command" << std::endl;
        this->blockType = 0;
        this->beginningCharacter = '\n';
        this->endingCharacter = ' ';
        this->beginningCharacterLocation = { currentLine, allCodeLines[currentLine].size() - 1 }; //extends to end of line
        return;
    }
    else if (allCodeLines[currentLine][placeInLine] == ';')
    {
        if (debugPrint) std::cout << "Found a standalone semi-colon" << std::endl;
        this->blockType = 3;
        this->beginningCharacter = ';';
        this->endingCharacter = ' ';
        this->beginningCharacterLocation = { currentLine, placeInLine }; //extends to end of line
        return;
    }

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
                this->blockType = 5;
                this->beginningCharacter = '{';
                this->endingCharacter = '}';
                this->beginningCharacterLocation = { scannedLine, location };
                return;
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
    else if (contains(types, firstWord) || contains(classes, firstWord))
    {
        //If the first word of the line is a class or built-in type it means one of two things.
        //Either we're about to define a function, i.e. void func() {}, or we're about to define
        //some variables, i.e. int x = 5, y = 6; It will depend on whether we first find an '=',
        //a ';', or a '(' character. If we encounter an '=' of ';' first then the line is for defining variables.

        bool functionFound = false;
        //iterate until we hit one of these characters
        while (true)
        {
            location++;
            if (location == allCodeLines[currentLine].size())
            {
                location = 0;
                currentLine++;
            }

            if (!functionFound)
            {
                if (allCodeLines[currentLine][location] == '=' || allCodeLines[currentLine][location] == ';')
                {
                    if (debugPrint) std::cout << "Found a variable definition block" << std::endl;
                    this->blockType = 3;
                    this->beginningCharacter = ';';
                    this->endingCharacter = ' ';
                    findFirstNonQuoteCharacter(allCodeLines, currentLine, location, ';');
                    this->beginningCharacterLocation = { currentLine, location };
                    return; //this is considered a standard line of code
                }
                else if (allCodeLines[currentLine][location] == '(') functionFound = true;
            }
            else
            {
                //we've either found a function definition, or a function declaration
                if (allCodeLines[currentLine][location] == '{')
                {
                    if (debugPrint) std::cout << "Found a function definition block" << std::endl;
                    this->blockType = 6;
                    this->beginningCharacter = '{';
                    this->endingCharacter = '}';
                    this->beginningCharacterLocation = { currentLine, location };
                    return;
                }
                else if (allCodeLines[currentLine][location] == ';')
                {
                    if (debugPrint) std::cout << "Found a function declaration block" << std::endl;
                    this->blockType = 3;
                    this->beginningCharacter = ';';
                    this->endingCharacter = ' ';
                    this->beginningCharacterLocation = { currentLine, location };
                    return;
                }
            }
        }
    }
    else if (contains(objects, firstWord))
    {
        if (debugPrint) std::cout << "Found a class/struct/enum definition block" << std::endl;
        this->blockType = 6;
        this->beginningCharacter = '{';
        this->endingCharacter = '}';
        findFirstNonQuoteCharacter(allCodeLines, currentLine, location, '{');
        this->beginningCharacterLocation = { currentLine, location };
        return;
    }
    else if (contains(scopes, firstWord))
    {
        //For my purposes I don't need to actually make this a block, treat it as a standard line
        //but have it end with the \n character instead of the ; character
        if (debugPrint) std::cout << "Found a scope block" << std::endl;
        this->blockType = 3;
        this->beginningCharacter = '\n';
        this->endingCharacter = ' ';
        this->beginningCharacterLocation = { currentLine, location };
        return;
    }
    else if (contains(keywords, firstWord))
    {
        //we've found a word like return
        if (debugPrint) std::cout << "Found a keyword block of code" << std::endl;
        this->blockType = 3;
        this->beginningCharacter = ';';
        this->endingCharacter = ' ';
        this->beginningCharacterLocation = { currentLine, location };
        return;
    }

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
    //what kind of block it is.
    CodeBlock* codeBlock = new CodeBlock();
    codeBlock->determineBlockType(allCodeLines, currentLineNumber, placeInLine);
    //if (debugPrint) std::cout << allCodeLines[currentLineNumber] << std::endl;

    //No matter what kind of code block we're looking at here, it will have something that signifies the end of the beginning
    //of the block. For an example, a function definition will have the form returnType functionName(parameters) { internal code }.
    //The beggining of the block is everything up until the first "{" symbol. It's not possible for another block of code to 
    //begin before the current code block's beginning has finshed, so we iterate character by character until the beginning is complete.
    char endingCharacter = codeBlock->getBeginningEndCharacter();
    char currentCharacter;
    std::string currentLine = "";
    //bool beginning = true;

    //multi-line comments can be a little strange so to make things easier they are initialized a little differently
    /*if (codeBlock->blockType == 2)
    {
        currentLine = "/*";
        placeInLine += 2;
    }
    int DEBUGGGY = 0;*/

    while (currentLineNumber < allCodeLines.size())
    {
        while (placeInLine < allCodeLines[currentLineNumber].size())
        {
            currentCharacter = allCodeLines[currentLineNumber][placeInLine]; //update the current character
            //if (currentCharacter == endingCharacter)
            if (currentLineNumber == codeBlock->beginningCharacterLocation.first && placeInLine == codeBlock->beginningCharacterLocation.second)
            {
                currentLine += currentCharacter;
                placeInLine++;

                //We want to include any white space after our ending character, which will include
                //tabs (/t), spaces (' '), return character (\r) and new line characters (\n)
                while (placeInLine < allCodeLines[currentLineNumber].size())
                {
                    currentCharacter = allCodeLines[currentLineNumber][placeInLine];
                    if (currentCharacter == ' ' || currentCharacter == '\n' || currentCharacter == '\r' || currentCharacter == '\t')
                    {
                        currentLine += currentCharacter;
                        placeInLine++;
                    }
                    else break; //break out of loop after finding a non-white space character
                }

                ////first we add the ending character to the current string and then advance our location by 1
                //if (codeBlock->blockType != 4)
                //{
                //    //the non-curly brace blocks don't include the end character as part of the block but
                //    //everything else does
                //    currentLine += currentCharacter;
                //    placeInLine++;


                //    if (codeBlock->blockType == 2)
                //    {
                //        //The multi-line comment is the only type of block which actually requires two characters
                //        //for the block to end, because of this it needs to be handled separately

                //        //First make sure that we're not looking at the '*' symbol that starts the comment instead
                //        //of the one that ends it

                //        //Make sure that we aren't at the very end of the line before checking the next character
                //        if (allCodeLines[currentLineNumber].size() != placeInLine)
                //        {
                //            //we're not at the end of the line so regardless we add the next character to the current string
                //            currentLine += allCodeLines[currentLineNumber][placeInLine];

                //            if ((allCodeLines[currentLineNumber][placeInLine]) != '/')
                //            {
                //                //the character we've just added isn't enough to end the multi-line comment so we need to 
                //                //just continue on with the current while loop
                //                continue;
                //            }
                //            else placeInLine++; //this is the end of the block advance the current position
                //        }
                //        else break; //If we are at the end of the line then break out of the nested while loop which will auto increment to the next line
                //    }

                //    //We want to include any white space after our ending character, which will include
                //    //tabs (/t), spaces (' '), return character (\r) and new line characters (\n)
                //    while (placeInLine < allCodeLines[currentLineNumber].size())
                //    {
                //        currentCharacter = allCodeLines[currentLineNumber][placeInLine];
                //        if (currentCharacter == ' ' || currentCharacter == '\n' || currentCharacter == '\r' || currentCharacter == '\t')
                //        {
                //            currentLine += currentCharacter;
                //            placeInLine++;
                //        }
                //        else break; //break out of loop after finding a non-white space character
                //    }
                //}
                //else
                //{
                //    //the non-bracket blocks are funky, to help define them we store the actual location of 
                //    //their ending character.

                //    //add the ending character
                //    currentLine += currentCharacter;
                //    placeInLine++;

                //    //see if we're actually in the right spot or not
                //    if (currentLineNumber != codeBlock->beginningCharacterLocation.first || placeInLine != (codeBlock->beginningCharacterLocation.second + 1)) continue;

                //    //capture any white space before the next block starts
                //    while (placeInLine < allCodeLines[currentLineNumber].size())
                //    {
                //        currentCharacter = allCodeLines[currentLineNumber][placeInLine];
                //        if (currentCharacter == ' ' || currentCharacter == '\n' || currentCharacter == '\r' || currentCharacter == '\t')
                //        {
                //            currentLine += currentCharacter;
                //            placeInLine++;
                //        }
                //        else break; //break out of loop after finding a non-white space character
                //    }
                //}
                //DEBUGGGY += 1;

                //Check to see if we're at the very end of the current line of code as we'll need to go to the next line
                //the next time we call this function.
                if (placeInLine >= allCodeLines[currentLineNumber].size())
                {
                    currentLineNumber++;
                    placeInLine = 0;
                }

                codeBlock->addBeginningLine(currentLine);
                //currentLine = "";

                //Once we get here, we've completed the beginning of the new code block. Only function/class
                //definitions or if/for type statements will have sub-blocks
                if (codeBlock->blockType < 4)
                {
                    //This code block will only have a beginning section, since it's done we add it to the
                    //current blockArray
                    blockArray.push_back(codeBlock);
                    return;
                }
                else
                {
                    //the code block will have some sub-blocks and potentially a separate end section so
                    //we recursively call this function with the sub-block array of the current code
                    //block at the current location.
                    endingCharacter = codeBlock->getEndingEndCharacter();
                    currentLine = "";

                    while (currentCharacter != endingCharacter)
                    {
                        //the recursive function will automatically increment the currentLineNumber and
                        //placeInLine variables for us, we just need to reassign currentCharacter each 
                        //time
                        recursiveCodeBlockCreation(codeBlock->subBlocks, allCodeLines, currentLineNumber, placeInLine);

                        if (currentLine != "") //we have some indentation to add to the front of the sub-block
                        {
                            codeBlock->subBlocks.back()->begginingLines.insert(codeBlock->subBlocks.back()->begginingLines.begin(), currentLine);
                            currentLine = ""; //reset the current line
                        }
                        currentCharacter = allCodeLines[currentLineNumber][placeInLine];

                        //a non-curly brace line of code has at most one sub-block, so add the block and move on
                        if (codeBlock->blockType == 4)
                        {
                            blockArray.push_back(codeBlock);
                            return;
                        }

                        //if the recursive call above has dumped us on a new line that begins with leading spaces,
                        //iterate until we hit the first non-white space character
                        while (placeInLine < allCodeLines[currentLineNumber].size())
                        {
                            currentCharacter = allCodeLines[currentLineNumber][placeInLine];
                            if (currentCharacter == ' ' || currentCharacter == '\r' || currentCharacter == '\t') //omit the \n character here as we want to keep it
                            {
                                if (allCodeLines[currentLineNumber].size() == 1) break; //this is a blank line so we're ok to move on
                                currentLine += currentCharacter;
                                placeInLine++;
                            }
                            else break; //break out of loop after finding a non-white space character
                        }
                    }
                    //we've found our ending character. reset the current line variable, add the ending character
                    //and then keep looping until we've run our of whitespace
                    currentLine += endingCharacter;
                    placeInLine++;

                    while (placeInLine < allCodeLines[currentLineNumber].size())
                    {
                        currentCharacter = allCodeLines[currentLineNumber][placeInLine];
                        if (currentCharacter == ' ' || currentCharacter == '\n' || currentCharacter == '\r' || currentCharacter == '\t')
                        {
                            currentLine += currentCharacter;
                            placeInLine++;
                        }
                        else break; //break out of loop after finding a non-white space character
                    }

                    //if necessary advance to the next line
                    if (placeInLine == allCodeLines[currentLineNumber].size())
                    {
                        currentLineNumber++;
                        placeInLine = 0;
                    }

                    codeBlock->addEndingLine(currentLine);
                    blockArray.push_back(codeBlock);
                    return;
                }

                //if (beginning)
                //{
                //    
                //}
                //else
                //{
                //    //TODO: I don't think this portion of the loop will every actually happen, can probably get rid of it
                //    
                //    //this is the end of the current block, add it to the ending lines and then return from the function
                //    codeBlock->addEndingLine(currentLine);
                //    return;
                //}
                
            }
            else currentLine += allCodeLines[currentLineNumber][placeInLine++]; //add the current character to our string and increment to the next character
        }

        //If we've reached this point it means we're at the end of the current line. We need to add the current line
        //to either the beginning or end portion of the current code block, reset the current line string and then 
        //increment to the beginning of the next line.
        //if (beginning) codeBlock->addBeginningLine(currentLine);
        //else codeBlock->addEndingLine(currentLine);
        codeBlock->addBeginningLine(currentLine);
        currentLine = "";
        currentLineNumber++;
        placeInLine = 0;
    }
}

void codeParse()
{
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

    //for (int i = 0; i < outermostBlocks.size(); i++) outermostBlocks[i]->printBlock();
    //after getting all of the blocks, search for any adjacent single line comment blocks as they 
    //can be combined into a single block for ease.
}