#pragma once

#include <vector>
#include <iostream>
#include <string>


std::vector<std::string> blockwords = { "for", "while", "if", "else" };
std::vector<std::string> keywords = { "continue", "break", "return" };
std::vector<std::string> types = { "short", "int", "long", "float", "double", "void" };
std::vector<std::string> classes = { "vector", "pair", "string"};
std::vector<std::string> objects = { "class", "struct", "enum" };

/*
Code Block types are:
0 - Preprocessor command
1 - Single line comment
2 - Multi-line comment
3 - Standard code line
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

    char getBeginningEndCharacter()
    {
        return this->beginningCharacter;
        //if (this->blockType == 0) return '\n';
        //else if (this->blockType == 1) return '\n';
        //else if (this->blockType == 2) return '*';
        //else if (this->blockType == 3) return ';';
        //else if (this->blockType == 4)
        //{
        //    //basically any letter would do here
        //    return 'a';
        //}
        //else if (this->blockType == 5) return '{';
        //else return '{';
    }
    char getEndingEndCharacter()
    {
        //if (this->blockType == 0) return ' ';
        //else if (this->blockType == 1) return ' ';
        //else if (this->blockType == 2) return ' ';
        //else if (this->blockType == 3) return ' ';
        //else if (this->blockType == 4)
        //{
        //    //this one is a little tricky because a block without
        //    //curly braces can either be on one or two lines,
        //    //i.e. else x += 1; vs.
        //    //else
        //    //  x += 1;
        //    //so it will either be a space or the \n character
        //    return ' ';
        //}
        //else if (this->blockType == 5) return '}';
        //else return '}';
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
        else if (line[i] == '<')
        {
            //we're dealing with some type of template class, see if there are any "::" namespace
            //operators so we can ignore them cna just get the class.
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
        }
        else firstWord += line[i];
    }

    return firstWord;
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
    while (currentCharacter == ' ' || currentCharacter == '\t' || currentCharacter == '\n' || currentCharacter == '\r') currentCharacter = allCodeLines[currentLine][++placeInLine];

    //Check to see if current block type is a comment, whether it's a single or multi-line comment
    //at least two characters are needed to initiate the comment
    if ((allCodeLines[currentLine].size() - (placeInLine + 1)) >= 2)
    {
        //std::string twoString = std::to_string((allCodeLines[currentLine][placeInLine] + allCodeLines[currentLine][placeInLine + 1]));
        std::string twoString({ allCodeLines[currentLine][placeInLine], allCodeLines[currentLine][placeInLine + 1] });
        if (twoString == "//")
        {
            //We're dealing with a single line comment. The block extends until we find 
            //the first line of code that doesn't start with //
            //std::cout << "Found a single line comment" << std::endl;
            this->blockType = 1;
            this->beginningCharacter = '\n';
            this->endingCharacter = ' ';
            return;
        }
        else if (twoString == "/*")
        {
            //We're dealing with a multi line comment (although it's possible to have this all
            //on a single line). The block extends until we find the "*/" character combo
            //std::cout << "Found a multi line comment" << std::endl;
            this->blockType = 2;
            this->beginningCharacter = '*';
            this->endingCharacter = ' ';
            return;
        }
    }

    //Check to see if the block is a pre-processor command
    if (allCodeLines[currentLine][placeInLine] == '#')
    {
        //std::cout << "Found a pre-processor command" << std::endl;
        this->blockType = 0;
        this->beginningCharacter = '\n';
        this->endingCharacter = ' ';
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
        //I so, or, if the word isn't "else" we increment location until we find the first ')'
        //character.
        if (firstWord == "else")
        {
            if (getFirstWord(allCodeLines[currentLine], location + 2) == "if")
            {
                while (allCodeLines[currentLine][location] != ')')
                {
                    location++;
                    if (location == allCodeLines[currentLine].size())
                    {
                        location = 0;
                        currentLine++;
                    }
                }
            }
        }
        else
        {
            while (allCodeLines[currentLine][location] != ')')
            {
                location++;
                if (location == allCodeLines[currentLine].size())
                {
                    location = 0;
                    currentLine++;
                }
            }
        }

        //At this point, the location variable will be on a ')' character, or 
        //an 'e' character. In both cases, if the next non-whitespace character is a
        //'{' character then we have a curly brace block, if it's anything else then
        //we have a non-curlybrace block.
        char currentCharacter;
        while (true)
        {
            location++;
            if (location == allCodeLines[currentLine].size())
            {
                location = 0;
                currentLine++;
            }
            
            currentCharacter = allCodeLines[currentLine][location];
            if (currentCharacter == ' ' || currentCharacter == '\t' || currentCharacter == '\n' || currentCharacter == '\r') continue;
            
            if (currentCharacter == '{')
            {
                //std::cout << "Found a curly brace block" << std::endl;
                this->blockType = 5;
                this->beginningCharacter = '{';
                this->endingCharacter = '}';
                return;
            }
            else 
            {
                //std::cout << "Found a non-curly brace block" << std::endl;
                this->blockType = 4;
                this->beginningCharacter = currentCharacter;
                this->endingCharacter = ' ';
                return;
            }
        }
    }
    else if (contains(types, firstWord) || contains(classes, firstWord))
    {
        //If the first word of the line is a class or built-in type it means one of two things.
        //Either we're about to define a function, i.e. void func() {}, or we're about to define
        //some variables, i.e. int x = 5, y = 6; It will depend on whether we first find an '='
        //or a '{' character. If we encounter an '=' first then the line is for defining variables.

        //iterate until we hit one of these characters
        while (true)
        {
            location++;
            if (location == allCodeLines[currentLine].size())
            {
                location = 0;
                currentLine++;
            }

            if (allCodeLines[currentLine][location] == '=')
            {
                //std::cout << "Found a variable definition block" << std::endl;
                this->blockType = 3;
                this->beginningCharacter = ';';
                this->endingCharacter = ' ';
                return; //this is considered a standard line of code
            }
            else if (allCodeLines[currentLine][location] == '{')
            {
                //std::cout << "Found a function definition block" << std::endl;
                this->blockType = 6;
                this->beginningCharacter = '{';
                this->endingCharacter = '}';
                return;
            }
        }
    }
    else if (contains(objects, firstWord))
    {
        //std::cout << "Found a class/struct/enum definition block" << std::endl;
        this->blockType = 6;
        this->beginningCharacter = '{';
        this->endingCharacter = '}';
        return;
    }

    //If we can't determine what the block is then it must just be a standard block
    //i.e. x += 10; where x is an already predefined variable
    //std::cout << "Found a standard block of code" << std::endl;
    this->blockType = 3;
    this->beginningCharacter = ';';
    this->endingCharacter = ' ';
    return;
}

void recursiveCodeBlockCreation(std::vector<CodeBlock*>& blockArray, std::vector<std::string>& allCodeLines, int& currentLineNumber, int& placeInLine)
{
    //We're always going to be starting at a new block when this function gets called, the first step is to determine
    //what kind of block it is.
    CodeBlock* codeBlock = new CodeBlock();
    codeBlock->determineBlockType(allCodeLines, currentLineNumber, placeInLine);

    //No matter what kind of code block we're looking at here, it will have something that signifies the end of the beginning
    //of the block. For an example, a function definition will have the form returnType functionName(parameters) { internal code }.
    //The beggining of the block is everything up until the first "{" symbol. It's not possible for another block of code to 
    //begin before the current code block's beginning has finshed, so we iterate character by character until the beginning is complete.
    char endingCharacter = codeBlock->getBeginningEndCharacter();
    char currentCharacter;
    std::string currentLine = "";
    bool beginning = true;

    //multi-line comments can be a little strange so to make things easier they are initialized a little differently
    if (codeBlock->blockType == 2)
    {
        currentLine = "/*";
        placeInLine += 2;
    }
    int DEBUGGGY = 0;

    while (currentLineNumber < allCodeLines.size())
    {
        while (placeInLine < allCodeLines[currentLineNumber].size())
        {
            currentCharacter = allCodeLines[currentLineNumber][placeInLine]; //update the current character
            if (currentCharacter == endingCharacter)
            {
                //first we add the ending character to the current string and then advance our location by 1
                if (codeBlock->blockType != 4)
                {
                    //the non-curly brace blocks don't include the end character as part of the block but
                    //everything else does
                    currentLine += currentCharacter;
                    placeInLine++;

                    if (codeBlock->blockType == 2)
                    {
                        //The multi-line comment is the only type of block which actually requires two characters
                        //for the block to end, because of this it needs to be handled separately

                        //First make sure that we're not looking at the '*' symbol that starts the comment instead
                        //of the one that ends it

                        //Make sure that we aren't at the very end of the line before checking the next character
                        if (allCodeLines[currentLineNumber].size() != placeInLine)
                        {
                            //we're not at the end of the line so regardless we add the next character to the current string
                            currentLine += allCodeLines[currentLineNumber][placeInLine];

                            if ((allCodeLines[currentLineNumber][placeInLine]) != '/')
                            {
                                //the character we've just added isn't enough to end the multi-line comment so we need to 
                                //just continue on with the current while loop
                                continue;
                            }
                            else placeInLine++; //this is the end of the block advance the current position
                        }
                        else break; //If we are at the end of the line then break out of the nested while loop which will auto increment to the next line
                    }

                    //We want to include any white space after our ending character, which will include
                    //tabs (/t), spaces (' '), return character (\r) and new line characters (\n)
                    while (placeInLine < allCodeLines[currentLineNumber].size())
                    {
                        currentCharacter = allCodeLines[currentLineNumber][placeInLine++];
                        if (currentCharacter == ' ' || currentCharacter == '\n' || currentCharacter == '\r' || currentCharacter == '\t')
                        {
                            currentLine += currentCharacter;
                        }
                        else break; //break out of loop after finding a non-white space character
                    }
                }
                DEBUGGGY += 1;

                //Check to see if we're at the very end of the current line of code as we'll need to go to the next line
                //the next time we call this function.
                if (placeInLine >= allCodeLines[currentLineNumber].size())
                {
                    currentLineNumber++;
                    placeInLine = 0;
                }

                if (beginning)
                {
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
                        while (currentCharacter != endingCharacter)
                        {
                            //the recursive function will automatically increment the currentLineNumber and
                            //placeInLine variables for us, we just need to reassign currentCharacter each 
                            //time
                            recursiveCodeBlockCreation(codeBlock->subBlocks, allCodeLines, currentLineNumber, placeInLine);
                            currentCharacter = allCodeLines[currentLineNumber][placeInLine];

                            //a non-curly brace line of code has at most one sub-block
                            if (codeBlock->blockType == 4) return;
                        }
                        //we've found our ending character. reset the current line variable, add the ending character
                        //and then keep looping until we've run our of whitespace
                        currentLine = endingCharacter;
                        placeInLine++;

                        while (placeInLine < allCodeLines[currentLineNumber].size())
                        {
                            currentCharacter = allCodeLines[currentLineNumber][placeInLine++];
                            if (currentCharacter == ' ' || currentCharacter == '\n' || currentCharacter == '\r' || currentCharacter == '\t')
                            {
                                currentLine += currentCharacter;
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
                }
                else
                {
                    //TODO: I don't think this portion of the loop will every actually happen, can probably get rid of it
                    
                    //this is the end of the current block, add it to the ending lines and then return from the function
                    codeBlock->addEndingLine(currentLine);
                    return;
                }
                
            }
            else currentLine += allCodeLines[currentLineNumber][placeInLine++]; //add the current character to our string and increment to the next character
        }

        //If we've reached this point it means we're at the end of the current line. We need to add the current line
        //to either the beginning or end portion of the current code block, reset the current line string and then 
        //increment to the beginning of the next line.
        if (beginning) codeBlock->addBeginningLine(currentLine);
        else codeBlock->addEndingLine(currentLine);
        currentLine = "";
        currentLineNumber++;
        placeInLine = 0;
    }
}

void codeParse()
{
    std::string sampleCode =
        "#include <iostream>\n"
        "#include <vector>\n\n"
        "/*Adding in a multi-line comment\n"
        "it spans two lines!* here's the real end*/\n\n"
        "std::nameyspace::vector<int> vectorAdd(std::vector<int> &v, int x)\n"
        "{\n"
        "    v.push_back(x);\n"
        "}\n\n"
        "int main()\n"
        "{\n"
        "    std::vector<int> vec;\n\n"
        "    for (int x = 0; x < 10; x++)\n"
        "        vectorAdd(vec, x);\n\n"
        "    std::cout << vec[0] << \", \"\n"
        "        << vec[1] << std::endl;\n\n"
        "   return 0;\n"
        "}";

    //std::cout << sampleCode << std::endl;

    std::vector<std::string> linesOfCode;
    std::string currentLine = "";

    for (int i = 0; i < sampleCode.size(); i++) {
        //check to see if we've hit a \n character
        if (sampleCode[i] == '\n')
        {
            //gather as many additional \n characters as there
            //are for this single line
            for (int j = i; j < sampleCode.size(); j++)
            {
                if (sampleCode[j] != '\n') {
                    //add the current line to the vector and change the
                    //i variable for the outer loop
                    linesOfCode.push_back(currentLine);
                    currentLine = "";
                    i = j - 1; //I get's incremented automatically by loop so go back a space
                    break;
                }
                else currentLine += sampleCode[i];
            }
        }
        else currentLine += sampleCode[i];
    }
    linesOfCode.push_back(currentLine); //need to add the final line after breaking loop

    std::vector<CodeBlock*> outermostBlocks;
    int currentLineNumber = 0, placeInLine = 0;

    while (currentLineNumber < linesOfCode.size()) recursiveCodeBlockCreation(outermostBlocks, linesOfCode, currentLineNumber, placeInLine);

    for (int i = 0; i < outermostBlocks.size(); i++) outermostBlocks[i]->printBlock();
    //after getting all of the blocks, search for any adjacent single line comment blocks as they 
    //can be combined into a single block for ease.
}