# AST_based_code_copykiller

clang based code-copykiller program using BFS and LCS 

data preprocessing
$ clang++ -cc1 -ast-dump hello.cpp > ast.txt
$ sed -i'' -e 's/\|-/`-/g' ast.txt
$ sed -i'' -e 's/\|//g' ast.txt 
$ sed -i'' -e 's/\`-/\\/g' ast.txt

input: 가공된 ast.txt