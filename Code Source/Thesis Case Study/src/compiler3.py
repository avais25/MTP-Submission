
# opening the flie
inpFile = open("constraints_specification.txt", "r")
inpFile2 = open("job_list.txt", "r")
outFile = open("compiler4.py", "w")
constraints = inpFile.read().splitlines()
jobList = inpFile2.read().splitlines()


# will contain code for compiler 4
codes = []

codes.append("from z3 import *\n")

for i in jobList:
    codes.append(i + " = Real(\'" + i + "\')\n")


codes.append("s = Solver()\n")

for x in constraints:
    codes.append("s.add("+ x + ")\n")


codes.append("print(s.check())\n")
codes.append("print(s.model())\n")
codes.append("outFile = open(\"schedule_specification.txt\", \"w\")\n")
codes.append("outFile.writelines(str(s.check()) + \"\\n\")\n")
codes.append("outFile.writelines(str(s.model()))\n")
outFile.writelines(codes)