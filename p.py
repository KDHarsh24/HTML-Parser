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

    print(i*"------>" , htmlNode.tagData['tagname'])
    i+=1
    for node in htmlNode.childs:
        printTree(node, i)


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
    for items in allBlock[1:]:
        tagElement = extractInfofromTag(items)
        if tagElement == "Notag":
            onGoingBerth[-1].innerHTML = onGoingBerth[-1].innerHTML + items
        else:
            if onGoingBerth[-1] != "None":   
                if tagElement['tagname'] == '/'+onGoingBerth[-1].tagData['tagname']:
                    head = onGoingBerth.pop()
                    if onGoingBerth[-1] != "None":
                        head.parent = onGoingBerth[-1]
                        onGoingBerth[-1].insertChild(head)
                else:
                    node = htmlNode(tagElement, "", onGoingBerth[-1])
                    onGoingBerth.append(node)
            else:
                node = htmlNode(tagElement, "", onGoingBerth[-1])
                onGoingBerth.append(node)

    return head


u = identifyTags(s)
print(u)
a = tagDefiners(u)
printTree(a, 0)
