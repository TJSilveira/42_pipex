!./bin/bash
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
<./tests/test1 grep as | sort | uniq > ./output2_sh.txt 
status=$?
echo 'Expected Status' $status
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
< ./tests/test1 cat | notexisting >./output7sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test that fails from nonexisting input file
echo 'TEST' $NUM_TESTS
echo "Information that must be truncated properly" > ./output8.txt
./pipex ./tests/nonexisting "grep inet" "wc -l" ./output8.txt
status=$?
echo 'Pipex Status' $status 
< ./tests/nonexisting grep inet | wc -l >./output8sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test that fails from nonexisting input file
echo 'TEST' $NUM_TESTS
./pipex "nonexistingfile" "cat -e" "ls" ./output9.txt
status=$?
echo 'Pipex Status' $status 
< nonexistingfile cat -e | ls > ./output9sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test that fails from nonexisting input file
echo 'TEST' $NUM_TESTS
./pipex "nonexistingfile" "cat -e" "ls" ./output9.txt
status=$?
echo 'Pipex Status' $status 
< nonexistingfile cat -e | ls > ./output9sh.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))

# Test with output file that has restrictions
echo 'TEST' $NUM_TESTS
./pipex ./tests/test1.txt "grep inet" "wc -l" .test/test2.txt
status=$?
echo 'Pipex Status' $status
<./tests/test1.txt grep inet | wc -l > .test/test2.txt
status=$?
echo 'Expected Status' $status
echo -e '--------------------------\n'
((NUM_TESTS++))