# Basic test with two commands
valgrind --leak-check=full --show-leak-kinds=all ./pipex ./tests/test1.txt "grep inet" "wc -l" output1.txt > valgrind1.txt 2>&1
<./tests/test1.txt grep inet | wc -l > output1_sh.txt

# Test with three commands
valgrind --leak-check=full --show-leak-kinds=all ./pipex ./tests/test1 "grep as" "sort" "uniq" output2.txt > valgrind2.txt 2>&1
<./tests/test1 grep as | sort | uniq > output2_sh.txt

# Test with more complex commands
valgrind --leak-check=full --show-leak-kinds=all ./pipex ./tests/test1 "cat -e" "grep a" "tr a-z A-Z" output3.txt > valgrind3.txt 2>&1
<./tests/test1 cat -e | grep a | tr a-z A-Z > output3_sh.txt

# Test with command that contains quotes
valgrind --leak-check=full --show-leak-kinds=all ./pipex ./tests/test1 "echo \"Hello World\"" "wc -w" output4.txt > valgrind4.txt 2>&1
<./tests/test1 echo \"Hello World\" | wc -w > output4_sh.txt

# Test with a command that might fail
valgrind --leak-check=full --show-leak-kinds=all ./pipex ./tests/test1 "ls non_existent_file" "wc -l" output5.txt > valgrind5.txt 2>&1

# Test that fails with one command
valgrind --leak-check=full --show-leak-kinds=all ./pipex ./tests/test1.txt "grep inet" output6.txt > valgrind6.txt 2>&1

# Test with non-existent input file
#./pipex non_existent_file.txt "cat" "wc -l" output6.txt
#<non_existent_file.txt cat | wc -l > output6_sh.txt

# Test with a command that reads from stdin
#./pipex input.txt "cat" "grep pattern" output7.txt