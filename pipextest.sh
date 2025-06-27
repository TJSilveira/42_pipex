# Basic test with two commands
NUM_TESTS=1

echo 'TEST' $NUM_TESTS
./pipex ./tests/test1.txt "grep inet" "wc -l" ./output1.txt
status=$?
echo 'Pipex Status' $status
<./tests/test1.txt grep inet | wc -l > output1_sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test with three commands
echo 'TEST' $NUM_TESTS
./pipex ./tests/test1 "grep as" "sort" "uniq" ./output2.txt
status=$?
echo 'Pipex Status' $status
echo "Expectd: Please provide 5 arguments"
echo "Expected Status: 1"
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test with more complex commands
echo 'TEST' $NUM_TESTS
./pipex ./tests/test1 "cat -e" "grep a" ./output3.txt
status=$?
echo 'Pipex Status' $status
<./tests/test1 cat -e | grep a > output3_sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test with command that contains quotes
echo 'TEST' $NUM_TESTS
./pipex ./tests/test1 "echo \"Hello World\"" "wc -w" ./output4.txt
status=$?
echo 'Pipex Status' $status
<./tests/test1 echo \"Hello World\" | wc -w > output4_sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test with a command that might fail
echo 'TEST' $NUM_TESTS
./pipex ./tests/test1 "ls non_existent_file" "wc -l" ./output5.txt
status=$?
echo 'Pipex Status' $status
<./tests/test1 ls non_existent_file | wc -l > output5_sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test that fails with one command
echo 'TEST' $NUM_TESTS
./pipex ./tests/test1.txt "grep inet" ./output6.txt
echo "Expected: Please provide 5 arguments"
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test that fails from nonexisting command
echo 'TEST' $NUM_TESTS
./pipex ./tests/test1 "cat" "notexisting" ./output7.txt
status=$?
echo 'Pipex Status' $status 
< ./tests/test1 cat | notexisting >./output7.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test that fails from nonexisting input file
echo 'TEST' $NUM_TESTS
./pipex ./tests/nonexisting "grep inet" "wc -l" ./output7.txt
status=$?
echo 'Pipex Status' $status 
< ./tests/nonexisting grep inet | wc -l >./output7.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test with non-existent input file
#./pipex non_existent_file.txt "cat" "wc -l" output6.txt
#<non_existent_file.txt cat | wc -l > output6_sh.txt

# Test with a command that reads from stdin
#./pipex input.txt "cat" "grep pattern" output7.txt
