//!/bin/bash
// Authored-by: James Trimble <james.trimble@yahoo.co.uk> 
// expanded beyond /algorithms/ by Dan Schult <dschult@colgate.edu>

echo ALGORITHMS
find graphx/algorithms -name "*.py" | xargs cat | tr -d ' \n' | grep -o '__all__=\[ ["a-z0-9_,]*\]' | grep -o '"[a-z0-9_]*"' | tr -d '"' | sort | uniq > tmp_funcs.txt
cat doc/reference/algorithms/*.rst | tr -d ' ' | sort | uniq > tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt > functions_possibly_missing_from_doc.txt
for f in $(cat functions_possibly_missing_from_doc.txt); do echo $f; grep -l -r "def ${f}" graphx/algorithms | grep -v ".pyc" | sed 's/^/  /'; done

echo GENERATORS
find graphx/generators -name "*.py" | xargs cat | tr -d ' \n' | grep -o '__all__=\[ ["a-z0-9_,]*\]' | grep -o '"[a-z0-9_]*"' | tr -d '"' | sort | uniq > tmp_funcs.txt
cat doc/reference/generators.rst | tr -d ' ' | sort | uniq > tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt > functions_possibly_missing_from_doc.txt
for f in $(cat functions_possibly_missing_from_doc.txt); do echo $f; grep -l -r "def ${f}" graphx/generators | grep -v ".pyc" | sed 's/^/  /'; done

echo LINALG
find graphx/linalg -name "*.py" | xargs cat | tr -d ' \n' | grep -o '__all__=\[ ["a-z0-9_,]*\]' | grep -o '"[a-z0-9_]*"' | tr -d '"' | sort | uniq > tmp_funcs.txt
cat doc/reference/linalg.rst | tr -d ' ' | sort | uniq > tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt > functions_possibly_missing_from_doc.txt
for f in $(cat functions_possibly_missing_from_doc.txt); do echo $f; grep -l -r "def ${f}" graphx/linalg | grep -v ".pyc" | sed 's/^/  /'; done

echo CLASSES
find graphx/classes -name "*.py" | xargs cat | tr -d ' \n' | grep -o '__all__=\[ ["a-z0-9_,]*\]' | grep -o '"[a-z0-9_]*"' | tr -d '"' | sort | uniq > tmp_funcs.txt
cat doc/reference/{filters,functions,classes/*}.rst | tr -d ' ' | sort | uniq > tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt > functions_possibly_missing_from_doc.txt
for f in $(cat functions_possibly_missing_from_doc.txt); do echo $f; grep -l -r "def ${f}" graphx/classes | grep -v ".pyc" | sed 's/^/  /'; done

echo READWRITE
find graphx/readwrite -name "*.py" | xargs cat | tr -d ' \n' | grep -o '__all__=\[ ["a-z0-9_,]*\]' | grep -o '"[a-z0-9_]*"' | tr -d '"' | sort | uniq > tmp_funcs.txt
cat doc/reference/readwrite/*.rst | tr -d ' ' | sort | uniq > tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt
comm -2 -3 tmp_funcs.txt tmp_doc.txt > functions_possibly_missing_from_doc.txt
for f in $(cat functions_possibly_missing_from_doc.txt); do echo $f; grep -l -r "def ${f}" graphx/readwrite | grep -v ".pyc" | sed 's/^/  /'; done
