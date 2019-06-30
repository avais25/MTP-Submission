import json


# some functions which are used
def gcd(x, y):
    while(y):
        x, y = y, x % y
    return x


def lcm(x, y):
    lcm = (x*y)/gcd(x, y)
    return lcm


# opening the flie
inpFile = open("giotto.json", "r")
outFile = open("constraints_specification.txt", "w")
outFile2 = open("job_list.txt", "w")

# reading giotto.json as dict
inpJson = inpFile.read()
inpDict = json.loads(inpJson)

# constrains specification
cnx = []

# job list
js = []

# jitter tolerance
jitter = int(inpDict["jitter"])

# time period of the mode
modePeriod = int(inpDict["mode"][0]["period"])

# calculating lcm of all the frequency
frequencyMax = 1
for i in inpDict["mode"][0]["definition"]:
    frequencyMax = lcm(frequencyMax, int(i["frequency"]))

# Time at which the giotto micro step must execute
configUpdate = modePeriod/frequencyMax


# finding actuator drivers

# list containing name of all the actuators
actList = []

for i in inpDict["actuator"]:
    actList.append(i["name"])

# list containing name of all the sensors
senList = []

for i in inpDict["sensor"]:
    senList.append(i["name"])

# Dictionary containing all  the task and its wcet
wcetDict = {}

for i in inpDict["mode"][0]["definition"]:
    # adding drivers with wcet
    for j in inpDict["driver"]:
        if j["name"] == i["driver"]:
            for k in range(int(i["frequency"])):
                x = j["name"] + "_" + str(k)
                wcetDict.update({x: float(j["wcet"])})
    # adding task with wcet
    if i["type"] == "task" or i["type"] == "sensor":
        for j in inpDict["task"]:
            if j["name"] == i["task"]:
                for k in range(int(i["frequency"])):
                    x = j["name"] + "_" + str(k)
                    wcetDict.update({x: float(j["wcet"])})
                    # x = j["name"] + "_update_" + str(k)
                    # wcetDict.update({x: float(j["wcet2"])})


# timing constraints for the actuator
for i in inpDict["mode"][0]["definition"]:
    if i["type"] == "actuator":
        for k in range(int(i["frequency"])):
            temp = i["driver"] + "_" + str(k)
            # lower bound
            cnx.append(temp + " >= " +
                       str((modePeriod/int(i["frequency"]))*(k+1) - jitter) + "\n")
            # upper bound
            cnx.append(temp + " + " + str(wcetDict[temp]) + " <= " + str((modePeriod /
                                                                          int(i["frequency"]))*(k+1)) + "\n")

cnx.append("\n")


# timing constraints for sensor/ task input
for i in inpDict["mode"][0]["definition"]:
    if i["type"] == "sensor":
        for k in range(int(i["frequency"])):
            temp = i["driver"] + "_" + str(k)
            # lower bound
            cnx.append(temp + " >= " +
                       str((modePeriod/int(i["frequency"]))*(k)) + "\n")
            # upper bound
            cnx.append(temp + " + " + str(wcetDict[temp]) + " <= " + str((modePeriod /
                                                                          int(i["frequency"]))*(k) + jitter) + "\n")
cnx.append("\n")

# precedence constraints

# task-update dependency
# for i in inpDict["mode"][0]["definition"]:
#     if i["type"] == "task" or i["type"] == "sensor":
#         for j in range(int(i["frequency"])):
#             temp = i["task"] + "_" + str(j)
#             cnx.append(temp + " + " + str(wcetDict[temp]) + " <= " +
#                        i["task"] + "_update_" + str(j) + "\n")

# cnx.append("\n")

#  Precedence between Task and its Input Drive
for i in inpDict["mode"][0]["definition"]:
    if i["type"] == "task" or i["type"] == "sensor":
        for j in range(int(i["frequency"])):
            temp = i["driver"] + "_" + str(j)
            temp2 = i["task"] + "_" + str(j)
            cnx.append(temp + " + " + str(wcetDict[temp]) + " <= " +
                       temp2 + "\n")
            if j != int(i["frequency"])-1:
                cnx.append(temp2 + " + " + str(wcetDict[temp2]) + " <= " + i["driver"] + "_" + str(j+1) + "\n")

cnx.append("\n")

# task instance ordering constraints
for i in inpDict["mode"][0]["definition"]:
    if i["type"] == "task" or i["type"] == "sensor":
        for j in range(int(i["frequency"])-1):
            cnx.append(i["task"] + "_" + str(j) + " < " +
                       i["task"] + "_" + str(j+1) + "\n")
        
            cnx.append(i["driver"] + "_" + str(j) + " < " +
                       i["driver"] + "_" + str(j+1) + "\n")
    if i["type"] == "actuator":
        for j in range(int(i["frequency"])-1):
            cnx.append(i["driver"] + "_" + str(j) + " < " +
                       i["driver"] + "_" + str(j+1) + "\n")

cnx.append("\n")

# actuator-task dependencies
for i in inpDict["mode"][0]["definition"]:
    if i["type"] == "actuator":
        # print("Actuator "+i["driver"])
        for j in inpDict["driver"]:
            if i["driver"] == j["name"]:
                for k in j["input"]:
                    # print("Input "+k)
                    for l in inpDict["task"]:
                        if k in l["output"]:
                            # print("Task "+ l["name"])
                            for m in inpDict["mode"][0]["definition"]:
                                if m["type"] != "actuator" and m["task"] == l["name"]:
                                    # print(l["name"])
                                    # finding the task's instance
                                    actPeriod = int(
                                        modePeriod/int(i["frequency"]))
                                    # print(modePeriod)
                                    for n in range(int(modePeriod/int(i["frequency"])), modePeriod+1, actPeriod):
                                        instance = int(
                                            n/(modePeriod/int(m["frequency"])))-1
                                        temp = m["task"] + "_" + str(instance)
                                        temp2 = i["driver"] + "_" + str(int(n/actPeriod)-1)
                                        if(n != modePeriod):
                                            cnx.append(temp + " + " + str(wcetDict[temp]) + " <= " + i["driver"] + "_" + str(int(n/actPeriod)-1) + "\n")
                                            cnx.append(m["task"] + "_" + str(
                                                instance+1) + " >= " + temp2 + " + " + str(wcetDict[temp2]) +  "\n")
                                        else:
                                            cnx.append(temp + " + " + str(wcetDict[temp]) + " <= " + i["driver"] + "_" + str(int(n/actPeriod)-1) + "\n")

cnx.append("\n")

# task-task dependency

for i in inpDict["driver"]:
    for j in i["input"]:
        for k in inpDict["task"]:
            if j in k["output"]:
                # input of driver i is dependent on task t
                #    print(i["name"]+ "  "+ k["name"])
                for l in i["output"]:
                    for m in inpDict["task"]:
                        if l in m["input"]:
                            # output and input are not the same task
                            if(k["name"] != m["name"]):
                                # print(i["name"])
                                # finding the task instnce
                                # t1 is the input task  forn the drver, t2 is the output task
                                t1 = -1
                                t2 = -1
                                for n in inpDict["mode"][0]["definition"]:
                                    if(n["type"] != "actuator" and n["task"] == k["name"]):
                                        t1 = int(
                                            modePeriod/int(n["frequency"]))
                                        # print(t1)
                                    if(n["type"] != "actuator" and n["task"] == m["name"]):
                                        t2 = int(
                                            modePeriod/int(n["frequency"]))
                                # 1st instance of the task will take the value form previous mode execution
                                for n in range(t2, modePeriod, t2):
                                    temp = k["name"] + "_" + str(int(n/t1)-1)
                                    temp2 =  i["name"] + "_" + str(int(n/t2))
                                    cnx.append(temp + " + " + str(wcetDict[temp]) + " <= " + i["name"] + "_" + str(int(n/t2)) + "\n")
                                    cnx.append(k["name"] + "_" + str(int(n/t1)) +" >= " + temp2 + " + " + str(wcetDict[temp2]) + "\n")
                                # first execution of these tasks
                            
                                cnx.append(i["name"] + "_0" + " + " + str(wcetDict[m["name"]+"_0"]) + " <= " +k["name"] + "_0\n" )


cnx.append("\n")


# default constraints
jobList = []
# adding drivers with wcet
for i in inpDict["mode"][0]["definition"]:
    for j in inpDict["driver"]:
        if j["name"] == i["driver"]:
            for k in range(int(i["frequency"])):
                x = (j["name"] + "_" + str(k), j["wcet"])
                jobList.append(x)
    # adding task with wcet
    if i["type"] == "task" or i["type"] == "sensor":
        for j in inpDict["task"]:
            if j["name"] == i["task"]:
                for k in range(int(i["frequency"])):
                    x = (j["name"] + "_" + str(k), j["wcet"])
                    jobList.append(x)
                    # x = (j["name"] + "_update_" + str(k), j["wcet2"])
                    # jobList.append(x)

for i, j in jobList:
    # writing job_list file
    js.append(i + "\n")
    for p, q in jobList:
        if(p != i):
            cnx.append("Or(" + i + " + " + j + " <= " + p +
                       " , " + p+" + " + q + " <= " + i+")\n")

cnx.append("\n")

# boundry constraints
for i, j in jobList:
    cnx.append(i + " >= 0\n")
    cnx.append(i + " <= " + str(modePeriod) + "\n")


# print(jobList)
outFile.writelines(cnx)
outFile2.writelines(js)
print("Done!")
