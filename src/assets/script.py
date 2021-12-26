import sys

orig = open(sys.argv[1], "r")
txt = orig.read()
orig.close()

lines = txt.split("\n")
txt_write = ""
for line in lines:
    line = line.replace("/", "-")
    txt_write += line + "\n"
txt_write = txt_write[:-1]

new_file = open(sys.argv[1], "w")
new_file.write(txt_write)
new_file.close()

