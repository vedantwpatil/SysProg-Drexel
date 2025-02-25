#!/usr/bin/env bats

# File: student_tests.sh
#
# Create your unit tests suit in this file

@test "Execute external command with arguments" {
  run "./dsh" <<EOF
  ls -la
EOF

  # Check that the command executed successfully
  [ "$status" -eq 0 ]

  # Verify that the output contains typical ls -la output elements
  [[ "$output" =~ "total" ]]
  [[ "$output" =~ "drwx" ]]
}

@test "Handle command not found error" {
  run "./dsh" <<EOF
  nonexistentcommand
EOF

  # The shell should continue running even if command fails
  [ "$status" -eq 0 ]

  # Output should contain some error message about command not found
  [[ "$output" =~ "not found" ]] || [[ "$output" =~ "No such file" ]]
}

@test "Exit command works properly" {
  run "./dsh" <<EOF
  exit
EOF

  # Check that the shell exited cleanly
  [ "$status" -eq 0 ]

  # Verify that the output contains the expected shell prompt
  [[ "$output" =~ "dsh2>" ]]

  # Verify that the shell loop returned with exit code
  [[ "$output" =~ "cmd loop returned" ]]
}
