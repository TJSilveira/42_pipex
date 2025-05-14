# Basic test with two commands
./pipex ./tests/test1.txt "grep inet" "wc -l" output1.txt
<./tests/test1.txt grep inet | wc -l > output1_sh.txt

# Test with three commands
./pipex ./tests/test1 "grep as" "sort" "uniq" output2.txt
echo "Please provide 5 arguments"

# Test with more complex commands
./pipex ./tests/test1 "cat -e" "grep a" output3.txt
<./tests/test1 cat -e | grep a > output3_sh.txt

# Test with command that contains quotes
./pipex ./tests/test1 "echo \"Hello World\"" "wc -w" output4.txt
<./tests/test1 echo \"Hello World\" | wc -w > output4_sh.txt

# Test with a command that might fail
./pipex ./tests/test1 "ls non_existent_file" "wc -l" output5.txt
<./tests/test1 ls non_existent_file | wc -l > output5_sh.txt

# Test that fails with one command
./pipex ./tests/test1.txt "grep inet" output6.txt
echo "Please provide 5 arguments"

# Test with non-existent input file
#./pipex non_existent_file.txt "cat" "wc -l" output6.txt
#<non_existent_file.txt cat | wc -l > output6_sh.txt

# Test with a command that reads from stdin
#./pipex input.txt "cat" "grep pattern" output7.txt