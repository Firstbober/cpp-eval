import sys

with open(sys.argv[1], 'r') as file:
	data = file.read()

	f = open(sys.argv[2], "w")
	f.write(data.replace("@@@_PREGEN_CPPEVALH_@@@", data.replace("\\", "\\\\").replace("\n", "\\n").replace(
                "\"", "\\\"").replace("\'", "\\'")))
	f.close()
