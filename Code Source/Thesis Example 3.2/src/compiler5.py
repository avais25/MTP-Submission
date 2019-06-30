import json

#  opening the flie
inpFile = open("schedule_specification.txt", "r")
outFile = open("main.c", "w")
giotto  = open("giotto.json", "r")

# reading giotto.json as dict
inpJson = giotto.read()
inpDict = json.loads(inpJson)

# to store code of the main file
mainx = []

# dictionay to store processed schedule
schedule = {}


def tick(x):
    y = int(float(14745600/64) * float(x/1000))
    return y

satisfy = inpFile.readline()
if(satisfy == "sat\n"):
    print("Satisfied")


    constraints = inpFile.readlines()

    # mainx.append("#include \"fblib/firebird.h\" \n")
    # mainx.append("#include \"giotto_input.c\" \n")
    mainx.append("#include \"giotto_output2.h\" \n")
    mainx.append("volatile unsigned int schedule = 0; \n")
    mainx.append("volatile unsigned int isr_schedule = 0; \n")
    mainx.append("int main(int argc, char *argv[]) { \n")
    mainx.append("init_devices(); \n")
    mainx.append("TCCR4A = 0 ;\n")
    mainx.append("TCCR4B = (1 << CS00) | (1 << CS01); \n")
    mainx.append("TIMSK4 = (1 << OCIE4A); \n")
    


    constraints[-1] += ","
    # print(constraints[-1])

    for i in constraints:
        i=i[1:]
        i=i[:-2]

        split1 = i.split("=")
        # print(split1)
        split2 = split1[1].split("/")
        if(len(split2) > 1):
            # print(split2)
            schedule.update({split1[0].strip() : float(split2[0])/float(split2[1])})
        else:
            schedule.update({split1[0].strip() : float(split1[1])})
        
    sortedSchedule = sorted(schedule , key=schedule.get)

    
    interruptDict = {}

    count = 0

    for i in sortedSchedule:

        splitList = i.split("_")
        splitList = splitList[:-1]

        # i2 stores the function name without the instance number
        i2=splitList[0]

        for j in splitList[1:]:
            i2 = i2 + "_" + j

        
        for j in inpDict["mode"][0]["definition"]:
            if i2 == j["driver"] and (j["type"] == "sensor" or j["type"] == "actuator"):
                interruptDict.update({str(count) : tick(schedule[i])+100})
                count = count + 1
                print(i2, schedule[i])
        interruptDict.update({str(count) : tick(float(inpDict["mode"][0]["period"]))+100})
    
    # we need to specify first schdule now
    if interruptDict['0'] == 100:
        # del interruptDict['0']
        firstInt = interruptDict['1']
    else:
        firstInt = interruptDict['0']

    mainx.append("OCR4A = "+str(firstInt)+"; \n")
    
    mainx.append("schedule=1; \n")
    mainx.append("isr_schedule = 1; \n")
    mainx.append("while(1) { \n")
    mainx.append("switch (schedule) { \n")
    mainx.append("case 0: \n")
     
    caseCount = 1
    # if first thing to be scheduled is not a sensor or actuator
    # we have to make a first case
    if interruptDict['0'] != 100:
        mainx.append("schedule = 0; \n")
        mainx.append("break;\n")
        mainx.append("case "+str(caseCount)+ ": \n")
        caseCount +=1

    for i in sortedSchedule:
        splitList = i.split("_")
        splitList = splitList[:-1]

        # i2 stores the function name without the instance number
        i2=splitList[0]

        for j in splitList[1:]:
            i2 = i2 + "_" + j

        


        for j in inpDict["mode"][0]["definition"]:
            if i2 == j["driver"] and (j["type"] == "sensor" or j["type"] == "actuator"):
                mainx.append("schedule = 0;\n")
                mainx.append("break;\n")
                mainx.append("case "+str(caseCount)+ ": \n")
                caseCount +=1

        
        mainx.append(i2+"();\n")
    

    mainx.append("schedule = 0;\n")
    mainx.append("break;\n")
    # ending the main file
    mainx.append("}} \n")
    mainx.append("return 0; \n")
    mainx.append("} \n")


    # the ISR CODE
    mainx.append("ISR(TIMER4_COMPA_vect) { \n")
    mainx.append("switch (isr_schedule) { \n")
   
    print(interruptDict)
    

    scheduleCount = 2
    if interruptDict['0'] == 100:
        del interruptDict['0']
        del interruptDict['1']
    else:
        del interruptDict['0']


    for i,j in interruptDict.items():
        mainx.append("case " + str(scheduleCount-1)+": \n")
        mainx.append("OCR4A = " + str(j)+"; \n")
        mainx.append("schedule = " + str(scheduleCount) + ";\n")
        mainx.append("isr_schedule = " + str(scheduleCount) + ";\n")
        mainx.append("break; \n")
        scheduleCount += 1

    # the last case
    mainx.append("case " + str(scheduleCount-1)+": \n")
    mainx.append("OCR4A = " + str(firstInt) + "; \n")
    mainx.append("schedule = 1; \n")
    mainx.append("isr_schedule = 1; \n")
    mainx.append("TCNT4 = 100;\n")
    mainx.append("break; \n")
    mainx.append("}} \n")


    outFile.writelines(mainx)

else:
    print("Unsatisfied")