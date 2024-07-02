import urllib.request, urllib.parse, urllib.error


class htmlNode:
    def __init__(self, tagData: object, innerHTML: str, parent: object):
        self.tagData = tagData
        self.innerHTML = innerHTML
        self.parent = parent
        self.childs = []
    def insertChild(self, childElement):
        if len(self.tagData) > 0: # type: ignore
            self.childs.append(childElement)


def printTree(htmlNode, i):
    #if htmlNode.parent != "None":
    print(i*"------>" , htmlNode.tagData['tagname'])
    i+=1
    for node in htmlNode.childs:
        printTree(node, i)


def liveHtmlCode(url):
    url = str(url)
    web_file = urllib.request.urlopen(url)
    stringLine = []
    for line in web_file:
        stringLine.append(str(line))
    for i in range(len(stringLine) - 1):
        stringLine[i] = stringLine[i][2:len(stringLine[i])-3]
    stringLine[-1] = stringLine[-1][2:len(stringLine[-1])-1]

    return stringLine


def readingFromLocal(url):
    file = str(url)
    file = open(file, "r+")
    stringLine = []
    for line in file:
        stringLine.append(str(line))   
            
    return stringLine


def identifyTags(htmlCode):
    tagData = ""
    blockHtml = []
    startTag = False
    for line in htmlCode:
        for i in range(len(line)):
            if startTag == True:
                tagData += line[i]
                if line[i] == '>':
                    startTag = False
                    blockHtml.append(tagData)
                    tagData = ""
            else:
                if line[i] == '<':
                    startTag = True
                    blockHtml.append(tagData)
                    tagData = '<'
                else:
                    tagData += line[i]
    return blockHtml


def extractInfofromTag(line):
    if len(line) == 0 or  line[0] != '<':
        return "Notag"
    

    def addspaceonEqual(sentence):
        sentence = sentence.replace("=", " = ")
        sentence = sentence.replace(">", "")
        sentence = sentence.replace("!--", "!-- ")
        return sentence

    
    def checkTwoQuotes(taglist, i):
        valueAnswer = ""
        no = 0
        for j in range(i, len(taglist)):
            lines = taglist[j]
            if no == 2:
                break
            for i in range(len(lines)):
                if lines[i] == '"':
                    no+=1
                else:
                    valueAnswer += lines[i]
                if no == 2:
                    break
            
        return valueAnswer


    line = line[1:]
    line = line.lstrip()
    line = addspaceonEqual(line)
    allSpaceWords = line.split()
    tagData = {}
    tagData['tagname'] = allSpaceWords[0]
    allSpaceWords.pop(0)
    for i in range(len(allSpaceWords)): 
        if allSpaceWords[i] == "=":
            tagData[allSpaceWords[i-1]] = checkTwoQuotes(allSpaceWords, i+1)

    return tagData


def tagDefiners(allBlock):
    onGoingBerth = ["None"]
    scriptrun = False
    selfClosingTags = ['area', 'base', 'br', 'col', 'embed', 'hr', 'img', 'input', 'link', 'meta', 'param', 'source', 'track', 'wbr', '!DOCTYPE', 'circle', '!--']
    for items in allBlock[1:]: # reading each tag separated by allblocks
        tagElement = extractInfofromTag(items) # storing each tags data
        if tagElement == "Notag": # if no tag then its innercontent only
            if onGoingBerth[-1] != "None":
                onGoingBerth[-1].innerHTML = onGoingBerth[-1].innerHTML + items # type: ignore
        else: # when an html element is found
            if onGoingBerth[-1] != "None":  # when root is not entered
                if tagElement['tagname'] == '/'+onGoingBerth[-1].tagData['tagname']: # type: ignore
                    # when it's matching close tag
                    scriptrun = False
                    head = onGoingBerth.pop()
                    if onGoingBerth[-1] != "None": # adding to parent and childs
                        head.parent = onGoingBerth[-1] # type: ignore
                        onGoingBerth[-1].insertChild(head) # type: ignore
                    else: # parent is none
                        head.parent = "None" # type: ignore
                else: # not a closing match tag
                    if tagElement['tagname'][0] == '/' and scriptrun == False: # not a match tag but closing tag actually it's a error, curently resolving error
                        head = onGoingBerth.pop()
                        if onGoingBerth[-1] != "None":
                            onGoingBerth[-1].insertChild(head) # type: ignore
                        head.tagData['Error'] = True # type: ignore
                    else: # it's a starting normal tag
                        node = htmlNode(tagElement, "", onGoingBerth[-1])
                        if tagElement['tagname'] not in selfClosingTags and scriptrun == False: # not self closing tag,  most used in new tag encounter
                            onGoingBerth.append(node) # type: ignore
                        else:
                            if tagElement['tagname'] != '!--' and scriptrun == False: # resolving selfclosing tag except the comment section as comment is also considered as self closing
                                node.parent = onGoingBerth[-1]
                                onGoingBerth[-1].insertChild(node) # type: ignore
                        if tagElement['tagname'] == 'script':
                            scriptrun = True
            else: # for entering root i.e. top none
                node = htmlNode(tagElement, "", onGoingBerth[-1])
                if tagElement['tagname'] not in selfClosingTags:
                    if tagElement['tagname'] == 'script':
                        scriptrun = True
                    onGoingBerth.append(node) # type: ignore
                else:
                    if tagElement['tagname'] != '!-- ':
                        node.parent = onGoingBerth[-1]

    return head


def searchTagClass(className, htmlNode):
    if 'class' in htmlNode.tagData.keys():
        if htmlNode.tagData['class'] == className:
            print(htmlNode.tagData, htmlNode.innerHTML)
    for head in htmlNode.childs:
        searchTagClass(className, head)
    

htmlArray = readingFromLocal('C:/Users/Harsh/Dropbox/HTML-Parser/p.html')
# htmlArray = liveHtmlCode('https://www.linkedin.com')
# print(htmlArray)
allBlocks = identifyTags(htmlArray)
# print(allBlocks)
a = tagDefiners(allBlocks)
printTree(a, 0)
searchTagClass('cursor', a)
