import urllib.request, urllib.parse, urllib.error


class htmlNode:
    def __init__(self, tagData, innerHTML, parent):
        self.tagData = tagData
        self.innerHTML = innerHTML
        self.parent = parent
        self.childs = []
    def insertChild(self, childElement):
        if len(self.tagData) > 0:
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
    for i in range(len(stringLine)):
        stringLine[i] = stringLine[i][2:len(stringLine[i])-3]

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

    
    def checkTwoQuotes(taglist):
        valueAnswer = ""
        no = 0
        for lines in taglist:
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
            tagData[allSpaceWords[i-1]] = checkTwoQuotes(allSpaceWords[i+1])

    return tagData


def tagDefiners(allBlock):
    onGoingBerth = ["None"]
    selfClosingTags = ['area', 'base', 'br', 'col', 'embed', 'hr', 'img', 'input', 'link', 'meta', 'param', 'source', 'track', 'wbr', '!DOCTYPE', 'circle', '!--']
    for items in allBlock[1:]:
        tagElement = extractInfofromTag(items)
        if tagElement == "Notag":
            if onGoingBerth[-1] != "None":
                onGoingBerth[-1].innerHTML = onGoingBerth[-1].innerHTML + items # type: ignore
        else:
            if onGoingBerth[-1] != "None":   
                if tagElement['tagname'] == '/'+onGoingBerth[-1].tagData['tagname']: # type: ignore
                    head = onGoingBerth.pop()
                    if onGoingBerth[-1] != "None":
                        head.parent = onGoingBerth[-1] # type: ignore
                        onGoingBerth[-1].insertChild(head) # type: ignore
                else:
                    if tagElement['tagname'][0] == '/':
                        head = onGoingBerth.pop()
                        head.tagData['Error'] = 1 # type: ignore
                        onGoingBerth[-1].insertChild(head) # type: ignore
                    else:
                        node = htmlNode(tagElement, "", onGoingBerth[-1])
                        if tagElement['tagname'] not in selfClosingTags:
                            onGoingBerth.append(node) # type: ignore
                        else:
                            if tagElement['tagname'] != '!--':
                                node.parent = onGoingBerth[-1]
                                onGoingBerth[-1].insertChild(node) # type: ignore
            else:
                node = htmlNode(tagElement, "", onGoingBerth[-1])
                if tagElement['tagname'] not in selfClosingTags:
                        onGoingBerth.append(node) # type: ignore
                else:
                    if tagElement['tagname'] != '!-- ':
                        node.parent = onGoingBerth[-1]

    return head

htmlArray = readingFromLocal('C:/Users/Harsh/Desktop/Parser/a.html')
# htmlArray = liveHtmlCode('http://127.0.0.1:5500/index.html')
# print(htmlArray)
allBlocks = identifyTags(htmlArray)
a = tagDefiners(allBlocks)
printTree(a, 0)
 # type: ignore