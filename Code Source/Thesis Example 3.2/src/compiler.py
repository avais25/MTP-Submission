import json

# opening the flie
outFile = open("giotto_output2.h", "w")
inpFile = open("giotto.json", "r")


tx = ["#include \"giotto_input.h\"\n\n"]
# tx = []

# reading giotto.json as dict
inpJson = inpFile.read()
inpDict = json.loads(inpJson)

sensorPorts = []

for i in inpDict["sensor"]:
    tx.append(i["type"] + " " + i["name"] + ";\n")
    sensorPorts.append(i["name"])

for i in inpDict["actuator"]:
    tx.append(i["type"] + " " + i["name"] + "=" + i["init"] + ";\n")

for i in inpDict["input"]:
    tx.append(i["type"] + " " + i["name"] + ";\n")

for i in inpDict["output"]:
    tx.append(i["type"] + " " + i["name"] + "=" + i["init"] + ";\n")
    # tx.append(i["type"] + " t_" + i["name"] + "=" + i["init"] + ";\n")

for i in inpDict["private"]:
    tx.append(i["type"] + " " + i["name"] + "=" + i["init"] + ";\n")


for i in inpDict["task"]:
    st=""
    tx.append("\nvoid "+i["name"] + "(){\n" + i["function"] + "(")
    for i2 in i["input"]:
        st = st + i2 + ","
    for i2 in i["output"]:
        st = st + "&" + i2 + ","
    st = st[:-1]
    st=st+");\n}\n"
    tx.append(st) 

# for i in inpDict["task"]:
#     tx.append("\nvoid "+i["name"] + "_update(){\n")
#     for i2 in i["output"]:
#         tx.append(i2+"=t_"+i2+";") 
#     tx.append("\n}\n")

for i in inpDict["driver"]:
    st=""
    tx.append("\nvoid "+i["name"] + "(){\n" + i["function"] + "(")
    for i2 in i["input"]:
        if(i2 in sensorPorts):
            st = st + "&" + i2 + ","
        else:
            st = st + i2 + ","
    for i2 in i["output"]:
        st = st + "&" + i2 + ","
    st = st[:-1]
    st=st+");\n}\n"
    tx.append(st) 
    
outFile.writelines(tx)
# outFile.write(json.dumps(inpDict))
# print(sensorPorts)
# closing the file
outFile.close()
inpFile.close()
