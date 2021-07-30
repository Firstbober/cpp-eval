import sys
import re
import clang.cindex

eval_count = 0
top_function_name = ""
top_function_params = {}
top_function_params_types = {}
top_function_extent = ()

output = ""


def walk(node):
    global top_function_name
    global top_function_params
    global top_function_params_types
    global eval_count
    global top_function_extent
    global output

    if node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        top_function_name = node.spelling
        top_function_params[top_function_name] = []
        top_function_params_types[top_function_name] = []
        top_function_extent = node.extent

    if node.kind == clang.cindex.CursorKind.LAMBDA_EXPR:
        return

    if node.kind == clang.cindex.CursorKind.PARM_DECL:
        if top_function_name in top_function_params.keys():
            top_function_params[top_function_name].append(node.spelling)
            top_function_params_types[top_function_name].append(node.type.spelling)

    if node.kind == clang.cindex.CursorKind.CALL_EXPR:
        if node.spelling == "eval":
            entire_call = data[node.extent.start.offset:node.extent.end.offset]

            varied_name = "__sys__eval_init_handle"

            new_call = entire_call.split(
                "eval")[0] + varied_name + "eval".join(entire_call.split("eval")[1:])

            new_call = new_call.removeprefix("std::")

            place_to_insert = "@@@__EVALCODEGEN_" + str(eval_count) + "__@@@"
            code_with_place = data.replace(entire_call + ";", place_to_insert)

            tmp_last = new_call[-1:]
            new_call = new_call[:-1] + ",\"" + code_with_place.replace("\\", "\\\\").replace("\n", "\\n").replace(
                "\"", "\\\"").replace("\'", "\\'") + "\",\"" + place_to_insert + "\""
            new_call += tmp_last

            new_call += ";\n__sys__eval_init_fncdef(CPPEVAL_CODEGEN_FUNCNAME_" + str(eval_count) + ",\"" + top_function_name + "\""

            if len(top_function_params_types[top_function_name]) > 0:
                new_call += ","
                new_call += ','.join(top_function_params_types[top_function_name])

            new_call += ");\n__sys__eval_run(CPPEVAL_CODEGEN_FUNCNAME_" + str(eval_count)

            if len(top_function_params[top_function_name]) > 0:
                new_call += ","
                new_call += ','.join(top_function_params[top_function_name])

            new_call += ")"

            output = output.replace(entire_call, new_call)

            """

                        if len(top_function_params[top_function_name]) > 0:
                new_call += ","
                new_call += ','.join(top_function_params[top_function_name])

            print("Parent:", top_function_name)

            print('name: %s, type: %s' %
                  (node.spelling or node.displayname, node.type.spelling))
            print("X:", node.displayname)
            print("L:", node.location)
            print("E:", node.extent)
            """
            eval_count += 1

    for c in node.get_children():
        walk(c)


with open(sys.argv[1], 'r') as file:
    data = file.read()

    output = data

    index = clang.cindex.Index.create()
    walk(index.parse(sys.argv[1], args=['-x', 'c++']).cursor)

    """
    for line in data.split("\n"):
        if "std::eval(" in line and not "//" in line:
            data.replace()

            print(line)
    """
    """
    positions = [m.start() for m in re.finditer("std::eval", data)]

    for position in positions:
        
    print(data[positions[0]+9])
    """


f = open(sys.argv[2], "w")
f.write(output)
f.close()
